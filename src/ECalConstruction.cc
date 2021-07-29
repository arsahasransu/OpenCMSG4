#include "ECalConstruction.hh"
#include "Constants.hh"

#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4PVPlacement.hh"

ECalConstruction::ECalConstruction()
  : cellEcalBarLogical(nofEmBarEta),
    cellEcalECLogical_r(kNofEmECCells), cellEcalECLogical_l(kNofEmECCells)
{
  for(G4int num=0; num<nofEmBarEta; num++) {
    std::vector<G4LogicalVolume*> temp(nofEmBarPhi);
    cellEcalBarLogical[num] = temp;
  }

  std::cout<<"ECal Initialization complete"<<std::endl;
}

ECalConstruction::~ECalConstruction()
{}

void ECalConstruction::makeBarrel(G4Material* ecalMat, G4LogicalVolume* ecalLogical, std::vector<G4VisAttributes*> fVisAttributes)
{
  std::cout<<"Making the barrel for electromagnetic calorimeter"<<std::endl;
  
  for(G4int copyNo=0; copyNo<nofEmBarCells; copyNo++) {
    G4double eta_max = 1.479;
    G4double eta_step = eta_max/85;
    G4double y = 1290.0*mm;
    G4double dy = 220*mm;
    
    G4int etaNum = (copyNo+(nofEmBarPhi-360)*30)/nofEmBarPhi;
    G4int phiNum = (copyNo+(nofEmBarPhi-360)*30)%nofEmBarPhi;
    
    G4double eta_b = -eta_max+etaNum*eta_step;
    G4double eta_e = -eta_max+(etaNum+1)*eta_step;
    G4double theta_b = 2*atan(exp(-eta_b))*180/M_PI;
    G4double theta_e = 2*atan(exp(-eta_e))*180/M_PI;
    G4double theta_avg = 0.5*(theta_b+theta_e);
    G4double rMin = y/sin(theta_avg*M_PI/180);
    G4double rMax = rMin+dy;
    auto cellSolid 
      = new G4Sphere("cellSolid", rMin,rMax,phiNum*deg, 1.*deg, theta_e*deg, (theta_b-theta_e)*deg);
    
    cellEcalBarLogical[etaNum][phiNum]
      = new G4LogicalVolume(cellSolid,ecalMat,"cellLogical");
    
    new G4PVPlacement(0, G4ThreeVector(), cellEcalBarLogical[etaNum][phiNum], "ECalCell", ecalLogical, false, copyNo, false);
  }

  for(G4int copyNo=0; copyNo<nofEmBarCells; copyNo++) {
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0,0));
    visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    if(copyNo<360*30)   cellEcalBarLogical[copyNo/360][copyNo%360]->SetVisAttributes(visAttributes);
    else  cellEcalBarLogical[(copyNo+(nofEmBarPhi-360)*30)/nofEmBarPhi][(copyNo+(nofEmBarPhi-360)*30)%nofEmBarPhi]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
  
}

void ECalConstruction::makeEndCap(G4Material* ecalMat, G4LogicalVolume* ecalECLogical_r, G4LogicalVolume* ecalECLogical_l, bool ECalR, bool ECalL, std::vector<G4VisAttributes*> fVisAttributes) {
  
  // ECal End Cap Parameterised Placement of Crytals
  
  G4double rmin_EC = 3205*tan(2*atan(exp(-3))), xpos_EC, ypos_EC;
  G4double rmax_EC = 3205*tan(2*atan(exp(-1.479+0.04)));
  G4double y1=25.7, y2=26.9, len=220.0;
  int k = 0;
  for(int i=-70; i<70; i++) {
    for(int j=-70; j<70; j++) {
      if(k<kNofEmECCells) {
	G4double d1 = sqrt(pow(y2*(i+0.5),2)+pow(y2*(j+0.5),2));
	if(d1>rmin_EC && d1<rmax_EC) {
	  xpos_EC = y2*(i+0.5);
	  ypos_EC = y2*(j+0.5);
	  G4ThreeVector* d_r = new G4ThreeVector(xpos_EC, ypos_EC, (3205+110+1300));
	  auto cellEcalECSolid_r = new G4Trap("cellEcalECSolid_r",
					      0.5*len*mm,
					      d_r->getTheta()*rad,
					      d_r->getPhi()*rad,
					      0.5*y1*mm,
					      0.5*y1*mm,
					      0.5*y1*mm,
					      0*deg,
					      0.5*y2*mm,
					      0.5*y2*mm,
					      0.5*y2*mm,
					      0*deg);
	  cellEcalECLogical_r[k] = new G4LogicalVolume(cellEcalECSolid_r,
						       ecalMat,
						       "CellEcalECLogical_r");
	  G4ThreeVector* d_l = new G4ThreeVector(xpos_EC, ypos_EC, -(3205+110+1300));
	  auto cellEcalECSolid_l = new G4Trap("cellEcalECSolid_l",
					      0.5*len*mm,
					      d_l->getTheta()*rad,
					      d_l->getPhi()*rad,
					      0.5*y2*mm,
					      0.5*y2*mm,
					      0.5*y2*mm,
					      0*deg,
					      0.5*y1*mm,
					      0.5*y1*mm,
					      0.5*y1*mm,
					      0*deg);
	  cellEcalECLogical_l[k] = new G4LogicalVolume(cellEcalECSolid_l,
						       ecalMat,
						       "CellEcalECLogical_l");
	  if(ECalR)
	    new G4PVPlacement(0, G4ThreeVector(xpos_EC*mm, ypos_EC*mm, 0.), cellEcalECLogical_r[k], "CellEcalEC_r", ecalECLogical_r, false, k, false);
	  if(ECalL)
	    new G4PVPlacement(0, G4ThreeVector(xpos_EC*mm, ypos_EC*mm, 0.), cellEcalECLogical_l[k], "CellEcalEC_l", ecalECLogical_l, false, k, false);
	  k++;
	}
      }
    }
  }
  
  for(G4int copyNo=0; copyNo<kNofEmECCells; copyNo++) {
    visAttributes = new G4VisAttributes(G4Colour(0.9115,0.4112,0.0042));
    visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    cellEcalECLogical_r[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.9115,0.4112,0.0042));
    visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    cellEcalECLogical_l[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
}
