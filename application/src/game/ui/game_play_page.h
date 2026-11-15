/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "game/ui/game_ui_manager.h"
#include "game/core/game_solver.h"
#include "game/scene.h"
#include "common/ui_style.h"
#include "game/game_graphics.h"
#include "game/core/game_levels.h"

typedef struct Scene Scene;

enum
{
    PAGE_ACTION_NONE = 0,
    PAGE_ACTION_OPEN_MENU,
    PAGE_ACTION_SOLVER,
    PAGE_ACTION_RESUME,
    PAGE_ACTION_QUIT,
    PAGE_ACTION_OPEN_CONTROLS,
    PAGE_ACTION_BACK_FROM_MENU,
    PAGE_ACTION_OPEN_SETTINGS,
    PAGE_ACTION_BACK_TO_PAUSE,
    PAGE_ACTION_REGENERATE,

   
    PAGE_ACTION_INC_SKEL,     
    PAGE_ACTION_DEC_SKEL,      
    PAGE_ACTION_INC_TOMB,     
    PAGE_ACTION_DEC_TOMB,      
    PAGE_ACTION_GENERATE_MANUAL, 

    PAGE_ACTION_GENERATE_DIFF, 
    PAGE_ACTION_CHANGE_DIFF_LIST,

        
    PAGE_ACTION_TOGGLE_EDITOR,
    PAGE_ACTION_EDITOR_PLAY,
    PAGE_ACTION_CHOOSE_MANUAL,
    PAGE_ACTION_CHOOSE_DIFFICULTY,
    PAGE_ACTION_SELECT_TYPE_A = 100,
 
};

typedef struct GamePlayPage
{
    Scene* m_scene;
    GameUIManager* m_manager;
    UIFocusManager* m_focusManager;

    int m_nextAction;

    UIObject* m_hudPanel;

    UILabel* m_lblMoves;
    UILabel* m_lblVict;

    UIObject* m_overlayPanel;
    UIObject* m_popupPanel;   // Popup Pause
    UIObject* m_settingsPanel;// Popup Settings
    UIObject* m_victoryPanel; // Popup Victoire
    UIObject* m_controlsPanel;//touche 
    UIObject* m_editorPanel;  //pièces


    UIObject* m_manualGenPanel;
    UIObject* m_diffGenPanel;

    UILabel* m_lblCountSkel;
    UILabel* m_lblCountTomb;
    int m_currentNbSkel;
    int m_currentNbTomb;

    UIList* m_difficultyList;

    bool m_isPaused;
    bool m_isSettingsOpen;
    bool m_isVictory;
} GamePlayPage;

GamePlayPage* GamePlayPage_create(Scene* scene, GameUIManager* manager);
void GamePlayPage_destroy(GamePlayPage* self);
void GamePlayPage_update(GamePlayPage* self, UIInput* input);
