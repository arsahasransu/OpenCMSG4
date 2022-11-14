#include "MuonConstruction.hh"
#include "Constants.hh"

#include "G4Trap.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"

// Variables for muon chambers
constexpr G4int NMuBarZSeg = 5; // Default = 5
constexpr G4int NMuBarPhiSeg = 12; // Default = 12
constexpr G4int NMuBarRSeg = 4; // Default = 4
constexpr G4int NMuBarCh = NMuBarZSeg*NMuBarPhiSeg*NMuBarRSeg;

constexpr G4double muBarR[] = {4038*mm, 4964*mm, 6024*mm, 7010*mm}; // Dimension equal to NMuBarRSeg
constexpr G4double muBardR[] = {290*mm, 290*mm, 290*mm, 290*mm}; // Dimension equal to NMuBarRSeg
constexpr G4double muBarRPCbeforedR[] = {6*mm, 6*mm, 6*mm, 0};
constexpr G4double muBarRPCafterdR[] = {6*mm, 6*mm, 0, 6*mm};
//constexpr G4String muBarLayerName[] = {"MB1", "MB2", "MB3", "MB4"};

constexpr G4int NMuECZSeg = 8; // Default = 8: ME1/2, ME1/3, ME2/1, ME2/2, ME3/1, ME3/2, ME4/1, ME4/2
constexpr G4int NMuECPhiSeg[] = {36, 36, 18, 36, 18, 36, 18, 36}; // Dimension equal to NMuECZSeg
constexpr G4double muECRin[] = {2750*mm, 5055*mm, 1385*mm, 3575*mm, 1605*mm, 3575*mm, 1785*mm, 3575*mm}; // Dimension equal to NMuECZSeg
constexpr G4double muECRout[] = {4550*mm, 6955*mm, 3450*mm, 6955*mm, 3450*mm, 6955*mm, 3450*mm, 6955*mm}; // Dimension equal to NMuECZSeg
constexpr G4double muECZpos[] = {6683*mm, 6880*mm, 7975*mm, 7975*mm, 9175*mm, 9175*mm, 10040*mm, 10040*mm}; // Dimension equal to NMuECZSeg
constexpr G4double muEChalfZwid[] = {250*mm, 125*mm, 250*mm, 250*mm, 250*mm, 250*mm, 250*mm, 250*mm}; // Dimension equal to NMuECZSeg

constexpr G4int NMuME11ZSeg = 2; // Default = 2
constexpr G4int NMuME11PhiSeg[] = {36, 36}; // Dimension equal to NMuME11ZSeg
constexpr G4double muME11Rin[] = {860*mm, 860*mm}; // Dimension equal to NMuME11ZSeg
constexpr G4double muME11Rout[] = {2540*mm, 2540*mm}; // Dimension equal to NMuME11ZSeg
constexpr G4double muME11Zpos[] = {5740*mm, 6050*mm}; // Dimension equal to NMuME11ZSeg
constexpr G4double muME11halfZwid[] = {74*mm, 74*mm}; // Dimension equal to NMuME11ZSeg

MuonConstruction::MuonConstruction()
  : chamberMuonBarLogical(NMuBarCh),
    chamberMuonBarRPCBeforeLogical(NMuBarCh), chamberMuonBarRPCAfterLogical(NMuBarCh) {
}

MuonConstruction::~MuonConstruction() {
}

