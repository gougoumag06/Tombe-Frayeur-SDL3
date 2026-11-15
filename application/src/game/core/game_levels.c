/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/
#include "game_levels.h"
#define QUEUE_CAPACITY 200000

static void resetCore(GameCore* core)
{
    AssertNew(core);

    memset(core, 0, sizeof(GameCore));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            core->map[i][j].id = -1;
        }
    }
    core->nbObject = 0;
}

static void GameLevels_addObject(GameCore* c, int type, int x, int y, int rot)
{
    AssertNew(c);

    if (x < 0 || x > 2 || y < 0 || y > 3) return;

    int id = c->nbObject;
    GameCore_createObject(&c->map[x][y], id, x, y, type, rot);
    c->nbObject++;
}

bool GameLevels_mapIsCorrect(GameCore core)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Object obj = core.map[i][j];
            if (obj.id != -1 && !GameCore_verifBorders(i, j, &core, obj.type, obj.rotationIndex)) return false;
        }
    }
    return true;
}

bool GameLevels_loadReportLevel(int levelID, GameCore* core) //utilisée uniquement pour la première génération
{
    AssertNew(core);

    resetCore(core);
    switch (levelID)
    {
    case 1:
        GameLevels_addObject(core, 8, 0, 0, 0);
        GameLevels_addObject(core, 8, 0, 2, 0);
        GameLevels_addObject(core, 8, 2, 2, 0);
        GameLevels_addObject(core, 2, 0, 1, 3);
        GameLevels_addObject(core, 0, 2, 1, 0);
        return true;

    case 13:
        GameLevels_addObject(core, 8, 0, 0, 0);
        GameLevels_addObject(core, 8, 1, 2, 0);
        GameLevels_addObject(core, 8, 2, 3, 0);
        GameLevels_addObject(core, 0, 1, 1, 0);
        GameLevels_addObject(core, 3, 0, 2, 3);
        GameLevels_addObject(core, 2, 1, 3, 0);
        return true;

    case 15:
        GameLevels_addObject(core, 8, 0, 1, 0);
        GameLevels_addObject(core, 8, 2, 2, 0);
        GameLevels_addObject(core, 0, 2, 0, 0);
        GameLevels_addObject(core, 7, 0, 2, 2);
        return true;

    case 158:
        GameLevels_addObject(core, 8, 0, 1, 0);
        GameLevels_addObject(core, 8, 1, 0, 0);
        GameLevels_addObject(core, 8, 2, 0, 0);
        GameLevels_addObject(core, 0, 2, 3, 3);
        GameLevels_addObject(core, 1, 2, 1, 1);
        GameLevels_addObject(core, 2, 0, 3, 3);
        GameLevels_addObject(core, 3, 0, 2, 2);
        GameLevels_addObject(core, 5, 2, 2, 0);
        GameLevels_addObject(core, 6, 1, 1, 1);
        GameLevels_addObject(core, 7, 1, 3, 3);
        return true;

    default:
        return false;
    }
}

bool GameLevels_loadRandomLevel(int nbSkel, int nbTomb, GameCore* core)
{
    AssertNew(core);

    if (nbSkel < 1) nbSkel = 1;
    if (nbTomb < 0) nbTomb = 0;
    if (nbSkel > 7) nbSkel = 7;
    if (nbTomb > 3) nbTomb = 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            core->map[i][j].id = -1;
            core->map[i][j].pos_x = i;
            core->map[i][j].pos_y = j;
            core->map[i][j].type = 0;
            core->map[i][j].rotationIndex = 0;
            core->map[i][j].canExit = false;
            core->map[i][j].canMove = false;
        }
    }
    core->nbObject = 0;

    //variables utiles dans la fonction     
    int a, b; //variables temporaires pour les coordonnées
    int c; //variable temporaire pour les types de squelettes
    int permutationsPositions[12]; //tableau de permutations
    int permutationsRotations[4] = { 2, 3, 1, 0 };

    for (int i = 0; i < 12; i++) permutationsPositions[i] = (i * 5 + 3) % 12;
    bool type[7]; //variable temporaire pour savoir quels types de squelettes sont déjà sur le jeu (true si déjà dans le jeu)
    for (int i = 0; i < 7; i++) type[i] = false;
    int rotationIndex;

    GameLevels_addObject(core, 0, 0, 3, 1);

    //creation des squelettes
    for (int i = 0; i < nbSkel; i++)
    {
        GameCore_permutationsTab12(&permutationsPositions);
        GameCore_permutationsTab4(&permutationsRotations);

        //choix d'un type de squelette qui n'est pas encore sur le plateau
        do
        {
            c = rand() % 7;
        } while (type[c]);
        c++; //les squelettes sont de type 1 à 7, pas 0 à 6

        //choix d'une position ou le squelette peut être placé
        for (int i = 0; i < 12; i++)
        {
            if (permutationsPositions[i] != 3)
            {
                a = permutationsPositions[i] / 4;
                b = permutationsPositions[i] % 4;
                rotationIndex = rand() % 4;
                if (GameCore_verifBorders(a, b, core, c, rotationIndex) && core->map[a][b].id == -1) break;
                rotationIndex = (rotationIndex + 1) % 4;
                if (GameCore_verifBorders(a, b, core, c, rotationIndex) && core->map[a][b].id == -1) break;
                rotationIndex = (rotationIndex + 1) % 4;
                if (GameCore_verifBorders(a, b, core, c, rotationIndex) && core->map[a][b].id == -1) break;
                rotationIndex = (rotationIndex + 1) % 4;
                if (GameCore_verifBorders(a, b, core, c, rotationIndex) && core->map[a][b].id == -1) break;

                if (i == 12) return false;
            }
        }
        type[c - 1] = true;
        GameLevels_addObject(core, c, a, b, rotationIndex);
    }

    //creation des tombes
    for (int i = 0; i < nbTomb; i++)
    {
        GameCore_permutationsTab12(&permutationsPositions);
        for (int i = 0; i < 12; i++)
        {
            if (permutationsPositions[i] != 3)
            {
                if (core->map[0][2].id != -1 && core->map[0][2].type == 8 && permutationsPositions[i] == 7) continue;
                if (core->map[1][3].id != -1 && core->map[1][3].type == 8 && permutationsPositions[i] == 2) continue;
                a = permutationsPositions[i] / 4;
                b = permutationsPositions[i] % 4;
                if (GameCore_verifBorders(a, b, core, 8, 0) && core->map[a][b].id == -1) break;
            }

            if (i == 12) return false;
        }
        GameLevels_addObject(core, 8, a, b, rotationIndex);
    }
    return true;
}

