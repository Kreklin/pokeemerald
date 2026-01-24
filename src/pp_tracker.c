#include "global.h"
#include "pp_tracker.h"
#include "constants/moves.h"
#include "pokemon.h"

static const u16 sPPTrackerExcludedMoves[] = {
    MOVE_NONE,
    MOVE_STRUGGLE,
};

bool8 IsMovePPTracked(u16 moveId)
{
    u8 i;

    for (i = 0; i < sizeof(sPPTrackerExcludedMoves); i++)
    {
        if (sPPTrackerExcludedMoves[i] == moveId)
            return FALSE;
    }

    return TRUE;
}

void NewGameInitPPTracker(void) 
{
    u16 moveId;

    for (moveId = 0; moveId < MOVES_COUNT; moveId++)
    {
        gSaveBlock1Ptr->ppTracker[moveId] = gBattleMoves[moveId].pp;
    }
}