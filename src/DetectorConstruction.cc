#include "DetectorConstruction.hh"
#include "MagneticField.hh"
#include "MagneticField2.hh"
#include "Constants.hh"
#include "VolumeControl.hh"
#include "ECalConstruction.hh"
#include "HCalConstruction.hh"
#include "TrackerConstruction.hh"
#include "MuonConstruction.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4AutoDelete.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;
G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField2 = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr2 = 0;
G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField3 = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr3 = 0;
G4ThreadLocal MagneticField2* DetectorConstruction::fMagneticField4 = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr4 = 0;
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), 
  fMessenger(nullptr),
  fMagneticLogical(nullptr),
  fMagneticLogical2(nullptr),
  fMagneticLogical3(nullptr),
  fMagneticLogical4(nullptr),
  fVisAttributes(), 
  ecalMode(111), tracMode(111111)
{
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fMessenger;
  
  for (auto visAttributes: fVisAttributes) {
    delete visAttributes;
  }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  G4bool checkOverlaps = true;

////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// MATERIALS //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	// World Material

	G4String name, symbol;
	G4double a, density, fractionmass;
	G4int ncomponents, z, natoms;

  a = 14.01*g/mole;
  G4Element* elN  = new G4Element(name="Nitrogen",symbol="N" , z= 7., a);
  a = 16.00*g/mole;
  G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);

  density = 1.290*mg/cm3;
  G4Material* world_mat = new G4Material(name="Air  ",density,ncomponents=2);
  world_mat->AddElement(elN, fractionmass=70*perCent);
  world_mat->AddElement(elO, fractionmass=30*perCent);

	// ECal Material
  a = 207.2*g/mole;
  G4Element* elPb  = new G4Element(name="Lead"  ,symbol="Pb" , z= 82., a);
  a = 183.84*g/mole;
  G4Element* elW  = new G4Element(name="Tungsten"  ,symbol="W" , z= 74., a);

	density = 8.28*g/cm3;
  G4Material* ecal_mat = new G4Material(name="Lead Tungstate",density,ncomponents=3);
  ecal_mat->AddElement(elPb, fractionmass=45.536*perCent);
  ecal_mat->AddElement(elW, fractionmass=40.4*perCent);
  ecal_mat->AddElement(elO, fractionmass=14.064*perCent);
  
	// Tracker Material
	a = 28.086*g/mole;
	density = 2.32*g/cm3;
  G4Material* elSi  = new G4Material(name="Silicon" ,z=14., a, density);
	G4Element* Si = new G4Element(name="Silicon_Element", symbol="Si", z = 14., a);

	// Materials for material budget in tracker
	a = 26.98*g/mole;
	density = 2.7*g/cm3;
    G4Material* elAl  = new G4Material(name="Aluminium" ,z=13., a, density);

	a = 12.01*g/mole;
	G4Element* elC = new G4Element(name="Carbon", symbol="C", z = 6, a);
	a = 1.008*g/mole;
	G4Element* elH = new G4Element(name="Hydrogen", symbol="H", z = 1, a);

	density = 3.17*g/cm3;
	G4Material*  t_baseStrip = new G4Material(name="Silicon Nitride",density,ncomponents=2);
	t_baseStrip->AddElement(Si, natoms=3);
	t_baseStrip->AddElement(elN, natoms=4);
	
	density = 1.1*g/cm3;
	G4Material* kapton = new G4Material(name="Kapton",density,ncomponents=3);
	kapton->AddElement(elC, natoms=5);
	kapton->AddElement(elH, natoms=8);
	kapton->AddElement(elO, natoms=2);	

	a = 63.546*g/mole;
	G4Element* elCu = new G4Element(name="Copper", symbol="Cu", z=29, a);
	density = 8.96*g/cm3;
    G4Material* Cu  = new G4Material(name="Copper" ,z=29., a, density);

	pixBarMat.push_back(t_baseStrip);
	pixBarMat.push_back(elSi);
	pixBarMat.push_back(Cu);

	a = 9.012*g/mole;
	density = 1.85*g/cm3;
    G4Material* elBe  = new G4Material(name="Beryllium" ,z=4., a, density);

	pixECMat.push_back(elAl);
	pixECMat.push_back(elBe);
	pixECMat.push_back(elSi);

	TIBMat.push_back(elAl);
	TIBMat.push_back(Cu);

	a = 207.2*g/mole;
	density=11.34*g/cm3;
	G4Material* Pb = new G4Material(name="Lead", z=82., a, density);

	a = 58.693*g/mole;
	G4Element* elNi = new G4Element(name="Nickel", symbol="Ni", z=28, a);

	density = 8.95*g/cm3;
	G4Material* CuNi = new G4Material(name="Copper Nickel Alloy",density,ncomponents=2);
	CuNi->AddElement(elCu, 0.7);
	CuNi->AddElement(elNi, 0.3);

	TOBMat.push_back(Pb);
	TOBMat.push_back(CuNi);

	TECMat.push_back(elAl);
	
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(Pb);
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(elSi);
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(Pb);
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(elAl);
	PShowerMat.push_back(elSi);
	
	density = 2.65*g/cm3;
	G4Material* quartz = new G4Material(name="Quartz",density,ncomponents=2);
	quartz->AddElement(Si, natoms=1);
	quartz->AddElement(elO, natoms=2);	

