#include "HCalConstruction.hh"
#include "Constants.hh"

#include "G4Tubs.hh"
#include "G4PVPlacement.hh"

HCalConstruction::HCalConstruction()
  : cellHcalBarAbsLogical(nofHcalBarEta),
    cellHcalBarScnLogical(nofHcalBarEta),
    cellHcalECAbsLogical_r1(4),
    cellHcalECAbsLogical_l1(4),
    cellHcalECScnLogical_r1(4),
    cellHcalECScnLogical_l1(4),
    cellHcalECAbsLogical_r2(8),
    cellHcalECAbsLogical_l2(8),
    cellHcalECScnLogical_r2(8),
    cellHcalECScnLogical_l2(8){

    for(G4int etaNum=0; etaNum<nofHcalBarEta; etaNum++) {
    std::vector< std::vector<G4LogicalVolume*> > tempR(nofHcalBarR);
    for(G4int rNum=0; rNum<nofHcalBarR; rNum++) {
      std::vector<G4LogicalVolume*> tempPhi(nofHcalBarPhi);
      tempR[rNum] = tempPhi;
    }
    cellHcalBarAbsLogical[etaNum] = tempR;
    cellHcalBarScnLogical[etaNum] = tempR;
  }

    std::cout<<"HCal Barrel Initialization complete"<<std::endl;
  
  for(G4int etaNum=0; etaNum<4; etaNum++) {
    std::vector< std::vector<G4LogicalVolume*> > tempR(nofHcalBarR);
    for(G4int rNum=0; rNum<nofHcalEndCapR; rNum++) {
      std::vector<G4LogicalVolume*> tempPhi(nofHcalECOPhi);
      tempR[rNum] = tempPhi;
    }
    cellHcalECAbsLogical_r1[etaNum] = tempR;
    cellHcalECScnLogical_r1[etaNum] = tempR;
    cellHcalECAbsLogical_l1[etaNum] = tempR;
    cellHcalECScnLogical_l1[etaNum] = tempR;
  }
  std::cout<<"HCal EC Outer(ieta=17,18,19,20) Initialization complete"<<std::endl;

    for(G4int etaNum=0; etaNum<8; etaNum++) {
    std::vector< std::vector<G4LogicalVolume*> > tempR(nofHcalBarR);
    for(G4int rNum=0; rNum<nofHcalEndCapR; rNum++) {
      std::vector<G4LogicalVolume*> tempPhi(nofHcalECOPhi);
      tempR[rNum] = tempPhi;
    }
    cellHcalECAbsLogical_r2[etaNum] = tempR;
    cellHcalECScnLogical_r2[etaNum] = tempR;
    cellHcalECAbsLogical_l2[etaNum] = tempR;
    cellHcalECScnLogical_l2[etaNum] = tempR;
  }
  std::cout<<"HCal EC Inner(ieta 21-28) Initialization complete"<<std::endl;
  

}

HCalConstruction::~HCalConstruction() {
}

