#include "DetectorConstruction.hh"
#include "MagneticField.hh"
#include "Constants.hh"
#include "VolumeControl.hh"
#include "ECalConstruction.hh"

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
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), 
  fMessenger(nullptr),
  fMagneticLogical(nullptr),
  fVisAttributes(), 
  PIB_Logical(PIB_num), PID_Logical(PID_num), TIB_Logical(TIB_num), TID_Logical(TID_num), TOB_Logical(TOB_num),
  TEC_Logical(TEC_num),
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
  G4double world_sizeXY = 4*m;
  G4double world_sizeZ  = 8*m;

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

	for(G4int i=0; i<PIB_num; i++)
	{
		auto PIBSupp_Solid = new G4Tubs("PIBSupport_Solid", PIB_rMin[i]*mm-PIBSup_thick*mm, PIB_rMin[i]*mm, 285*mm, 90*deg, trPhiAng*deg);
		auto PIBSupp_Logical = new G4LogicalVolume(PIBSupp_Solid,elAl,"PIBSupport_Logical");
		if(nonSens && PIB)
		new G4PVPlacement(0, G4ThreeVector(), PIBSupp_Logical, "PixelInnerBarrelSupport", trackerLogical, false, i, true);

		auto PIB_Solid 
			= new G4Tubs("PIB_Solid", PIB_rMin[i]*mm, PIB_rMin[i]*mm+PIB_thick*um, PIB_halfz*mm, 90*deg, trPhiAng*deg);
		PIB_Logical[i]
    		= new G4LogicalVolume(PIB_Solid,elSi,"PIB_Logical");
		if(PIB)
		new G4PVPlacement(0, G4ThreeVector(), PIB_Logical[i], "PixelInnerBarrel", trackerLogical, false, i, true);
		
		G4double Pix_rMat = PIB_rMin[i]*mm+PIB_thick*um;
		for(unsigned int mat_ctr=0; mat_ctr<pixBarMat.size(); mat_ctr++)
		{
			auto PixBarMat_Solid = new G4Tubs("PIBMaterials_Solid", Pix_rMat, Pix_rMat+PIBMat_thick[mat_ctr]*um, PIB_halfz*mm, 90*deg, trPhiAng*deg);
			Pix_rMat = Pix_rMat+PIBMat_thick[mat_ctr]*um;
			auto PixBarMat_Logical = new G4LogicalVolume(PixBarMat_Solid,pixBarMat[mat_ctr],"PIBMaterials_Logical");
			if(nonSens && PIB)
			new G4PVPlacement(0, G4ThreeVector(), PixBarMat_Logical, "PixelInnerBarrelMaterial", trackerLogical, false, 0, true);
		}
	}

	auto PIBMat_Solid = new G4Tubs("PIBCables_Solid", PIB_thick*um+44555*um, PIB_thick*um+44555*um+300*um, 0.5*(177-66)*mm, 90*deg, trPhiAng*deg);
	auto PIBMat_Logical = new G4LogicalVolume(PIBMat_Solid,Cu,"PIBCables_Logical");
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,0.5*(66+177)*mm), PIBMat_Logical, "PixelInnerBarrelCables", trackerLogical, false, 0, true);
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(66+177)*mm), PIBMat_Logical, "PixelInnerBarrelCables", trackerLogical, false, 1, true);

	PIBMat_Solid = new G4Tubs("PIBCables_Solid", PIB_thick*um+44555*um, PIB_thick*um+44555*um+700*um, 0.5*(285-177)*mm, 90*deg, trPhiAng*deg);
	PIBMat_Logical = new G4LogicalVolume(PIBMat_Solid,Cu,"PIBCables_Logical");
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,0.5*(285+177)*mm), PIBMat_Logical, "PixelInnerBarrelCables", trackerLogical, false, 2, true);
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(285+177)*mm), PIBMat_Logical, "PixelInnerBarrelCables", trackerLogical, false, 3, true);

	PIBMat_Solid = new G4Tubs("PIBCables_Solid", PIB_thick*um+73555*um, PIB_thick*um+73555*um+300*um, 0.5*(285-110)*mm, 90*deg, trPhiAng*deg);
	PIBMat_Logical = new G4LogicalVolume(PIBMat_Solid,Cu,"PIBCables_Logical");
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,0.5*(285+110)*mm), PIBMat_Logical, "PixelInnerBarrelCables", trackerLogical, false, 4, true);
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(285+110)*mm), PIBMat_Logical, "PixelInnerBarrelCables", trackerLogical, false, 5, true);

	PIBMat_Solid = new G4Tubs("PIBCables_Solid", PIB_thick*um+102555*um, PIB_thick*um+102555*um+300*um, 0.5*(285-153)*mm, 90*deg, trPhiAng*deg);
	PIBMat_Logical = new G4LogicalVolume(PIBMat_Solid,Cu,"PIBCables_Logical");
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,0.5*(285+153)*mm), PIBMat_Logical, "PixelInnerBarrelCables", trackerLogical, false, 6, true);
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(285+153)*mm), PIBMat_Logical, "PixelInnerBarrelCables", trackerLogical, false, 7, true);

	auto PIBSupp_Solid = new G4Tubs("PIBSupport_Solid", 40*mm, 180*mm, 0.5*PIBSup_thick*mm, 90*deg, trPhiAng*deg);
	auto PIBSupp_Logical = new G4LogicalVolume(PIBSupp_Solid,elAl,"PIBSupport_Logical");
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,285*mm+0.5*PIBSup_thick*mm), PIBSupp_Logical, "PixelInnerBarrelSupport", trackerLogical, false, PIB_num, true);
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,-285*mm-0.5*PIBSup_thick*mm), PIBSupp_Logical, "PixelInnerBarrelSupport", trackerLogical, false, PIB_num+1, true);

	auto PIBCables_Solid = new G4Tubs("PIBCables_Solid", 40*mm, 180*mm, 0.5*PIBCables_thick*um, 90*deg, trPhiAng*deg);
	auto PIBCables_Logical = new G4LogicalVolume(PIBCables_Solid,Cu,"PIBCables_Logical");
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,285*mm+PIBSup_thick*mm+0.5*PIBCables_thick*um), PIBCables_Logical, "PixelInnerBarrelCables", trackerLogical, false, 0, true);
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,-285*mm-PIBSup_thick*mm-0.5*PIBCables_thick*um), PIBCables_Logical, "PixelInnerBarrelCables", trackerLogical, false, 1, true);

	auto PIBCables_Solid2 = new G4Tubs("PIBCables_Solid2", 180*mm, 180*mm+1000*um, 0.5*290*mm, 90*deg, trPhiAng*deg);
	auto PIBCables_Logical2 = new G4LogicalVolume(PIBCables_Solid2,elAl,"PIBCables_Logical2");
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,285*mm+PIBSup_thick*mm+PIBCables_thick*um+0.5*290*mm), PIBCables_Logical2, "PixelInnerBarrelCables2", trackerLogical, false, 0, true);
	if(nonSens && PIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,-285*mm-PIBSup_thick*mm-PIBCables_thick*um-0.5*290*mm), PIBCables_Logical2, "PixelInnerBarrelCables2", trackerLogical, false, 1, true);

	for(G4int i=0; i<PID_num; i++)
	{
		auto PID_Solid 
			= new G4Tubs("PID_Solid", PID_r[0]*mm, PID_r[1]*mm, PID_halfz*um, 90*deg, trPhiAng*deg);
		PID_Logical[i]
    		= new G4LogicalVolume(PID_Solid,elSi,"PID_Logical");
		if(PID)
		new G4PVPlacement(0, G4ThreeVector(0.,0.,PID_posz[i]*mm), PID_Logical[i], "PixelInnerDiscs", trackerLogical, false, i, true);

		G4double Pix_zMat = PID_posz[i]*mm+PID_halfz*um;
		for(unsigned int mat_ctr=0; mat_ctr<pixECMat.size(); mat_ctr++)
		{
			auto PixECMat_Solid = new G4Tubs("PIDMaterials_Solid", PID_r[0]*mm, PID_r[1]*mm, 0.5*PIDMat_thick[mat_ctr]*um, 90*deg, trPhiAng*deg);
			auto PixECMat_Logical = new G4LogicalVolume(PixECMat_Solid,pixECMat[mat_ctr],"PIDMaterials_Logical");
			Pix_zMat += 0.5*PIDMat_thick[mat_ctr]*um;
			if(nonSens && PID)
			new G4PVPlacement(0, G4ThreeVector(0.,0.,Pix_zMat), PixECMat_Logical, "PixelInnerDiskMaterial", trackerLogical, false, 0, true);
			Pix_zMat += 0.5*PIDMat_thick[mat_ctr]*um;
		}
	}

	for(G4int i=0; i<4; i++)
	{
		auto PIDMat_Solid = new G4Tubs("PIDSupportSolid", 150*mm, 150*mm+PIDMat_thick[0]*um, 0.5*25*mm, 90*deg, trPhiAng*deg);
		auto PIDMat_Logical = new G4LogicalVolume(PIDMat_Solid, elAl, "PIDSupportLogical");
		if(nonSens && PID)
		new G4PVPlacement(0, G4ThreeVector(0,0,PID_posz[i]), PIDMat_Logical, "PixelInnerDiskSupport", trackerLogical, false, 2+2*i, true);
		PIDMat_Solid = new G4Tubs("PIDSupportSolid", 150*mm+PIDMat_thick[0]*um, 150*mm+PIDMat_thick[0]*um+PIDMat_thick[1]*um, 0.5*25*mm, 90*deg, trPhiAng*deg);
		PIDMat_Logical = new G4LogicalVolume(PIDMat_Solid, elAl, "PIDSupportLogical");
		if(nonSens && PID)
		new G4PVPlacement(0, G4ThreeVector(0,0,PID_posz[i]), PIDMat_Logical, "PixelInnerDiskSupport", trackerLogical, false, 3+2*i, true);
	}

	auto PIDCables_Solid = new G4Tubs("PIDCables_Solid", 150*mm+PIDMat_thick[0]*um+PIDMat_thick[1]*um, 150*mm+PIDMat_thick[0]*um+PIDMat_thick[1]*um+750*um, 0.5*2000*mm, 90*deg, trPhiAng*deg);
	auto PIDCables_Logical = new G4LogicalVolume(PIDCables_Solid,Cu,"PIDCables_Logical");
	if(nonSens && PID)
	new G4PVPlacement(0, G4ThreeVector(0,0,PID_posz[0]+1000*mm), PIDCables_Logical, "PixelInnerDiskCables", trackerLogical, false, 0, true);
	if(nonSens && PID)
	new G4PVPlacement(0, G4ThreeVector(0,0,-PID_posz[0]-1000*mm), PIDCables_Logical, "PixelInnerDiskCables", trackerLogical, false, 1, true);


