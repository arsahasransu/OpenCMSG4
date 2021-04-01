#ifndef Constants_h
#define Constants_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

constexpr G4double world_sizeXY = 20*m;
constexpr G4double world_sizeZ  = 20*m;

constexpr G4int nofEmBarEta = 170; 	// Max Value = 170
constexpr G4int nofEmBarPhi = 360; 		// Max Value = 360
constexpr G4int nofEmBarCells = nofEmBarEta*nofEmBarPhi;

//constexpr G4int kNofEmECCells = 2653*4;	// Max Value = 2691
constexpr G4int kNofEmECCells = 2708*4;	// Max Value = 2691

constexpr G4double kBz = 4.0*tesla;

constexpr G4double trPhiAng = 360;
constexpr G4int PIB_num = 3;
constexpr G4double PIB_rMin[] = {44,73,102};
constexpr G4double PIB_thick = 285*(4.0/3);
constexpr G4double PIB_halfz = 0.5*530;
constexpr G4int PID_num = 4;
constexpr G4double PID_posz[] = {345,465,-345,-465};
constexpr G4double PID_r[] = {60, 150};
constexpr G4double PID_halfz = 0.5*285*(4.0/3);
constexpr G4int TIB_num = 4;
constexpr G4double TIB_rMin[] = {255,339,418.5,498};
constexpr G4double TIB_thick[] = {2*320,2*320,320,320};
constexpr G4int TID_num = 12;
constexpr G4double TID_posz[] = {800,900,1000};
constexpr G4int TOB_num = 6;
constexpr G4double TOB_rMin[] = {608,692,780,868,965,1080};
constexpr G4double TOB_thick[] = {2*500,2*500,500,500,500,500};
constexpr G4int TEC_num = 53;
constexpr G4double TEC_ring[][2] = {{225,325},{320,405},{380,520},{515,640},{620,770},{740,930},{880,1100}};
constexpr G4double TEC_disk[][2] = {{1240,1280},{1390,1420},{1540,1580},{1660,1700},{1840,1870},{2020,2050},{2220,2250},{2460,2490},{2700,2740}};

constexpr G4int PIB_nZPixel = 100;
constexpr G4int PIB_nPhiPixel = 40;

constexpr G4double PIBMat_thick[] = {250, 180, 125};
constexpr G4double PIBSup_thick = 0.6;
constexpr G4double PIBCables_thick = 250;
constexpr G4double PIDMat_thick[] = {2000, 1000, 450};

constexpr G4double TIBSupp_thick[] = {1200, 250};
constexpr G4double TIBSupp2_thick = 100;
constexpr G4double TIBSupp_pos[] = {226, 301, 372, 442, 513};
constexpr G4double TIBEle_thick = 150;
//constexpr G4double TIBSupp_pos[] = {226, 301, 372, 442};

constexpr G4double TIDSupp_thick = 3000;
constexpr G4double TIDECSupp_pos = 1150;
constexpr G4double TIDEle_thick = 350;

constexpr G4double TOBEleCool_dr[] = {100, 50};
constexpr G4double rWiring1 = 100;
constexpr G4double zWiring1 = 97.7;
constexpr G4double rWiring2 = 100;
constexpr G4double zWiring2 = 170;
constexpr G4double rSupp = 8;
constexpr G4double zSupp1 = 10;
constexpr G4double zSupp2 = 6;
constexpr G4double zSupp3 = 2;
constexpr G4double zSupp3Pos = 225;

constexpr G4double TECMat_thick = 8;

constexpr G4double PShower_thick[] = {1,1,2,9.3,2,2,1,1,1,2,3.7,2,2,1};

// Tracker Pixelation Variables
constexpr G4double lPix = 50*um;
constexpr G4double wPix = 50*um;
constexpr G4double etashift = 4;
constexpr G4double phishift = 1.571;
constexpr long rhbit = 0b1000000000000000000;
constexpr long phbit = 0b1000000000000000000000;
constexpr long etbit = 0b10000000000000000000000;
constexpr double rhbitres = 10*um;
constexpr double phbitres = 4*pow(10,-6);
constexpr double etbitres = pow(10,-6);

// HCal Barrel Variables
constexpr G4int nofHcalBarEta = 2*15;  // 2*17 - default value
constexpr G4int nofHcalBarPhi = 72;    // 72 - default value
constexpr G4int nofHcalBarR = 18;       // 18 - default value
constexpr G4int nofHcalBarCells = nofHcalBarEta*nofHcalBarPhi*nofHcalBarR;
constexpr G4double diffRAbsBar = 55*mm;
constexpr G4double diffRScnBar = 8*mm;

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

constexpr G4double EtaArray[] = {1.740,1.830,1.930,2.043,2.172,2.322,2.5,2.65,3.0};

// Variables for the solenoid cylinder
constexpr G4double solenoidInnerR = 4000*mm;
constexpr G4double solenoidOuterR = 4600*mm;
constexpr G4double solenoidHalfZ = 625*cm;

// Variables for muon chambers
constexpr G4int NMuBarZSeg = 5; // Default = 5
constexpr G4int NMuBarPhiSeg = 12; // Default = 12
constexpr G4int NMuBarRSeg = 4; // Default = 4
constexpr G4int NMuBarCh = NMuBarZSeg*NMuBarPhiSeg*NMuBarRSeg;

constexpr G4double muBarR[] = {4700*mm, 5000*mm, 5300*mm, 5700*mm}; // Dimension equal to NMuBarRSeg
constexpr G4double muBardR[] = {100*mm, 150*mm, 200*mm, 300*mm}; // Dimension equal to NMuBarRSeg
constexpr G4double muBarZLen[] = {1250*cm, 1250*cm, 1250*cm, 1250*cm}; // Dimension equal to NMuBarRSeg

constexpr G4int NMuECZSeg = 5; // Default = 5
constexpr G4int NMuECPhiSeg = 12; // Default = 12

constexpr G4double muECRin[] = {100*cm, 100*cm, 100*cm, 100*cm, 100*cm}; // Dimension equal to NMuECZSeg
constexpr G4double muECRout[] = {600*cm, 600*cm, 600*cm, 600*cm, 600*cm}; // Dimension equal to NMuECZSeg
constexpr G4double muECZpos[] = {700*cm, 740*cm, 800*cm, 870*cm, 940*cm}; // Dimension equal to NMuECZSeg
constexpr G4double muEChalfZwid[] = {10*cm, 20*cm, 30*cm, 30*cm, 30*cm}; // Dimension equal to NMuECZSeg

#endif