void HCalConstruction::makeBarrel(G4Material* hcal_absMat,
				  G4Material* hcal_scnMat,
				  G4LogicalVolume* hcalLogical,
				  std::vector<G4VisAttributes*> fVisAttributes) {
  
  std::cout<<"Making the barrel for hadronic calorimeter"<<std::endl;
  
    G4double maxEta = 1.305;
    G4double diffEta = maxEta/15;	// Barrel and part of EndCap
    G4double minR = 1790*mm;
    
    for(int copyNo=0; copyNo<nofHcalBarCells; copyNo++) {
      G4int etaNum = copyNo/(nofHcalBarPhi*nofHcalBarR);
      G4int rNum = (copyNo%(nofHcalBarPhi*nofHcalBarR))/nofHcalBarPhi;
      G4int phiNum = (copyNo%(nofHcalBarPhi*nofHcalBarR))%nofHcalBarPhi;
      
      G4double eta_b = -maxEta+etaNum*diffEta;
      G4double eta_e = -maxEta+(etaNum+1)*diffEta;
      G4double theta_b = 2*atan(exp(-eta_b))*180/M_PI;
      G4double theta_e = 2*atan(exp(-eta_e))*180/M_PI;
      
      // TODO: Need to change the eta to z conversion to either the bottom layer
      //       or the middle layer
      G4double R_bAbs = minR+diffRAbsBar*rNum+diffRScnBar*rNum;
      G4double R_eAbs = R_bAbs+diffRAbsBar;
      G4double R_eScn = R_eAbs+diffRScnBar;
      
      G4double z_bb = R_bAbs/tan(theta_b*M_PI/180);
      G4double z_eb = R_eAbs/tan(theta_b*M_PI/180);
      G4double z_bAvg = 0.5*(z_bb+z_eb);
      G4double z_be = R_bAbs/tan(theta_e*M_PI/180);
      G4double z_ee = R_eAbs/tan(theta_e*M_PI/180);
      G4double z_eAvg = 0.5*(z_be+z_ee);
      
      G4double diffAng = 5*deg;	// Barrel and part of EndCap
      G4double sAng = diffAng*phiNum;
      
      auto hcalAbsSolid = new G4Tubs("HCalAbsorberSingleCell",
				     R_bAbs,
				     R_eAbs,
				     0.5*abs(z_eAvg-z_bAvg),
				     sAng,
				     diffAng);
      
      auto hcalScnSolid = new G4Tubs("HCalScintillatorSingleCell",
				     R_eAbs,
				     R_eScn,
				     0.5*abs(z_eAvg-z_bAvg),
				     sAng,
				     diffAng);
      
      cellHcalBarAbsLogical[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalAbsSolid,
									hcal_absMat,
									"cellHCalAbsorberLogical");
      
      cellHcalBarScnLogical[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalScnSolid,
									hcal_scnMat,
									"cellHCalScintillatorLogical");
      
      G4double dispCell = 0.5*(z_eAvg+z_bAvg);
      
      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCell),
			cellHcalBarAbsLogical[etaNum][rNum][phiNum],
			"HCalAbsorberCell",
			hcalLogical,
			false,
			copyNo,
			false);
      
      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCell),
			cellHcalBarScnLogical[etaNum][rNum][phiNum],
			"HCalScintillatorCell",
			hcalLogical,
			false,
			copyNo,
			false);
      
      visAttributes = new G4VisAttributes(G4Colour(0.00,0.00,0.50));
      visAttributes->SetVisibility(false);
      cellHcalBarAbsLogical[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
      visAttributes = new G4VisAttributes(G4Colour(0.00,0.50,0.00));
      visAttributes->SetVisibility(false);
      cellHcalBarScnLogical[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
    }
}

