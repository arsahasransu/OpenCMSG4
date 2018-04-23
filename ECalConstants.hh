#ifndef ECalConstants_h
#define ECalConstants_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

constexpr G4int kNofEmColumns = 170; 	// Max Value = 170
constexpr G4int kNofEmRows = 360; 		// Max Value = 360
constexpr G4int kNofEmCells = kNofEmColumns * kNofEmRows;

constexpr G4int kNofEmECCells = 2653*4;	// Max Value = 2691

constexpr G4int kNofEvents = 20;
constexpr G4double kBz = 4.0*tesla;

constexpr G4int tr_num = 13;
constexpr G4double tr_brl_r[] = {43,72,110,250,340,430,520,610,696,782,868,965,1080};
constexpr G4double tr_brl_halfz[] = {64.5,64.5,64.5,650,650,650,650,1100,1100,1100,1100,1100,1100};
constexpr G4double tr_brl_dr[] = {285,285,285,500,500,320,320,500,500,320,320,320,320};

#endif