void MuonConstruction::makeBarrel(G4Material* muMat, G4LogicalVolume* muLogical, std::vector<G4VisAttributes*> fVisAttributes) {
  std::cout<<"Making the barrel for muon chambers"<<std::endl;
  
  for(G4int copyNo=0; copyNo<NMuBarCh; copyNo++) {

    G4int phiSegNum = copyNo%NMuBarPhiSeg;
    G4int zSegNum = (copyNo/NMuBarPhiSeg)%NMuBarZSeg;
    G4int rSegNum = (copyNo/NMuBarPhiSeg)/NMuBarZSeg;

    G4double dPhi = 2*acos(-1)/NMuBarPhiSeg;
    
    G4double pDz = muBarZHalfLen;
    G4double pTheta = 0.0;
    G4double pPhi = 0.0;
    G4double pDy1 = 0.5*muBardR[rSegNum];
    G4double pDy2 = 0.5*muBardR[rSegNum];
    G4double pDyRPCbefore1 = 0.5*muBarRPCbeforedR[rSegNum];
    G4double pDyRPCbefore2 = 0.5*muBarRPCbeforedR[rSegNum];
    G4double pDyRPCafter1 = 0.5*muBarRPCafterdR[rSegNum];
    G4double pDyRPCafter2 = 0.5*muBarRPCafterdR[rSegNum];
    G4double pDxRPCbefore1 = (muBarR[rSegNum]-muBarRPCbeforedR[rSegNum])*tan(0.5*dPhi);
    G4double pDx1 = muBarR[rSegNum]*tan(0.5*dPhi);
    G4double pDx2 = (muBarR[rSegNum]+muBardR[rSegNum])*tan(0.5*dPhi);
    G4double pDxRPCafter2 = (muBarR[rSegNum]+muBardR[rSegNum]+muBarRPCafterdR[rSegNum])*tan(0.5*dPhi);
    G4double pDxRPCbefore3 = (muBarR[rSegNum]-muBarRPCbeforedR[rSegNum])*tan(0.5*dPhi);
    G4double pDx3 = muBarR[rSegNum]*tan(0.5*dPhi);
    G4double pDx4 = (muBarR[rSegNum]+muBardR[rSegNum])*tan(0.5*dPhi);
    G4double pDxRPCafter4 = (muBarR[rSegNum]+muBardR[rSegNum]+muBarRPCafterdR[rSegNum])*tan(0.5*dPhi);
    G4double pAlp1 = 0.0*dPhi;
    G4double pAlp2 = 0.0*dPhi;

    G4double xpos = (muBarR[rSegNum]+0.5*muBardR[rSegNum])*cos((phiSegNum)*dPhi);
    G4double ypos = (muBarR[rSegNum]+0.5*muBardR[rSegNum])*sin((phiSegNum)*dPhi);
    G4double xRPCbeforepos = (muBarR[rSegNum]-0.5*muBarRPCbeforedR[rSegNum])*cos((phiSegNum)*dPhi);
    G4double yRPCbeforepos = (muBarR[rSegNum]-0.5*muBarRPCbeforedR[rSegNum])*sin((phiSegNum)*dPhi);
    G4double xRPCafterpos = (muBarR[rSegNum]+muBardR[rSegNum]+0.5*muBarRPCafterdR[rSegNum])*cos((phiSegNum)*dPhi);
    G4double yRPCafterpos = (muBarR[rSegNum]+muBardR[rSegNum]+0.5*muBarRPCafterdR[rSegNum])*sin((phiSegNum)*dPhi);
    G4double gapMuBarCumu = 0;
    for(int ctr=0; ctr<zSegNum; ctr++) {
      gapMuBarCumu += gapMuBar[ctr];
    }
    G4double zpos = -muBarEnvelopZHalfLen+(2*zSegNum+1)*muBarZHalfLen+gapMuBarCumu;
    
    auto muBarSolid = new G4Trap("muBarSolid", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);

    chamberMuonBarLogical[copyNo] = new G4LogicalVolume(muBarSolid, muMat, "muonBarrelLogical");

    G4Rep3x3 rottemp(cos(2*atan(1)-(phiSegNum)*dPhi),-sin(2*atan(1)-(phiSegNum)*dPhi),0,  // matrix representation
		     sin(2*atan(1)-(phiSegNum)*dPhi),cos(2*atan(1)-(phiSegNum)*dPhi),0,  // (inverted)
		     0,0,1);
    G4RotationMatrix *rot = new G4RotationMatrix(rottemp);
    
    new G4PVPlacement(rot, G4ThreeVector(xpos,ypos,zpos), chamberMuonBarLogical[copyNo], "MB1", muLogical, false, copyNo, true);

    // Place an RPC before muon barrel
    if(muBarRPCbeforedR[rSegNum]>0) {
      auto muBarRPCbeforeSolid = new G4Trap("muBarRPCbeforeSolid", pDz, pTheta, pPhi, pDyRPCbefore1, pDxRPCbefore1, pDx1, pAlp1, pDyRPCbefore2, pDxRPCbefore3, pDx3, pAlp2);
      chamberMuonBarRPCBeforeLogical[copyNo] = new G4LogicalVolume(muBarRPCbeforeSolid, muMat, "muonBarrelRPCbeforeLogical");
      new G4PVPlacement(rot, G4ThreeVector(xRPCbeforepos,yRPCbeforepos,zpos), chamberMuonBarRPCBeforeLogical[copyNo], "RPCbeforeMuonBarrel", muLogical, false, copyNo, true);
    }
    // Place an RPC after muon barrel
    if(muBarRPCafterdR[rSegNum]>0) {
      auto muBarRPCafterSolid = new G4Trap("muBarRPCafterSolid", pDz, pTheta, pPhi, pDyRPCafter1, pDx2, pDxRPCafter2, pAlp1, pDyRPCafter2, pDx4, pDxRPCafter4, pAlp2);
      chamberMuonBarRPCAfterLogical[copyNo] = new G4LogicalVolume(muBarRPCafterSolid, muMat, "muonBarrelRPCafterLogical");
      new G4PVPlacement(rot, G4ThreeVector(xRPCafterpos,yRPCafterpos,zpos), chamberMuonBarRPCAfterLogical[copyNo], "RPCafterMuonBarrel", muLogical, false, copyNo, true);
    }
    
    visAttributes = new G4VisAttributes(G4Colour(0.698,0.792,0.757));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    chamberMuonBarLogical[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes); 
    
    if(muBarRPCbeforedR[rSegNum]>0) {
      visAttributes = new G4VisAttributes(G4Colour(0.698,0.792,0.757));
      //visAttributes->SetVisibility(false);
      //visAttributes->SetForceLineSegmentsPerCircle(10);
      chamberMuonBarRPCBeforeLogical[copyNo]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
    }
    
    if(muBarRPCafterdR[rSegNum]>0) {
      visAttributes = new G4VisAttributes(G4Colour(0.698,0.792,0.757));
      //visAttributes->SetVisibility(false);
      //visAttributes->SetForceLineSegmentsPerCircle(10);
      chamberMuonBarRPCAfterLogical[copyNo]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
    }
    
  }
}