////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// VOLUMES ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	// World Solid
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
	// World Logical
  logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
	// World Physical
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// TRACKER ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	if(tracMode%10 == 0)
	TEC = false;
	else
	TEC = true;

	if((tracMode/10)%10 == 0)
	TOB = false;
	else
	TOB = true;

	if((tracMode/100)%10 == 0)
	TID = false;
	else
	TID = true;

	if((tracMode/1000)%10 == 0)
	TIB = false;
	else
	TIB = true;

	if((tracMode/10000)%10 == 0)
	PID = false;
	else
	PID = true;

	if((tracMode/100000)%10 == 0)
	PIB = false;
	else
	PIB = true;

	// Tracker Envelope
	auto trackerSolid = new G4Tubs("TrackerSolid", 40*mm, 1200*mm, 3000*mm, 90*deg, 360*deg);
	trackerLogical = new G4LogicalVolume(trackerSolid,world_mat,"TrackerLogical");
	new G4PVPlacement(0, G4ThreeVector(), trackerLogical, "TrackerEnvelope", logicWorld, false, 0);

	TrackerConstruction* trac = new TrackerConstruction();

	if(PIB){
	  trac->makePIB(elAl, elSi, Cu, pixBarMat, trackerLogical, nonSens, fVisAttributes);
	}
	
	if(PID){
	  trac->makePID(elAl, elSi, Cu, pixECMat, trackerLogical, nonSens, fVisAttributes);
	}

	if(TIB){
	  trac->makeTIB(elSi, Cu, TIBMat, trackerLogical, nonSens, fVisAttributes);
	}

	if(TID){
	  trac->makeTID(elAl, elSi, Cu, trackerLogical, nonSens, fVisAttributes);
	}

	if(TOB){
	  trac->makeTOB(elSi, elAl, Pb, TOBMat, trackerLogical, nonSens, fVisAttributes);
	}
	if(TEC){
	  trac->makeTEC(elSi, TECMat, trackerLogical, nonSens, fVisAttributes);
	}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// ECAL ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	if(ecalMode%10 == 0)
	ECalR = false;
	else
	ECalR = true;

	if((ecalMode/10)%10 == 0)
	ECalBar = false;
	else
	ECalBar = true;

	if((ecalMode/100)%10 == 0)
	ECalL = false;
	else
	ECalL = true;

	// ECal Envelope
	auto ecalSolid = new G4Tubs("ECalSolid", 1200*mm, 1600*mm, 3000*mm, 0*deg, 360*deg);
	ecalLogical = new G4LogicalVolume(ecalSolid,world_mat,"ECalLogical");
	new G4PVPlacement(0, G4ThreeVector(), ecalLogical, "ECalEnvelope", logicWorld, false, 0);

	ECalConstruction* ecal = new ECalConstruction();
	if(ECalBar) {
	  ecal->makeBarrel(ecal_mat, ecalLogical, fVisAttributes);
	}

	// ECal Endcap Envelope
	G4double ecalECenv_z = 250.0;
	G4double ecalECenv_r = 1711.0;
	auto ecalECSolid = new G4Tubs("ECalECSolid", 0, ecalECenv_r*mm, 0.5*ecalECenv_z*mm, 0, 360.0*deg);
	ecalECLogical_r = new G4LogicalVolume(ecalECSolid, world_mat, "ECalECLogical");
	new G4PVPlacement(0, G4ThreeVector(0, 0, (3205+110)*mm), ecalECLogical_r, "ECalECEnvelopeRight", logicWorld, false, 0, true);
	ecalECLogical_l = new G4LogicalVolume(ecalECSolid, world_mat, "ECalECLogical");
	new G4PVPlacement(0, G4ThreeVector(0, 0, -(3205+110)*mm), ecalECLogical_l, "ECalECEnvelopeLeft", logicWorld, false, 0, true);

	if(ECalR || ECalL) {
	  ecal->makeEndCap(ecal_mat, ecalECLogical_r, ecalECLogical_l, ECalR, ECalL, fVisAttributes);
	}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// HCAL ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	// HCal Envelope
	//auto hcalSolid = new G4Tubs("HCalSolid", 1800*mm, 3500*mm, 10000*mm, 0*deg, 360*deg);
	auto hcalSolid = new G4Tubs("HCalSolid", 1770*mm, 3500*mm, 10000*mm, 0*deg, 360*deg);
	hcalLogical = new G4LogicalVolume(hcalSolid,world_mat,"HCalLogical");
	new G4PVPlacement(0, G4ThreeVector(), hcalLogical, "HCalEnvelope", logicWorld, false, 0);

	auto hcalECOSolid = new G4Tubs("HCalECOSolid", 200*mm, 3500*mm,0.5*nofHcalEndCapR*(diffRAbsEC+diffRScnEC), 0*deg, 360*deg);
	hcalECOLogical_r = new G4LogicalVolume(hcalECOSolid,world_mat,"HCalECOLogical");
	hcalECOLogical_l = new G4LogicalVolume(hcalECOSolid,world_mat,"HCalECOLogical");

	G4double hcalECOLogica_z = HcalECEnvZmin + 0.5*nofHcalEndCapR*(diffRAbsEC+diffRScnEC);
	new G4PVPlacement(0, G4ThreeVector(0,0,hcalECOLogica_z), hcalECOLogical_r, "HCalECEnvelopeRight", logicWorld, false, 0);
	new G4PVPlacement(0, G4ThreeVector(0,0,-hcalECOLogica_z), hcalECOLogical_l, "HCalECEnvelopeleft", logicWorld, false, 0);
	
	HCalConstruction* hcal = new HCalConstruction();
	hcal->makeBarrel(Cu, quartz, hcalLogical, fVisAttributes);
	/*
	hcal->makeEndCapOuter(Cu, Cu, hcalECOLogical_r, hcalECOLogical_l, fVisAttributes);
	hcal->makeEndCapInner(Cu, Cu, hcalECOLogical_r, hcalECOLogical_l, fVisAttributes);
	*/

	/*
	// HCal EndCap Envelope
	G4double HcalEC17to20ThetaOut = 2*atan(exp(-HcalEC17to20EtaOut));
	G4double HcalEC17to20ThetaIn = 2*atan(exp(-HcalEC17to20EtaIn));
	G4double HCalEC17to20ROut1 = HcalECEnvZmin*tan(HcalEC17to20ThetaOut);
	G4double HCalEC17to20RIn1 = HcalECEnvZmin*tan(HcalEC17to20ThetaIn);
	G4double HCalEC17to20ROut2 = (HcalECEnvZmin+HcalECEnvDelZ)*tan(HcalEC17to20ThetaOut);
	G4double HCalEC17to20RIn2 = (HcalECEnvZmin+HcalECEnvDelZ)*tan(HcalEC17to20ThetaIn);

	
	auto hcalECSolid = new G4Cons("HCalECSolid",
				      HCalEC17to20RIn1,
				      HCalEC17to20ROut1,
				      HCalEC17to20RIn2,
				      HCalEC17to20ROut2,
				      0.5*HcalECEnvDelZ,
				      0*deg,
				      360*deg);
	hcalECLogical_r = new G4LogicalVolume(hcalECSolid,
					      world_mat,
					      "HCalECLogical");
	new G4PVPlacement(0,
			  G4ThreeVector(0, 0, HcalECEnvZmin+0.5*HcalECEnvDelZ),
			  hcalECLogical_r,
			  "HCalECEnvelopeRight",
			  logicWorld,
			  false,
			  0,
			  true);
	*/
