/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/
#include "game_solver.h"

#define QUEUE_INIT_CAPACITY 4096
#define QUEUE_CAPACITY 200000 
#define MAX_DEPTH 500

static bool growQueue(GameCore** queue, int* capacity, int* head, int* tail)
{
    int oldCap = *capacity;
    if (oldCap >= QUEUE_CAPACITY) return false;

    int newCap = oldCap * 2;
    if (newCap > QUEUE_CAPACITY) newCap = QUEUE_CAPACITY;

    GameCore* newQueue = (GameCore*)malloc(newCap * sizeof(GameCore));
    AssertNew(newQueue);

    int count = 0;
    int idx = *head;
    while (idx != *tail)
    {
        newQueue[count++] = (*queue)[idx];
        idx = (idx + 1) % oldCap;
    }

    free(*queue);
    *queue = newQueue;
    *capacity = newCap;
    *head = 0;
    *tail = count;
    return true;
}

// on met les object de la map dans un list 
static void extractObjects(GameCore* c, Object list[12])
{
    AssertNew(c);


    for (int i = 0; i < 12; i++) list[i].id = -1;
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            int id = c->map[x][y].id;
            if (id >= 0 && id < 12)
            {
                list[id] = c->map[x][y];
            }
        }
    }
}

// affichage de la solution
static void printSolver(GameHashmap* map, GameCore finalState, GameCore initialState)
{
    AssertNew(map);


    //on part de la victoire pour remonte vers l etat initiale
    GameCore path[MAX_DEPTH];
    int d = 0;              //on compte le nombre d etape 
    GameCore curr = finalState; //etat final 

    // Tant que l on a pas fait la limite de profondeur on arrete pas 
    while (d < MAX_DEPTH)
    {
        path[d++] = curr;
        // on compare les hash pour verifié que c'est pas le meme etat que l etat initial
        if (HashMap_lists8AreEquals(curr.forHash, initialState.forHash)) break;
        //on recupere l etat precedant 
        curr = HashMap_get(map, curr);
    }
    printf("\nSOLUTION TROUVEE EN %d COUP\n", d - 1);//nombre de coup total 

    Object listA[12]; // liste pour connaitre l etat avant le mouvement 
    Object listB[12]; //  liste pour connaitre l etat apres le mouvement 
    for (int i = d - 1; i > 0; i--)
    {
        GameCore StateA = path[i];
        GameCore StateB = path[i - 1];
        extractObjects(&StateA, listA);
        extractObjects(&StateB, listB);

        // on regarde quel piece a changé 
        for (int k = 0; k < StateA.nbObject; k++)
        {
            if (listA[k].id == -1 || listB[k].id == -1) continue;
            if (listA[k].pos_x != listB[k].pos_x || listA[k].pos_y != listB[k].pos_y || listA[k].rotationIndex != listB[k].rotationIndex)
            {
                printf("Coup %02d : Piece %d (Type %d) en [%d,%d] -> ", (d - i), k, listA[k].type, listA[k].pos_x, listA[k].pos_y);
                int hasMoved = 0;
                //on regarde comment elle a bougé 
                if (listB[k].pos_x < listA[k].pos_x)
                {
                    printf("HAUT"); hasMoved = 1;
                }
                else if (listB[k].pos_x > listA[k].pos_x)
                {
                    printf("BAS"); hasMoved = 1;
                }
                else if (listB[k].pos_y < listA[k].pos_y)
                {
                    printf("GAUCHE"); hasMoved = 1;
                }
                else if (listB[k].pos_y > listA[k].pos_y)
                {
                    printf("DROITE"); hasMoved = 1;
                }

                if (listB[k].rotationIndex != listA[k].rotationIndex)//es que la piece a tourné?
                {
                    if (hasMoved) printf(" ET ");

                    int diff = (listB[k].rotationIndex - listA[k].rotationIndex + 4) % 4;

                    if (diff == 1)
                    {
                        printf("ROTATION HORAIRE");
                    }
                    else if (diff == 3)
                    {
                        printf("ROTATION ANTI-HORAIRE");
                    }
                    else
                    {
                        printf("ROTATION 180");
                    }
                }
                printf("\n");
            }
        }
    }
}

