#include "MuonConstruction.hh"
#include "Constants.hh"

#include "G4Trap.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"

MuonConstruction::MuonConstruction()
  : chamberMuonBarLogical(NMuBarCh),
    chamberMuonECLogical_r(NMuECZSeg*NMuECPhiSeg), chamberMuonECLogical_l(NMuECZSeg*NMuECPhiSeg) {
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
    
    G4double pDz = 0.5*muBarZLen[rSegNum]/NMuBarZSeg;
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
    G4double zpos = -0.5*muBarZLen[rSegNum]+(zSegNum+0.5)*muBarZLen[rSegNum]/NMuBarZSeg;
    
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
}

void MuonConstruction::makeEndCap_posz(G4Material* muMat, G4LogicalVolume* muLogical, bool, std::vector<G4VisAttributes*> fVisAttributes) {

  std::cout<<"Making the right endcap for muon chambers"<<std::endl;
  
  for(G4int zNo=0; zNo<NMuECZSeg; zNo++) {

    for(G4int phiNo=0; phiNo<NMuECPhiSeg; phiNo++) {

      G4int copyNo = zNo*NMuECPhiSeg+phiNo;
      std::cout<<phiNo<<"\t || \t";
      
      G4double dPhi = 2.0*acos(-1)/NMuECPhiSeg;
      
      G4double pDz = 0.5*muEChalfZwid[zNo];
      G4double pTheta = 0.0;
      G4double pPhi = 0.0;
      G4double pDy1 = 0.5*(muECRout[zNo]-muECRin[zNo]);
      G4double pDy2 = 0.5*(muECRout[zNo]-muECRin[zNo]);
      G4double pDx1 = muECRin[zNo]*tan(0.5*dPhi);
      G4double pDx2 = muECRout[zNo]*tan(0.5*dPhi);
      G4double pDx3 = muECRin[zNo]*tan(0.5*dPhi);
      G4double pDx4 = muECRout[zNo]*tan(0.5*dPhi);
      G4double pAlp1 = 0.0*dPhi;
      G4double pAlp2 = 0.0*dPhi;
    
      //std::cout<<pDz<<"\t"<<pTheta<<"\t"<<pPhi<<"\t"<<pDy1<<"\t"<<pDy2<<"\t"<<pDx1<<"\t"<<pDx2<<"\t"<<pDx3<<"\t"<<pDx4<<"\t"<<pAlp1<<"\t"<<pAlp2<<std::endl;
      G4double xpos = 0.5*(muECRin[zNo]+muECRout[zNo])*sin(phiNo*dPhi);
      G4double ypos = 0.5*(muECRin[zNo]+muECRout[zNo])*cos(phiNo*dPhi);
      G4double zpos = muECZpos[zNo];
    
      auto muECSolid = new G4Trap("muECSolid", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);

    chamberMuonECLogical_r[copyNo] = new G4LogicalVolume(muECSolid, muMat, "muonECLogical_r");

    G4Rep3x3 rottemp(cos((phiNo)*dPhi),-sin((phiNo)*dPhi),0,  // matrix representation
		     sin((phiNo)*dPhi),cos((phiNo)*dPhi),0,  // (inverted)
		     0,0,1);
    G4RotationMatrix *rot = new G4RotationMatrix(rottemp);
    
    new G4PVPlacement(rot, G4ThreeVector(xpos,ypos,zpos-0.5*(0.5*world_sizeZ+solenoidHalfZ)), chamberMuonECLogical_r[copyNo], "muonECChamber_r", muLogical, false, copyNo, true);
    //new G4PVPlacement(0, G4ThreeVector(0,0,0), chamberMuonBarLogical[copyNo], "muonBarrelChamber", muLogical, false, copyNo, false);

    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    chamberMuonECLogical_r[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes); 
    
    }
  }
  
}

void MuonConstruction::makeEndCap_negz(G4Material* muMat, G4LogicalVolume* muLogical, bool, std::vector<G4VisAttributes*> fVisAttributes) {

  std::cout<<"Making the left endcap for muon chambers"<<std::endl;
  
  for(G4int zNo=0; zNo<NMuECZSeg; zNo++) {

    for(G4int phiNo=0; phiNo<NMuECPhiSeg; phiNo++) {

      G4int copyNo = zNo*NMuECPhiSeg+phiNo;
      std::cout<<phiNo<<"\t || \t";
      
      G4double dPhi = 2.0*acos(-1)/NMuECPhiSeg;
      
      G4double pDz = 0.5*muEChalfZwid[zNo];
      G4double pTheta = 0.0;
      G4double pPhi = 0.0;
      G4double pDy1 = 0.5*(muECRout[zNo]-muECRin[zNo]);
      G4double pDy2 = 0.5*(muECRout[zNo]-muECRin[zNo]);
      G4double pDx1 = muECRin[zNo]*tan(0.5*dPhi);
      G4double pDx2 = muECRout[zNo]*tan(0.5*dPhi);
      G4double pDx3 = muECRin[zNo]*tan(0.5*dPhi);
      G4double pDx4 = muECRout[zNo]*tan(0.5*dPhi);
      G4double pAlp1 = 0.0*dPhi;
      G4double pAlp2 = 0.0*dPhi;
    
      //std::cout<<pDz<<"\t"<<pTheta<<"\t"<<pPhi<<"\t"<<pDy1<<"\t"<<pDy2<<"\t"<<pDx1<<"\t"<<pDx2<<"\t"<<pDx3<<"\t"<<pDx4<<"\t"<<pAlp1<<"\t"<<pAlp2<<std::endl;
      G4double xpos = 0.5*(muECRin[zNo]+muECRout[zNo])*sin(phiNo*dPhi);
      G4double ypos = 0.5*(muECRin[zNo]+muECRout[zNo])*cos(phiNo*dPhi);
      G4double zpos = -muECZpos[zNo];
    
      auto muECSolid = new G4Trap("muECSolid", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);

    chamberMuonECLogical_l[copyNo] = new G4LogicalVolume(muECSolid, muMat, "muonECLogical_l");

    G4Rep3x3 rottemp(cos((phiNo)*dPhi),-sin((phiNo)*dPhi),0,  // matrix representation
		     sin((phiNo)*dPhi),cos((phiNo)*dPhi),0,  // (inverted)
		     0,0,1);
    G4RotationMatrix *rot = new G4RotationMatrix(rottemp);
    
    new G4PVPlacement(rot, G4ThreeVector(xpos,ypos,zpos+0.5*(0.5*world_sizeZ+solenoidHalfZ)), chamberMuonECLogical_l[copyNo], "muonECChamber_l", muLogical, false, copyNo, true);
    //new G4PVPlacement(0, G4ThreeVector(0,0,0), chamberMuonBarLogical[copyNo], "muonBarrelChamber", muLogical, false, copyNo, false);

    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    chamberMuonECLogical_l[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes); 
    
    }
  }
  
}