void MuonConstruction::makeEndCap_posz(G4Material* muMat, G4LogicalVolume* muLogical, bool, std::vector<G4VisAttributes*> fVisAttributes) {

  std::cout<<"Making the right endcap for muon chambers"<<std::endl;

  G4int copyNo = 0;
  for(G4int zNo=0; zNo<NMuECZSeg; zNo++) {

    for(G4int phiNo=0; phiNo<NMuECPhiSeg[zNo]; phiNo++) {

      std::cout<<copyNo<<"\t || \t";
      
      G4double dPhi = 2*acos(-1)/NMuECPhiSeg[zNo];
      
      G4double pDz = muEChalfZwid[zNo];
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
      G4double xpos = 0.5*(muECRin[zNo]+muECRout[zNo])*cos((phiNo+0.5)*dPhi);
      G4double ypos = 0.5*(muECRin[zNo]+muECRout[zNo])*sin((phiNo+0.5)*dPhi);
      G4double zpos = muECZpos[zNo]-0.5*(muECZout+muECZin)+muEChalfZwid[zNo];
      
      auto muECSolid = new G4Trap("muECSolid", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);

      chamberMuonECLogical_r.push_back(new G4LogicalVolume(muECSolid, muMat, "muonECLogical_r"));
      
      G4Rep3x3 rottemp(cos(2*atan(1)-(phiNo+0.5)*dPhi),-sin(2*atan(1)-(phiNo+0.5)*dPhi),0,  // matrix representation
		       sin(2*atan(1)-(phiNo+0.5)*dPhi),cos(2*atan(1)-(phiNo+0.5)*dPhi),0,  // (inverted)
		       0,0,1);
      G4RotationMatrix *rot = new G4RotationMatrix(rottemp);
      
      new G4PVPlacement(rot, G4ThreeVector(xpos,ypos,zpos), chamberMuonECLogical_r[copyNo], "muonECChamber_r", muLogical, false, copyNo, true);
      
      visAttributes = new G4VisAttributes(G4Colour(0.698,0.792,0.757));
      //visAttributes->SetVisibility(false);
      //visAttributes->SetForceLineSegmentsPerCircle(10);
      chamberMuonECLogical_r[copyNo]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes); 

      copyNo++;
    }
  }
  
}