///////////////////////////////// TRACKER PIXEL REGION ENDS ///////////////////////////////////
//////////////////////////// TRACKER STRIP INNER REGION STARTS ////////////////////////////////

	G4double TIBr;

	for(G4int i=0; i<TIB_num; i++)
	{
		auto TIB_Solid 
			= new G4Tubs("TIB_Solid", TIB_rMin[i]*mm, TIB_rMin[i]*mm+TIB_thick[i]*um*(4.0/3), 700*mm, 90*deg, trPhiAng*deg);
		TIB_Logical[i]
    		= new G4LogicalVolume(TIB_Solid,elSi,"TIB_Logical");
		if(TIB)
		new G4PVPlacement(0, G4ThreeVector(), TIB_Logical[i], "TrackerStripInnerBarrel", trackerLogical, false, i, true);

		TIBr = TIB_rMin[i]*mm+TIB_thick[i]*um*(4.0/3);
		for(unsigned int j=0; j<TIBMat.size(); j++)
		{
			auto TIBSupp_Solid = new G4Tubs("TIBSupp_Solid", TIBr, TIBr+((i==0||i==1)&&j==1?2*TIBSupp_thick[j]:TIBSupp_thick[j])*um, 700*mm, 90*deg, trPhiAng*deg);
			TIBr += ((i==0||i==1)&&j==1?2*TIBSupp_thick[j]:TIBSupp_thick[j])*um;
			auto TIBSupp_Logical = new G4LogicalVolume(TIBSupp_Solid,TIBMat[j],"TIBSupp_Logical");
			if(nonSens && TIB)
			new G4PVPlacement(0, G4ThreeVector(), TIBSupp_Logical, "TIBSupport", trackerLogical, false, 0, true);
			if(nonSens && TIB)
			new G4PVPlacement(0, G4ThreeVector(), TIBSupp_Logical, "TIBSupport", trackerLogical, false, 0, true);
		}

		for(unsigned int j=0; j<5; j++)
		{
			auto TIBEle_Solid = new G4Tubs("TIBEle_Solid", TIBr, TIBr+TIBEle_thick*um, 0.5*(700-TIBSupp_pos[j])*mm, 90*deg, trPhiAng*deg);
			TIBr += TIBEle_thick*um;
			auto TIBEle_Logical = new G4LogicalVolume(TIBEle_Solid, Cu, "TIBEle_Logical");
			if(nonSens && TIB)
			new G4PVPlacement(0, G4ThreeVector(0,0,0.5*(700+TIBSupp_pos[j])*mm), TIBEle_Logical, "TIBElectricals", trackerLogical, false, i*j, true);		
			if(nonSens && TIB)
			new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(700+TIBSupp_pos[j])*mm), TIBEle_Logical, "TIBElectricals", trackerLogical, false, i*j, true);		
		}		
	}

	auto TIBEC_Solid = new G4Tubs("TIBEC_Solid", TIB_rMin[0]*mm, 500*mm, TIBSupp_thick[0]*um, 90*deg, trPhiAng*deg);
	auto TIBEC_Logical = new G4LogicalVolume(TIBEC_Solid,Cu,"TIBEC_Logical");
	if(nonSens && TIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,700*mm+TIBSupp_thick[0]*um), TIBEC_Logical, "TIBEndCapSupport", trackerLogical, false, 0, true);
	if(nonSens && TIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,-700*mm-TIBSupp_thick[0]*um), TIBEC_Logical, "TIBEndCapSupport", trackerLogical, false, 1, true);

	auto TIBD_Solid = new G4Tubs("TIBtoD_Solid", TIBr, TIBr+2400*um, 0.5*(TIDECSupp_pos-TIBSupp_pos[4])*mm, 90*deg, trPhiAng*deg);
	auto TIBD_Logical = new G4LogicalVolume(TIBD_Solid,Cu,"TIBtoD_Logical");
	if(nonSens && TIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,0.5*(TIDECSupp_pos+TIBSupp_pos[4])*mm), TIBD_Logical, "TIBtoDSupport", trackerLogical, false, 0, true);
	if(nonSens && TIB)
	new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(TIDECSupp_pos+TIBSupp_pos[4])*mm), TIBD_Logical, "TIBtoDSupport", trackerLogical, false, 1, true);

	for(G4int i=0; i<TID_num;i=i+2)
	{
		auto TID_Solid 
			= new G4Tubs("TID_Solid", 200*mm, 400*mm, 320*um*(4.0/3), 90*deg, trPhiAng*deg);
		TID_Logical[i]
    		= new G4LogicalVolume(TID_Solid,elSi,"TID_Logical");
		if(TID)
		new G4PVPlacement(0, G4ThreeVector(0.,0.,pow(-1,i>=TID_num/2?1:0)*TID_posz[i%3]*mm), TID_Logical[i], "TrackerStripInnerDisks", trackerLogical, false, i, true);
		TID_Solid 
			= new G4Tubs("TID_Solid", 400*mm, 500*mm, 160*um*(4.0/3), 90*deg, trPhiAng*deg);
		TID_Logical[i+1]
    		= new G4LogicalVolume(TID_Solid,elSi,"TID_Logical");
		if(TID)
		new G4PVPlacement(0, G4ThreeVector(0.,0.,pow(-1,i>=TID_num/2?1:0)*TID_posz[i%3]*mm), TID_Logical[i+1], "TrackerStripInnerDisks", trackerLogical, false, i+1, true);

		auto TIDSupp_Solid 
			= new G4Tubs("TIDSupp_Solid", 200*mm, 400*mm, 0.5*TIDSupp_thick*um, 90*deg, trPhiAng*deg);
		auto TIDSupp_Logical
    		= new G4LogicalVolume(TIDSupp_Solid,elAl,"TIDSupp_Logical");
		if(nonSens && TID)
		new G4PVPlacement(0, G4ThreeVector(0.,0.,pow(-1,i>=TID_num/2?1:0)*(TID_posz[i%3]*mm+2*320*um*(4.0/3)+0.5*TIDSupp_thick*um)), TIDSupp_Logical, "TIDSupport", trackerLogical, false, i, true);
		TIDSupp_Solid 
			= new G4Tubs("TIDSupp_Solid", 400*mm, 500*mm, 0.5*TIDSupp_thick*um, 90*deg, trPhiAng*deg);
		TIDSupp_Logical
    		= new G4LogicalVolume(TIDSupp_Solid,elAl,"TIDSupp_Logical");
		if(nonSens && TID)
		new G4PVPlacement(0, G4ThreeVector(0.,0.,pow(-1,i>=TID_num/2?1:0)*(TID_posz[i%3]*mm+2*160*um*(4.0/3)+0.5*TIDSupp_thick*um)), TIDSupp_Logical, "TIDSupport", trackerLogical, false, i+1, true);
	}

	auto TIDSupp_Solid = new G4Tubs("TIDSupp_Solid", 220, 500*mm+2400*um, 0.5*TIDSupp_thick*um, 90*deg, trPhiAng*deg);
	auto TIDSupp_Logical = new G4LogicalVolume(TIDSupp_Solid,elAl,"TIDSupp_Logical");
	if(nonSens && TID)
	new G4PVPlacement(0, G4ThreeVector(0,0,TIDECSupp_pos*mm+0.5*TIDSupp_thick*um), TIDSupp_Logical, "TIDSupport", trackerLogical, false, 0, true);
	if(nonSens && TID)
	new G4PVPlacement(0, G4ThreeVector(0,0,-TIDECSupp_pos*mm-0.5*TIDSupp_thick*um), TIDSupp_Logical, "TIDSupport", trackerLogical, false, 1, true);

	auto TIDEle_Solid = new G4Tubs("TIDEle_Solid", 200*mm-TIDEle_thick*um, 200*mm, 0.5*(1050-564)*mm, 90*deg, trPhiAng*deg);
	auto TIDEle_Logical = new G4LogicalVolume(TIDEle_Solid, Cu, "TIDEle_Logical");
	if(nonSens && TID)
	new G4PVPlacement(0, G4ThreeVector(0,0,0.5*(1050+564)*mm), TIDEle_Logical, "TIDElectricals", trackerLogical, false, 0, true);
	if(nonSens && TID)		
	new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(1050+564)*mm), TIDEle_Logical, "TIDElectricals", trackerLogical, false, 1, true);		

