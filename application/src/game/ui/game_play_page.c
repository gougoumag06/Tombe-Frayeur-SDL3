/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/ui/game_play_page.h"
#include "game/core/game_levels.h" 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// les fonction pour caché ou montré 
//fonction pour caché et montre le popup(il sont tous sous le memes
static void GamePlayPage_setPopupVisible(GamePlayPage* self, bool visible)
{
    UIRect rect = { 0 };
    if (visible)
    {
        // Position visible 
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(-100.0f, -100.0f);
        rect.offsetMax = Vec2_set(+100.0f, +100.0f);
    }
    else
    {
        //position pas visible 
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(10000.0f, 10000.0f);
        rect.offsetMax = Vec2_set(10200.0f, 10200.0f);
    }
    UIObject_setRect(self->m_popupPanel, rect);
}

static void GamePlayPage_setSubPanelVisible(GamePlayPage* self, UIObject* panel, bool visible)
{
    if (!panel) return;
    UIRect rect = { 0 };
    if (visible)
    {
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(-200.0f, -200.0f);
        rect.offsetMax = Vec2_set(200.0f, 200.0f);
    }
    else
    {
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(10000.0f, 10000.0f);
        rect.offsetMax = Vec2_set(10000.0f, 10000.0f);
    }
    UIObject_setRect(panel, rect);
}

static void GamePlayPage_setControlsVisible(GamePlayPage* self, bool visible)
{
    if (!self->m_controlsPanel) return;

    UIRect rect = { 0 };
    if (visible)
    {
        // Position visible 
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(-250.0f, -200.0f);
        rect.offsetMax = Vec2_set(+250.0f, +200.0f);
    }
    else
    {
        // position pas visible
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(10000.0f, 10000.0f);
        rect.offsetMax = Vec2_set(10000.0f, 10000.0f);
    }
    UIObject_setRect(self->m_controlsPanel, rect);
}

static void GamePlayPage_setEditorVisible(GamePlayPage* self, bool visible)
{
    if (!self->m_editorPanel) return;

    UIRect rect = { 0 };
    if (visible)
    {
        // Position visible (à droite)
        rect.anchorMin = Vec2_set(1.0f, 0.5f);
        rect.anchorMax = Vec2_set(1.0f, 0.5f);
        rect.offsetMin = Vec2_set(-80.0f, -300.0f);
        rect.offsetMax = Vec2_set(-10.0f, 300.0f);
    }
    else
    {
        // Position cachée (très loin)
        rect.anchorMin = Vec2_set(1.0f, 0.5f);
        rect.anchorMax = Vec2_set(1.0f, 0.5f);
        rect.offsetMin = Vec2_set(10000.0f, 10000.0f);
        rect.offsetMax = Vec2_set(10000.0f, 10000.0f);
    }
    UIObject_setRect(self->m_editorPanel, rect);
}

//permet de cacher ou rendre visible la popup victoire 
static void GamePlayPage_setVictoryVisible(GamePlayPage* self, bool visible)
{
    UIRect rect = { 0 };

    if (visible)
    {
        // on centre le popup 
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(-150.0f, -100.0f);
        rect.offsetMax = Vec2_set(+150.0f, +100.0f);
    }
    else
    {
        // position pas visible 
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(10000.0f, 10000.0f);
        rect.offsetMax = Vec2_set(10200.0f, 10200.0f);
    }
    UIObject_setRect(self->m_victoryPanel, rect);
}

//on cache ou pas le bouton Menu 
static void GamePlayPage_setHudVisible(GamePlayPage* self, bool visible)
{
    UIRect rect = { 0 };

    if (visible)
    {
        // Position visible 
        rect.anchorMin = Vec2_set(0.0f, 0.0f);
        rect.anchorMax = Vec2_set(1.0f, 1.0f);
        rect.offsetMin = Vec2_set(0.0f, 0.0f);
        rect.offsetMax = Vec2_set(0.0f, 0.0f);
    }
    else
    {
        // position pas visible
        rect.anchorMin = Vec2_set(0.0f, 0.0f);
        rect.anchorMax = Vec2_set(0.0f, 0.0f);
        rect.offsetMin = Vec2_set(-10000.0f, -10000.0f);
        rect.offsetMax = Vec2_set(-9000.0f, -9000.0f);
    }
    UIObject_setRect(self->m_hudPanel, rect);
}

//on cache ou pas le setting(ne sert plus actuelement mais peut etre on va le remettre 
static void GamePlayPage_setSettingsVisible(GamePlayPage* self, bool visible)
{
    if (!self->m_settingsPanel) return;

    UIRect rect = { 0 };
    if (visible)
    {
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(-200.0f, -150.0f);
        rect.offsetMax = Vec2_set(+200.0f, +150.0f);
    }
    else
    {
        rect.anchorMin = Vec2_set(0.5f, 0.5f);
        rect.anchorMax = Vec2_set(0.5f, 0.5f);
        rect.offsetMin = Vec2_set(10000.0f, 10000.0f);
        rect.offsetMax = Vec2_set(10000.0f, 10000.0f);
    }
    UIObject_setRect(self->m_settingsPanel, rect);

    // J'ai enlevé l'appel refreshSettingsCompteur qui faisait planter
}