int GameSolver_solve(GameCore* initialLevel, bool silent)
{
    AssertNew(initialLevel);

    //table de hash pour le stockage
    GameHashmap* map = HashMap_create(8192);
    //file d attente des chose a teste 
    int queueCap = QUEUE_INIT_CAPACITY;
    GameCore* queue = (GameCore*)malloc(queueCap * sizeof(GameCore));
    AssertNew(queue);
    int head = 0;
    int tail = 0;
    GameCore_updateListForHash(initialLevel);//tableau initial 
    HashMap_insert(map, *initialLevel, *initialLevel);//on l insert avec l ui meme comme parent 
    queue[tail] = *initialLevel;
    tail++;
    int steps = -1;       // -1 ==pas de solution trouvé 
    int visited = 0;
    Object tempList[12];  //tableau avec les object 

    while (head != tail)
    {
        visited++;
        //si il y a trop de possibilité on arrete 
        if (visited > 500000)
        {
            break;
        }
        GameCore s = queue[head];
        head = (head + 1) % queueCap;

        //verification de la victoire 
        if (GameCore_isWon(&s))
        {
            if (!silent) printSolver(map, s, *initialLevel);//on print la solution final 
            steps = 0;
            GameCore tmp = s;
            while (!HashMap_lists8AreEquals(tmp.forHash, initialLevel->forHash))//on regarde en combien de coup 
            {
                tmp = HashMap_get(map, tmp);
                steps++;
            }
            break;
        }
        for (int i = 0; i < 12; i++) tempList[i].id = -1;
        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                if (s.map[x][y].id != -1) tempList[s.map[x][y].id] = s.map[x][y];
            }
        }
        //on esseye de bougé tout les object present sur le plateau 
        for (int i = 0; i < s.nbObject; i++)
        {
            Object currentObj = tempList[i];
            if (currentObj.id == -1 || !currentObj.canMove) continue;//si c'est un tombe 

            //il y a 5 action, sur place, haut, bas, droit, gauche 
            int dx[] = { 0, -1, 1, 0, 0 };
            int dy[] = { 0, 0, 0, -1, 1 };

            for (int dir = 0; dir < 5; dir++)// on les teste toute 
            {
                int nx = currentObj.pos_x + dx[dir];
                int ny = currentObj.pos_y + dy[dir];
                bool is_valid_pos = (nx >= 0 && nx < 3 && ny >= 0 && ny < 4);
                bool is_empty = is_valid_pos && (s.map[nx][ny].id == -1);
                bool is_self = (nx == currentObj.pos_x && ny == currentObj.pos_y);

                if (is_valid_pos && (is_empty || is_self))
                {
                    for (int rot = 0; rot < 4; rot++)
                    {
                        if (dir == 0 && rot == currentObj.rotationIndex) continue;

                        GameCore next = s;

                        next.map[currentObj.pos_x][currentObj.pos_y].id = -1;

                        Object nextObj = currentObj;
                        nextObj.pos_x = nx;
                        nextObj.pos_y = ny;
                        nextObj.rotationIndex = rot;
                        next.map[nx][ny] = nextObj;


                        if (GameCore_verifBorders(nx, ny, &next, currentObj.type, rot))
                        {
                            GameCore_updateListForHash(&next);
                            // si on a j amais eu cette etat 
                            if (!HashMap_isIn(map, next))
                            {
                                HashMap_insert(map, next, s);
                                //on l ajoute dans la file a exploré 
                                int nextTail = (tail + 1) % queueCap;
                                if (nextTail == head)
                                {
                                    if (!growQueue(&queue, &queueCap, &head, &tail))
                                    {
                                        printf("probleme la file est pleine \n");
                                        free(queue);
                                        HashMap_destroy(map);
                                        return -1;
                                    }
                                    nextTail = (tail + 1) % queueCap;
                                }
                                queue[tail] = next;
                                tail = nextTail;
                            }
                        }
                    }
                }
            }
        }
    }

    // on libere tout 
    if (queue) free(queue);
    if (map) HashMap_destroy(map);
    if (steps == -1 && !silent) printf("Pas de solution trouvee.\n");
    return steps;
}


