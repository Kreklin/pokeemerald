#ifndef GUARD_PP_TRACKER_H
#define GUARD_PP_TRACKER_H

bool8 IsMovePPTracked(u16);
void NewGameInitPPTracker(void);

u8 GetGlobalPP(u16);
bool8 HealGlobalPP(u16, u8);
void DeductGlobalPP(u16, u8);

u8 GetGlobalMaxPP(u16);
bool8 IncreaseGlobalMaxPP(u16, u8);

bool8 IsPressureTrainerClass(u8);

#endif