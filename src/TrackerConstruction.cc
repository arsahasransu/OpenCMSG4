#include "TrackerConstruction.hh"
#include "Constants.hh"

#include "G4PVPlacement.hh"
#include "G4Tubs.hh"

TrackerConstruction::TrackerConstruction()
  : PIB_Logical(PIB_num), PID_Logical(PID_num),
    TIB_Logical(TIB_num), TID_Logical(TID_num),
    TOB_Logical(TOB_num), TEC_Logical(TEC_num) {
}
    
TrackerConstruction::~TrackerConstruction() {
}

void TrackerConstruction::makePIB(G4Material* elAl, G4Material* elSi, G4Material* Cu, std::vector<G4Material*> pixBarMat, G4LogicalVolume* trackerLogical, bool nonSens, std::vector<G4VisAttributes*> fVisAttributes) {

  for(G4int i=0; i<PIB_num; i++) {
    auto PIBSupp_Solid = new G4Tubs("PIBSupport_Solid",
				    PIB_rMin[i]*mm-PIBSup_thick*mm,
				    PIB_rMin[i]*mm,
				    285*mm, 90*deg,
				    trPhiAng*deg);
    auto PIBSupp_Logical = new G4LogicalVolume(PIBSupp_Solid,
					       elAl,
					       "PIBSupport_Logical");
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(),
			PIBSupp_Logical,
			"PixelInnerBarrelSupport",
			trackerLogical,
			false,
			i,
			true);

    auto PIB_Solid = new G4Tubs("PIB_Solid",
				PIB_rMin[i]*mm,
				PIB_rMin[i]*mm+PIB_thick*um,
				PIB_halfz*mm,
				90*deg,
				trPhiAng*deg);
    PIB_Logical[i] = new G4LogicalVolume(PIB_Solid,
					 elSi,
					 "PIB_Logical");
    new G4PVPlacement(0,
		      G4ThreeVector(),
		      PIB_Logical[i],
		      "PixelInnerBarrel",
		      trackerLogical,
		      false,
		      i,
		      true);
    
    G4double Pix_rMat = PIB_rMin[i]*mm+PIB_thick*um;
    for(unsigned int mat_ctr=0; mat_ctr<pixBarMat.size(); mat_ctr++) {
      auto PixBarMat_Solid = new G4Tubs("PIBMaterials_Solid",
					Pix_rMat,
					Pix_rMat+PIBMat_thick[mat_ctr]*um,
					PIB_halfz*mm,
					90*deg,
					trPhiAng*deg);
      Pix_rMat = Pix_rMat+PIBMat_thick[mat_ctr]*um;
      auto PixBarMat_Logical = new G4LogicalVolume(PixBarMat_Solid,
						   pixBarMat[mat_ctr],
						   "PIBMaterials_Logical");
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(),
			  PixBarMat_Logical,
			  "PixelInnerBarrelMaterial",
			  trackerLogical,
			  false,
			  0,
			  true);
    }
  }

  auto PIBMat_Solid = new G4Tubs("PIBCables_Solid",
				 PIB_thick*um+44555*um,
				 PIB_thick*um+44555*um+300*um,
				 0.5*(177-66)*mm,
				 90*deg,
				 trPhiAng*deg);
  auto PIBMat_Logical = new G4LogicalVolume(PIBMat_Solid,
					    Cu,
					    "PIBCables_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,0.5*(66+177)*mm),
		      PIBMat_Logical,
		      "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      0,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-0.5*(66+177)*mm),
		      PIBMat_Logical,
		      "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      1,
		      true);

  PIBMat_Solid = new G4Tubs("PIBCables_Solid",
			    PIB_thick*um+44555*um,
			    PIB_thick*um+44555*um+700*um,
			    0.5*(285-177)*mm,
			    90*deg,
			    trPhiAng*deg);
  PIBMat_Logical = new G4LogicalVolume(PIBMat_Solid,
				       Cu,
				       "PIBCables_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,0.5*(285+177)*mm),
		      PIBMat_Logical,
		      "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      2,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-0.5*(285+177)*mm),
		      PIBMat_Logical,
		      "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      3,
		      true);
  
  PIBMat_Solid = new G4Tubs("PIBCables_Solid",
			    PIB_thick*um+73555*um,
			    PIB_thick*um+73555*um+300*um,
			    0.5*(285-110)*mm,
			    90*deg,
			    trPhiAng*deg);
  PIBMat_Logical = new G4LogicalVolume(PIBMat_Solid,
				       Cu,
				       "PIBCables_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,0.5*(285+110)*mm),
		      PIBMat_Logical,
		      "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      4,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-0.5*(285+110)*mm),
		      PIBMat_Logical,
		      "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      5,
		      true);
  
  PIBMat_Solid = new G4Tubs("PIBCables_Solid",
			    PIB_thick*um+102555*um,
			    PIB_thick*um+102555*um+300*um,
			    0.5*(285-153)*mm,
			    90*deg,
			    trPhiAng*deg);
  PIBMat_Logical = new G4LogicalVolume(PIBMat_Solid,
					     Cu,
				       "PIBCables_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,0.5*(285+153)*mm),
		      PIBMat_Logical,
			    "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      6,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-0.5*(285+153)*mm),
		      PIBMat_Logical,
		      "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      7,
		      true);
  
  auto PIBSupp_Solid = new G4Tubs("PIBSupport_Solid",
				  40*mm,
				  180*mm,
				  0.5*PIBSup_thick*mm,
				  90*deg,
				  trPhiAng*deg);
  auto PIBSupp_Logical = new G4LogicalVolume(PIBSupp_Solid,
					     elAl,
					     "PIBSupport_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,285*mm+0.5*PIBSup_thick*mm),
		      PIBSupp_Logical,
		      "PixelInnerBarrelSupport",
		      trackerLogical,
		      false,
		      PIB_num,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-285*mm-0.5*PIBSup_thick*mm),
		      PIBSupp_Logical,
		      "PixelInnerBarrelSupport",
		      trackerLogical,
		      false,
		      PIB_num+1,
		      true);
  
  auto PIBCables_Solid = new G4Tubs("PIBCables_Solid",
				    40*mm,
				    180*mm,
				    0.5*PIBCables_thick*um,
				    90*deg,
				    trPhiAng*deg);
  auto PIBCables_Logical = new G4LogicalVolume(PIBCables_Solid,
					       Cu,
					       "PIBCables_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,285*mm+PIBSup_thick*mm+0.5*PIBCables_thick*um),
		      PIBCables_Logical,
		      "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      0,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-285*mm-PIBSup_thick*mm-0.5*PIBCables_thick*um),
		      PIBCables_Logical,
		      "PixelInnerBarrelCables",
		      trackerLogical,
		      false,
		      1,
		      true);
  
  auto PIBCables_Solid2 = new G4Tubs("PIBCables_Solid2",
				     180*mm,
				     180*mm+1000*um,
				     0.5*290*mm,
				     90*deg,
				     trPhiAng*deg);
  auto PIBCables_Logical2 = new G4LogicalVolume(PIBCables_Solid2,
						elAl,
						"PIBCables_Logical2");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,285*mm+PIBSup_thick*mm+PIBCables_thick*um+0.5*290*mm),
		      PIBCables_Logical2,
		      "PixelInnerBarrelCables2",
		      trackerLogical,
		      false,
		      0,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-285*mm-PIBSup_thick*mm-PIBCables_thick*um-0.5*290*mm),
		      PIBCables_Logical2,
		      "PixelInnerBarrelCables2",
		      trackerLogical,
		      false,
		      1,
		      true);

  for(G4int copyNo=0; copyNo<PIB_num; copyNo++)	{
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    PIB_Logical[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
}

void TrackerConstruction::makePID(G4Material* elAl, G4Material* elSi, G4Material* Cu, std::vector<G4Material*> pixECMat, G4LogicalVolume* trackerLogical, bool nonSens, std::vector<G4VisAttributes*> fVisAttributes) {
  for(G4int i=0; i<PID_num; i++) {
    auto PID_Solid = new G4Tubs("PID_Solid",
				PID_r[0]*mm,
				PID_r[1]*mm,
				PID_halfz*um,
				90*deg,
				trPhiAng*deg);
    PID_Logical[i] = new G4LogicalVolume(PID_Solid,
					 elSi,
					 "PID_Logical");
    new G4PVPlacement(0,
		      G4ThreeVector(0.,0.,PID_posz[i]*mm),
		      PID_Logical[i],
		      "PixelInnerDisks",
		      trackerLogical,
		      false,
		      i,
		      true);

    G4double Pix_zMat = PID_posz[i]*mm+PID_halfz*um;
    for(unsigned int mat_ctr=0; mat_ctr<pixECMat.size(); mat_ctr++) {
      auto PixECMat_Solid = new G4Tubs("PIDMaterials_Solid",
				       PID_r[0]*mm,
				       PID_r[1]*mm,
				       0.5*PIDMat_thick[mat_ctr]*um,
				       90*deg,
				       trPhiAng*deg);
      auto PixECMat_Logical = new G4LogicalVolume(PixECMat_Solid,
						  pixECMat[mat_ctr],
						  "PIDMaterials_Logical");
      Pix_zMat += 0.5*PIDMat_thick[mat_ctr]*um;
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(0.,0.,Pix_zMat),
			  PixECMat_Logical,
			  "PixelInnerDiskMaterial",
			  trackerLogical,
			  false,
			  0,
			  true);
      Pix_zMat += 0.5*PIDMat_thick[mat_ctr]*um;
    }
  }

  for(G4int i=0; i<4; i++) {
    auto PIDMat_Solid = new G4Tubs("PIDSupportSolid",
				   150*mm,
				   150*mm+PIDMat_thick[0]*um,
				   0.5*25*mm,
				   90*deg,
				   trPhiAng*deg);
    auto PIDMat_Logical = new G4LogicalVolume(PIDMat_Solid,
					      elAl,
					      "PIDSupportLogical");
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0,0,PID_posz[i]),
			PIDMat_Logical,
			"PixelInnerDiskSupport",
			trackerLogical,
			false,
			2+2*i,
			true);
    PIDMat_Solid = new G4Tubs("PIDSupportSolid",
			      150*mm+PIDMat_thick[0]*um,
			      150*mm+PIDMat_thick[0]*um+PIDMat_thick[1]*um,
			      0.5*25*mm,
			      90*deg,
			      trPhiAng*deg);
    PIDMat_Logical = new G4LogicalVolume(PIDMat_Solid,
					 elAl,
					 "PIDSupportLogical");
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0,0,PID_posz[i]),
			PIDMat_Logical,
			"PixelInnerDiskSupport",
			trackerLogical,
			false,
			3+2*i,
			true);
  }

  auto PIDCables_Solid = new G4Tubs("PIDCables_Solid",
				    150*mm+PIDMat_thick[0]*um+PIDMat_thick[1]*um,
				    150*mm+PIDMat_thick[0]*um+PIDMat_thick[1]*um+750*um,
				    0.5*2000*mm,
				    90*deg,
				    trPhiAng*deg);
  auto PIDCables_Logical = new G4LogicalVolume(PIDCables_Solid,
					       Cu,
					       "PIDCables_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,PID_posz[0]+1000*mm),
		      PIDCables_Logical,
		      "PixelInnerDiskCables",
		      trackerLogical,
		      false,
		      0,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-PID_posz[0]-1000*mm),
		      PIDCables_Logical,
		      "PixelInnerDiskCables",
		      trackerLogical,
		      false,
		      1,
		      true);

  for(G4int copyNo=0; copyNo<PID_num; copyNo++)	{
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    PID_Logical[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
}

void TrackerConstruction::makeTIB(G4Material* elSi, G4Material* Cu, std::vector<G4Material*> TIBMat, G4LogicalVolume* trackerLogical, bool nonSens, std::vector<G4VisAttributes*> fVisAttributes) {
  G4double TIBr;
  
  for(G4int i=0; i<TIB_num; i++) {
    auto TIB_Solid = new G4Tubs("TIB_Solid",
				TIB_rMin[i]*mm,
				TIB_rMin[i]*mm+TIB_thick[i]*um*(4.0/3),
				700*mm,
				90*deg,
				trPhiAng*deg);
    TIB_Logical[i] = new G4LogicalVolume(TIB_Solid,
					 elSi,
					 "TIB_Logical");
    new G4PVPlacement(0,
		      G4ThreeVector(),
		      TIB_Logical[i],
		      "TrackerStripInnerBarrel",
		      trackerLogical,
		      false,
		      i,
		      true);

    TIBr = TIB_rMin[i]*mm+TIB_thick[i]*um*(4.0/3);
    for(unsigned int j=0; j<TIBMat.size(); j++)	{
      auto TIBSupp_Solid = new G4Tubs("TIBSupp_Solid",
				      TIBr,
				      TIBr+((i==0||i==1)&&j==1?2*TIBSupp_thick[j]:TIBSupp_thick[j])*um,
				      700*mm,
				      90*deg,
				      trPhiAng*deg);
      TIBr += ((i==0||i==1)&&j==1?2*TIBSupp_thick[j]:TIBSupp_thick[j])*um;
      auto TIBSupp_Logical = new G4LogicalVolume(TIBSupp_Solid,
						 TIBMat[j],
						 "TIBSupp_Logical");
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(),
			  TIBSupp_Logical,
			  "TIBSupport",
			  trackerLogical,
			  false,
			  0,
			  true);
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(),
			  TIBSupp_Logical,
			  "TIBSupport",
			  trackerLogical,
			  false,
			  0,
			  true);
    }
    
    for(unsigned int j=0; j<5; j++) {
      auto TIBEle_Solid = new G4Tubs("TIBEle_Solid",
				     TIBr,
				     TIBr+TIBEle_thick*um,
				     0.5*(700-TIBSupp_pos[j])*mm,
				     90*deg,
				     trPhiAng*deg);
      TIBr += TIBEle_thick*um;
      auto TIBEle_Logical = new G4LogicalVolume(TIBEle_Solid,
						Cu,
						"TIBEle_Logical");
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(0,0,0.5*(700+TIBSupp_pos[j])*mm),
			  TIBEle_Logical,
			  "TIBElectricals",
			  trackerLogical,
			  false,
			  i*j,
			  true);		
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(0,0,-0.5*(700+TIBSupp_pos[j])*mm),
			  TIBEle_Logical,
			  "TIBElectricals",
			  trackerLogical,
			  false,
			  i*j,
			  true);		
    }		
  }
  
  auto TIBEC_Solid = new G4Tubs("TIBEC_Solid",
				TIB_rMin[0]*mm,
				500*mm,
				TIBSupp_thick[0]*um,
				90*deg,
				trPhiAng*deg);
  auto TIBEC_Logical = new G4LogicalVolume(TIBEC_Solid,
					   Cu,
					   "TIBEC_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,700*mm+TIBSupp_thick[0]*um),
		      TIBEC_Logical,
		      "TIBEndCapSupport",
		      trackerLogical,
		      false,
		      0,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-700*mm-TIBSupp_thick[0]*um),
		      TIBEC_Logical,
		      "TIBEndCapSupport",
		      trackerLogical,
		      false,
		      1,
		      true);

  auto TIBD_Solid = new G4Tubs("TIBtoD_Solid",
			       TIBr,
			       TIBr+2400*um,
			       0.5*(TIDECSupp_pos-TIBSupp_pos[4])*mm,
			       90*deg,
			       trPhiAng*deg);
  auto TIBD_Logical = new G4LogicalVolume(TIBD_Solid,
					  Cu,
					  "TIBtoD_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,0.5*(TIDECSupp_pos+TIBSupp_pos[4])*mm),
		      TIBD_Logical,
		      "TIBtoDSupport",
		      trackerLogical,
		      false,
		      0,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-0.5*(TIDECSupp_pos+TIBSupp_pos[4])*mm),
		      TIBD_Logical,
		      "TIBtoDSupport",
		      trackerLogical,
		      false,
		      1,
		      true);

  for(G4int copyNo=0; copyNo<TIB_num; copyNo++)	{
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    TIB_Logical[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }

}