static void GamePlayPage_setOverlayVisible(GamePlayPage* self, bool visible)
{
    UIRect rect = { 0 };
    UIFillRect* overlay = (UIFillRect*)self->m_overlayPanel;

    if (visible)
    {
        rect.anchorMin = Vec2_set(0.0f, 0.0f);
        rect.anchorMax = Vec2_set(1.0f, 1.0f);
        rect.offsetMin = Vec2_set(0.0f, 0.0f);
        rect.offsetMax = Vec2_set(0.0f, 0.0f);
        overlay->m_color = (SDL_Color){ 0, 0, 0, 200 };
    }
    else
    {
        rect.anchorMin = Vec2_set(0.0f, 0.0f);
        rect.anchorMax = Vec2_set(0.0f, 0.0f);
        rect.offsetMin = Vec2_set(10000.0f, 10000.0f);
        rect.offsetMax = Vec2_set(10000.0f, 10000.0f);
        overlay->m_color = g_colors.transparent;
    }
    UIObject_setRect(self->m_overlayPanel, rect);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void GamePlayPage_onClick(void* selectable)
{
    GamePlayPage* self = (GamePlayPage*)UISelectable_getUserData(selectable);
    assert(self && "self must not be NULL");
    int action = UISelectable_getUserId(selectable);
    self->m_nextAction = action;
    printf("UI Click: Action %d\n", action);
}


//on creeee touuuuuuuuuuuuuuuut
GamePlayPage* GamePlayPage_create(Scene* scene, GameUIManager* manager)
{
    UICanvas* canvas = manager->m_canvas;
    AssetManager* assets = Scene_getAssetManager(scene);
    TTF_Font* fontNormal = AssetManager_getFont(assets, FONT_NORMAL);
    TTF_Font* fontBig = AssetManager_getFont(assets, FONT_BIG_BOLD);
    UIRect rect = { 0 };

    GamePlayPage* self = (GamePlayPage*)calloc(1, sizeof(GamePlayPage));
    AssertNew(self);

    self->m_manager = manager;
    self->m_scene = scene;

    self->m_hudPanel = (UIObject*)UIFillRect_create("hud-panel", g_colors.transparent);

    self->m_nextAction = PAGE_ACTION_NONE;
    self->m_isPaused = false;
    self->m_focusManager = UIFocusManager_create();
    UIFocusManager_setCanvas(self->m_focusManager, canvas);

    //page principale 
    UIObject_setParent(self->m_hudPanel, canvas);
    GamePlayPage_setHudVisible(self, true);

    // le bouton menu 
    UIButton* btnMenu = UIButton_create("btn-menu", fontNormal);
    UIButton_setLabelString(btnMenu, "Menu");
    UIButton_setOnClickCallback(btnMenu, GamePlayPage_onClick);
    UISelectable_setUserData(btnMenu, self);
    UISelectable_setUserId(btnMenu, PAGE_ACTION_OPEN_MENU);
    UIStyle_setDefaultButton(btnMenu);
    rect.anchorMin = Vec2_set(0.0f, 1.0f);
    rect.anchorMax = Vec2_set(0.0f, 1.0f);
    rect.offsetMin = Vec2_set(10.0f, -60.0f);
    rect.offsetMax = Vec2_set(80.0f, -10.0f);

    UIObject_setRect(btnMenu, rect);
    UIObject_setParent(btnMenu, self->m_hudPanel);
    UIFocusManager_addSelectable(self->m_focusManager, btnMenu);

    ///bouton passé solution
    UIButton* btnSolve = UIButton_create("btn-solve", fontNormal);
    UIButton_setLabelString(btnSolve, ">");
    UIButton_setOnClickCallback(btnSolve, GamePlayPage_onClick);
    UISelectable_setUserData(btnSolve, self);
    UISelectable_setUserId(btnSolve, PAGE_ACTION_SOLVER);
    UIStyle_setDefaultButton(btnSolve);
    rect.anchorMin = Vec2_set(0.0f, 0.5f);
    rect.anchorMax = Vec2_set(0.0f, 0.5f);
    rect.offsetMin = Vec2_set(10.0f, -60.0f);
    rect.offsetMax = Vec2_set(80.0f, -10.0f);

    UIObject_setRect(btnSolve, rect);
    UIObject_setParent(btnSolve, self->m_hudPanel);
    UIFocusManager_addSelectable(self->m_focusManager, btnSolve);

    ////texte nb coup
    self->m_lblMoves = UILabel_create("lbl-soluce-count", fontNormal);
    UILabel_setTextString(self->m_lblMoves, "Coups restants:");
    UILabel_setColor(self->m_lblMoves, g_colors.white);
    rect.anchorMin = Vec2_set(0.0f, 0.5f);
    rect.anchorMax = Vec2_set(0.0f, 0.5f);
    rect.offsetMin = Vec2_set(10.0f, 0.0f);
    rect.offsetMax = Vec2_set(90.0f, 0.0f);
    UIObject_setRect(self->m_lblMoves, rect);
    UIObject_setParent(self->m_lblMoves, self->m_hudPanel);


    //texte victoire
    self->m_lblVict = UILabel_create("lbl-soluce-count", fontBig);
    UILabel_setTextString(self->m_lblVict, "<-victoire");
    UILabel_setColor(self->m_lblVict, g_colors.red4);

    rect.anchorMin = Vec2_set(0.91f, 0.81f);
    rect.anchorMax = Vec2_set(0.91f, 0.81f);

    // Décalage pour l'éloigner du bord et définir sa taille
    rect.offsetMin = Vec2_set(0.0f, 0.0f);
    rect.offsetMax = Vec2_set(-0.0f, 0.0f);
    UIObject_setRect(self->m_lblVict, rect);
    UIObject_setParent(self->m_lblVict, self->m_hudPanel);


    // Overlay comme sa on a me petit fond plus foncé 
    self->m_overlayPanel = (UIObject*)UIFillRect_create("overlay-panel", g_colors.transparent);
    UIObject_setParent(self->m_overlayPanel, canvas);
    UIRect overlayRect = { 0 };
    overlayRect.anchorMin = Vec2_set(0.0f, 0.0f);
    overlayRect.anchorMax = Vec2_set(1.0f, 1.0f);
    overlayRect.offsetMin = Vec2_set(10000.0f, 10000.0f);
    overlayRect.offsetMax = Vec2_set(10000.0f, 10000.0f);
    UIObject_setRect(self->m_overlayPanel, overlayRect);

    //pause 
    self->m_popupPanel = UIObject_create("popup-panel");
    UIObject_setParent(self->m_popupPanel, self->m_overlayPanel);
    GamePlayPage_setPopupVisible(self, false);
    UIGridLayout* layout = UIGridLayout_create("popup-layout", 6, 1);
    UIObject_setParent(layout, self->m_popupPanel);

    rect.anchorMin = Vec2_set(0.0f, 0.0f);
    rect.anchorMax = Vec2_set(1.0f, 1.0f);
    rect.offsetMin = Vec2_set(0.0f, 0.0f);
    rect.offsetMax = Vec2_set(0.0f, 0.0f);
    UIObject_setRect(layout, rect);

    UIGridLayout_setRowSizes(layout, 35.0f);
    UIGridLayout_setRowSpacings(layout, 10.f);
    UIGridLayout_setRowSpacing(layout, 0, 30.f);

    // texte
    UILabel* label = UILabel_create("title-label", fontBig);
    UILabel_setTextString(label, "PAUSE");
    UILabel_setAnchor(label, Vec2_anchor_center);
    UILabel_setColor(label, g_colors.orange2);
    UIGridLayout_addObject(layout, label, 0, 0, 1, 1);

    // les different bouton
    const char* buttonNames[5] = { "btn-resume", "btn-editor", "btn-settings", "btn-controls", "btn-quit" };
    const char* buttonLabels[5] = { "Reprendre", "Mode Editeur", "Parametres", "Touches", "Quitter" };
    int buttonActions[5] = { PAGE_ACTION_RESUME, PAGE_ACTION_TOGGLE_EDITOR, PAGE_ACTION_OPEN_SETTINGS, PAGE_ACTION_OPEN_CONTROLS, PAGE_ACTION_QUIT };
    for (int i = 0; i < 5; ++i)
    {
        UIButton* button = UIButton_create(buttonNames[i], fontNormal);
        UIButton_setLabelString(button, buttonLabels[i]);
        UIButton_setOnClickCallback(button, GamePlayPage_onClick);
        UISelectable_setUserData(button, self);
        UISelectable_setUserId(button, buttonActions[i]);
        UIStyle_setDefaultButton(button);
        UIGridLayout_addObject(layout, button, i + 1, 0, 1, 1);
        UIFocusManager_addSelectable(self->m_focusManager, button);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///la popup de la victoire 
    self->m_victoryPanel = UIObject_create("victory-panel");
    UIObject_setParent(self->m_victoryPanel, self->m_overlayPanel);
    GamePlayPage_setVictoryVisible(self, false);

    UIGridLayout* victoryLayout = UIGridLayout_create("victory-layout", 3, 1);
    UIObject_setParent(victoryLayout, self->m_victoryPanel);

    rect.anchorMin = Vec2_set(0.0f, 0.0f);
    rect.anchorMax = Vec2_set(1.0f, 1.0f);
    rect.offsetMin = Vec2_set(0.0f, 0.0f);
    rect.offsetMax = Vec2_set(0.0f, 0.0f);
    UIObject_setRect(victoryLayout, rect);
    UIGridLayout_setRowSizes(victoryLayout, 40.0f);
    UIGridLayout_setRowSpacings(victoryLayout, 10.f); 
    UIGridLayout_setRowSpacing(victoryLayout, 0, 20.f);

    // victoire texte 
    UILabel* victoryLabel = UILabel_create("victory-label", fontBig);
    UILabel_setTextString(victoryLabel, "VICTOIRE !");
    UILabel_setAnchor(victoryLabel, Vec2_anchor_center);
    UILabel_setColor(victoryLabel, g_colors.orange2);
    UIGridLayout_addObject(victoryLayout, victoryLabel, 0, 0, 1, 1);

    //Bouton menu pour pourvoir relancé 
    UIButton* btnVictoryMenu = UIButton_create("btn-victory-menu", fontNormal);
    UIButton_setLabelString(btnVictoryMenu, "Menu");
    UIButton_setOnClickCallback(btnVictoryMenu, GamePlayPage_onClick);
    UISelectable_setUserData(btnVictoryMenu, self);
    UISelectable_setUserId(btnVictoryMenu, PAGE_ACTION_OPEN_MENU);
    UIStyle_setDefaultButton(btnVictoryMenu);

    UIGridLayout_addObject(victoryLayout, btnVictoryMenu, 1, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnVictoryMenu);
    //bouton quité 
    UIButton* btnVictoryQuit = UIButton_create("btn-victory-quit", fontNormal);
    UIButton_setLabelString(btnVictoryQuit, "Quitter");
    UIButton_setOnClickCallback(btnVictoryQuit, GamePlayPage_onClick);
    UISelectable_setUserData(btnVictoryQuit, self);
    UISelectable_setUserId(btnVictoryQuit, PAGE_ACTION_QUIT);
    UIStyle_setDefaultButton(btnVictoryQuit);

    UIGridLayout_addObject(victoryLayout, btnVictoryQuit, 2, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnVictoryQuit);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //les touche 
    self->m_controlsPanel = UIObject_create("controls-panel");
    UIObject_setParent(self->m_controlsPanel, self->m_overlayPanel);
    GamePlayPage_setControlsVisible(self, false);
    UIGridLayout* ctrlLayout = UIGridLayout_create("controls-layout", 7, 1);
    UIObject_setParent(ctrlLayout, self->m_controlsPanel);
    rect.anchorMin = Vec2_set(0.0f, 0.0f);
    rect.anchorMax = Vec2_set(1.0f, 1.0f);
    rect.offsetMin = Vec2_set(0.0f, 0.0f);
    rect.offsetMax = Vec2_set(0.0f, 0.0f);
    UIObject_setRect(ctrlLayout, rect);
    UIGridLayout_setRowSizes(ctrlLayout, 40.0f);
    UIGridLayout_setRowSpacings(ctrlLayout, 5.0f);
    UILabel* lblCtrlTitle = UILabel_create("lbl-ctrl-title", fontBig);
    UILabel_setTextString(lblCtrlTitle, "COMMANDES");//titre
    UILabel_setColor(lblCtrlTitle, g_colors.orange2);
    UILabel_setAnchor(lblCtrlTitle, Vec2_anchor_center);
    UIGridLayout_addObject(ctrlLayout, lblCtrlTitle, 0, 0, 1, 1);

    //texte explication touche 
    const char* helpTexts[5] = { "Clic Droit : Selectionner piece","Fleches : Deplacement","R : Rotation Horaire","T : Rotation Anti-horaire","Espace : Valider emplacement" };
    for (int i = 0; i < 5; ++i)
    {
        UILabel* lblHelp = UILabel_create("lbl-help", fontNormal);
        UILabel_setTextString(lblHelp, helpTexts[i]);
        UILabel_setColor(lblHelp, g_colors.white);
        UILabel_setAnchor(lblHelp, Vec2_anchor_center);
        UIGridLayout_addObject(ctrlLayout, lblHelp, i + 1, 0, 1, 1);
    }


    // editeur 
    self->m_editorPanel = UIObject_create("editor-panel");
    UIObject_setParent(self->m_editorPanel, canvas);
    UIGridLayout* editLayout = UIGridLayout_create("edit-layout", 10, 1);
    UIObject_setParent(editLayout, self->m_editorPanel);
    rect.anchorMin = Vec2_set(0.0f, 0.0f);
    rect.anchorMax = Vec2_set(1.1f, 0.9f);
    rect.offsetMin = Vec2_set(0.0f, 0.0f);
    rect.offsetMax = Vec2_set(0.0f, 0.0f);
    UIObject_setRect(editLayout, rect);
    UIGridLayout_setRowSizes(editLayout, 30.0f);

    const char* pNames[] = { "A (Hero)", "B", "C", "D", "E", "F", "G", "H", "Tombe" };
    for (int i = 0; i < 9; i++)
    {
        UIButton* btn = UIButton_create("btn-piece", fontNormal);
        UIButton_setLabelString(btn, pNames[i]);
        UIStyle_setDefaultButton(btn);
        UISelectable_setUserData(btn, self);
        UISelectable_setUserId(btn, PAGE_ACTION_SELECT_TYPE_A + i);
        UIButton_setOnClickCallback(btn, GamePlayPage_onClick);
        UIGridLayout_addObject(editLayout, btn, i, 0, 1, 1);
        UIFocusManager_addSelectable(self->m_focusManager, btn);
    }
    //bouton lancé 
    UIButton* btnPlay = UIButton_create("btn-edit-play", fontNormal);
    UIButton_setLabelString(btnPlay, "LANCER !");
    UIStyle_setDefaultButton(btnPlay);
    UISelectable_setUserData(btnPlay, self);
    UISelectable_setUserId(btnPlay, PAGE_ACTION_EDITOR_PLAY);
    UIButton_setOnClickCallback(btnPlay, GamePlayPage_onClick);
    UIGridLayout_addObject(editLayout, btnPlay, 9, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnPlay);

    GamePlayPage_setEditorVisible(self, false);

    //bouton retour 
    UIButton* btnCtrlBack = UIButton_create("btn-ctrl-back", fontNormal);
    UIButton_setLabelString(btnCtrlBack, "Retour");
    UIStyle_setDefaultButton(btnCtrlBack);
    UISelectable_setUserData(btnCtrlBack, self);
    UISelectable_setUserId(btnCtrlBack, PAGE_ACTION_BACK_FROM_MENU);
    UIButton_setOnClickCallback(btnCtrlBack, GamePlayPage_onClick);
    UIGridLayout_addObject(ctrlLayout, btnCtrlBack, 6, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnCtrlBack);


    ///paramettre 
    self->m_settingsPanel = UIObject_create("settings-panel");
    UIObject_setParent(self->m_settingsPanel, self->m_overlayPanel); // On le met dans l'overlay

    // on cache 
    GamePlayPage_setSettingsVisible(self, false);
    UIGridLayout* settingsLayout = UIGridLayout_create("settings-layout", 4, 1);
    UIObject_setParent(settingsLayout, self->m_settingsPanel);
    rect.anchorMin = Vec2_set(0.0f, 0.0f);
    rect.anchorMax = Vec2_set(1.0f, 1.0f);
    rect.offsetMin = Vec2_set(0.0f, 0.0f);
    rect.offsetMax = Vec2_set(0.0f, 0.0f);
    UIObject_setRect(settingsLayout, rect);
    UIGridLayout_setRowSizes(settingsLayout, 40.0f);
    UIGridLayout_setRowSpacings(settingsLayout, 15.f);
    UIGridLayout_setRowSpacing(settingsLayout, 0, 30.f); 

    // titre 
    UILabel* lblSettings = UILabel_create("lbl-settings-title", fontBig);
    UILabel_setTextString(lblSettings, "GENERATION");
    UILabel_setColor(lblSettings, g_colors.orange2);
    UILabel_setAnchor(lblSettings, Vec2_anchor_center);
    UIGridLayout_addObject(settingsLayout, lblSettings, 0, 0, 1, 1);

    // bouton 
    UIButton* btnModeManual = UIButton_create("btn-mode-manual", fontNormal);
    UIButton_setLabelString(btnModeManual, "Par Elements");
    UIButton_setOnClickCallback(btnModeManual, GamePlayPage_onClick);
    UISelectable_setUserData(btnModeManual, self);
    UISelectable_setUserId(btnModeManual, PAGE_ACTION_CHOOSE_MANUAL); 
    UIStyle_setDefaultButton(btnModeManual);
    UIGridLayout_addObject(settingsLayout, btnModeManual, 1, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnModeManual);

    //bouton
    UIButton* btnModeDiff = UIButton_create("btn-mode-diff", fontNormal);
    UIButton_setLabelString(btnModeDiff, "Par Difficulte");
    UIButton_setOnClickCallback(btnModeDiff, GamePlayPage_onClick);
    UISelectable_setUserData(btnModeDiff, self);
    UISelectable_setUserId(btnModeDiff, PAGE_ACTION_CHOOSE_DIFFICULTY); 
    UIStyle_setDefaultButton(btnModeDiff);
    UIGridLayout_addObject(settingsLayout, btnModeDiff, 2, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnModeDiff);

    // bouton retour
    UIButton* btnSetBack = UIButton_create("btn-settings-back", fontNormal);
    UIButton_setLabelString(btnSetBack, "Retour");
    UIButton_setOnClickCallback(btnSetBack, GamePlayPage_onClick);
    UISelectable_setUserData(btnSetBack, self);
    UISelectable_setUserId(btnSetBack, PAGE_ACTION_BACK_TO_PAUSE); 
    UIStyle_setDefaultButton(btnSetBack);
    UIGridLayout_addObject(settingsLayout, btnSetBack, 3, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnSetBack);


    //valeur par default 
    self->m_currentNbSkel = 3; 
    self->m_currentNbTomb = 1;


    //generation par nb element 
    self->m_manualGenPanel = UIObject_create("manual-gen-panel");
    UIObject_setParent(self->m_manualGenPanel, self->m_overlayPanel);

    rect.anchorMin = Vec2_set(0.5f, 0.5f);
    rect.anchorMax = Vec2_set(0.5f, 0.5f);
    rect.offsetMin = Vec2_set(10000.0f, 10000.0f); 
    rect.offsetMax = Vec2_set(10000.0f, 10000.0f);
    UIObject_setRect(self->m_manualGenPanel, rect);
    UIGridLayout* manLayout = UIGridLayout_create("man-layout", 6, 1);
    UIObject_setParent(manLayout, self->m_manualGenPanel);
    rect.offsetMin = Vec2_set(-200.0f, -200.0f);
    rect.offsetMax = Vec2_set(200.0f, 200.0f);
    UIGridLayout_setRowSizes(manLayout, 40.0f);
    UIGridLayout_setRowSpacings(manLayout, 10.f);

    // Titre
    UILabel* lblMan = UILabel_create("lbl-man-title", fontBig);
    UILabel_setTextString(lblMan, "MODE MANUEL");
    UILabel_setColor(lblMan, g_colors.orange2);
    UILabel_setAnchor(lblMan, Vec2_anchor_center);
    UIGridLayout_addObject(manLayout, lblMan, 0, 0, 1, 1);

    UIGridLayout* rowSkel = UIGridLayout_create("row-skel", 1, 3);
    UIGridLayout_setColumnSizes(rowSkel, 60.0f); 
    UIGridLayout_setColumnSpacings(rowSkel, 10.0f);
    UIButton* btnDecSkel = UIButton_create("btn-dec-skel", fontNormal);
    UIButton_setLabelString(btnDecSkel, "-");
    UIButton_setOnClickCallback(btnDecSkel, GamePlayPage_onClick);
    UISelectable_setUserData(btnDecSkel, self);
    UISelectable_setUserId(btnDecSkel, PAGE_ACTION_DEC_SKEL);
    UIStyle_setDefaultButton(btnDecSkel);
    UIGridLayout_addObject(rowSkel, btnDecSkel, 0, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnDecSkel);

    self->m_lblCountSkel = UILabel_create("lbl-count-skel", fontNormal);
    UILabel_setTextString(self->m_lblCountSkel, "Skel: 3"); 
    UILabel_setAnchor(self->m_lblCountSkel, Vec2_anchor_center);
    UIGridLayout_addObject(rowSkel, self->m_lblCountSkel, 0, 1, 1, 1);

    UIButton* btnIncSkel = UIButton_create("btn-inc-skel", fontNormal);
    UIButton_setLabelString(btnIncSkel, "+");
    UIButton_setOnClickCallback(btnIncSkel, GamePlayPage_onClick);
    UISelectable_setUserData(btnIncSkel, self);
    UISelectable_setUserId(btnIncSkel, PAGE_ACTION_INC_SKEL);
    UIStyle_setDefaultButton(btnIncSkel);
    UIGridLayout_addObject(rowSkel, btnIncSkel, 0, 2, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnIncSkel);

    UIGridLayout_addObject(manLayout, rowSkel, 1, 0, 1, 1); 
    UIGridLayout* rowTomb = UIGridLayout_create("row-tomb", 1, 3);
    UIGridLayout_setColumnSizes(rowTomb, 60.0f);
    UIGridLayout_setColumnSpacings(rowTomb, 10.0f);

    UIButton* btnDecTomb = UIButton_create("btn-dec-tomb", fontNormal);
    UIButton_setLabelString(btnDecTomb, "-");
    UIButton_setOnClickCallback(btnDecTomb, GamePlayPage_onClick);
    UISelectable_setUserData(btnDecTomb, self);
    UISelectable_setUserId(btnDecTomb, PAGE_ACTION_DEC_TOMB);
    UIStyle_setDefaultButton(btnDecTomb);
    UIGridLayout_addObject(rowTomb, btnDecTomb, 0, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnDecTomb);

    self->m_lblCountTomb = UILabel_create("lbl-count-tomb", fontNormal);
    UILabel_setTextString(self->m_lblCountTomb, "Tomb: 1");
    UILabel_setAnchor(self->m_lblCountTomb, Vec2_anchor_center);
    UIGridLayout_addObject(rowTomb, self->m_lblCountTomb, 0, 1, 1, 1);

    UIButton* btnIncTomb = UIButton_create("btn-inc-tomb", fontNormal);
    UIButton_setLabelString(btnIncTomb, "+");
    UIButton_setOnClickCallback(btnIncTomb, GamePlayPage_onClick);
    UISelectable_setUserData(btnIncTomb, self);
    UISelectable_setUserId(btnIncTomb, PAGE_ACTION_INC_TOMB);
    UIStyle_setDefaultButton(btnIncTomb);
    UIGridLayout_addObject(rowTomb, btnIncTomb, 0, 2, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnIncTomb);

    UIGridLayout_addObject(manLayout, rowTomb, 2, 0, 1, 1);

    // Bouton genere
    UIButton* btnGenMan = UIButton_create("btn-gen-man", fontNormal);
    UIButton_setLabelString(btnGenMan, "GENERER !");
    UIButton_setOnClickCallback(btnGenMan, GamePlayPage_onClick);
    UISelectable_setUserData(btnGenMan, self);
    UISelectable_setUserId(btnGenMan, PAGE_ACTION_GENERATE_MANUAL);
    UIStyle_setDefaultButton(btnGenMan);
    UIGridLayout_addObject(manLayout, btnGenMan, 3, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnGenMan);

    // Bouton retour
    UIButton* btnBackMan = UIButton_create("btn-back-man", fontNormal);
    UIButton_setLabelString(btnBackMan, "Retour");
    UIButton_setOnClickCallback(btnBackMan, GamePlayPage_onClick);
    UISelectable_setUserData(btnBackMan, self);
    UISelectable_setUserId(btnBackMan, PAGE_ACTION_OPEN_SETTINGS); 
    UIStyle_setDefaultButton(btnBackMan);
    UIGridLayout_addObject(manLayout, btnBackMan, 4, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnBackMan);


    //generation par niveau 
    self->m_diffGenPanel = UIObject_create("diff-gen-panel");
    UIObject_setParent(self->m_diffGenPanel, self->m_overlayPanel);

    UIObject_setRect(self->m_diffGenPanel, rect); 

    UIGridLayout* diffLayout = UIGridLayout_create("diff-layout", 5, 1);
    UIObject_setParent(diffLayout, self->m_diffGenPanel);
    UIGridLayout_setRowSizes(diffLayout, 40.0f);
    UIGridLayout_setRowSpacings(diffLayout, 15.f);

    UILabel* lblDiff = UILabel_create("lbl-diff-title", fontBig);
    UILabel_setTextString(lblDiff, "PAR DIFFICULTE");
    UILabel_setColor(lblDiff, g_colors.orange2);
    UILabel_setAnchor(lblDiff, Vec2_anchor_center);
    UIGridLayout_addObject(diffLayout, lblDiff, 0, 0, 1, 1);

    // Liste déroulante 
    const char* diffItems[] = { "Facile", "Moyen", "Difficile" };
    self->m_difficultyList = UIList_create(
        "list-difficulty", fontNormal, 3,
        UI_LIST_CONFIG_CYCLE | UI_LIST_CONFIG_AUTO_NAVIGATION
    );
    UIList_setLabelString(self->m_difficultyList, "Niveau :");
    for (int i = 0; i < 3; ++i)
    {
        UIList_setItemString(self->m_difficultyList, i, diffItems[i]);
    }
    UIStyle_setDefaultList(self->m_difficultyList, assets); 
    UISelectable_setUserData(self->m_difficultyList, self);
    UISelectable_setUserId(self->m_difficultyList, PAGE_ACTION_CHANGE_DIFF_LIST);
    UIGridLayout_addObject(diffLayout, self->m_difficultyList, 1, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, self->m_difficultyList);

    // Bouton gezere
    UIButton* btnGenDiff = UIButton_create("btn-gen-diff", fontNormal);
    UIButton_setLabelString(btnGenDiff, "GENERER !");
    UIButton_setOnClickCallback(btnGenDiff, GamePlayPage_onClick);
    UISelectable_setUserData(btnGenDiff, self);
    UISelectable_setUserId(btnGenDiff, PAGE_ACTION_GENERATE_DIFF);
    UIStyle_setDefaultButton(btnGenDiff);
    UIGridLayout_addObject(diffLayout, btnGenDiff, 2, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnGenDiff);

    // Bouton retour 
    UIButton* btnBackDiff = UIButton_create("btn-back-diff", fontNormal);
    UIButton_setLabelString(btnBackDiff, "Retour");
    UIButton_setOnClickCallback(btnBackDiff, GamePlayPage_onClick);
    UISelectable_setUserData(btnBackDiff, self);
    UISelectable_setUserId(btnBackDiff, PAGE_ACTION_OPEN_SETTINGS);
    UIStyle_setDefaultButton(btnBackDiff);
    UIGridLayout_addObject(diffLayout, btnBackDiff, 3, 0, 1, 1);
    UIFocusManager_addSelectable(self->m_focusManager, btnBackDiff);

    GamePlayPage_setSettingsVisible(self, false);
    GamePlayPage_setSubPanelVisible(self, self->m_manualGenPanel, false);
    GamePlayPage_setSubPanelVisible(self, self->m_diffGenPanel, false);
    GamePlayPage_setPopupVisible(self, false);
    GamePlayPage_setOverlayVisible(self, false);

    return self;
}

void GamePlayPage_destroy(GamePlayPage* self)
{
    if (!self) return;
    UIObject_destroy(self->m_hudPanel);
    UIObject_destroy(self->m_popupPanel);
    UIObject_destroy(self->m_victoryPanel);

    //On ne détruit que si ça existe
    if (self->m_settingsPanel) UIObject_destroy(self->m_settingsPanel);
    if (self->m_controlsPanel) UIObject_destroy(self->m_controlsPanel);
    if (self->m_editorPanel) UIObject_destroy(self->m_editorPanel);
    if (self->m_manualGenPanel) UIObject_destroy(self->m_manualGenPanel);
    if (self->m_diffGenPanel) UIObject_destroy(self->m_diffGenPanel);

    UIFocusManager_destroy(self->m_focusManager);
    free(self);
}

void GamePlayPage_update(GamePlayPage* self, UIInput* input)
{
    char buffer[64];

    if (!self->m_isVictory && GameCore_isWon(&g_gameConfig.core))
    {
        self->m_isVictory = true;
        GamePlayPage_setOverlayVisible(self, true);
        GamePlayPage_setVictoryVisible(self, true);
        GamePlayPage_setHudVisible(self, false);
    }
    static Object lastMapState[3][4];
    GameCore* core = &g_gameConfig.core;

    //on a fais un mouvement ?
    bool hasChanged = (memcmp(core->map, lastMapState, sizeof(lastMapState)) != 0);

    static int frameCounter = 0;
    frameCounter++;
    bool isEditor = self->m_scene->m_gameGraphics->m_isEditorMode;
    // Si ça a change
    if (hasChanged && frameCounter > 10 && !self->m_isPaused && !self->m_isVictory && !isEditor)
    {
        // on sauvegarde pour le prochain
        memcpy(lastMapState, core->map, sizeof(lastMapState));
        frameCounter = 0;

        int coups = GameSolver_solve(core, true);

        char moveBuffer[32]; // autre petit buffer pour les coups
        if (coups >= 0) sprintf(moveBuffer, "Coups restants: %d", coups);
        else sprintf(moveBuffer, "Impossible");

        UILabel_setTextString(self->m_lblMoves, moveBuffer);
    }

    UIInput localInput = *input;

    if (!self->m_isPaused && !self->m_isVictory)
    {
        localInput.validatePressed = false;
        localInput.upPressed = false;
        localInput.downPressed = false;
        localInput.leftPressed = false;
        localInput.rightPressed = false;
    }
    UIFocusManager_update(self->m_focusManager, &localInput);

    // Detection du clic sur bouton de piece
    if (self->m_nextAction >= PAGE_ACTION_SELECT_TYPE_A && self->m_nextAction <= PAGE_ACTION_SELECT_TYPE_A + 9)
    {
        // On informe GameGraphics de la pièce choisie
        int type = self->m_nextAction - PAGE_ACTION_SELECT_TYPE_A;
        self->m_scene->m_gameGraphics->m_editorSelectedType = type;
        self->m_scene->m_gameGraphics->m_editorRotation = 0;
        self->m_nextAction = PAGE_ACTION_NONE;
    }

    //les action
    switch (self->m_nextAction)
    {
    case PAGE_ACTION_OPEN_MENU:
        self->m_isPaused = true;
        self->m_isSettingsOpen = false;

        GamePlayPage_setOverlayVisible(self, true);
        GamePlayPage_setHudVisible(self, false);
        GamePlayPage_setPopupVisible(self, true);
        GamePlayPage_setSettingsVisible(self, false);
        GamePlayPage_setVictoryVisible(self, false);
        GameGraphics_setEnabled(self->m_scene->m_gameGraphics, false);
        break;

    case PAGE_ACTION_OPEN_CONTROLS:
        // on cache le menu et on montre les touche 
        GamePlayPage_setPopupVisible(self, false);
        GamePlayPage_setControlsVisible(self, true);
        break;

    case PAGE_ACTION_BACK_FROM_MENU:
        // on cache le touche et on revien au menu 
        GamePlayPage_setControlsVisible(self, false);
        GamePlayPage_setPopupVisible(self, true);
        break;

    case PAGE_ACTION_TOGGLE_EDITOR:
        // ferme menu pause 
        GamePlayPage_setPopupVisible(self, false);
        GamePlayPage_setOverlayVisible(self, false);
        self->m_isPaused = false;

        // affice le mode editeur et active de la gam graphique 
        GamePlayPage_setEditorVisible(self, true);
        self->m_scene->m_gameGraphics->m_isEditorMode = true;
        self->m_scene->m_gameGraphics->m_editorSelectedType = 0;
        GameGraphics_setEnabled(self->m_scene->m_gameGraphics, true);
        break;

    case PAGE_ACTION_EDITOR_PLAY:
    {
        GameCore* core = &g_gameConfig.core; 
        core->nbObject = 0;
        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < 4; x++)
            {
               
                if (core->map[y][x].id != -1)
                {
                    core->map[y][x].id = core->nbObject;
                    core->nbObject++;
                }
            }
        }
        // on verifi qu il y a le hero 
        bool hasHero = false;
        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (g_gameConfig.core.map[y][x].id != -1 &&
                    g_gameConfig.core.map[y][x].type == 0)
                {
                    hasHero = true;
                }
            }
        }
        if (!hasHero)
        {
            printf("Placez le Heros (Piece A) avant de lancer !\n");
            self->m_nextAction = PAGE_ACTION_NONE; // On annule le lancement
            break;
        }
        // c'est possible a resoudre ?
        int moves = GameSolver_solve(&g_gameConfig.core, false);
        if (moves == -1)
        {
            printf("pas soluble!\n");
            self->m_nextAction = PAGE_ACTION_NONE;
            break;
        }
        printf("Solution en %d coups.\n", moves);

        self->m_isPaused = false;
        self->m_isVictory = false;
        GamePlayPage_setEditorVisible(self, false);
        self->m_scene->m_gameGraphics->m_isEditorMode = false;
        if (self->m_scene->m_gameGraphics->m_ghostObject)
        {
            free(self->m_scene->m_gameGraphics->m_ghostObject);
            self->m_scene->m_gameGraphics->m_ghostObject = NULL;
        }
        GamePlayPage_setHudVisible(self, true);
        GameGraphics_setEnabled(self->m_scene->m_gameGraphics, true);
    }
    break;

    case PAGE_ACTION_RESUME:
        if(self->m_isVictory)
        {
            GamePlayPage_setPopupVisible(self, false);
            GamePlayPage_setVictoryVisible(self, true);
        }
    else
    {
        self->m_isPaused = false;
        self->m_isSettingsOpen = false;

        GamePlayPage_setOverlayVisible(self, false);
        GamePlayPage_setPopupVisible(self, false);
        GamePlayPage_setSettingsVisible(self, false);
        GamePlayPage_setControlsVisible(self, false);
        GamePlayPage_setEditorVisible(self, false);

        self->m_scene->m_gameGraphics->m_isEditorMode = false;
        if (self->m_scene->m_gameGraphics->m_ghostObject)
        {
            free(self->m_scene->m_gameGraphics->m_ghostObject);
            self->m_scene->m_gameGraphics->m_ghostObject = NULL;
        }
        GamePlayPage_setHudVisible(self, true);
        GameGraphics_setEnabled(self->m_scene->m_gameGraphics, true);
    }
        break;

    case PAGE_ACTION_SOLVER:
        GameSolver_playNextMove(&g_gameConfig.core);
        break;


    case PAGE_ACTION_QUIT:
        Scene_quit(self->m_scene);
        break;

    case PAGE_ACTION_OPEN_SETTINGS:
        //affiche le menu de choix
        GamePlayPage_setSettingsVisible(self, true);
        GamePlayPage_setSubPanelVisible(self, self->m_manualGenPanel, false);
        GamePlayPage_setSubPanelVisible(self, self->m_diffGenPanel, false);
        GamePlayPage_setPopupVisible(self, false);
        break;

    case PAGE_ACTION_BACK_TO_PAUSE:
        GamePlayPage_setSettingsVisible(self, false);
        GamePlayPage_setPopupVisible(self, true);
        break;

    case PAGE_ACTION_CHOOSE_MANUAL:
        GamePlayPage_setSettingsVisible(self, false);
        GamePlayPage_setSubPanelVisible(self, self->m_manualGenPanel, true);
        break;

    case PAGE_ACTION_CHOOSE_DIFFICULTY:
        GamePlayPage_setSettingsVisible(self, false);
        GamePlayPage_setSubPanelVisible(self, self->m_diffGenPanel, true);
        break;

    //on gere ici le nombre de squellette / tomb
    case PAGE_ACTION_INC_SKEL:
        if (self->m_currentNbSkel < 7) self->m_currentNbSkel++;
        sprintf(buffer, "Skel: %d", self->m_currentNbSkel);
        UILabel_setTextString(self->m_lblCountSkel, buffer);
        break;

    case PAGE_ACTION_DEC_SKEL:
        if (self->m_currentNbSkel > 1) self->m_currentNbSkel--; 
        sprintf(buffer, "Skel: %d", self->m_currentNbSkel);
        UILabel_setTextString(self->m_lblCountSkel, buffer);
        break;

    case PAGE_ACTION_INC_TOMB:
        if (self->m_currentNbTomb < 3) self->m_currentNbTomb++;
        sprintf(buffer, "Tomb: %d", self->m_currentNbTomb);
        UILabel_setTextString(self->m_lblCountTomb, buffer);
        break;

    case PAGE_ACTION_DEC_TOMB:
        if (self->m_currentNbTomb > 0) self->m_currentNbTomb--;
        sprintf(buffer, "Tomb: %d", self->m_currentNbTomb);
        UILabel_setTextString(self->m_lblCountTomb, buffer);
        break;


    case PAGE_ACTION_GENERATE_MANUAL:
    {
        self->m_isVictory = false;
        bool hasSuccess;
        do
        {
            GameLevels_loadRandomLevel(self->m_currentNbSkel, self->m_currentNbTomb, &g_gameConfig.core);
            hasSuccess = GameLevels_mapIsCorrect(g_gameConfig.core);
        } while (!hasSuccess);
        GameHashmap* map = GameLevels_findLevel(&g_gameConfig.core);
        g_gameConfig.core = GameLevels_findCoreOnMap(map, 0, 200, &hasSuccess);
        HashMap_destroy(map);
        map = NULL;
        self->m_isPaused = false;
        GamePlayPage_setSubPanelVisible(self, self->m_manualGenPanel, false);
        GamePlayPage_setOverlayVisible(self, false);
        GamePlayPage_setHudVisible(self, true);

        // mise a jour graphique 
        GameGraphics_setEnabled(self->m_scene->m_gameGraphics, true);
        UILabel_setTextString(self->m_lblMoves, "Coups: Calcul...");
    }
    break;

    case PAGE_ACTION_GENERATE_DIFF:
    {
        self->m_isVictory = false;
        int selection = UIList_getSelectedItem(self->m_difficultyList);
        Difficulty diff = EASY;
        if (selection == 1) diff = NORMAL;
        if (selection == 2) diff = HARD;
        g_gameConfig.core = GameLevels_loadGameByDifficulty(diff);

        self->m_isPaused = false;
        GamePlayPage_setSubPanelVisible(self, self->m_diffGenPanel, false);
        GamePlayPage_setOverlayVisible(self, false);
        GamePlayPage_setHudVisible(self, true);
        GameGraphics_setEnabled(self->m_scene->m_gameGraphics, true);
        UILabel_setTextString(self->m_lblMoves, "Coups: Calcul...");
    }
    break;

    default:
        break;
    }
    self->m_nextAction = PAGE_ACTION_NONE;
}