void HCalConstruction::makeEndCapOuter(G4Material* hcal_absMat,
				  G4Material* hcal_scnMat,
			      G4LogicalVolume* hcalECOLogical_r,
				  G4LogicalVolume* hcalECOLogical_l,
				  std::vector<G4VisAttributes*> fVisAttributes) {
  
  std::cout<<"Making the end cap (outer) for hadronic calorimeter"<<std::endl;
// Right side end cap (for the time being)

    G4double diffEta = (HcalEC17to20EtaIn-HcalEC17to20EtaOut)/4;	// Barrel and part of EndCap
    //G4double diffRAbs = 100*mm;
    //G4double diffRScn = 10*mm;
    G4double maxEta = 1.392;
    
    for(int copyNo=0; copyNo<nofHcalEndCapR*nofHcalECOPhi*4; copyNo++) {            //4 is for 4(17,18,19,20) ieta region in endcap as they have same delta eta, delta phi 
      
      G4int etaNum = copyNo/(nofHcalECOPhi*nofHcalEndCapR);
      G4int rNum = (copyNo%(nofHcalECOPhi*nofHcalEndCapR))/nofHcalECOPhi;
      G4int phiNum = (copyNo%(nofHcalECOPhi*nofHcalEndCapR))%nofHcalECOPhi;
      
      //G4double eta_b = -maxEta+etaNum*diffEta;
      //G4double eta_e = -maxEta+(etaNum+1)*diffEta;

      G4double eta_b = maxEta+etaNum*diffEta;
      G4double eta_e = maxEta+(etaNum+1)*diffEta;
      //if(copyNo==nofHcalEndCapR*nofHcalECOPhi*4 -1)std::cout<<"eta ->" <<eta_b<<"  "<<eta_e<<std::endl;
      G4double theta_b = 2*atan(exp(-eta_b))*180/M_PI;
      G4double theta_e = 2*atan(exp(-eta_e))*180/M_PI;
      //if(copyNo==nofHcalEndCapR*nofHcalECOPhi*4 -1)std::cout<<"theta ->" <<theta_b<<"  "<<theta_e<<std::endl;
	    
      G4double zminA = HcalECEnvZmin + rNum*(diffRAbsEC+diffRScnEC);
      //G4double zminA = rNum*(diffRAbsEC+diffRScnEC);
      G4double zmaxA = zminA + diffRAbsEC;
      G4double zmaxS = zmaxA + diffRScnEC;
      if(copyNo==nofHcalEndCapR*nofHcalECOPhi*4 -1)std::cout<<"zminA zmaxA zmaxS ->" <<zminA<<"  "<<zmaxA<<"  "<<zmaxS<<std::endl;
      
      G4double RAbs_e = zminA*tan(theta_b*M_PI/180);
      G4double RAbs_b = zminA*tan(theta_e*M_PI/180);
      //if(copyNo==nofHcalEndCapR*nofHcalECOPhi*4 -1)std::cout<<"RAbs ->" <<RAbs_b<<"  "<<RAbs_e<<std::endl;
      //if(copyNo==nofHcalEndCapR*nofHcalECOPhi*4 -1)std::cout<<""<<std::endl;
      //G4double R_bAvg = 0.5*(RAbs_b+RAbs_e);
      
      G4double RSc_e = zmaxA*tan(theta_b*M_PI/180);
      G4double RSc_b = zmaxA*tan(theta_e*M_PI/180);
      //G4double R_eAvg = 0.5*(RSc_b+RSc_e);
      
      
      G4double diffAngECO = 5*deg;	// Barrel and part of EndCap
      G4double sAngECO = diffAngECO*phiNum;
      
      auto hcalAbsECOSolid = new G4Tubs("HCalAbsorberECOSingleCell",
					RAbs_b,
					RAbs_e,
					0.5*diffRAbsEC,
					sAngECO,
					diffAngECO);
      
      auto hcalScnECOSolid = new G4Tubs("HCalScintillatorECOSingleCell",
					RSc_b,
					RSc_e,
					0.5*diffRScnEC,
					sAngECO,
					diffAngECO);
      
      cellHcalECAbsLogical_r1[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalAbsECOSolid,
									hcal_absMat,
									"cellHcalECAbsLogical_r1");
      
      cellHcalECScnLogical_r1[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalScnECOSolid,
									hcal_scnMat,
									"cellHcalECScnLogical_r1");

      G4double zminA_wrtMother = -0.5*nofHcalEndCapR*(diffRAbsEC+diffRScnEC)+rNum*(diffRAbsEC+diffRScnEC);
      G4double zmaxA_wrtMother = zminA_wrtMother + diffRAbsEC;
      G4double zmaxS_wrtMother = zmaxA_wrtMother + diffRScnEC;

      G4double dispCellAbsECO = 0.5*(zminA_wrtMother+zmaxA_wrtMother);
      G4double dispCellScnECO = 0.5*(zmaxA_wrtMother+zmaxS_wrtMother);
  
      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCellAbsECO),
			cellHcalECAbsLogical_r1[etaNum][rNum][phiNum],
			"HcalECAbsCell_r1",
			hcalECOLogical_r,
			false,
			copyNo,
			false);
      
      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCellScnECO),
			cellHcalECScnLogical_r1[etaNum][rNum][phiNum],
			"HcalECScnCell_r1",
			hcalECOLogical_r,
			false,
			copyNo,
			false);

      visAttributes = new G4VisAttributes(G4Colour(0.00,0.00,0.50));
      cellHcalECAbsLogical_r1[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
      visAttributes = new G4VisAttributes(G4Colour(0.00,0.50,0.00));
      cellHcalECScnLogical_r1[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
      
      
      cellHcalECAbsLogical_l1[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalAbsECOSolid,
									hcal_absMat,
									"cellHcalECAbsLogical_l1");
      
      cellHcalECScnLogical_l1[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalScnECOSolid,
									hcal_scnMat,
									"cellHcalECScnLogical_l1");
      dispCellAbsECO = -dispCellAbsECO;
      dispCellScnECO = -dispCellScnECO;

      
      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCellAbsECO),
			cellHcalECAbsLogical_l1[etaNum][rNum][phiNum],
			"HcalECAbsCell_l1",
			hcalECOLogical_l,
			false,
			copyNo,
			false);
      
      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCellScnECO),
			cellHcalECScnLogical_l1[etaNum][rNum][phiNum],
			"HcalECScnCell_l1",
			hcalECOLogical_l,
			false,
			copyNo,
			false);

      visAttributes = new G4VisAttributes(G4Colour(0.00,0.00,0.50));
      cellHcalECAbsLogical_l1[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
      visAttributes = new G4VisAttributes(G4Colour(0.00,0.50,0.00));
      cellHcalECScnLogical_l1[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
      
    }
}