void MuonConstruction::makeEndCap_negz(G4Material* muMat, G4LogicalVolume* muLogical, bool, std::vector<G4VisAttributes*> fVisAttributes) {

  std::cout<<"Making the right endcap for muon chambers"<<std::endl;

  G4int copyNo = 0;
  for(G4int zNo=0; zNo<NMuECZSeg; zNo++) {

    for(G4int phiNo=0; phiNo<NMuECPhiSeg[zNo]; phiNo++) {

      std::cout<<copyNo<<"\t || \t";
      
      G4double dPhi = 2*acos(-1)/NMuECPhiSeg[zNo];
      
      G4double pDz = muEChalfZwid[zNo];
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
      G4double xpos = 0.5*(muECRin[zNo]+muECRout[zNo])*cos((phiNo+0.5)*dPhi);
      G4double ypos = 0.5*(muECRin[zNo]+muECRout[zNo])*sin((phiNo+0.5)*dPhi);
      G4double zpos = -muECZpos[zNo]+0.5*(muECZout+muECZin)-muEChalfZwid[zNo];
      
      auto muECSolid = new G4Trap("muECSolid", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);

      chamberMuonECLogical_l.push_back(new G4LogicalVolume(muECSolid, muMat, "muonECLogical_l"));
      
      G4Rep3x3 rottemp(cos(2*atan(1)-(phiNo+0.5)*dPhi),-sin(2*atan(1)-(phiNo+0.5)*dPhi),0,  // matrix representation
		       sin(2*atan(1)-(phiNo+0.5)*dPhi),cos(2*atan(1)-(phiNo+0.5)*dPhi),0,  // (inverted)
		       0,0,1);
      G4RotationMatrix *rot = new G4RotationMatrix(rottemp);
      
      new G4PVPlacement(rot, G4ThreeVector(xpos,ypos,zpos), chamberMuonECLogical_l[copyNo], "muonECChamber_l", muLogical, false, copyNo, true);
      
      visAttributes = new G4VisAttributes(G4Colour(0.698,0.792,0.757));
      //visAttributes->SetVisibility(false);
      //visAttributes->SetForceLineSegmentsPerCircle(10);
      chamberMuonECLogical_l[copyNo]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes); 

      copyNo++;
    }
  }
  
}

void MuonConstruction::makeME11_posz(G4Material* muMat, G4LogicalVolume* muLogical, bool, std::vector<G4VisAttributes*> fVisAttributes) {
  
  G4int copyNo = 0;
  for(G4int zNo=0; zNo<NMuME11ZSeg; zNo++) {

    for(G4int phiNo=0; phiNo<NMuME11PhiSeg[zNo]; phiNo++) {

      std::cout<<copyNo<<"\t || \t";
      
      G4double dPhi = 2*acos(-1)/NMuME11PhiSeg[zNo];
      
      G4double pDz = muME11halfZwid[zNo];
      G4double pTheta = 0.0;
      G4double pPhi = 0.0;
      G4double pDy1 = 0.5*(muME11Rout[zNo]-muME11Rin[zNo]);
      G4double pDy2 = 0.5*(muME11Rout[zNo]-muME11Rin[zNo]);
      G4double pDx1 = muME11Rin[zNo]*tan(0.5*dPhi);
      G4double pDx2 = muME11Rout[zNo]*tan(0.5*dPhi);
      G4double pDx3 = muME11Rin[zNo]*tan(0.5*dPhi);
      G4double pDx4 = muME11Rout[zNo]*tan(0.5*dPhi);
      G4double pAlp1 = 0.0*dPhi;
      G4double pAlp2 = 0.0*dPhi;
    
      //std::cout<<pDz<<"\t"<<pTheta<<"\t"<<pPhi<<"\t"<<pDy1<<"\t"<<pDy2<<"\t"<<pDx1<<"\t"<<pDx2<<"\t"<<pDx3<<"\t"<<pDx4<<"\t"<<pAlp1<<"\t"<<pAlp2<<std::endl;
      G4double xpos = 0.5*(muME11Rin[zNo]+muME11Rout[zNo])*cos((phiNo+0.5)*dPhi);
      G4double ypos = 0.5*(muME11Rin[zNo]+muME11Rout[zNo])*sin((phiNo+0.5)*dPhi);
      G4double zpos = muME11Zpos[zNo]-0.5*(muECZin+muME11Zin)+muME11halfZwid[zNo];
      
      auto muME11Solid = new G4Trap("muME11Solid", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);

      chamberMuonME11Logical_r.push_back(new G4LogicalVolume(muME11Solid, muMat, "muonME11Logical_r"));
      
      G4Rep3x3 rottemp(cos(2*atan(1)-(phiNo+0.5)*dPhi),-sin(2*atan(1)-(phiNo+0.5)*dPhi),0,  // matrix representation
		       sin(2*atan(1)-(phiNo+0.5)*dPhi),cos(2*atan(1)-(phiNo+0.5)*dPhi),0,  // (inverted)
		       0,0,1);
      G4RotationMatrix *rot = new G4RotationMatrix(rottemp);
      
      new G4PVPlacement(rot, G4ThreeVector(xpos,ypos,zpos), chamberMuonME11Logical_r[copyNo], "muonME11Chamber_r", muLogical, false, copyNo, true);
      
      visAttributes = new G4VisAttributes(G4Colour(0.698,0.792,0.757));
      //visAttributes->SetVisibility(false);
      //visAttributes->SetForceLineSegmentsPerCircle(10);
      chamberMuonME11Logical_r[copyNo]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes); 

      copyNo++;
    }
  }
  
}

