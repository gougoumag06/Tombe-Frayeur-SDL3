/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/
#pragma once
#include "game_core.h" 
#include "settings.h"  
#include "hash_map.h" 


 // @brief Résout un niveau de jeu en utilisant un algorithme de parcours en largeur (BFS).
 //@param initialLevel Pointeur vers l'état initial du jeu (la structure GameCore).
 //@param silent Si true, le solveur n'affiche rien dans la console (mode silencieux).
 //Si false, il affiche la solution 
 //@return int Le nombre de coups minimal pour résoudre le niveau, ou -1 si impossible.
int GameSolver_solve(GameCore* initialLevel, bool silent);


// @brief Calcule le chemin le plus court vers la victoire et joue automatiquement le premier coup.
// @param currentLevel Pointeur vers l'état actuel du jeu. Le pointeur est mi a jour apres le mouv 
// Si aucune solution n'est trouvée, l'état reste inchangé.
void GameSolver_playNextMove(GameCore* currentLevel);
