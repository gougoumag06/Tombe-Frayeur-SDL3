/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/game_graphics.h"
#include "common/assets.h"
#include "common/common.h"
#include "game/input.h"
#include "game/scene.h"
#include "game/game_config.h"

GameGraphics* GameGraphics_create(Scene* scene)
{
    GameGraphics* self = (GameGraphics*)calloc(1, sizeof(GameGraphics));
    AssertNew(self);

    self->m_scene = scene;
    self->m_padding = Vec2_set(0.0f, 0.0f);
    self->m_spacing = Vec2_set(0.1f, 0.1f);
    self->m_gridAABB.lower = Vec2_add(Vec2_set(-5.35f, -4.0f), Vec2_set(8.0f, 4.5f));
    self->m_gridAABB.upper = Vec2_add(Vec2_set(+5.35f, +4.0f), Vec2_set(8.0f, 4.5f));
    self->m_enabled = false;

    AssetManager* assets = Scene_getAssetManager(scene);

    // on charge les sprite des differente piece
    SpriteSheet* spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_GAME);
    AssertNew(spriteSheet);
    self->m_spritePieceA = SpriteSheet_getGroupByName(spriteSheet, "piece_A");
    self->m_spritePieceB = SpriteSheet_getGroupByName(spriteSheet, "piece_B");
    self->m_spritePieceC = SpriteSheet_getGroupByName(spriteSheet, "piece_C");
    self->m_spritePieceD = SpriteSheet_getGroupByName(spriteSheet, "piece_D");
    self->m_spritePieceE = SpriteSheet_getGroupByName(spriteSheet, "piece_E");
    self->m_spritePieceF = SpriteSheet_getGroupByName(spriteSheet, "piece_F");
    self->m_spritePieceG = SpriteSheet_getGroupByName(spriteSheet, "piece_G");
    self->m_spritePieceH = SpriteSheet_getGroupByName(spriteSheet, "piece_H");
    self->m_spritePieceTB = SpriteSheet_getGroupByName(spriteSheet, "block_striped");

    // on charge les sprite des differente piece en fantome 
    SpriteSheet* spriteSheetGhost = AssetManager_getSpriteSheet(assets, SPRITE_GAME_GHOST);
    AssertNew(spriteSheetGhost);
    self->m_spritePieceAGhost = SpriteSheet_getGroupByName(spriteSheetGhost, "piece_A_Ghost");
    self->m_spritePieceBGhost = SpriteSheet_getGroupByName(spriteSheetGhost, "piece_B_Ghost");
    self->m_spritePieceCGhost = SpriteSheet_getGroupByName(spriteSheetGhost, "piece_C_Ghost");
    self->m_spritePieceDGhost = SpriteSheet_getGroupByName(spriteSheetGhost, "piece_D_Ghost");
    self->m_spritePieceEGhost = SpriteSheet_getGroupByName(spriteSheetGhost, "piece_E_Ghost");
    self->m_spritePieceFGhost = SpriteSheet_getGroupByName(spriteSheetGhost, "piece_F_Ghost");
    self->m_spritePieceGGhost = SpriteSheet_getGroupByName(spriteSheetGhost, "piece_G_Ghost");
    self->m_spritePieceHGhost = SpriteSheet_getGroupByName(spriteSheetGhost, "piece_H_Ghost");
    self->m_spritePieceTBGhost = SpriteSheet_getGroupByName(spriteSheetGhost, "block_striped_Ghost");

    self->m_selectedRowIndex = 0;
    self->m_selectedColIndex = 0;
    self->m_isVictory = false;


    self->m_isEditorMode = false;
    self->m_editorSelectedType = 0;
    self->m_editorRotation = 0;


    return self;
}

static void GameGraphics_destroyGhostObject(GameGraphics* self)
{
    if (self->m_ghostObject)
    {
        free(self->m_ghostObject);
        self->m_ghostObject = NULL;
    }
    self->m_originObject = NULL;
}


void GameGraphics_destroy(GameGraphics* self)
{
    if (!self) return;
    GameGraphics_destroyGhostObject(self);
    free(self);
}