GameCore GameLevels_loadGameByDifficulty(Difficulty difficultyLevel)
{
    GameCore core;
    int nbSkel, nbTomb, minToSolve, maxToSolve;
    bool levelIsOk = false, creationSuccess;
    GameHashmap* map;
    do
    {
        switch (difficultyLevel)
        {
        case EASY:
            nbTomb = 3;
            nbSkel = (rand() % 4) + 3;
            break;
        case NORMAL:
            nbTomb = 3;
            nbSkel = (rand() % 3) + 4;
            break;
        case HARD:
            nbTomb = 3;
            nbSkel = (rand() % 3) + 5;
            break;
        default:
            assert(false);
            break;
        }
        do
        {
            GameLevels_loadRandomLevel(nbSkel, nbTomb, &core);
            creationSuccess = GameLevels_mapIsCorrect(core);
        } while (!creationSuccess);

        switch (difficultyLevel)
        {
        case EASY:
            minToSolve = 5;
            maxToSolve = 10;
            break;
        case NORMAL:
            minToSolve = 10;
            maxToSolve = 20;
            break;
        case HARD:
            minToSolve = 20;
            maxToSolve = 250;
            break;
        }
        map = GameLevels_findLevel(&core);
        core = GameLevels_findCoreOnMap(map, minToSolve, maxToSolve, &levelIsOk);
        HashMap_destroy(map);
        map = NULL;
    } while (!levelIsOk);

    return core;
}

GameHashmap* GameLevels_findLevel(GameCore* initialLevel) //le niveau est mis dans initialLevel à la fin
{
    AssertNew(initialLevel);

    GameHashmap* map = HashMap_create(16384);

    GameCore* queue = calloc(QUEUE_CAPACITY, sizeof(GameCore));
    AssertNew(queue);
    int head = 0;
    int tail = 0;

    GameCore_updateListForHash(initialLevel); //tableau initial 
    HashMap_insert(map, *initialLevel, *initialLevel); //on l insert avec l ui meme comme parent

    queue[tail] = *initialLevel;
    tail++;
    int visited = 0;
    Object tempList[12];  //tableau avec les object

    while (head != tail)
    {
        GameCore s = queue[head];
        visited++;

        //si il y a trop de possibilité on arrete 
        if (visited > 500000)
        {
            break;
        }
        head = (head + 1) % QUEUE_CAPACITY;

        for (int i = 0; i < 12; i++) tempList[i].id = -1;
        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                if (s.map[x][y].id != -1) tempList[s.map[x][y].id] = s.map[x][y];
            }
        }

        //on essaye de bouger tous les object presents sur le plateau 
        for (int i = 0; i < s.nbObject; i++)
        {
            Object currentObj = tempList[i];
            if (currentObj.id == -1 || !currentObj.canMove) continue;//si c'est un tombe ou s'il n'y a rien

            //il y a 5 action, sur place, haut, bas, droit, gauche 
            int dx[] = { 0, -1, 1, 0, 0 };
            int dy[] = { 0, 0, 0, -1, 1 };

            for (int dir = 0; dir < 5; dir++)// on les test toutes
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
                                //on l ajoute dans la file a explorer
                                queue[tail] = next;
                                tail = (tail + 1) % QUEUE_CAPACITY;

                                if (tail == head)
                                {
                                    free(queue);
                                    return map;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // on libere tout
    GameCore a;
    if (queue) free(queue);
    return map;
}

GameCore GameLevels_findCoreOnMap(GameHashmap* map, int minAttempt, int maxAttempt, bool* hasWorked)
{
    AssertNew(map);
    AssertNew(hasWorked);

    int len = map->m_size - 1;
    GameCore core = map->m_entries[len].currState;
    int nbLvl = GameSolver_solve(&core, true);
    while (nbLvl > maxAttempt)
    {
        core = HashMap_get(map, core);
        nbLvl--;
    }
    if (nbLvl >= minAttempt) *hasWorked = true;
    else *hasWorked = false;
    return core;
}