//////////////////////////// TRACKER STRIP INNER REGION ENDS //////////////////////////////////
//////////////////////////// TRACKER STRIP OUTER REGION STARTS ////////////////////////////////
	G4double rEleCool_min_hist = 0;
	for(G4int i=0; i<TOB_num; i++)
	{
		auto TOB_Solid 
			= new G4Tubs("TOB_Solid", TOB_rMin[i]*mm, TOB_rMin[i]*mm+TOB_thick[i]*um*(4.0/3), 1180*mm, 90*deg, trPhiAng*deg);
		TOB_Logical[i]
    		= new G4LogicalVolume(TOB_Solid,elSi,"TOB_Logical");
		if(TOB)
		new G4PVPlacement(0, G4ThreeVector(), TOB_Logical[i], "TrackerStripOuterBarrel", trackerLogical, false, i, true);

		G4double rEleCool_min = TOB_rMin[i]*mm+TOB_thick[i]*um*(4.0/3);
		for(unsigned int j=0; j<TOBMat.size(); j++)
		{
			auto TOBEleCool_Solid = new G4Tubs("TOBEleCool_Solid", rEleCool_min, rEleCool_min+TOBEleCool_dr[j]*um, 1180*mm, 90*deg, trPhiAng*deg);
			rEleCool_min = rEleCool_min+TOBEleCool_dr[j]*um;
			auto TOBEleCool_Logical = new G4LogicalVolume(TOBEleCool_Solid, TOBMat[j], "TOBEleCool_Logical");
			if(nonSens && TOB)
			new G4PVPlacement(0, G4ThreeVector(), TOBEleCool_Logical, "TOBElectricalsCoolants", trackerLogical, false, i*TOBMat.size()+j, true);		
		}		

		auto TOBWiring1_Solid = new G4Tubs("TOBWiring1_Solid", rEleCool_min, rEleCool_min+rWiring1*um, 0.5*(1180*mm-zWiring1*mm), 90*deg, trPhiAng*deg);
		rEleCool_min = rEleCool_min+rWiring1*um;
		auto TOBWiring1_Logical = new G4LogicalVolume(TOBWiring1_Solid, Pb, "TOBWiring1_Logical");
		if(nonSens && TOB)
		new G4PVPlacement(0, G4ThreeVector(0,0,0.5*zWiring1*mm+0.5*1180*mm), TOBWiring1_Logical, "TOBWiringFirstLayer", trackerLogical, false, i, true);
		if(nonSens && TOB)
		new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*zWiring1*mm-0.5*1180*mm), TOBWiring1_Logical, "TOBWiringFirstLayer", trackerLogical, false, i, true);		

		auto TOBWiring2_Solid = new G4Tubs("TOBWiring2_Solid", rEleCool_min, rEleCool_min+rWiring2*um, 0.5*(1180*mm-zWiring2*mm), 90*deg, trPhiAng*deg);
		rEleCool_min = rEleCool_min+rWiring2*um;
		auto TOBWiring2_Logical = new G4LogicalVolume(TOBWiring2_Solid, Pb, "TOBWiring2_Logical");
		if(nonSens && TOB)
		new G4PVPlacement(0, G4ThreeVector(0,0,0.5*zWiring2*mm+0.5*1180*mm), TOBWiring2_Logical, "TOBWiringSecondLayer", trackerLogical, false, i, true);
		if(nonSens && TOB)
		new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*zWiring2*mm-0.5*1180*mm), TOBWiring2_Logical, "TOBWiringSecondLayer", trackerLogical, false, i, true);		

		auto TOBSupp1_Solid = new G4Tubs("TOBSupp1_Solid", rEleCool_min-0.5*rSupp*mm, rEleCool_min+0.5*rSupp*mm, 0.5*zSupp1*mm, 90*deg, trPhiAng*deg);
		auto TOBSupp1_Logical = new G4LogicalVolume(TOBSupp1_Solid, elAl, "TOBSupp1_Logical");
		if(nonSens && TOB)
		new G4PVPlacement(0, G4ThreeVector(0,0,1180*mm+0.5*zSupp1*mm), TOBSupp1_Logical, "TOBSupportFirstLayer", trackerLogical, false, i, true);
		if(nonSens && TOB)
		new G4PVPlacement(0, G4ThreeVector(0,0,-1180*mm-0.5*zSupp1*mm), TOBSupp1_Logical, "TOBSupportFirstLayer", trackerLogical, false, i, true);		

		if(i==0)
		{
			rEleCool_min_hist = rEleCool_min;
		}
		else
		{
			auto TOBSupp2_Solid = new G4Tubs("TOBSupp2_Solid", rEleCool_min_hist+0.5*rSupp*mm, rEleCool_min-0.5*rSupp*mm, 0.5*zSupp2*mm, 90*deg, trPhiAng*deg);
			auto TOBSupp2_Logical = new G4LogicalVolume(TOBSupp2_Solid, elAl, "TOBSupp2_Logical");
			if(nonSens && TOB)
			new G4PVPlacement(0, G4ThreeVector(0,0,1180*mm+zSupp1*mm+0.5*zSupp2*mm), TOBSupp2_Logical, "TOBSupportSecondLayer", trackerLogical, false, i, true);
			if(nonSens && TOB)
			new G4PVPlacement(0, G4ThreeVector(0,0,-1180*mm-zSupp1*mm-0.5*zSupp2*mm), TOBSupp2_Logical, "TOBSupportSecondLayer", trackerLogical, false, i, true);	

			auto TOBSupp3_Solid = new G4Tubs("TOBSupp3_Solid", rEleCool_min_hist, TOB_rMin[i]*mm, 0.5*zSupp3*mm, 90*deg, trPhiAng*deg);
			auto TOBSupp3_Logical = new G4LogicalVolume(TOBSupp3_Solid, elAl, "TOBSupp3_Logical");
			if(nonSens && TOB)
			new G4PVPlacement(0, G4ThreeVector(0,0,zSupp3Pos*mm), TOBSupp3_Logical, "TOBSupportThirdLayer", trackerLogical, false, i, true);
			if(nonSens && TOB)
			new G4PVPlacement(0, G4ThreeVector(0,0,-zSupp3Pos*mm), TOBSupp3_Logical, "TOBSupportThirdLayer", trackerLogical, false, i, true);	
			rEleCool_min_hist = rEleCool_min;	
		}
	}