void HCalConstruction::makeEndCapInner(G4Material* hcal_absMat,
				  G4Material* hcal_scnMat,
				       G4LogicalVolume* hcalECOLogical_r, G4LogicalVolume* hcalECOLogical_l,
				  std::vector<G4VisAttributes*> fVisAttributes) {
  
  std::cout<<"Making the end cap (outer) for hadronic calorimeter"<<std::endl;
// Right side end cap (for the time being)

  //G4double diffEta = (HcalEC17to20EtaIn-HcalEC17to20EtaOut)/4;	// Barrel and part of EndCap
  //G4double diffRAbs = 100*mm;
  //G4double diffRScn = 10*mm;
    //G4double maxEta = 1.479;
    
    for(int copyNo=0; copyNo<nofHcalEndCapR*nofHcalECIPhi*8; copyNo++) {            //4 is for 4(17,18,19,20) ieta region in endcap as they have same delta eta, delta phi 
      
      G4int etaNum = copyNo/(nofHcalECIPhi*nofHcalEndCapR);
      G4int rNum = (copyNo%(nofHcalECIPhi*nofHcalEndCapR))/nofHcalECIPhi;
      G4int phiNum = (copyNo%(nofHcalECIPhi*nofHcalEndCapR))%nofHcalECIPhi;
      
      //G4double eta_b = -maxEta+etaNum*diffEta;
      //G4double eta_e = -maxEta+(etaNum+1)*diffEta;

      G4double eta_b = EtaArray[etaNum];
      G4double eta_e = EtaArray[etaNum+1];

      //if(copyNo==0)std::cout<<"eta ->" <<eta_b<<"  "<<eta_e<<std::endl;
      G4double theta_b = 2*atan(exp(-eta_b))*180/M_PI;
      G4double theta_e = 2*atan(exp(-eta_e))*180/M_PI;
      //if(copyNo==0)std::cout<<"theta ->" <<theta_b<<"  "<<theta_e<<std::endl;
	    
      G4double zminA = HcalECEnvZmin + rNum*(diffRAbsEC+diffRScnEC);
      G4double zmaxA = zminA + diffRAbsEC;
      G4double zmaxS = zmaxA + diffRScnEC;
      if(copyNo==0)std::cout<<"zminA zmaxA zmaxS ->" <<zminA<<"  "<<zmaxA<<"  "<<zmaxS<<std::endl;
      
      G4double RAbs_e = zminA*tan(theta_b*M_PI/180);
      G4double RAbs_b = zminA*tan(theta_e*M_PI/180);
      //if(copyNo==0)std::cout<<"RAbs ->" <<RAbs_b<<"  "<<RAbs_e<<std::endl;
      //if(copyNo==0)std::cout<<""<<std::endl;
      //G4double R_bAvg = 0.5*(RAbs_b+RAbs_e);
      
      G4double RSc_e = zmaxA*tan(theta_b*M_PI/180);
      G4double RSc_b = zmaxA*tan(theta_e*M_PI/180);
      //G4double R_eAvg = 0.5*(RSc_b+RSc_e);
      
      
      G4double diffAngECI = 10*deg;	// Barrel and part of EndCap
      G4double sAngECI = diffAngECI*phiNum;
      
      auto hcalAbsECISolid = new G4Tubs("HCalAbsorberECISingleCell",
					RAbs_b,
					RAbs_e,
					0.5*diffRAbsEC,
					sAngECI,
					diffAngECI);
      
      auto hcalScnECISolid = new G4Tubs("HCalScintillatorECOSingleCell",
					RSc_b,
					RSc_e,
					0.5*diffRScnEC,
					sAngECI,
					diffAngECI);
      
      cellHcalECAbsLogical_r2[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalAbsECISolid,
									hcal_absMat,
									"cellHcalECAbsLogical_r2");
      
      cellHcalECScnLogical_r2[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalScnECISolid,
									hcal_scnMat,
									"cellHcalECScnLogical_r2");

      G4double zminA_wrtMother = -0.5*nofHcalEndCapR*(diffRAbsEC+diffRScnEC)+rNum*(diffRAbsEC+diffRScnEC);
      G4double zmaxA_wrtMother = zminA_wrtMother + diffRAbsEC;
      G4double zmaxS_wrtMother = zmaxA_wrtMother + diffRScnEC;

      G4double dispCellAbsECI = 0.5*(zminA_wrtMother+zmaxA_wrtMother);
      G4double dispCellScnECI = 0.5*(zmaxA_wrtMother+zmaxS_wrtMother);

      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCellAbsECI),
			cellHcalECAbsLogical_r2[etaNum][rNum][phiNum],
			"HcalECAbsCell_r2",
			hcalECOLogical_r,
			false,
			copyNo,
			false);
      
      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCellScnECI),
			cellHcalECScnLogical_r2[etaNum][rNum][phiNum],
			"HcalECScnCell_r2",
			hcalECOLogical_r,
			false,
			copyNo,
			false);
      
      visAttributes = new G4VisAttributes(G4Colour(0.50,0.00,0.00));
      cellHcalECAbsLogical_r2[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
      visAttributes = new G4VisAttributes(G4Colour(0.00,0.50,0.00));
      cellHcalECScnLogical_r2[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
      



      cellHcalECAbsLogical_l2[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalAbsECISolid,
									hcal_absMat,
									"cellHcalECAbsLogical_l2");
      
      cellHcalECScnLogical_l2[etaNum][rNum][phiNum] = new G4LogicalVolume(hcalScnECISolid,
									hcal_scnMat,
									"cellHcalECScnLogical_l2");

      dispCellAbsECI = -dispCellAbsECI;
      dispCellScnECI = -dispCellScnECI;

      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCellAbsECI),
			cellHcalECAbsLogical_l2[etaNum][rNum][phiNum],
			"HcalECAbsCell_l2",
			hcalECOLogical_l,
			false,
			copyNo,
			false);
      
      new G4PVPlacement(0,
			G4ThreeVector(0,0,dispCellScnECI),
			cellHcalECScnLogical_l2[etaNum][rNum][phiNum],
			"HcalECScnCell_l2",
			hcalECOLogical_l,
			false,
			copyNo,
			false);
      
      visAttributes = new G4VisAttributes(G4Colour(0.50,0.00,0.00));
      cellHcalECAbsLogical_l2[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
      visAttributes = new G4VisAttributes(G4Colour(0.00,0.50,0.00));
      cellHcalECScnLogical_l2[etaNum][rNum][phiNum]->SetVisAttributes(visAttributes);
      fVisAttributes.push_back(visAttributes);
    }
}
