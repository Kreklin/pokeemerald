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
    u16 i;

    for (i = 0; i < PP_TRACKER_COUNT; i++)
    {
        gSaveBlock1Ptr->ppTracker[i] = gBattleMoves[i + 1].pp;
    }
    // four max PP uses are stored per byte
    for (i = 0; i < NUM_PP_INCREASE_BYTES; i++)
    {
        gSaveBlock1Ptr->ppIncreaseTracker[i] = 0;
    }
}

u8 GetGlobalPP(u16 moveId)
{
    // moves are stored offset by one to skip MOVE_NONE
    return gSaveBlock1Ptr->ppTracker[moveId - 1];
}

/// @return Returns false if move is at max PP already, otherwise true.
bool8 HealGlobalPP(u16 moveId, u8 value)
{
    u16 newPP;
    u8 maxPP = GetGlobalMaxPP(moveId);

    if (gSaveBlock1Ptr->ppTracker[moveId - 1] >= maxPP)
        return FALSE;
    
    newPP = gSaveBlock1Ptr->ppTracker[moveId - 1] + value;
    if (newPP > maxPP)
        gSaveBlock1Ptr->ppTracker[moveId - 1] = maxPP;
    else
        gSaveBlock1Ptr->ppTracker[moveId - 1] = newPP;

    return TRUE;
}

void DeductGlobalPP(u16 moveId, u8 value)
{
    if (gSaveBlock1Ptr->ppTracker[moveId - 1] < value)
        gSaveBlock1Ptr->ppTracker[moveId - 1] = 0;
    else
        gSaveBlock1Ptr->ppTracker[moveId - 1] -= value;
}

#define PP_UP_SHIFTS(val)           val,        (val) << 2,        (val) << 4,        (val) << 6
#define PP_UP_SHIFTS_INV(val) (u8)~(val), (u8)~((val) << 2), (u8)~((val) << 4), (u8)~((val) << 6)

// PP Up bonuses are stored grouped together in fours as a single byte.
// There are 2 bits (a value 0-3) for each move ID that represent how many PP Ups have been applied.
// The following arrays take the remainder of a move ID divided by 4 and return:
// sPPUpGetMask - A mask to get the number of PP Ups applied to that move
// sPPUpClearMask - A mask to clear the number of PP Ups applied to that move
// sPPUpAddValues - A value to add to the PP Bonuses byte to apply 1 PP Up to that move
static const u8 sPPUpGetMask[4]   = {PP_UP_SHIFTS(3)};
static const u8 sPPUpClearMask[4] = {PP_UP_SHIFTS_INV(3)};
static const u8 sPPUpAddValues[4] = {PP_UP_SHIFTS(1)};

u8 GetGlobalMaxPP(u16 moveId)
{
    u8 basePP = gBattleMoves[moveId].pp;
    u8 moveSubindex = (moveId - 1) % 4;
    return basePP + ((basePP * 20 * ((sPPUpGetMask[moveSubindex] & gSaveBlock1Ptr->ppIncreaseTracker[(moveId - 1) / 4]) >> (2 * moveSubindex))) / 100);
}

/// @return Returns false if move PP has been increased the maximum number of times already, otherwise true.
bool8 IncreaseGlobalMaxPP(u16 moveId, u8 value)
{
    u8 moveIndex = (moveId - 1) / 4;
    u8 moveSubindex = (moveId - 1) % 4;
    u8 ppIncreases = (gSaveBlock1Ptr->ppIncreaseTracker[moveIndex] & gPPUpGetMask[moveSubindex]) >> (moveSubindex * 2);

    // Max number of PP ups have already been applied.
    if (ppIncreases == 3)
        return FALSE;

    ppIncreases = min(3, ppIncreases + value);
    gSaveBlock1Ptr->ppIncreaseTracker[moveIndex] &= sPPUpClearMask[moveSubindex];
    gSaveBlock1Ptr->ppIncreaseTracker[moveIndex] += sPPUpAddValues[moveSubindex] * ppIncreases;

    return TRUE;
}