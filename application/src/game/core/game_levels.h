/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/
#pragma once
#include "game_core.h"
#include "game_solver.h"
#include "settings.h"

typedef enum Difficulty
{
    EASY,
    NORMAL,
    HARD
} Difficulty;

/// @brief Vérifie que la carte du jeu ne contient pas d'erreurs, qu'aucune pièce ne dépasse sur une autre.
/// @param core L'état du jeu.
/// @return false si elle contient des erreurs, true sinon.
bool GameLevels_mapIsCorrect(GameCore core);

/// @brief Charge un niveau spécifique du livret dans le GameCore.
/// @param levelID L'ID du niveau.
/// @param core L'endroit ou stocker le niveau.
/// @return true si le niveau existe.
bool GameLevels_loadReportLevel(int levelID, GameCore* core);

/// @brief Charge un niveau aléatoire.
/// @param nbSkel Le nombre de squelettes dans le niveau.
/// @param nbTomb Le nombre de tombes dans le niveau.
/// @param core Pointeur vers le noyaux du niveau à charger.
/// @return true si la génération est réussie.
bool GameLevels_loadRandomLevel(int nbSkel, int nbTomb, GameCore* core);

/// @brief Génère de jeu dont le niveau dépend de la difficulté demandée.
/// @param difficultyLevel Un niveau de difficulté entre simple (soluble en 10 à 20 coups) à impossible (soluble en plus de 80 coups)
/// @return Le noyaux du jeu.
GameCore GameLevels_loadGameByDifficulty(Difficulty difficultyLevel);

/// @brief Trouve tous les niveaux possible à partir d'un niveau résolu et les mets dans une GameHashmap.
/// @param initialLevel Niveau initial.
/// @return La GameHashmap créée.
GameHashmap* GameLevels_findLevel(GameCore* initialLevel);

/// @brief Trouve un niveau soluble en un nombre de coups défini à partir d'une hashmap.
/// @param map La hashmap.
/// @param minAttemps Le nombre minimal de coups que doit avoir le niveau.
/// @param maxAttemps Le nombre maximal de coups que doit avoir le niveau.
/// @param hasWorked Est mis à true si un niveau est trouvé, à false sinon.
/// @return Le niveau ainsi trouvé.
GameCore GameLevels_findCoreOnMap(GameHashmap* map, int minAttemps, int maxAttemps, bool* hasWorked);
