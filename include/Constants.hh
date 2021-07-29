#ifndef Constants_h
#define Constants_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

constexpr G4double kBz = 4.0*tesla;

// HCal EndCap Envelope Variables
constexpr G4int nofHcalECOPhi = 72;    // 72 - default value
constexpr G4int nofHcalECIPhi = 36;    // 36 - default value
constexpr G4double HcalEC17to20EtaOut = 0.087*16;
constexpr G4double HcalEC17to20EtaIn = 0.087*20;
constexpr G4int nofHcalEndCapR= 18;       // 18 - defaultvalue
constexpr G4double diffRAbsEC = 55*mm;
constexpr G4double diffRScnEC = 8*mm;

constexpr G4double HcalECEnvZmin = 3.88*m;
constexpr G4double HcalECEnvDelZ = 2*m;

// Variables for the solenoid cylinder
constexpr G4double solenoidInnerR = 4000*mm;
constexpr G4double solenoidOuterR = 4600*mm;
constexpr G4double solenoidHalfZ = 625*cm;

// Variables for muon chambers
constexpr G4double muECRin[] = {100*cm, 100*cm, 100*cm, 100*cm, 100*cm}; // Dimension equal to NMuECZSeg
constexpr G4double muECRout[] = {600*cm, 600*cm, 600*cm, 600*cm, 600*cm}; // Dimension equal to NMuECZSeg

#endif
