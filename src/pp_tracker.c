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

/// @return Returns false if move is at max PP already, otherwise true.
bool8 HealGlobalPP(u16 moveId, u8 value)
{
    u16 newPP;

    if (gSaveBlock1Ptr->ppTracker[moveId] >= gBattleMoves[moveId].pp)
        return FALSE;
    
    newPP = gSaveBlock1Ptr->ppTracker[moveId] + value;
    if (newPP > gBattleMoves[moveId].pp)
        gSaveBlock1Ptr->ppTracker[moveId] = gBattleMoves[moveId].pp;
    else
        gSaveBlock1Ptr->ppTracker[moveId] = newPP;

    return TRUE;
}

void DeductGlobalPP(u16 moveId, u8 value)
{
    if (gSaveBlock1Ptr->ppTracker[moveId] < value)
        gSaveBlock1Ptr->ppTracker[moveId] = 0;
    else
        gSaveBlock1Ptr->ppTracker[moveId] -= value;
}