void GameSolver_playNextMove(GameCore* currentLevel)
{
    AssertNew(currentLevel);

    //table de hash pour le stockage
    GameHashmap* map = HashMap_create(8192);
    //file d attente des chose a teste 
    int queueCap = QUEUE_INIT_CAPACITY;
    GameCore* queue = (GameCore*)calloc(queueCap, sizeof(GameCore));
    AssertNew(queue);

    int head = 0;
    int tail = 0;

    GameCore_updateListForHash(currentLevel);//tableau initial 
    HashMap_insert(map, *currentLevel, *currentLevel);//on l insert avec l ui meme comme parent 
    queue[tail] = *currentLevel;
    tail++;
    int visited = 0;
    bool found = false;
    GameCore finalState;
    Object tempList[12];  //tableau avec les object 

    while (head != tail)
    {
        visited++;
        //si il y a trop de possibilité on arrete 
        if (visited > 500000)
        {
            break;
        }

        GameCore s = queue[head];
        head = (head + 1) % queueCap;

        //verification de la victoire 
        if (s.map[0][3].id != -1 && s.map[0][3].type == 0)
        {
            finalState = s;
            found = true;
            break;
        }

        for (int i = 0; i < 12; i++) tempList[i].id = -1;
        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                if (s.map[x][y].id != -1) tempList[s.map[x][y].id] = s.map[x][y];
            }
        }

        //on esseye de bougé tout les object present sur le plateau 
        for (int i = 0; i < s.nbObject; i++)
        {
            Object currentObj = tempList[i];
            if (currentObj.id == -1 || !currentObj.canMove) continue;//si c'est un tombe 

            //il y a 5 action, sur place, haut, bas, droit, gauche 
            int dx[] = { 0, -1, 1, 0, 0 };
            int dy[] = { 0, 0, 0, -1, 1 };

            for (int dir = 0; dir < 5; dir++)// on les teste toute 
            {
                int nx = currentObj.pos_x + dx[dir];
                int ny = currentObj.pos_y + dy[dir];

                // Vérifications 
                bool is_valid_pos = (nx >= 0 && nx < 3 && ny >= 0 && ny < 4);
                bool is_empty = is_valid_pos && (s.map[nx][ny].id == -1);
                bool is_self = (nx == currentObj.pos_x && ny == currentObj.pos_y);

                if (is_valid_pos && (is_empty || is_self))
                {
                    for (int rot = 0; rot < 4; rot++)
                    {
                        if (dir == 0 && rot == currentObj.rotationIndex) continue;

                        GameCore next = s;
                        next.map[currentObj.pos_x][currentObj.pos_y].id = -1;
                        Object nextObj = currentObj;
                        nextObj.pos_x = nx; nextObj.pos_y = ny; nextObj.rotationIndex = rot;
                        next.map[nx][ny] = nextObj;

                        if (GameCore_verifBorders(nx, ny, &next, currentObj.type, rot))
                        {
                            GameCore_updateListForHash(&next);
                            // si on a j amais eu cette etat 
                            if (!HashMap_isIn(map, next))
                            {
                                HashMap_insert(map, next, s);
                                //on l ajoute dans la file a exploré 
                                int nextTail = (tail + 1) % queueCap;
                                if (nextTail == head) // Cas file pleine 
                                {
                                    if (!growQueue(&queue, &queueCap, &head, &tail))
                                    {
                                        free(queue);
                                        HashMap_destroy(map);
                                        return;
                                    }
                                    nextTail = (tail + 1) % queueCap;
                                }
                                queue[tail] = next;
                                tail = nextTail;
                            }
                        }
                    }
                }
            }
        }
    }
    //on cherche le mouv a jouer 
    if (found)//si c'est soluble 
    {
        GameCore curr = finalState;
        GameCore prev = HashMap_get(map, curr);

        if (!HashMap_lists8AreEquals(curr.forHash, currentLevel->forHash))
        {
            // On remonte jusqu'au tout premier coup
            while (!HashMap_lists8AreEquals(prev.forHash, currentLevel->forHash))
            {
                curr = prev;
                prev = HashMap_get(map, curr);
            }
            // on deplace la peice
            *currentLevel = curr;
        }
    }
    else
    {
        printf("il y a pas de solution\n");
    }

    // on libere tout 
    if (queue) free(queue);
    if (map) HashMap_destroy(map);
}
