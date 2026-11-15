/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/
#pragma once
#include "settings.h"
#include "game_core.h"

typedef struct GameHashmapEntry
{
    GameCore prevState;
    GameCore currState;
} GameHashmapEntry;

typedef struct GameHashmap
{
    GameHashmapEntry* m_entries;
    int* m_idMap;
    size_t m_capacity;
    size_t m_size;
} GameHashmap;

/// @brief Crée une table de hash.
/// @param capacity La capacité de la table de hash.
/// @return Un pointeur sur la table de hash.
GameHashmap* HashMap_create(int capacity);

/// @brief Détruit une table de hash.
/// @param self La table de hash a détruire.
void HashMap_destroy(GameHashmap* self);

/// @brief Insère des valeurs (de la structure GameCore) dans une table de hash.
/// @param self La table de hash.
/// @param currState L'état actuel du jeu.
/// @param prevState L'état précédant du jeu.
void HashMap_insert(GameHashmap* self, GameCore currState, GameCore prevState);

/// @brief Indique si le jeu est présent dans cet état dans la table de hash.
/// @param self La table de hash.
/// @param currState L'état actuel du jeu.
/// @return true si le currState est présent dans la table.
bool HashMap_isIn(GameHashmap* self, GameCore currState);

/// @brief Donne l'état précédant du jeu à partir d'un état du jeu.
/// 
/// ATTENTION, NE PAS UTILISER CETTE FONCTION SI ON N'EST PAS CERTAIN QUE L'ETAT DU JEU EST PRESENT DANS LA TABLE DE HASH !
/// @param self La table de hash.
/// @param currState L'état actuel du jeu.
/// @return L'état précédant currState.
GameCore HashMap_get(GameHashmap* self, GameCore currState);

/// @brief Change la capacité de la table de hash.
/// @param self La table de hash.
/// @param newCapacity La nouvelle capacité de la table hash.
void HashMap_resize(GameHashmap* self, int newCapacity);

/// @brief Indique si les 2 listes de 8 entiers sont identiques.
/// @param list1 La première liste.
/// @param list2 La deuxième liste.
/// @return true si les 2 listes sont identiques.
bool HashMap_lists8AreEquals(int list1[8], int list2[8]);