void TrackerConstruction::makeTID(G4Material* elAl, G4Material* elSi, G4Material* Cu, G4LogicalVolume* trackerLogical, bool nonSens, std::vector<G4VisAttributes*> fVisAttributes) {

  for(G4int i=0; i<TID_num;i=i+2) {
    auto TID_Solid = new G4Tubs("TID_Solid",
				200*mm,
				400*mm,
				320*um*(4.0/3),
				90*deg,
				trPhiAng*deg);
    TID_Logical[i] = new G4LogicalVolume(TID_Solid,
					 elSi,
					 "TID_Logical");
    new G4PVPlacement(0,
		      G4ThreeVector(0.,0.,pow(-1,i>=TID_num/2?1:0)*TID_posz[i%3]*mm),
		      TID_Logical[i],
		      "TrackerStripInnerDisks",
		      trackerLogical,
		      false,
		      i,
		      true);
    TID_Solid = new G4Tubs("TID_Solid",
			   400*mm,
			   500*mm,
			   160*um*(4.0/3),
			   90*deg,
			   trPhiAng*deg);
    TID_Logical[i+1] = new G4LogicalVolume(TID_Solid,
					   elSi,
					   "TID_Logical");
    new G4PVPlacement(0,
		      G4ThreeVector(0.,0.,pow(-1,i>=TID_num/2?1:0)*TID_posz[i%3]*mm),
		      TID_Logical[i+1],
		      "TrackerStripInnerDisks",
		      trackerLogical,
		      false,
		      i+1,
		      true);

    auto TIDSupp_Solid = new G4Tubs("TIDSupp_Solid",
				    200*mm,
				    400*mm,
				    0.5*TIDSupp_thick*um,
				    90*deg,
				    trPhiAng*deg);
    auto TIDSupp_Logical = new G4LogicalVolume(TIDSupp_Solid,
					       elAl,
					       "TIDSupp_Logical");
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0.,0.,pow(-1,i>=TID_num/2?1:0)*(TID_posz[i%3]*mm+2*320*um*(4.0/3)+0.5*TIDSupp_thick*um)),
			TIDSupp_Logical,
			"TIDSupport",
			trackerLogical,
			false,
			i,
			true);
    TIDSupp_Solid = new G4Tubs("TIDSupp_Solid",
			       400*mm,
			       500*mm,
			       0.5*TIDSupp_thick*um,
			       90*deg,
			       trPhiAng*deg);
    TIDSupp_Logical = new G4LogicalVolume(TIDSupp_Solid,
					  elAl,
					  "TIDSupp_Logical");
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0.,0.,pow(-1,i>=TID_num/2?1:0)*(TID_posz[i%3]*mm+2*160*um*(4.0/3)+0.5*TIDSupp_thick*um)),
			TIDSupp_Logical,
			"TIDSupport",
			trackerLogical,
			false,
			i+1,
			true);
  }
  
  auto TIDSupp_Solid = new G4Tubs("TIDSupp_Solid",
				  220,
				  500*mm+2400*um,
				  0.5*TIDSupp_thick*um,
				  90*deg,
				  trPhiAng*deg);
  auto TIDSupp_Logical = new G4LogicalVolume(TIDSupp_Solid,
					     elAl,
					     "TIDSupp_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,TIDECSupp_pos*mm+0.5*TIDSupp_thick*um),
		      TIDSupp_Logical,
		      "TIDSupport",
		      trackerLogical,
		      false,
		      0,
		      true);
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-TIDECSupp_pos*mm-0.5*TIDSupp_thick*um),
		      TIDSupp_Logical,
		      "TIDSupport",
		      trackerLogical,
		      false,
		      1,
		      true);

  auto TIDEle_Solid = new G4Tubs("TIDEle_Solid",
				 200*mm-TIDEle_thick*um,
				 200*mm,
				 0.5*(1050-564)*mm,
				 90*deg,
				 trPhiAng*deg);
  auto TIDEle_Logical = new G4LogicalVolume(TIDEle_Solid,
					    Cu,
					    "TIDEle_Logical");
  if(nonSens)
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,0.5*(1050+564)*mm),
		      TIDEle_Logical,
		      "TIDElectricals",
		      trackerLogical,
		      false,
		      0,
		      true);
  if(nonSens)		
    new G4PVPlacement(0,
		      G4ThreeVector(0,0,-0.5*(1050+564)*mm),
		      TIDEle_Logical,
		      "TIDElectricals",
		      trackerLogical,
		      false,
		      1,
		      true);		

  for(G4int copyNo=0; copyNo<TID_num; copyNo++)	{
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    TID_Logical[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
}

void TrackerConstruction::makeTOB(G4Material* elSi, G4Material* elAl, G4Material* Pb, std::vector<G4Material*> TOBMat, G4LogicalVolume* trackerLogical, bool nonSens, std::vector<G4VisAttributes*> fVisAttributes) {
  G4double rEleCool_min_hist = 0;
  for(G4int i=0; i<TOB_num; i++) {
    auto TOB_Solid = new G4Tubs("TOB_Solid",
				TOB_rMin[i]*mm,
				TOB_rMin[i]*mm+TOB_thick[i]*um*(4.0/3),
				1180*mm,
				90*deg,
				trPhiAng*deg);
    TOB_Logical[i] = new G4LogicalVolume(TOB_Solid,
					 elSi,
					 "TOB_Logical");
    new G4PVPlacement(0,
		      G4ThreeVector(),
		      TOB_Logical[i],
		      "TrackerStripOuterBarrel",
		      trackerLogical,
		      false,
		      i,
		      true);

    G4double rEleCool_min = TOB_rMin[i]*mm+TOB_thick[i]*um*(4.0/3);
    for(unsigned int j=0; j<TOBMat.size(); j++)	{
      auto TOBEleCool_Solid = new G4Tubs("TOBEleCool_Solid",
					 rEleCool_min,
					 rEleCool_min+TOBEleCool_dr[j]*um,
					 1180*mm,
					 90*deg,
					 trPhiAng*deg);
      rEleCool_min = rEleCool_min+TOBEleCool_dr[j]*um;
      auto TOBEleCool_Logical = new G4LogicalVolume(TOBEleCool_Solid,
						    TOBMat[j],
						    "TOBEleCool_Logical");
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(),
			  TOBEleCool_Logical,
			  "TOBElectricalsCoolants",
			  trackerLogical,
			  false,
			  i*TOBMat.size()+j,
			  true);		
    }		
    
    auto TOBWiring1_Solid = new G4Tubs("TOBWiring1_Solid",
				       rEleCool_min,
				       rEleCool_min+rWiring1*um,
				       0.5*(1180*mm-zWiring1*mm),
				       90*deg,
				       trPhiAng*deg);
    rEleCool_min = rEleCool_min+rWiring1*um;
    auto TOBWiring1_Logical = new G4LogicalVolume(TOBWiring1_Solid,
						  Pb,
						  "TOBWiring1_Logical");
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0,0,0.5*zWiring1*mm+0.5*1180*mm),
			TOBWiring1_Logical,
			"TOBWiringFirstLayer",
			trackerLogical,
			false,
			i,
			true);
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0,0,-0.5*zWiring1*mm-0.5*1180*mm),
			TOBWiring1_Logical,
			"TOBWiringFirstLayer",
			trackerLogical,
			false,
			i,
			true);		

    auto TOBWiring2_Solid = new G4Tubs("TOBWiring2_Solid",
				       rEleCool_min,
				       rEleCool_min+rWiring2*um,
				       0.5*(1180*mm-zWiring2*mm),
				       90*deg,
				       trPhiAng*deg);
    rEleCool_min = rEleCool_min+rWiring2*um;
    auto TOBWiring2_Logical = new G4LogicalVolume(TOBWiring2_Solid,
						  Pb,
						  "TOBWiring2_Logical");
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0,0,0.5*zWiring2*mm+0.5*1180*mm),
			TOBWiring2_Logical,
			"TOBWiringSecondLayer",
			trackerLogical,
			false,
			i,
			true);
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0,0,-0.5*zWiring2*mm-0.5*1180*mm),
			TOBWiring2_Logical,
			"TOBWiringSecondLayer",
			trackerLogical,
			false,
			i,
			true);		

    auto TOBSupp1_Solid = new G4Tubs("TOBSupp1_Solid",
				     rEleCool_min-0.5*rSupp*mm,
				     rEleCool_min+0.5*rSupp*mm,
				     0.5*zSupp1*mm,
				     90*deg,
				     trPhiAng*deg);
    auto TOBSupp1_Logical = new G4LogicalVolume(TOBSupp1_Solid,
						elAl,
						"TOBSupp1_Logical");
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0,0,1180*mm+0.5*zSupp1*mm),
			TOBSupp1_Logical,
			"TOBSupportFirstLayer",
			trackerLogical,
			false,
			i,
			true);
    if(nonSens)
      new G4PVPlacement(0,
			G4ThreeVector(0,0,-1180*mm-0.5*zSupp1*mm),
			TOBSupp1_Logical,
			"TOBSupportFirstLayer",
			trackerLogical,
			false,
			i,
			true);		

    if(i==0) {
      rEleCool_min_hist = rEleCool_min;
    }
    else {
      auto TOBSupp2_Solid = new G4Tubs("TOBSupp2_Solid",
				       rEleCool_min_hist+0.5*rSupp*mm,
				       rEleCool_min-0.5*rSupp*mm,
				       0.5*zSupp2*mm,
				       90*deg,
				       trPhiAng*deg);
      auto TOBSupp2_Logical = new G4LogicalVolume(TOBSupp2_Solid,
						  elAl,
						  "TOBSupp2_Logical");
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(0,0,1180*mm+zSupp1*mm+0.5*zSupp2*mm),
			  TOBSupp2_Logical,
			  "TOBSupportSecondLayer",
			  trackerLogical,
			  false,
			  i,
			  true);
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(0,0,-1180*mm-zSupp1*mm-0.5*zSupp2*mm),
			  TOBSupp2_Logical,
			  "TOBSupportSecondLayer",
			  trackerLogical,
			  false,
			  i,
			  true);	

      auto TOBSupp3_Solid = new G4Tubs("TOBSupp3_Solid",
				       rEleCool_min_hist,
				       TOB_rMin[i]*mm,
				       0.5*zSupp3*mm,
				       90*deg,
				       trPhiAng*deg);
      auto TOBSupp3_Logical = new G4LogicalVolume(TOBSupp3_Solid,
						  elAl,
						  "TOBSupp3_Logical");
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(0,0,zSupp3Pos*mm),
			  TOBSupp3_Logical,
			  "TOBSupportThirdLayer",
			  trackerLogical,
			  false,
			  i,
			  true);
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(0,0,-zSupp3Pos*mm),
			  TOBSupp3_Logical,
			  "TOBSupportThirdLayer",
			  trackerLogical,
			  false,
			  i,
			  true);	
      rEleCool_min_hist = rEleCool_min;	
    }
  }
  for(G4int copyNo=0; copyNo<TOB_num; copyNo++) {
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    TOB_Logical[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
}

void TrackerConstruction::makeTEC(G4Material* elSi, std::vector<G4Material*> TECMat, G4LogicalVolume* trackerLogical, bool nonSens, std::vector<G4VisAttributes*> fVisAttributes) {
  
  G4int tec_count=0;
  for(G4int ring=1; ring<=7; ring++) {
    for(G4int disk=1; disk<=9; disk++) {
      if((ring==1 && disk>3)||(ring==2 && disk>6)||(ring==3 && disk>8)) continue;
      G4double TEC_thick = ring<=4?160*(4.0/3):250*(4.0/3);
      if((ring==1)||(ring==2)||(ring==5)) TEC_thick *= 2;

      auto TEC_Solid = new G4Tubs("TEC_Solid",
				  TEC_ring[ring-1][0]*mm,
				  TEC_ring[ring-1][1]*mm,
				  TEC_thick*um,
				  90*deg,
				  trPhiAng*deg);
      TEC_Logical[tec_count] = new G4LogicalVolume(TEC_Solid,
						   elSi,
						   "TEC_z+_Logical");
      new G4PVPlacement(0,
			G4ThreeVector(0,0,TEC_disk[disk-1][(ring-1)%2]*mm),
			TEC_Logical[tec_count],
			"TrackerEndCap",
			trackerLogical,
			false,
			tec_count,
			true);
      new G4PVPlacement(0,
			G4ThreeVector(0,0,-1*TEC_disk[disk-1][(ring-1)%2]*mm),
			TEC_Logical[tec_count],
			"TrackerEndCap",
			trackerLogical,
			false,
			tec_count,
			true);

      auto TECMat_Solid	= new G4Tubs("TECMat_Solid",
				     TEC_ring[ring-1][0]*mm,
				     TEC_ring[ring-1][1]*mm,
				     0.5*TECMat_thick*mm,
				     90*deg,
				     trPhiAng*deg);
      auto TECMat_Logical = new G4LogicalVolume(TECMat_Solid,
						TECMat[0],
						"TECMat_Logical");
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(0,0,TEC_disk[disk-1][(ring-1)%2]*mm+TEC_thick*um+0.5*TECMat_thick*mm),
			  TECMat_Logical,
			  "TrackerEndCapMaterial",
			  trackerLogical,
			  false,
			  tec_count,
			  true);
      if(nonSens)
	new G4PVPlacement(0,
			  G4ThreeVector(0,0,-TEC_disk[disk-1][(ring-1)%2]*mm-TEC_thick*um-0.5*TECMat_thick*mm),
			  TECMat_Logical,
			  "TrackerEndCapMaterial",
			  trackerLogical,
			  false,
			  tec_count,
			  true);

      tec_count++;
    }
  }
  for(G4int copyNo=0; copyNo<tec_count; copyNo++) {
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0));
    //visAttributes->SetVisibility(false);
    //visAttributes->SetForceLineSegmentsPerCircle(10);
    TEC_Logical[copyNo]->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }
}
