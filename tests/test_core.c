#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "game/core/game_core.h"
#include "game/core/game_levels.h"
#include "game/core/game_solver.h"
#include "game/core/hash_map.h"

static void initEmptyCore(GameCore* core)
{
    memset(core, 0, sizeof(GameCore));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            core->map[i][j].id = -1;
        }
    }
    core->nbObject = 0;
}

static void testGameCore(void)
{
    assert(GameCore_valide(0, 0) == true);
    assert(GameCore_valide(2, 3) == true);
    assert(GameCore_valide(-1, 0) == false);
    assert(GameCore_valide(3, 0) == false);
    assert(GameCore_valide(0, 4) == false);

    Object skelA;
    GameCore_createObject(&skelA, 0, 1, 1, 0, 0);
    assert(skelA.canMove == true);
    assert(skelA.canExit == true);

    GameCore_rotateObjectCW(&skelA);
    assert(skelA.rotationIndex == 1);
    GameCore_rotateObjectCCW(&skelA);
    assert(skelA.rotationIndex == 0);

    Object tomb;
    GameCore_createObject(&tomb, 1, 0, 0, 8, 0);
    assert(tomb.canMove == false);
    assert(tomb.canExit == false);

    GameCore core;
    initEmptyCore(&core);
    core.map[0][3] = skelA;
    assert(GameCore_isWon(&core) == true);

    core.map[0][3] = tomb;
    assert(GameCore_isWon(&core) == false);

    // Bordure haut interdite en ligne 0 pour le squelette A (orientation 0)
    initEmptyCore(&core);
    assert(GameCore_verifBorders(0, 1, &core, 0, 0) == false);
    // En [0][3] oriente a droite (rotation 1), la sortie est autorisee
    assert(GameCore_verifBorders(0, 3, &core, 0, 1) == true);
    // En [1][3] oriente a droite (rotation 1), sortie interdite
    assert(GameCore_verifBorders(1, 3, &core, 0, 1) == false);

    printf("[OK] testGameCore\n");
}

static void testHashMap(void)
{
    GameHashmap* map = HashMap_create(4);
    GameCore states[20];
    GameCore prevs[20];

    for (int i = 0; i < 20; i++)
    {
        int x = i % 3;
        int y = (i / 3) % 4;
        int rot = (i / 12) % 4;

        initEmptyCore(&states[i]);
        initEmptyCore(&prevs[i]);
        GameCore_createObject(&states[i].map[x][y], 0, x, y, 0, rot);
        states[i].nbObject = 1;
        GameCore_updateListForHash(&states[i]);

        GameCore_createObject(&prevs[i].map[0][0], 0, 0, 0, 0, rot);
        prevs[i].nbObject = 1;
        GameCore_updateListForHash(&prevs[i]);

        HashMap_insert(map, states[i], prevs[i]);
    }

    assert(map->m_size == 20);
    for (int i = 0; i < 20; i++)
    {
        assert(HashMap_isIn(map, states[i]) == true);
        GameCore got = HashMap_get(map, states[i]);
        assert(HashMap_lists8AreEquals(got.forHash, prevs[i].forHash) == true);
    }

    GameCore absent;
    initEmptyCore(&absent);
    GameCore_createObject(&absent.map[2][2], 1, 2, 2, 2, 1);
    absent.nbObject = 1;
    GameCore_updateListForHash(&absent);
    assert(HashMap_isIn(map, absent) == false);

    HashMap_destroy(map);
    printf("[OK] testHashMap\n");
}

static void testSolverAndLevels(void)
{
    GameCore core;
    int levelIds[4] = { 1, 13, 15, 158 };

    for (int i = 0; i < 4; i++)
    {
        assert(GameLevels_loadReportLevel(levelIds[i], &core) == true);

        int steps = GameSolver_solve(&core, true);
        assert(steps > 0);

        GameCore next = core;
        GameSolver_playNextMove(&next);
        int nextSteps = GameSolver_solve(&next, true);
        assert(nextSteps == steps - 1);
    }

    // Niveau bloque sans squelette A -> -1
    initEmptyCore(&core);
    GameCore_createObject(&core.map[0][0], 0, 0, 0, 8, 0);
    core.nbObject = 1;
    assert(GameSolver_solve(&core, true) == -1);

    printf("[OK] testSolverAndLevels\n");
}

int main(void)
{
    testGameCore();
    testHashMap();
    testSolverAndLevels();
    printf("Tous les tests sont passes.\n");
    return 0;
}
