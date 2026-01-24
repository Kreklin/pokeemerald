#ifndef GUARD_PP_TRACKER_H
#define GUARD_PP_TRACKER_H

bool8 IsMovePPTracked(u16);
void NewGameInitPPTracker(void);
bool8 HealGlobalPP(u16, u8);
void DeductGlobalPP(u16, u8);

#endif