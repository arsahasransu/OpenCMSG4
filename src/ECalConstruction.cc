#include "ECalConstruction.hh"

#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

constexpr G4int nofEmBarEta = 170; 	                         // Default Value = 170
constexpr G4int nofEmBarPhi = 360; 	                         // Default Value = 360
constexpr G4int nofEmBarCells = nofEmBarEta*nofEmBarPhi;
constexpr G4double maxBarEta = 1.479;                            // Default value = 1.479
constexpr G4double barRmin = 1290.0*mm;                          // Default value = 1290 mm
constexpr G4double barCrysLen = 230.0*mm;                        // Default value = 230 mm
constexpr G4double barCrysPhi = 1*deg;                           // Default value = 1 deg = 0.01745 rad
constexpr G4int kNofEmECCells = 15000;                           // Default value = Arbitrary(15000) > ecCrysCnt below to prevent seg fault due to insufficient memory allocation
constexpr G4double ecFrontFaceZ = 3205*mm;                       // Default value = 3205 mm
constexpr G4double ecCrysLen = 220*mm;                           // Default value = 220 mm
constexpr G4double ecBackFaceOuterR = 1711*mm;                   // Default value = 1711 mm
constexpr G4double maxECEta = 3;                               // Default value = 3.1
constexpr G4int maxECCrysNum1Q1D = 62;                           // Default value = 62
constexpr G4int actECCrysNum1Q1D = 50;                           // Default value = 50
constexpr G4double ecFacingPoint = 1300*mm;                      // Default Value = 1300

ECalConstruction::ECalConstruction()
  : cellEcalBarLogical(nofEmBarEta),
    cellEcalECLogical_r(kNofEmECCells), cellEcalECLogical_l(kNofEmECCells) {
  for(G4int num=0; num<nofEmBarEta; num++) {
    std::vector<G4LogicalVolume*> temp(nofEmBarPhi);
    cellEcalBarLogical[num] = temp;
  }

  std::cout<<"ECal Initialization complete"<<std::endl;
}

ECalConstruction::~ECalConstruction() {
}