static void GameGraphics_updateCells(GameGraphics* self)
{
    float totalPaddingX = self->m_padding.x * 2.f;
    float totalPaddingY = self->m_padding.y * 2.f;
    float totalSpacingX = self->m_spacing.x * (GAME_GRID_SIZE_W - 1);
    float totalSpacingY = self->m_spacing.y * (GAME_GRID_SIZE_H - 1);
    Vec2 gridSize = AABB_getSize(&(self->m_gridAABB));
    float cellW = (gridSize.x - totalPaddingX - totalSpacingX) / GAME_GRID_SIZE_W;
    float cellH = (gridSize.y - totalPaddingY - totalSpacingY) / GAME_GRID_SIZE_H;

    for (int i = 0; i < GAME_GRID_SIZE_H; i++)
    {
        for (int j = 0; j < GAME_GRID_SIZE_W; j++)
        {
            float cellX = self->m_gridAABB.lower.x + self->m_padding.x + j * (cellW + self->m_spacing.x);
            float cellY = self->m_gridAABB.lower.y + self->m_padding.y + i * (cellH + self->m_spacing.y);
            self->m_cells[GAME_GRID_SIZE_H - 1 - i][j].lower = Vec2_set(cellX, cellY);
            self->m_cells[GAME_GRID_SIZE_H - 1 - i][j].upper = Vec2_set(cellX + cellW, cellY + cellH);
        }
    }
}

static Object* GameGraphics_createGhostObject(Object* src)
{
    if (!src || src->id == -1) return NULL;

    Object* ghost = (Object*)malloc(sizeof(Object));
    AssertNew(ghost);

    // Copie des valeur 
    ghost->id = src->id;
    ghost->pos_x = src->pos_x;
    ghost->pos_y = src->pos_y;
    ghost->type = src->type;
    ghost->rotationIndex = src->rotationIndex;
    ghost->canMove = src->canMove;
    ghost->canExit = src->canExit;

    return ghost;
}


//fonction pour gere l edition de map 
static void GameGraphics_updateEditor(GameGraphics* self)
{
    Scene* scene = self->m_scene;
    Input* input = Scene_getInput(scene);
    PlayerInput* pInput = &(input->players[0]);
    Camera* camera = Scene_getCamera(scene);
    GameCore* core = &(g_gameConfig.core);

    Vec2 mouseWorldPos = { 0 };
    Camera_viewToWorld(camera, input->mouse.position.x, input->mouse.position.y, &mouseWorldPos);

    //on cherche la case sous la souris 
    int r = -1;
    int c = -1;
    for (int i = 0; i < GAME_GRID_SIZE_H; i++)
    {
        for (int j = 0; j < GAME_GRID_SIZE_W; j++)
        {
            if (AABB_containsPoint(&(self->m_cells[i][j]), mouseWorldPos))
            {
                r = i; c = j;
            }
        }
    }
    if (pInput->rotatePressed)
    {
        self->m_editorRotation = (self->m_editorRotation + 1) % 4;
        pInput->rotatePressed = false;
    }
    if (pInput->rotateAntiPressed)
    {
        self->m_editorRotation = (self->m_editorRotation + 3) % 4;
        pInput->rotateAntiPressed = false;
    }
    if (self->m_ghostObject)
    {
        free(self->m_ghostObject);
        self->m_ghostObject = NULL;
    }
    self->m_originObject = NULL; // Pas besoin 

    if (r != -1 && c != -1) // si la souris es sur la grille 
    {
        //creation d un fantome 
        self->m_ghostObject = (Object*)calloc(1, sizeof(Object));
        if (self->m_ghostObject)
        {
            self->m_ghostObject->id = -3;
            self->m_ghostObject->pos_x = r;
            self->m_ghostObject->pos_y = c;
            self->m_ghostObject->type = self->m_editorSelectedType;
            self->m_ghostObject->rotationIndex = self->m_editorRotation;
        }

        //on pose avec clique gauche 
        if (input->mouse.leftPressed)
        {
            int nbSkel = 0;
            int nbTomb = 0;
            int nbHero = 0; // Ajout pour le héros

            // On compte le nombre de squellette et de tomb sur le plateau et de hero 
            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (y == r && x == c) continue;

                    int t = core->map[y][x].type;
                    if (core->map[y][x].id != -1)
                    {
                        if (t == 8) nbTomb++;
                        else if (t >= 0 && t <= 7)
                        {
                            nbSkel++;
                            if (t == 0) nbHero++; 
                        }
                    }
                }
            }

            bool canAdd = true;

            // ajout tomb possible ?
            if (self->m_editorSelectedType == 8)
            {
                if (nbTomb >= 3)
                {
                    printf("Max 3 Tombes atteintes !\n");
                    canAdd = false;
                }
            }
            // ajout Hero possible ? 
            else if (self->m_editorSelectedType == 0)
            {
                if (nbHero >= 1)
                {
                    printf("Il ne peut y avoir qu'UN SEUL Heros !\n");
                    canAdd = false;
                }
            }
            // ajout autre squellet possible ? 
            else if (self->m_editorSelectedType >= 1 && self->m_editorSelectedType <= 7)
            {
                if (nbSkel >= 7)
                {
                    printf("Max 7 Squelettes atteints !\n");
                    canAdd = false;
                }
            }

            if (canAdd)
            {
                //on verifit les condition de pose 
                if (GameCore_verifBorders(r, c, core, self->m_editorSelectedType, self->m_editorRotation))
                {
                    core->map[r][c].id = -1; //on enleve se qu il y a sur la case actuelment
                    //et on cree la piece que on veut 
                    GameCore_createObject(&(core->map[r][c]), 0, r, c, self->m_editorSelectedType, self->m_editorRotation);

                }
            }
        }
        //clique droit on efface 
        else if (input->mouse.rightPressed)
        {
            core->map[r][c].id = -1;
        }
    }
}

