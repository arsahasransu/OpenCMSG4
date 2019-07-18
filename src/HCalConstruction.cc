#include "HCalConstruction.hh"
#include "Constants.hh"

#include "G4Tubs.hh"
#include "G4PVPlacement.hh"

HCalConstruction::HCalConstruction()
  : cellHcalBarLogical(nofHcalBarEta) {
  for(G4int etaNum=0; etaNum<nofHcalBarEta; etaNum++) {
    std::vector< std::vector<G4LogicalVolume*> > tempR(nofHcalBarR);
    for(G4int rNum=0; rNum<nofHcalBarR; rNum++) {
      std::vector<G4LogicalVolume*> tempPhi(nofHcalBarPhi);
      tempR[rNum] = tempPhi;
    }
    cellHcalBarLogical[etaNum] = tempR;
  }
}

HCalConstruction::~HCalConstruction() {
}

void HCalConstruction::makeBarrel(G4Material* hcal_absMat, G4Material* hcal_scnMat, G4LogicalVolume* hcalLogical, std::vector<G4VisAttributes*> fVisAttributes) {
  std::cout<<"Making the barrel for hadronic calorimeter"<<std::endl;

  for(int copyNo=0; copyNo<nofHcalBarCells; copyNo++) {
    G4int etaNum = copyNo/(nofHcalBarPhi*nofHcalBarR);
    G4int phiNum = (copyNo%(nofHcalBarPhi*nofHcalBarR))/nofHcalBarPhi;
    G4int rNum = (copyNo%(nofHcalBarPhi*nofHcalBarR))%nofHcalBarPhi;

    G4double maxEta = 1.479;
    G4double diffEta = 2*maxEta/nofHcalBarEta;
    G4double minR = 2000*mm;
    G4double diffR = 100*mm;

    G4double eta_b = -maxEta+etaNum*diffEta;
    G4double eta_e = -maxEta+(etaNum+1)*diffEta;
    G4double theta_b = 2*atan(exp(-eta_b))*180/M_PI;
    G4double theta_e = 2*atan(exp(-eta_e))*180/M_PI;

    G4double R_b = minR+diffR*rNum;
    G4double R_e = R_b+diffR;

    G4double z_bb = R_b/sin(theta_b);
    G4double z_eb = R_e/sin(theta_b);
    G4double z_bAvg = 0.5*(z_bb+z_eb);
    G4double z_be = R_b/sin(theta_e);
    G4double z_ee = R_e/sin(theta_e);
    G4double z_eAvg = 0.5*(z_be+z_ee);
    
    G4double diffAng = 360*deg/nofHcalBarPhi;
    G4double sAng = diffAng*nofHcalBarPhi;

    auto hcalAbsSolid = new G4Tubs("HCalAbsorberSingleCell",
				   R_b,
				   R_e,
				   0.5*(z_eAvg-z_bAvg),
				   sAng,
				   diffAng);

    cellHcalBarLogical[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalAbsSolid, hcal_absMat, "cellHCalAbsorberLogical");

    new G4PVPlacement(0, G4ThreeVector(0,0,0.5*(z_eAvg+z_bAvg)), cellHcalBarLogical[etaNum][rNum][phiNum], "HCalAbsorberCell", hcalLogical, false, copyNo, false);

  }
}