/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// PRE-SHOWER PLACEMENT ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

	// PreShower Endcap Envelope
	G4double PSECenv_zpos = 3070*mm;
	G4double PSECenv_dz = 16*mm;
	G4double PSECenv_rMin = 457*mm;
	G4double PSECenv_rMax = 1230*mm;
	auto PSECenvSolid = new G4Tubs("PSECenvSolid", PSECenv_rMin, PSECenv_rMax, PSECenv_dz, 0, 360.0*deg);
	ecalPSLogical_r = new G4LogicalVolume(PSECenvSolid, world_mat, "PSECenvLogical_r");
	new G4PVPlacement(0, G4ThreeVector(0, 0, PSECenv_zpos), ecalPSLogical_r, "PSECEnvelopeRight", logicWorld, false, 0, true);
	ecalPSLogical_l = new G4LogicalVolume(PSECenvSolid, world_mat, "PSECenvLogical_l");
	new G4PVPlacement(0, G4ThreeVector(0, 0, -PSECenv_zpos), ecalPSLogical_l, "PSECEnvelopeLeft", logicWorld, false, 0, true);

	// PreShower End Cap Parameterised Placement of Crytals
	G4double PShowPos = -PSECenv_dz;
	for(unsigned int cNo=0; cNo<PShowerMat.size(); cNo++)
	{
		PShowPos += 0.5*PShower_thick[cNo];
		auto PSECSolid = new G4Tubs("PSECSolid", PSECenv_rMin, PSECenv_rMax, 0.5*PShower_thick[cNo], 0, 360.0*deg);
		cellEcalPS_l.push_back(new G4LogicalVolume(PSECSolid, PShowerMat[cNo], "PSECLogical_l"));
		new G4PVPlacement(0, G4ThreeVector(0, 0, PShowPos), cellEcalPS_l[cNo], "PSECLeftPhysical", ecalPSLogical_l, false, 0, true);
		cellEcalPS_r.push_back(new G4LogicalVolume(PSECSolid, PShowerMat[cNo], "PSECLogical_r"));
		new G4PVPlacement(0, G4ThreeVector(0, 0, PShowPos), cellEcalPS_r[cNo], "PSECRightPhysical", ecalPSLogical_r, false, 0, true);
		PShowPos += 0.5*PShower_thick[cNo];
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// SOLENOID CYLINDER ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

	// Solenoid
	auto solenoidSolid = new G4Tubs("SolenoidSolid", solenoidInnerR, solenoidOuterR, solenoidHalfZ, 0*deg, 360*deg);
	solenoidLogical = new G4LogicalVolume(solenoidSolid,CuNi,"SolenoidLogical");
	new G4PVPlacement(0, G4ThreeVector(), solenoidLogical, "Solenoid", logicWorld, false, 0);
	
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// MUON CHAMBER //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

	// Muon Chamber Logical
	auto muBrChSolid = new G4Tubs("MuonBarrelChamberSolid", 4600*mm, 7000*mm, solenoidHalfZ, 0*deg, 360*deg);
	muBrChLogical = new G4LogicalVolume(muBrChSolid,world_mat,"Muon Barrel Chamber Logical");
	new G4PVPlacement(0, G4ThreeVector(), muBrChLogical, "Muon Barrel", logicWorld, false, 0);

	auto muECChSolid_r = new G4Tubs("MuonECChamberSolid_r", 0.9*muECRin[0], 0.5*world_sizeXY, 0.5*(0.5*world_sizeZ-solenoidHalfZ), 0*deg, 360*deg);
	muECChLogical_r = new G4LogicalVolume(muECChSolid_r,world_mat,"Muon Right EndCap Chamber Logical");
	new G4PVPlacement(0, G4ThreeVector(0,0,0.5*(0.5*world_sizeZ+solenoidHalfZ)), muECChLogical_r, "Muon Right EndCap", logicWorld, false, 0);

	auto muECChSolid_l = new G4Tubs("MuonECChamberSolid_l", 0.9*muECRin[0], 0.5*world_sizeXY, 0.5*(0.5*world_sizeZ-solenoidHalfZ), 0*deg, 360*deg);
	muECChLogical_l = new G4LogicalVolume(muECChSolid_l,world_mat,"Muon Left EndCap Chamber Logical");
	new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(0.5*world_sizeZ+solenoidHalfZ)), muECChLogical_l, "Muon Left EndCap", logicWorld, false, 0);

	MuonConstruction* muon = new MuonConstruction();
	muon->makeBarrel(CuNi, muBrChLogical, fVisAttributes);
	muon->makeEndCap_posz(CuNi, muECChLogical_r, false, fVisAttributes);
	muon->makeEndCap_negz(CuNi, muECChLogical_l, false, fVisAttributes);

