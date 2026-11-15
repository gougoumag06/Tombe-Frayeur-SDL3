/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"


extern int g_objectType[9][4];

typedef struct Object
{
    int id; //Identifiant de l'objet. (unique pour chaque objet)
    int pos_x; //Position de l'objet sur l'axe des x.
    int pos_y; //Position de l'objet sur l'axe des y
    bool canMove; //true si l'objet peut bouger.
    bool canExit; //true si l'objet peut sortir. (seul la fille peut sortir)
    int type; //Type de l'objet, en lien avec g_objectType. (entier entre 0 et 8)
    int rotationIndex; //Indice de rotation de l'objet. (entier entre 0 et 4)
} Object;


typedef struct GameCore
{
    Object map[3][4]; //Matrice de cartographie des éléments sur le jeu, l'axe des x est géré par le 1er indice, l'axe des y par le 2nd.
    int nbObject;
    int forHash[8]; //Liste d'entier utiles pour la GameHashmap. (unique pour chaque core)
} GameCore;

/// @brief Crée un objet
/// @param id L'identifiant de l'objet.
/// @param pos_x La position de l'objet selon l'axe x.
/// @param pos_y La position de l'objet selon l'axe y.
/// @param type Le type de l'objet (entier entre 0 et 8).
/// @param rotationIndex L'indice de rotation (entier entre 0 et 3).
/// @return L'objet créé (de la structure Object).
void GameCore_createObject(Object* obj, int id, int pos_x, int pos_y, int type, int rotationIndex);

/// @brief Calcule les bordures indices de bordure d'un objet (1 si ça dépasse, -1 si c'est un creux, 0 sinon).
/// @param type Le type de l'objet (entier entre 0 et 8).
/// @param rotationIndex L'indice de rotation de l'objet (entier entre 0 et 3).
/// @param list La liste dans laquelle on met les indices de bordure.
void GameCore_bordersObject(int type, int rotationIndex, int (*list)[4]);

/// @brief Vérifie si les bords se piétinent.
/// @param x La position sur l'axe x de la case a vérifier.
/// @param y La position sur l'axe y de la case a vérifier.
/// @param core Pointeur vers le noyau du jeu.
/// @param type Le type de l'objet qu'on vérifie (entier entre 0 et 8).
/// @param rotationIndex L'indice de rotation de l'objet (entier entre 0 et 3).
/// @return false si les bords se piétinent.
bool GameCore_verifBorders(int x, int y, GameCore* core, int type, int rotationIndex);

/// @brief Mélange un tableau de 12 entiers.
/// @param tab Pointeur sur le tableau a mélanger.
void GameCore_permutationsTab12(int (*tab)[12]);

/// @brief Mélange un tableau de 4 entiers.
/// @param tab Pointeur sur le tableau a mélanger.
void GameCore_permutationsTab4(int (*tab)[4]);

/// @brief Vérifie si les coordonnées sont valides dans la grille.
/// @param x La coordonnée selon l'axe x.
/// @param y La coordonnée selon l'axe y.
/// @return true si les coordonnées sont valides.
bool GameCore_valide(int x, int y);

/// @brief Fait tourner l'objet en argument de 90° dans le sens horaire.
/// @param obj L'objet a tourner.
void GameCore_rotateObjectCW(Object* obj);

/// @brief Fait tourner l'objet en argument de 90° dans le sens anti horaire.
/// @param obj Pointeur sur l'objet a tourner.
void GameCore_rotateObjectCCW(Object* obj);

/// @brief Met à jour la liste pour le Hash.
/// @param core Pointeur sur l'objet a tourner.
void GameCore_updateListForHash(GameCore* core);

/// @brief Indique si la partie est gagnée.
/// @param core Pointeur sur le noyaux du jeu.
/// @return true si la partie est gagnée.
INLINE bool GameCore_isWon(GameCore* core)
{
    return (core->map[0][3].type == 0) && (core->map[0][3].id >= 0);
}