void MuonConstruction::makeME11_negz(G4Material* muMat, G4LogicalVolume* muLogical, bool, std::vector<G4VisAttributes*> fVisAttributes) {
  
  G4int copyNo = 0;
  for(G4int zNo=0; zNo<NMuME11ZSeg; zNo++) {

    for(G4int phiNo=0; phiNo<NMuME11PhiSeg[zNo]; phiNo++) {

      std::cout<<copyNo<<"\t || \t";
      
      G4double dPhi = 2*acos(-1)/NMuME11PhiSeg[zNo];
      
      G4double pDz = muME11halfZwid[zNo];
      G4double pTheta = 0.0;
      G4double pPhi = 0.0;
      G4double pDy1 = 0.5*(muME11Rout[zNo]-muME11Rin[zNo]);
      G4double pDy2 = 0.5*(muME11Rout[zNo]-muME11Rin[zNo]);
      G4double pDx1 = muME11Rin[zNo]*tan(0.5*dPhi);
      G4double pDx2 = muME11Rout[zNo]*tan(0.5*dPhi);
      G4double pDx3 = muME11Rin[zNo]*tan(0.5*dPhi);
      G4double pDx4 = muME11Rout[zNo]*tan(0.5*dPhi);
      G4double pAlp1 = 0.0*dPhi;
      G4double pAlp2 = 0.0*dPhi;
    
      //std::cout<<pDz<<"\t"<<pTheta<<"\t"<<pPhi<<"\t"<<pDy1<<"\t"<<pDy2<<"\t"<<pDx1<<"\t"<<pDx2<<"\t"<<pDx3<<"\t"<<pDx4<<"\t"<<pAlp1<<"\t"<<pAlp2<<std::endl;
      G4double xpos = 0.5*(muME11Rin[zNo]+muME11Rout[zNo])*cos((phiNo+0.5)*dPhi);
      G4double ypos = 0.5*(muME11Rin[zNo]+muME11Rout[zNo])*sin((phiNo+0.5)*dPhi);
      G4double zpos = -muME11Zpos[zNo]+0.5*(muECZin+muME11Zin)-muME11halfZwid[zNo];
      
      auto muME11Solid = new G4Trap("muME11Solid", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);

      chamberMuonME11Logical_l.push_back(new G4LogicalVolume(muME11Solid, muMat, "muonME11Logical_l"));
      
      G4Rep3x3 rottemp(cos(2*atan(1)-(phiNo+0.5)*dPhi),-sin(2*atan(1)-(phiNo+0.5)*dPhi),0,  // matrix representation
		       sin(2*atan(1)-(phiNo+0.5)*dPhi),cos(2*atan(1)-(phiNo+0.5)*dPhi),0,  // (inverted)
		       0,0,1);
      G4RotationMatrix *rot = new G4RotationMatrix(rottemp);
      
      new G4PVPlacement(rot, G4ThreeVector(xpos,ypos,zpos), chamberMuonME11Logical_l[copyNo], "muonME11Chamber_l", muLogical, false, copyNo, true);
      
      visAttributes = new G4VisAttributes(G4Colour(0.698,0.792,0.757));
      //visAttributes->SetVisibility(false);
      //visAttributes->SetForceLineSegmentsPerCircle(10);
      chamberMuonME11Logical_l[copyNo]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes); 

      copyNo++;
    }
  }
  
}