////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// VISUALS ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	//
	// Visualization Control
	//

  auto visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  //visAttributes->SetVisibility(false);
  logicWorld->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.8888,0,0));
  //visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  ecalLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
  visAttributes = new G4VisAttributes(G4Colour(0,0,0.8888));
  //visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  trackerLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.56,0,1));
  //visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  solenoidLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.56,0.3,1));
  //visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  muBrChLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  /*  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
  //visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  singleTrackerLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
*/
  // return the world physical volume ----------------------------------------
  
  //fMagneticLogical = logicWorld;
  fMagneticLogical = ecalLogical;
  fMagneticLogical2 = trackerLogical;
  fMagneticLogical3 = hcalLogical;
  fMagneticLogical4 = muBrChLogical;

  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // magnetic field ----------------------------------------------------------
  fMagneticField = new MagneticField();
  fFieldMgr = new G4FieldManager();
  fFieldMgr->SetDetectorField(fMagneticField);
  fFieldMgr->CreateChordFinder(fMagneticField);
  G4bool forceToAllDaughters = true;
  fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);

  fMagneticField2 = new MagneticField();
  fFieldMgr2 = new G4FieldManager();
  fFieldMgr2->SetDetectorField(fMagneticField2);
  fFieldMgr2->CreateChordFinder(fMagneticField2);
  fMagneticLogical2->SetFieldManager(fFieldMgr2, forceToAllDaughters);

  fMagneticField3 = new MagneticField();
  fFieldMgr3 = new G4FieldManager();
  fFieldMgr3->SetDetectorField(fMagneticField3);
  fFieldMgr3->CreateChordFinder(fMagneticField3);
  fMagneticLogical3->SetFieldManager(fFieldMgr3, forceToAllDaughters);

  fMagneticField4 = new MagneticField2();
  fFieldMgr4 = new G4FieldManager();
  fFieldMgr4->SetDetectorField(fMagneticField4);
  fFieldMgr4->CreateChordFinder(fMagneticField4);
  fMagneticLogical4->SetFieldManager(fFieldMgr4, forceToAllDaughters);

  // Register the field and its manager for deleting
  G4AutoDelete::Register(fMagneticField);
  G4AutoDelete::Register(fFieldMgr);
  G4AutoDelete::Register(fMagneticField2);
  G4AutoDelete::Register(fFieldMgr2);
  G4AutoDelete::Register(fMagneticField3);
  G4AutoDelete::Register(fFieldMgr3);
  G4AutoDelete::Register(fMagneticField4);
  G4AutoDelete::Register(fFieldMgr4);
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineCommands()
{

	// Define /detector command directory using generic messenger class
	fMessenger = new G4GenericMessenger(this, 
				"/detector/", 
				"Detector control");

	// Non-Sensitive volume control command
	auto& noSensCmd = fMessenger->DeclareProperty("nonSensitiveMode", nonSens, "Non Sensitive volume presence");
	noSensCmd.SetParameterName("noSens", true);
	noSensCmd.SetDefaultValue("true");

	// ECal volume control command
	auto& ecalCmd = fMessenger->DeclareProperty("ecalMode", ecalMode, "Mode of ECal");
	ecalCmd.SetParameterName("ecal", true);
	ecalCmd.SetRange("ecal>=0 && ecal<1000");
	ecalCmd.SetDefaultValue("111");

	// Tracker volume control command
	auto& trackerCmd = fMessenger->DeclareProperty("trackerMode", tracMode, "Mode of Tracker");
	trackerCmd.SetParameterName("tracker", true);
	trackerCmd.SetRange("tracker>=0 && tracker<1000000");
	trackerCmd.SetDefaultValue("111111");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
