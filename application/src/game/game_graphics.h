/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"
#include "game/game_config.h"
#include "game/core/game_core.h"

typedef struct Scene Scene;

#define GAME_GRID_SIZE_W 4
#define GAME_GRID_SIZE_H 3

/// @brief Structure représentant la scène du menu principal du jeu.
typedef struct GameGraphics
{
    Scene* m_scene;
    AABB m_gridAABB;
    Vec2 m_padding;
    Vec2 m_spacing;

    AABB m_cells[GAME_GRID_SIZE_H][GAME_GRID_SIZE_W];

    Object* m_ghostObject;
    Object* m_originObject;


    bool m_isEditorMode;
    int m_editorSelectedType;
    int m_editorRotation;


    int m_selectedRowIndex;
    int m_selectedColIndex;


    SpriteGroup* m_spritePieceA;
    SpriteGroup* m_spritePieceB;
    SpriteGroup* m_spritePieceC;
    SpriteGroup* m_spritePieceD;
    SpriteGroup* m_spritePieceE;
    SpriteGroup* m_spritePieceF;
    SpriteGroup* m_spritePieceG;
    SpriteGroup* m_spritePieceH;
    SpriteGroup* m_spritePieceTB;
    ///Sprite pour les Ghost
    SpriteGroup* m_spritePieceAGhost;
    SpriteGroup* m_spritePieceBGhost;
    SpriteGroup* m_spritePieceCGhost;
    SpriteGroup* m_spritePieceDGhost;
    SpriteGroup* m_spritePieceEGhost;
    SpriteGroup* m_spritePieceFGhost;
    SpriteGroup* m_spritePieceGGhost;
    SpriteGroup* m_spritePieceHGhost;
    SpriteGroup* m_spritePieceTBGhost;


    bool m_isVictory;
    bool m_enabled;

} GameGraphics;


GameGraphics* GameGraphics_create(Scene* scene);
void GameGraphics_destroy(GameGraphics* self);



void GameGraphics_update(GameGraphics* self);
void GameGraphics_render(GameGraphics* self);



INLINE void GameGraphics_setEnabled(GameGraphics* self, bool enabled)
{
    assert(self && "self must not be NULL");
    self->m_enabled = enabled;
}
