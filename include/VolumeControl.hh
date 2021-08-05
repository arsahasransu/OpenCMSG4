#ifndef VolumeControl_h
#define VolumeControl_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

G4bool trackSens = false;

// Tracker
G4bool PIB = true;
G4bool PID = true;
G4bool TIB = true;
G4bool TID = true;
G4bool TOB = true;
G4bool TEC = true;

// ECal
G4bool ECalL = true;
G4bool ECalBar = true;
G4bool ECalR = true;

// HCal
G4bool HCalL = true;
G4bool HCalBar = true;
G4bool HCalR = true;
G4bool HCalScn = false;

// MuChm
G4bool MuChmL = true;
G4bool MuChmBar = true;
G4bool MuChmR = true;

#endif