void ECalConstruction::makeBarrel(G4Material* ecalMat, G4LogicalVolume* ecalLogical, std::vector<G4VisAttributes*> fVisAttributes) {
  std::cout<<"Making the barrel for electromagnetic calorimeter"<<std::endl;
  
  for(G4int copyNo=0; copyNo<nofEmBarCells; copyNo++) {

    G4double eta_max = maxBarEta;
    G4double eta_step = 2*eta_max/nofEmBarEta; // From -eta_max to +eta_max
    G4double y = barRmin;
    G4double dy = barCrysLen;
    
    G4int etaNum = copyNo/nofEmBarPhi;
    G4int phiNum = copyNo%nofEmBarPhi;
    
    G4double eta_b = -eta_max+etaNum*eta_step;
    G4double eta_e = -eta_max+(etaNum+1)*eta_step;
    G4double theta_b = 2*atan(exp(-eta_b))*180/M_PI;
    G4double theta_e = 2*atan(exp(-eta_e))*180/M_PI;
    G4double theta_avg = 0.5*(theta_b+theta_e);
    G4double rMin = y/sin(theta_avg*M_PI/180);
    G4double rMax = rMin+dy;
    auto cellSolid 
      = new G4Sphere("cellSolid", rMin,rMax,phiNum*deg, barCrysPhi, theta_e*deg, (theta_b-theta_e)*deg);
    
    cellEcalBarLogical[etaNum][phiNum]
      = new G4LogicalVolume(cellSolid,ecalMat,"cellLogical");
    
    new G4PVPlacement(0, G4ThreeVector(), cellEcalBarLogical[etaNum][phiNum], "ECalCell", ecalLogical, false, copyNo, false);
  }

  for(G4int copyNo=0; copyNo<nofEmBarCells; copyNo++) {
    visAttributes = new G4VisAttributes(G4Colour(0.4,0.698,1));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    cellEcalBarLogical[copyNo/nofEmBarPhi][copyNo%nofEmBarPhi]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
  
}

void ECalConstruction::makeEndCap(G4Material* ecalMat, G4LogicalVolume* ecalECLogical_r, G4LogicalVolume* ecalECLogical_l, bool ECalR, bool ECalL, std::vector<G4VisAttributes*> fVisAttributes) {
  
  // ECal End Cap Parameterised Placement of Crytals

  G4double ecMaxTheta = asin(ecBackFaceOuterR/(ecFrontFaceZ+ecCrysLen));
  // Default value: Theta = 29.97deg, Eta = 1.318
  // G4double minECEta = -1*log(tan(0.5*ecMaxTheta));
  G4double ecFrontFaceOuterR = ecFrontFaceZ*sin(ecMaxTheta);
  G4double ecMinTheta = 2*atan(exp(-1*maxECEta));
  G4double ecFrontFaceInnerR = ecFrontFaceZ*sin(ecMinTheta);
  G4double ecBackFaceInnerR = (ecFrontFaceZ+ecCrysLen)*sin(ecMinTheta);
  G4double ecCrysDimBackFace = (ecBackFaceOuterR-ecBackFaceInnerR)/actECCrysNum1Q1D;
  G4double ecCrysDimFrontFace = (ecFrontFaceOuterR-ecFrontFaceInnerR)/actECCrysNum1Q1D;
  G4double ecLookAtOffsett = 1300*mm;
  
  G4double xpos_EC, ypos_EC;
  int ecCrysCnt = 0;
  std::vector<G4bool> visdec;

  for(int i=maxECCrysNum1Q1D; i>-1*maxECCrysNum1Q1D; i--) {
    
    if(ecCrysCnt>=kNofEmECCells) { // Protection against seg fault
      std::cout<<"=============================================="<<std::endl;
      std::cout<<"Warning in EC crystals: Not enough memory allocated to creation of the required EC crystals. Please check!!!"<<std::endl;
      std::cout<<"=============================================="<<std::endl;
      break;
    }
    
    for(int j=maxECCrysNum1Q1D; j>-1*maxECCrysNum1Q1D; j--) {
      
      if(ecCrysCnt>=kNofEmECCells) { // Protection against seg fault
	break;
      }
      
      G4bool dec = true;
      
      xpos_EC = ecCrysDimBackFace*(i+0.5);
      ypos_EC = ecCrysDimBackFace*(j+0.5);
      G4ThreeVector* d_r = new G4ThreeVector(xpos_EC, ypos_EC, ecFrontFaceZ+ecCrysLen+ecLookAtOffsett);
      G4ThreeVector* d_l = new G4ThreeVector(xpos_EC, ypos_EC, -(ecFrontFaceZ+ecCrysLen+ecLookAtOffsett));

      // Make a decision whether its inside the required bouandary of the EC depending on the quadrant in question
      G4double d1 = sqrt(pow(ecCrysDimBackFace*(i),2)+pow(ecCrysDimBackFace*(j),2));
      G4double d2 = sqrt(pow(ecCrysDimBackFace*(i),2)+pow(ecCrysDimBackFace*(j+1),2));
      G4double d3 = sqrt(pow(ecCrysDimBackFace*(i+1),2)+pow(ecCrysDimBackFace*(j+1),2));
      G4double d4 = sqrt(pow(ecCrysDimBackFace*(i+1),2)+pow(ecCrysDimBackFace*(j),2));
      if(i>=0 && j>=0) dec = d3>ecBackFaceInnerR && d3<ecBackFaceOuterR;
      if(i<0 && j<0) dec = d1>ecBackFaceInnerR && d1<ecBackFaceOuterR;
      if(i>=0 && j<0) dec = d4>ecBackFaceInnerR && d4<ecBackFaceOuterR;
      if(i<0 && j>=0) dec = d2>ecBackFaceInnerR && d2<ecBackFaceOuterR;
      
      if(dec) {
	
	//if(xpos_EC>0 && ypos_EC<0) visdec.push_back(false);
	//else visdec.push_back(true);
	visdec.push_back(true);
	
	auto cellEcalECSolid_r = new G4Trap("cellEcalECSolid_r",
					    0.5*ecCrysLen*mm,
					    d_r->getTheta(),
					    d_r->getPhi(),
					    0.5*ecCrysDimFrontFace*mm,
					    0.5*ecCrysDimFrontFace*mm,
					    0.5*ecCrysDimFrontFace*mm,
					    0*deg,
					    0.5*ecCrysDimBackFace*mm,
					    0.5*ecCrysDimBackFace*mm,
					    0.5*ecCrysDimBackFace*mm,
					    0*deg);
	cellEcalECLogical_r[ecCrysCnt] = new G4LogicalVolume(cellEcalECSolid_r,
						     ecalMat,
						     "CellEcalECLogical_r");

	auto cellEcalECSolid_l = new G4Trap("cellEcalECSolid_l",
					    0.5*ecCrysLen*mm,
					    d_l->getTheta(),
					    d_l->getPhi(),
					    0.5*ecCrysDimBackFace*mm,
					    0.5*ecCrysDimBackFace*mm,
					    0.5*ecCrysDimBackFace*mm,
					    0*deg,
					    0.5*ecCrysDimFrontFace*mm,
					    0.5*ecCrysDimFrontFace*mm,
					    0.5*ecCrysDimFrontFace*mm,
					    0*deg);
	cellEcalECLogical_l[ecCrysCnt] = new G4LogicalVolume(cellEcalECSolid_l,
						     ecalMat,
						     "CellEcalECLogical_l");
	if(ECalR) {
	  new G4PVPlacement(new G4RotationMatrix(),
			    G4ThreeVector(xpos_EC*mm, ypos_EC*mm, 0.), cellEcalECLogical_r[ecCrysCnt], "CellEcalEC_r", ecalECLogical_r, false, ecCrysCnt, false);
	}
	
	if(ECalL) {
	  new G4PVPlacement(new G4RotationMatrix(),
			    G4ThreeVector(xpos_EC*mm, ypos_EC*mm, 0.), cellEcalECLogical_l[ecCrysCnt], "CellEcalEC_l", ecalECLogical_l, false, ecCrysCnt, false);
	}
	ecCrysCnt++;
      }
    }
  }
  std::cout<<"=============================================="<<std::endl;
  std::cout<<"Count of EC crystals: "<<ecCrysCnt<<std::endl;
  std::cout<<ecBackFaceOuterR<<"\t"<<ecBackFaceInnerR<<"\t"<<ecFrontFaceOuterR<<"\t"<<ecFrontFaceInnerR<<std::endl;
  std::cout<<"=============================================="<<std::endl;

  for(G4int copyNo=0; copyNo<ecCrysCnt; copyNo++) {
    visAttributes = new G4VisAttributes(G4Colour(0.4,0.698,1));
    visAttributes->SetVisibility(visdec[copyNo]);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    cellEcalECLogical_r[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.4,0.698,1));
    visAttributes->SetVisibility(visdec[copyNo]);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    cellEcalECLogical_l[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
}