void GameGraphics_update(GameGraphics* self)
{
    assert(self && "self must not be NULL");
    Scene* scene = self->m_scene;
    Input* input = Scene_getInput(scene);
    PlayerInput* pInput = &(input->players[0]);
    Camera* camera = Scene_getCamera(scene);
    GameCore* core = &(g_gameConfig.core);

    GameGraphics_updateCells(self);

    // on fait rien si le jeu est en pause /victoire 
    if (!self->m_enabled || self->m_isVictory) return;
    //si on est en mode editor on skip cette fonction et on va dans celle de l editor
    if (self->m_isEditorMode)
    {
        GameGraphics_updateEditor(self);
        return;
    }
    Vec2 mouseWorldPos = { 0 };
    Camera_viewToWorld(camera, input->mouse.position.x, input->mouse.position.y, &mouseWorldPos);

    //selection de l'object avec le  clique  droit 
    if (input->mouse.rightPressed)
    {
        for (int i = 0; i < GAME_GRID_SIZE_H; i++)
        {
            for (int j = 0; j < GAME_GRID_SIZE_W; j++)
            {
                AABB* cellAABB = &(self->m_cells[i][j]);
                if (AABB_containsPoint(cellAABB, mouseWorldPos))
                {
                    //on detruit l ancien fantome qui etait cree 
                    GameGraphics_destroyGhostObject(self);

                    //on regarde si il y a un object dedans
                    if (core->map[i][j].id != -1)
                    {
                        Object* obj = &core->map[i][j];

                        // on ne selectionne pas les tombe car on peut pas les bougé 
                        if (obj->canMove && obj->type != 8)
                        {
                            self->m_originObject = obj;//////on retien la position actuel de la piece 
                            self->m_ghostObject = GameGraphics_createGhostObject(obj);////creation d'un Ghost avec le bon type et avec les meme info que l obj original

                            self->m_selectedRowIndex = i;
                            self->m_selectedColIndex = j;
                        }
                    }
                }
            }
        }
    }
    /////:les deplacement du fantome 
    char touche = 0;
    if (pInput->upPressed)         touche = 'z';
    if (pInput->downPressed)       touche = 's';
    if (pInput->leftPressed)       touche = 'q';
    if (pInput->rightPressed)      touche = 'd';
    if (pInput->rotatePressed)     touche = 'r';
    if (pInput->rotateAntiPressed) touche = 't';

    if (touche != 0 && self->m_ghostObject != NULL && self->m_originObject != NULL)
    {
        Object* ghost = self->m_ghostObject;
        Object* origin = self->m_originObject;

        // Sauvegarde de l'état
        int oldX = ghost->pos_x;
        int oldY = ghost->pos_y;
        int oldRot = ghost->rotationIndex;
        bool hasChanged = false;

        //on enleve l ancien pour evité les colision 
        int savedID = origin->id;
        origin->id = -1;

        switch (touche)
        {
        case 'z': ghost->pos_x--; hasChanged = true; break;
        case 's': ghost->pos_x++; hasChanged = true; break;
        case 'q': ghost->pos_y--; hasChanged = true; break;
        case 'd': ghost->pos_y++; hasChanged = true; break;
        case 'r': GameCore_rotateObjectCW(ghost); break;
        case 't': GameCore_rotateObjectCCW(ghost); break;
        }
        if (hasChanged)
        {
            if (!GameCore_valide(ghost->pos_x, ghost->pos_y))
            {
                //dans la grille ?
                ghost->pos_x = oldX;
                ghost->pos_y = oldY;
                ghost->rotationIndex = oldRot;
            }
            else
            {
                // la case est vide ?
                bool targetIsFree = (core->map[ghost->pos_x][ghost->pos_y].id == -1);

                if (!targetIsFree)
                {
                    ghost->pos_x = oldX;
                    ghost->pos_y = oldY;
                    ghost->rotationIndex = oldRot;
                }
                else
                {
                    //distance correcte ?
                    bool distanceIsCorrect = ((abs(ghost->pos_x - origin->pos_x) + abs(ghost->pos_y - origin->pos_y)) <= 1);

                    if (!distanceIsCorrect)
                    {
                        ghost->pos_x = oldX;
                        ghost->pos_y = oldY;
                        ghost->rotationIndex = oldRot;
                    }
                    else
                    {
                        self->m_selectedRowIndex = ghost->pos_x;
                        self->m_selectedColIndex = ghost->pos_y;
                    }
                }
            }
        }

        //on remet l original 
        origin->id = savedID;
    }
    //validation 
    if (pInput->validatePressed && self->m_ghostObject != NULL)
    {
        Object* ghost = self->m_ghostObject;
        Object* origin = self->m_originObject;

        //on reenlve l original 
        int savedID = origin->id;
        origin->id = -1;

        //on reverifie tout les condition 
        bool posValide = GameCore_valide(ghost->pos_x, ghost->pos_y);
        bool caseLibre = (core->map[ghost->pos_x][ghost->pos_y].id == -1);
        bool formeValide = GameCore_verifBorders(ghost->pos_x, ghost->pos_y, core, ghost->type, ghost->rotationIndex);
        bool distanceIsCorrect = ((abs(ghost->pos_x - origin->pos_x) + abs(ghost->pos_y - origin->pos_y)) <= 1);

        if (posValide && caseLibre && formeValide && distanceIsCorrect)///verification des differente condition 
        {
            origin->id = savedID;//on remet l original 


            if (origin->pos_x != ghost->pos_x || origin->pos_y != ghost->pos_y)
            {
                //on deplace l original 
                core->map[ghost->pos_x][ghost->pos_y] = *origin;
                core->map[ghost->pos_x][ghost->pos_y].pos_x = ghost->pos_x;
                core->map[ghost->pos_x][ghost->pos_y].pos_y = ghost->pos_y;

                //on met rien a l ancien case 
                origin->id = -1;
                origin->type = -1;
            }

            // application de la rotation 
            core->map[ghost->pos_x][ghost->pos_y].rotationIndex = ghost->rotationIndex;


            GameGraphics_destroyGhostObject(self);//destruction de l object 
        }
        else
        {
            origin->id = savedID; // sa fonctionne pas on remet l original 

        }
    }
}

