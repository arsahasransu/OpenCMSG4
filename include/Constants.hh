#ifndef Constants_h
#define Constants_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

constexpr G4double kBz = 4.0*tesla;

// HCal EndCap Envelope Variables
constexpr G4int nofHcalECOPhi = 54;    // 72 - default value
constexpr G4int nofHcalECIPhi = 27;    // 36 - default value
constexpr G4double HcalEC17to20EtaOut = 0.087*16;
constexpr G4double HcalEC17to20EtaIn = 0.087*20;
constexpr G4int nofHcalEndCapR= 18;       // 18 - defaultvalue
constexpr G4double diffRAbsEC = 55*mm;
constexpr G4double diffRScnEC = 8*mm;

constexpr G4double HcalECEnvZmin = 3.88*m;
constexpr G4double HcalECEnvDelZ = 2*m;

// Variables for the solenoid cylinder
constexpr G4double solenoidInnerR = 3600*mm;
constexpr G4double solenoidOuterR = 4020*mm; // TODO Fix the value
constexpr G4double solenoidHalfZ = 650*cm;

// Variables for muon chambers
constexpr G4double muBarRin = 4025*mm;
constexpr G4double muBarRout = 7800*mm;
constexpr G4double muBarZHalfLen = 0.5*2536*mm;
constexpr G4double gapMuBar[] = {120*mm, 150*mm, 150*mm, 120*mm}; // Should have size atleast NMuBarZSeg-1 in MuonConstruction.cc
constexpr G4double muBarEnvelopZHalfLen = 5*muBarZHalfLen+gapMuBar[0]+gapMuBar[1];

constexpr G4double muECZin = 6683*mm;
constexpr G4double muECZout = 10730*mm;
constexpr G4double muECRmax = 7955*mm;
constexpr G4double muME11Zin = 5740*mm;
constexpr G4double muME11Rmax = 2600*mm;

#endif
