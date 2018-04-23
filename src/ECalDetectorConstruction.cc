#include "ECalDetectorConstruction.hh"
#include "ECalMagneticField.hh"
#include "ECalCellParameterisation.hh"
#include "ECalCellParameterisation_tracker.hh"
#include "ECalECParameterisation.hh"
#include "ECalEmCalorimeterSD.hh"
#include "ECalECSD.hh"
#include "ECalECSD_left.hh"
#include "ECaltrackerSD.hh"
#include "ECalWorldSD.hh"
//#include "ECalHadCalorimeterSD.hh"

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
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>

#define PI 3.14159265

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal ECalMagneticField* ECalDetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* ECalDetectorConstruction::fFieldMgr = 0;
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalDetectorConstruction::ECalDetectorConstruction()
: G4VUserDetectorConstruction(), 
  fMessenger(nullptr),
  fCellLogical(nullptr),
  fMagneticLogical(nullptr),
  fVisAttributes()
{
  
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalDetectorConstruction::~ECalDetectorConstruction()
{
  delete fMessenger;
  
  for (auto visAttributes: fVisAttributes) {
    delete visAttributes;
  }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ECalDetectorConstruction::Construct()
{

  G4bool checkOverlaps = true;

	// World Material
  G4double world_sizeXY = 4*m;
  G4double world_sizeZ  = 8*m;

	G4String name, symbol;
	G4double a, density, fractionmass;
	G4int ncomponents, z;

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

	// ECal Barrel Envelope
	auto ecalSolid = new G4Tubs("ECalSolid", 1200*mm, 1600*mm, 3000*mm, 0*deg, 360*deg);
	ecalLogical = new G4LogicalVolume(ecalSolid,world_mat,"ECalLogical");
	new G4PVPlacement(0, G4ThreeVector(), ecalLogical, "ECalEnvelope", logicWorld, false, 0, true);

	// ECal Barrel Parameterised Placement of Crystals
  auto cellSolid 
    = new G4Sphere("cellSolid",1290.0*mm,1290.0*mm+22*cm,0*deg, 360*deg, 89*deg, 1*deg); // All parameters will be overidden in parameterised implementation
  fCellLogical
    = new G4LogicalVolume(cellSolid,ecal_mat,"cellLogical");
	G4VPVParameterisation* cellParam = new ECalCellParameterisation();
  new G4PVParameterised("cellPhysical",fCellLogical,ecalLogical,
                        kXAxis,kNofEmCells,cellParam,false);

	// ECal Endcap Envelope
	G4double ecalECenv_z = 250.0;
	G4double ecalECenv_r = 1711.0;
	auto ecalECSolid = new G4Tubs("ECalECSolid", 0, ecalECenv_r*mm, 0.5*ecalECenv_z*mm, 0, 360.0*deg);
	ecalECLogical = new G4LogicalVolume(ecalECSolid, world_mat, "ECalECLogical");
	new G4PVPlacement(0, G4ThreeVector(0, 0, (3205+110)*mm), ecalECLogical, "ECalECEnvelopeRight", logicWorld, false, 0, true);
	ecalECLogical_left = new G4LogicalVolume(ecalECSolid, world_mat, "ECalECLogical");
	new G4PVPlacement(0, G4ThreeVector(0, 0, -(3205+110)*mm), ecalECLogical_left, "ECalECEnvelopeLeft", logicWorld, false, 0, true);
//	new G4PVPlacement(0, G4ThreeVector(0, 0, -3560.0*mm), ecalECLogical, "ECalECEnvelopeRight", logicWorld, false, 0, true);

	// ECal End Cap Parameterised Placement of Crytals
	G4double y1=24.7, y2=25.9, len=220.0;
	G4ThreeVector* d = new G4ThreeVector(0.5*(y2-y1), 0.5*(y2-y1), len);
	auto cellEcalECSolid = new G4Trap("cellEcalECSolid", 0.5*len*mm, d->getTheta()*rad,
             45*deg,  0.5*y1*mm,
             0.5*y1*mm, 0.5*y1*mm,
             0*deg, 0.5*y2*mm,
             0.5*y2*mm, 0.5*y2*mm,
             0*deg);
	cellEcalECLogical = new G4LogicalVolume(cellEcalECSolid, ecal_mat, "CellEcalECLogical");
	cellEcalECLogical_left = new G4LogicalVolume(cellEcalECSolid, ecal_mat, "CellEcalECLogical_left");
	G4VPVParameterisation* ecalECparam = new ECalECParameterisation();
	new G4PVParameterised("ecalECPhysical", cellEcalECLogical, ecalECLogical, kXAxis, kNofEmECCells, ecalECparam, false);
	new G4PVParameterised("ecalECPhysical_left", cellEcalECLogical_left, ecalECLogical_left, kXAxis, kNofEmECCells, ecalECparam, false);

	// Tracker Envelope
	auto trackerSolid = new G4Tubs("TrackerSolid", 40*mm, 1200*mm, 1500*mm, 0*deg, 360*deg);
	trackerLogical = new G4LogicalVolume(trackerSolid,world_mat,"TrackerLogical");
	new G4PVPlacement(0, G4ThreeVector(), trackerLogical, "TrackerEnvelope", logicWorld, false, 0);

	// Tracker Parameterised Placement of Barrels
  auto singleTrackerSolid 
    = new G4Tubs("singleTrackerSolid", 43*mm, 43.285*mm, 70*mm, 0*deg, 360*deg);
  singleTrackerLogical
    = new G4LogicalVolume(singleTrackerSolid,elSi,"singleTrackerLogical");
	G4VPVParameterisation* trackerParam = new ECalCellParameterisation_tracker();
  new G4PVParameterised("trackerPhysical",singleTrackerLogical,trackerLogical,
                        kXAxis,tr_num,trackerParam,false);


	//
	// Visualization Control
	//

  auto visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  logicWorld->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.8888,0,0));
  visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  ecalLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0,0.8888,0));
  //visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  fCellLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.7645,0.6446,0.0));
  visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  ecalECLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.7645,0.6446,0.0));
  visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  ecalECLogical_left->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.9115,0.4112,0.0042));
  //visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  cellEcalECLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.9115,0.4112,0.0042));
  //visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  cellEcalECLogical_left->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0,0,0.8888));
  visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  trackerLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
  visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  singleTrackerLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  // return the world physical volume ----------------------------------------
  
	fMagneticLogical = logicWorld;

  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalDetectorConstruction::ConstructSDandField()
{
  // sensitive detectors -----------------------------------------------------
  auto sdManager = G4SDManager::GetSDMpointer();
  G4String SDname;

  auto world = new ECalWorldSD(SDname="/World");
  sdManager->AddNewDetector(world);
  logicWorld->SetSensitiveDetector(world);

  auto emCalorimeter = new ECalEmCalorimeterSD(SDname="/EMcalorimeter");
  sdManager->AddNewDetector(emCalorimeter);
  fCellLogical->SetSensitiveDetector(emCalorimeter);

  auto emCalorimeterEC = new ECalECSD(SDname="/EMcalorimeterEC");
  sdManager->AddNewDetector(emCalorimeterEC);
  cellEcalECLogical->SetSensitiveDetector(emCalorimeterEC);

  auto emCalorimeterEC_left = new ECalECSD_left(SDname="/EMcalorimeterEC_left");
  sdManager->AddNewDetector(emCalorimeterEC_left);
  cellEcalECLogical_left->SetSensitiveDetector(emCalorimeterEC_left);

  auto tracker = new ECaltrackerSD(SDname="/Tracker");
  sdManager->AddNewDetector(tracker);
  singleTrackerLogical->SetSensitiveDetector(tracker);
 
  auto trackerenv = new ECaltrackerSD(SDname="/TrackerEnv");
  sdManager->AddNewDetector(trackerenv);
  trackerLogical->SetSensitiveDetector(trackerenv);

  // magnetic field ----------------------------------------------------------
  fMagneticField = new ECalMagneticField();
  fFieldMgr = new G4FieldManager();
  fFieldMgr->SetDetectorField(fMagneticField);
  fFieldMgr->CreateChordFinder(fMagneticField);
  G4bool forceToAllDaughters = true;
  fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);

  // Register the field and its manager for deleting
  G4AutoDelete::Register(fMagneticField);
  G4AutoDelete::Register(fFieldMgr);
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalDetectorConstruction::DefineCommands()
{

  // Define /ECal/detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, 
                                      "/ECal/detector/", 
                                      "Detector control");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