void GameGraphics_render(GameGraphics* self)
{
    assert(self && "self must not be NULL");

    Scene* scene = self->m_scene;
    Camera* camera = Scene_getCamera(scene);
    float scale = Camera_getWorldToViewScale(camera);
    GameCore* core = &(g_gameConfig.core);

    SDL_FRect rect = { 0 };
    for (int i = 0; i < GAME_GRID_SIZE_H; i++)
    {
        for (int j = 0; j < GAME_GRID_SIZE_W; j++)
        {
            AABB* cellAABB = &(self->m_cells[i][j]);
            rect.x = Camera_worldToViewX(camera, cellAABB->lower.x);
            rect.y = Camera_worldToViewY(camera, cellAABB->upper.y);
            rect.w = (cellAABB->upper.x - cellAABB->lower.x) * scale;
            rect.h = (cellAABB->upper.y - cellAABB->lower.y) * scale;

            bool isHovered = (i == self->m_selectedRowIndex && j == self->m_selectedColIndex);
            SDL_Color color = isHovered ? g_colors.orange9 : g_colors.gray8;
            SDL_SetRenderDrawColor(g_renderer, color.r, color.g, color.b, 255);
            SDL_RenderFillRect(g_renderer, &rect);

            SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
            SDL_RenderRect(g_renderer, &rect);

            if (core->map[i][j].id != -1)
            {
                //si fantome on fait disparaitre l original 
                bool isHiddenOrigin = (self->m_ghostObject &&
                    self->m_originObject &&
                    self->m_originObject->pos_x == i &&
                    self->m_originObject->pos_y == j);

                if (!isHiddenOrigin)
                {
                    Object* obj = &core->map[i][j];
                    SpriteGroup* spriteDraw = NULL;

                    switch (obj->type)
                    {
                    case 0: spriteDraw = self->m_spritePieceA; break;
                    case 1: spriteDraw = self->m_spritePieceB; break;
                    case 2: spriteDraw = self->m_spritePieceC; break;
                    case 3: spriteDraw = self->m_spritePieceD; break;
                    case 4: spriteDraw = self->m_spritePieceE; break;
                    case 5: spriteDraw = self->m_spritePieceF; break;
                    case 6: spriteDraw = self->m_spritePieceG; break;
                    case 7: spriteDraw = self->m_spritePieceH; break;
                    case 8: spriteDraw = self->m_spritePieceTB; break;
                    }

                    if (spriteDraw)
                    {
                        double angle = obj->rotationIndex * 90.0;
                        SDL_FRect centeredRect = rect;
                        centeredRect.x += rect.w / 2.0f;
                        centeredRect.y += rect.h / 2.0f;
                        SpriteGroup_renderRotated(spriteDraw, 0, &centeredRect, Vec2_anchor_center, angle, NULL, SDL_FLIP_NONE);
                    }
                }
            }
        }
    }

    //le rendu du ghost 
    if (self->m_ghostObject != NULL)
    {
        Object* obj = self->m_ghostObject;
        SpriteGroup* spriteDraw = NULL;

        switch (obj->type)
        {
        case 0: spriteDraw = self->m_spritePieceAGhost; break;
        case 1: spriteDraw = self->m_spritePieceBGhost; break;
        case 2: spriteDraw = self->m_spritePieceCGhost; break;
        case 3: spriteDraw = self->m_spritePieceDGhost; break;
        case 4: spriteDraw = self->m_spritePieceEGhost; break;
        case 5: spriteDraw = self->m_spritePieceFGhost; break;
        case 6: spriteDraw = self->m_spritePieceGGhost; break;
        case 7: spriteDraw = self->m_spritePieceHGhost; break;
        case 8: spriteDraw = self->m_spritePieceTBGhost; break;
        }

        if (spriteDraw)
        {
            int r = obj->pos_x;
            int c = obj->pos_y;

            if (r >= 0 && r < GAME_GRID_SIZE_H && c >= 0 && c < GAME_GRID_SIZE_W)
            {
                AABB* cellAABB = &(self->m_cells[r][c]);
                SDL_FRect rect;
                rect.x = Camera_worldToViewX(camera, cellAABB->lower.x);
                rect.y = Camera_worldToViewY(camera, cellAABB->upper.y);
                rect.w = (cellAABB->upper.x - cellAABB->lower.x) * scale;
                rect.h = (cellAABB->upper.y - cellAABB->lower.y) * scale;

                double angle = obj->rotationIndex * 90.0;
                SDL_FRect centeredRect = rect;
                centeredRect.x += rect.w / 2.0f;
                centeredRect.y += rect.h / 2.0f;

                SpriteGroup_renderRotated(spriteDraw, 0, &centeredRect, Vec2_anchor_center, angle, NULL, SDL_FLIP_NONE);
            }
        }
    }
}
