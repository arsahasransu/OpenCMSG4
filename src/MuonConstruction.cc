#include "MuonConstruction.hh"
#include "Constants.hh"

#include "G4Trap.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"

MuonConstruction::MuonConstruction()
  : chamberMuonBarLogical(NMuBarCh),
    chamberMuonECLogical_r(0), chamberMuonECLogical_l(0) {
}

MuonConstruction::~MuonConstruction() {
}

void MuonConstruction::makeBarrel(G4Material* muMat, G4LogicalVolume* muLogical, std::vector<G4VisAttributes*> fVisAttributes) {
  std::cout<<"Making the barrel for muon chambers"<<std::endl;
  
  for(G4int copyNo=0; copyNo<NMuBarCh; copyNo++) {

    G4int phiSegNum = copyNo%NMuBarPhiSeg;
    G4int zSegNum = (copyNo/NMuBarPhiSeg)%NMuBarZSeg;
    G4int rSegNum = (copyNo/NMuBarPhiSeg)/NMuBarZSeg;

    G4double dPhi = 2.0*acos(-1)/NMuBarPhiSeg;
    
    G4double pDz = 0.5*muBarZLen/NMuBarZSeg;
    G4double pTheta = 0.0;
    G4double pPhi = 0.0;
    G4double pDy1 = 0.5*muBardR[rSegNum];
    G4double pDy2 = 0.5*muBardR[rSegNum];
    G4double pDx1 = muBarR[rSegNum]*tan(0.5*dPhi);
    G4double pDx2 = (muBarR[rSegNum]+muBardR[rSegNum])*tan(0.5*dPhi);
    G4double pDx3 = muBarR[rSegNum]*tan(0.5*dPhi);
    G4double pDx4 = (muBarR[rSegNum]+muBardR[rSegNum])*tan(0.5*dPhi);
    G4double pAlp1 = 0.0*dPhi;
    G4double pAlp2 = 0.0*dPhi;

    //std::cout<<pDz<<"\t"<<pTheta<<"\t"<<pPhi<<"\t"<<pDy1<<"\t"<<pDy2<<"\t"<<pDx1<<"\t"<<pDx2<<"\t"<<pDx3<<"\t"<<pDx4<<"\t"<<pAlp1<<"\t"<<pAlp2<<std::endl;
    G4double xpos = (muBarR[rSegNum]+0.5*muBardR[rSegNum])*sin((phiSegNum)*dPhi);
    G4double ypos = (muBarR[rSegNum]+0.5*muBardR[rSegNum])*cos((phiSegNum)*dPhi);
    G4double zpos = -0.5*muBarZLen+(zSegNum+0.5)*muBarZLen/NMuBarZSeg;
    
    auto muBarSolid = new G4Trap("muBarSolid", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);

    chamberMuonBarLogical[copyNo] = new G4LogicalVolume(muBarSolid, muMat, "muonBarrelLogical");

    G4Rep3x3 rottemp(cos((phiSegNum)*dPhi),-sin((phiSegNum)*dPhi),0,  // matrix representation
		     sin((phiSegNum)*dPhi),cos((phiSegNum)*dPhi),0,  // (inverted)
		     0,0,1);
    G4RotationMatrix *rot = new G4RotationMatrix(rottemp);
    
    new G4PVPlacement(rot, G4ThreeVector(xpos,ypos,zpos), chamberMuonBarLogical[copyNo], "muonBarrelChamber", muLogical, false, copyNo, false);
    //new G4PVPlacement(0, G4ThreeVector(0,0,0), chamberMuonBarLogical[copyNo], "muonBarrelChamber", muLogical, false, copyNo, false);

    visAttributes = new G4VisAttributes(G4Colour(0.8888,0,0));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    chamberMuonBarLogical[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes); 
    
  }
  /*  
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
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    if(copyNo<360*30)   cellEcalBarLogical[copyNo/360][copyNo%360]->SetVisAttributes(visAttributes);
    else  cellEcalBarLogical[(copyNo+(nofEmBarPhi-360)*30)/nofEmBarPhi][(copyNo+(nofEmBarPhi-360)*30)%nofEmBarPhi]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
  */
}
