/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/
#include "hash_map.h"

static size_t hash(GameCore core, size_t capacity)
{
    uint64_t hash = 0;
    for (int i = 0; i < 8; i++)
    {
        if (core.forHash[i] != -1)
        {
            hash ^= (uint64_t)(core.forHash[i]);
            hash = hash * 0xbf58476d1ce4e5b9ULL + 0x9e3779b97f4a7c15ULL;
        }
    }
    return (size_t)hash % capacity;
}

GameHashmap* HashMap_create(int capacity)
{
    GameHashmap* self = (GameHashmap*)calloc(1, sizeof(GameHashmap));
    AssertNew(self);

    self->m_capacity = capacity;
    self->m_size = 0;
    self->m_entries = (GameHashmapEntry*)malloc(capacity * sizeof(GameHashmapEntry));
    AssertNew(self->m_entries);

    self->m_idMap = (int*)calloc(capacity, sizeof(int));
    AssertNew(self->m_idMap);

    for (int i = 0; i < capacity; i++) self->m_idMap[i] = -1;
    return self;
}

void HashMap_destroy(GameHashmap* self)
{
    if (!self) return;

    free(self->m_idMap);
    free(self->m_entries);
    free(self);
}

void HashMap_insert(GameHashmap* self, GameCore currState, GameCore prevState)
{
    AssertNew(self);

    if (self->m_size >= self->m_capacity / 2)
    {
        HashMap_resize(self, self->m_capacity * 2);
    }
    size_t h = hash(currState, self->m_capacity);
    int id = self->m_idMap[h];
    while (id != -1)
    {
        if (HashMap_lists8AreEquals(self->m_entries[id].currState.forHash, currState.forHash))
        {
            return;
        }
        h = (h + 1) % self->m_capacity;
        id = self->m_idMap[h];
    }
    int c = self->m_size;
    self->m_entries[c].currState = currState;
    self->m_entries[c].prevState = prevState;
    self->m_idMap[h] = c;
    self->m_size++;
}

bool HashMap_isIn(GameHashmap* self, GameCore currState)
{
    AssertNew(self);

    size_t h = hash(currState, self->m_capacity);
    int id = self->m_idMap[h];
    while (id != -1)
    {
        if (HashMap_lists8AreEquals(
            self->m_entries[id].currState.forHash,
            currState.forHash))
        {
            return true;
        }
        h = (h + 1) % self->m_capacity;
        id = self->m_idMap[h];
    }
    return false;
}

GameCore HashMap_get(GameHashmap* self, GameCore currState)
{
    AssertNew(self);

    size_t h = hash(currState, self->m_capacity);
    while (self->m_idMap[h] != -1)
    {
        if (HashMap_lists8AreEquals(
            self->m_entries[self->m_idMap[h]].currState.forHash,
            currState.forHash))
        {
            return self->m_entries[self->m_idMap[h]].prevState;
        }
        h = (h + 1) % self->m_capacity;
    }
    return currState; //on sait jamais
}

void HashMap_resize(GameHashmap* self, int newCapacity)
{
    AssertNew(self);

    if (newCapacity <= self->m_size * 2) newCapacity = self->m_size * 3;

    GameHashmap* newHashmap = HashMap_create(newCapacity);
    for (int i = 0; i < self->m_capacity; i++)
    {
        if (self->m_idMap[i] != -1)
        {
            HashMap_insert(newHashmap, self->m_entries[self->m_idMap[i]].currState, self->m_entries[self->m_idMap[i]].prevState);
        }
    }
    free(self->m_idMap);
    free(self->m_entries);
    *self = *newHashmap;
    free(newHashmap);
}

bool HashMap_lists8AreEquals(int list1[8], int list2[8])
{
    for (int i = 0; i < 8; i++) if (list1[i] != list2[i]) return false;
    return true;
}
