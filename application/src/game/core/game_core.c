/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game_core.h"



int g_objectType[9][4] = { //up, right, down, left
    {1, 0, 0, 0},       //squelette A (celui qui doit sortir)
    {1, -1, -1, 0},     //squelette B
    {1, -1, 0, 0},      //squelette C
    {1, -1, 0, -1},     //squelette D
    {1, 0, 0, -1},      //squelette E
    {1, -1, -1, -1},    //squelette F
    {1, 0, -1, -1},     //squelette G
    {1, 0, -1, 0},      //squelette H
    {0, 0, 0, 0},       //tombes
};

void GameCore_createObject(Object* obj, int id, int pos_x, int pos_y, int type, int rotationIndex)
{
    AssertNew(obj);


    obj->id = id;
    obj->pos_x = pos_x;
    obj->pos_y = pos_y;
    obj->type = type;
    obj->rotationIndex = rotationIndex;

    if (!obj->type) obj->canExit = true;
    else obj->canExit = false;

    if (obj->type == 8) obj->canMove = false;
    else obj->canMove = true;
}

void GameCore_bordersObject(int type, int rotationIndex, int (*list)[4])
{
    AssertNew(list);
    type %= 9;


    for (int i = 0; i < 4; i++)
    {
        (*list)[i] = g_objectType[type][(i - rotationIndex + 4) % 4];
    }
}

bool GameCore_verifBorders(int x, int y, GameCore* core, int type, int rotationIndex)
{
    AssertNew(core);


    int currentBorders[4];
    GameCore_bordersObject(type, rotationIndex, &currentBorders);

    // Limites grille
    if (x == 0 && currentBorders[0] == 1) return false;
    if (x == 2 && currentBorders[2] == 1) return false;
    if (y == 0 && currentBorders[3] == 1) return false;
    if (y == 3 && currentBorders[1] == 1 && (x != 0)) return false;

    int neighborBorders[4];
    //Voisin haut
    if (x > 0)
    {
        Object* neighbor = &core->map[x - 1][y];
        if (neighbor->id != -1)
        {
            GameCore_bordersObject(neighbor->type, neighbor->rotationIndex, &neighborBorders);
            if (currentBorders[0] + neighborBorders[2] > 0) return false;
        }
    }
    //Voisin bas
    if (x < 2)
    {
        Object* neighbor = &core->map[x + 1][y];
        if (neighbor->id != -1)
        {
            GameCore_bordersObject(neighbor->type, neighbor->rotationIndex, &neighborBorders);
            if (currentBorders[2] + neighborBorders[0] > 0) return false;
        }
    }
    //Voisin gauche
    if (y > 0)
    {
        Object* neighbor = &core->map[x][y - 1];
        if (neighbor->id != -1)
        {
            GameCore_bordersObject(neighbor->type, neighbor->rotationIndex, &neighborBorders);
            if (currentBorders[3] + neighborBorders[1] > 0) return false;
        }
    }
    //Voisin droite
    if (y < 3)
    {
        Object* neighbor = &core->map[x][y + 1];
        if (neighbor->id != -1)
        {
            GameCore_bordersObject(neighbor->type, neighbor->rotationIndex, &neighborBorders);
            if (currentBorders[1] + neighborBorders[3] > 0) return false;
        }
    }

    return true; //c'est ok 
}

void GameCore_permutationsTab12(int (*tab)[12])
{
    AssertNew(tab);


    int a, tmp;
    for (int i = 0; i < 12; i++)
    {
        a = rand() % 12;
        tmp = (*tab)[a];
        (*tab)[a] = (*tab)[i];
        (*tab)[i] = tmp;
    }
}

void GameCore_permutationsTab4(int (*tab)[4])
{
    AssertNew(tab);


    int a, tmp;
    for (int i = 0; i < 4; i++)
    {
        a = rand() % 4;
        tmp = (*tab)[a];
        (*tab)[a] = (*tab)[i];
        (*tab)[i] = tmp;
    }
}

bool GameCore_valide(int x, int y)
{
    return (x >= 0 && x < 3 && y >= 0 && y < 4);
}

void GameCore_rotateObjectCW(Object* obj)
{
    AssertNew(obj);


    obj->rotationIndex = (obj->rotationIndex + 1) % 4;
}

void GameCore_rotateObjectCCW(Object* obj)
{
    AssertNew(obj);


    obj->rotationIndex = (obj->rotationIndex + 3) % 4;
}

void GameCore_updateListForHash(GameCore* core)
{
    AssertNew(core);


    int count = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (core->map[i][j].canMove && core->map[i][j].id != -1)
            {
                core->forHash[count] = (core->map[i][j].id << 6) + (core->map[i][j].pos_x << 4) + (core->map[i][j].pos_y << 2) + core->map[i][j].rotationIndex;
                count++;
            }
        }
    }
    for (int i = count; i < 8; i++)
    {
        core->forHash[i] = -1;
    }
}