//////////////////////////// TRACKER STRIP OUTER REGION ENDS //////////////////////////////////
////////////////////////////// TRACKER STRIP END CAP STARTS ///////////////////////////////////

	G4int tec_count=0;
	for(G4int ring=1; ring<=7; ring++)
	{
		for(G4int disk=1; disk<=9; disk++)
		{
			if((ring==1 && disk>3)||(ring==2 && disk>6)||(ring==3 && disk>8)) continue;
			G4double TEC_thick = ring<=4?160*(4.0/3):250*(4.0/3);
			if((ring==1)||(ring==2)||(ring==5)) TEC_thick *= 2;

//			std::cout<<tec_count++<<"\t"<<TEC_ring[ring-1][0]<<"\t"<<TEC_ring[ring-1][1]<<"\t"<<TEC_thick<<"\t"<<TEC_disk[disk-1][(ring-1)%2]<<std::endl;

			auto TEC_Solid 
				= new G4Tubs("TEC_Solid", TEC_ring[ring-1][0]*mm, TEC_ring[ring-1][1]*mm, TEC_thick*um, 90*deg, trPhiAng*deg);
			TEC_Logical[tec_count]
    			= new G4LogicalVolume(TEC_Solid,elSi,"TEC_z+_Logical");
			if(TEC)
			new G4PVPlacement(0, G4ThreeVector(0,0,TEC_disk[disk-1][(ring-1)%2]*mm), TEC_Logical[tec_count], "TrackerEndCap", trackerLogical, false, tec_count, true);
			if(TEC)
			new G4PVPlacement(0, G4ThreeVector(0,0,-1*TEC_disk[disk-1][(ring-1)%2]*mm), TEC_Logical[tec_count], "TrackerEndCap", trackerLogical, false, tec_count, true);

			auto TECMat_Solid 
				= new G4Tubs("TECMat_Solid", TEC_ring[ring-1][0]*mm, TEC_ring[ring-1][1]*mm, 0.5*TECMat_thick*mm, 90*deg, trPhiAng*deg);
			auto TECMat_Logical
    			= new G4LogicalVolume(TECMat_Solid,TECMat[0],"TECMat_Logical");
			if(nonSens && TEC)
			new G4PVPlacement(0, G4ThreeVector(0,0,TEC_disk[disk-1][(ring-1)%2]*mm+TEC_thick*um+0.5*TECMat_thick*mm), TECMat_Logical, "TrackerEndCapMaterial", trackerLogical, false, tec_count, true);
			if(nonSens && TEC)
			new G4PVPlacement(0, G4ThreeVector(0,0,-TEC_disk[disk-1][(ring-1)%2]*mm-TEC_thick*um-0.5*TECMat_thick*mm), TECMat_Logical, "TrackerEndCapMaterial", trackerLogical, false, tec_count, true);

			tec_count++;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// ECAL BARREL /////////////////////////////////////////////
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// PRE-SHOWER PLACEMENT ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// VISUALS ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

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
  
  visAttributes = new G4VisAttributes(G4Colour(0,0,0.8888));
  visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  trackerLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

	for(G4int copyNo=0; copyNo<PIB_num; copyNo++)
	{
  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
//  visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  PIB_Logical[copyNo]->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
	}

	for(G4int copyNo=0; copyNo<TIB_num; copyNo++)
	{
  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
//  visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  TIB_Logical[copyNo]->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
	}

	for(G4int copyNo=0; copyNo<TOB_num; copyNo++)
	{
  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
//  visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  TOB_Logical[copyNo]->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
	}

/*  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
  //visAttributes->SetVisibility(false);
	//visAttributes->SetForceLineSegmentsPerCircle(10);
  singleTrackerLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
*/
  // return the world physical volume ----------------------------------------
  
	fMagneticLogical = logicWorld;

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

  // Register the field and its manager for deleting
  G4AutoDelete::Register(fMagneticField);
  G4AutoDelete::Register(fFieldMgr);
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
