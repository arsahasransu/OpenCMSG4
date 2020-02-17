#ifndef EventAction_h
#define EventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include <vector>

/// Event action

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();
    
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

  std::vector<G4double>& GetEmCalEdep() { return ecalBarEdep; }
  std::vector<G4double>& GetEmCalECEdep_r() { return ecalECEdep_r; }
  std::vector<G4double>& GetEmCalECEdep_l() { return ecalECEdep_l; }

  std::vector<G4double>& GetHCalBarAbsEdep() { return HCalBarAbsEdep; }
  std::vector<G4double>& GetHCalBarScintillatorEdep() { return HCalBarScintillatorEdep; }
  std::vector<G4double>& GetHCalBarEdep() { return HCalBarEdep; }

  std::vector<G4double>& GetHCalECAbs_r1Edep() { return HCalECAbs_r1Edep; }
  std::vector<G4double>& GetHCalECScn_r1Edep() { return HCalECScn_r1Edep; }
  std::vector<G4double>& GetHCalEC_r1Edep() { return HCalEC_r1Edep; }

  std::vector<G4double>& GetHCalECAbs_l1Edep() { return HCalECAbs_l1Edep; }
  std::vector<G4double>& GetHCalECScn_l1Edep() { return HCalECScn_l1Edep; }
  std::vector<G4double>& GetHCalEC_l1Edep() { return HCalEC_l1Edep; }
  
  std::vector<G4double>& GetHCalECAbs_r2Edep() { return HCalECAbs_r2Edep; }
  std::vector<G4double>& GetHCalECScn_r2Edep() { return HCalECScn_r2Edep; }
  std::vector<G4double>& GetHCalEC_r2Edep() { return HCalEC_r2Edep; }

  std::vector<G4double>& GetHCalECAbs_l2Edep() { return HCalECAbs_l2Edep; }
  std::vector<G4double>& GetHCalECScn_l2Edep() { return HCalECScn_l2Edep; }
  std::vector<G4double>& GetHCalEC_l2Edep() { return HCalEC_l2Edep; }
  
  std::vector<G4double>& GetConvertedX() { return convX; }
  std::vector<G4double>& GetConvertedY() { return convY; }
  std::vector<G4double>& GetConvertedZ() { return convZ; }
  std::vector<G4int>& GetEmCalCrysNum() { return EmBarCrysNum; }
  std::vector<G4int>& GetEmCalECCrysNum_r() { return EmECCrysNum_r; }
  std::vector<G4int>& GetEmCalECCrysNum_l() { return EmECCrysNum_l; }
  
  std::vector<G4int>& GetHCalBarAbsCrysNum() { return HCalBarAbsCrysNum; }
  std::vector<G4int>& GetHCalBarScintillatorCrysNum() { return HCalBarScintillatorCrysNum; }
  std::vector<G4int>& GetHCalBarCrysNum() { return HCalBarCrysNum; }

  std::vector<G4int>& GetHCalECAbs_r1CrysNum() { return HCalECAbs_r1CrysNum; }
  std::vector<G4int>& GetHCalECScn_r1CrysNum() { return HCalECScn_r1CrysNum; }
  std::vector<G4int>& GetHCalEC_r1CrysNum() { return HCalEC_r1CrysNum; }

  std::vector<G4int>& GetHCalECAbs_l1CrysNum() { return HCalECAbs_l1CrysNum; }
  std::vector<G4int>& GetHCalECScn_l1CrysNum() { return HCalECScn_l1CrysNum; }
  std::vector<G4int>& GetHCalEC_l1CrysNum() { return HCalEC_l1CrysNum; }

  std::vector<G4int>& GetHCalECAbs_r2CrysNum() { return HCalECAbs_r2CrysNum; }
  std::vector<G4int>& GetHCalECScn_r2CrysNum() { return HCalECScn_r2CrysNum; }
  std::vector<G4int>& GetHCalEC_r2CrysNum() { return HCalEC_r2CrysNum; }

  std::vector<G4int>& GetHCalECAbs_l2CrysNum() { return HCalECAbs_l2CrysNum; }
  std::vector<G4int>& GetHCalECScn_l2CrysNum() { return HCalECScn_l2CrysNum; }
  std::vector<G4int>& GetHCalEC_l2CrysNum() { return HCalEC_l2CrysNum; }

  std::vector<G4double>& GetTrackPosX() { return TrackPosX; }
  std::vector<G4double>& GetTrackPosY() { return TrackPosY; }
  std::vector<G4double>& GetTrackPosZ() { return TrackPosZ; }
  
  std::vector<G4double> ecalBarEdep;
  std::vector<G4double> ecalECEdep_r;
  std::vector<G4double> ecalECEdep_l;
  
  std::vector<G4double> HCalBarAbsEdep;
  std::vector<G4double> HCalBarScintillatorEdep;
  std::vector<G4double> HCalBarEdep;
  
  std::vector<G4double> HCalECAbs_r1Edep;
  std::vector<G4double> HCalECScn_r1Edep;
  std::vector<G4double> HCalEC_r1Edep;

  std::vector<G4double> HCalECAbs_l1Edep;
  std::vector<G4double> HCalECScn_l1Edep;
  std::vector<G4double> HCalEC_l1Edep;

  std::vector<G4double> HCalECAbs_r2Edep;
  std::vector<G4double> HCalECScn_r2Edep;
  std::vector<G4double> HCalEC_r2Edep;

  std::vector<G4double> HCalECAbs_l2Edep;
  std::vector<G4double> HCalECScn_l2Edep;
  std::vector<G4double> HCalEC_l2Edep;
  
  std::vector<G4double> convX;
  std::vector<G4double> convY;
  std::vector<G4double> convZ;
  std::vector<G4int> EmBarCrysNum;
  std::vector<G4int> EmECCrysNum_r;
  std::vector<G4int> EmECCrysNum_l;
  
  std::vector<G4int> HCalBarAbsCrysNum;
  std::vector<G4int> HCalBarScintillatorCrysNum;
  std::vector<G4int> HCalBarCrysNum;
  
  std::vector<G4int> HCalECAbs_r1CrysNum;
  std::vector<G4int> HCalECScn_r1CrysNum;
  std::vector<G4int> HCalEC_r1CrysNum;

  std::vector<G4int> HCalECAbs_l1CrysNum;
  std::vector<G4int> HCalECScn_l1CrysNum;
  std::vector<G4int> HCalEC_l1CrysNum;

  std::vector<G4int> HCalECAbs_r2CrysNum;
  std::vector<G4int> HCalECScn_r2CrysNum;
  std::vector<G4int> HCalEC_r2CrysNum;

  std::vector<G4int> HCalECAbs_l2CrysNum;
  std::vector<G4int> HCalECScn_l2CrysNum;
  std::vector<G4int> HCalEC_l2CrysNum;
  
  std::vector<G4double> TrackPosX;
  std::vector<G4double> TrackPosY;
  std::vector<G4double> TrackPosZ;

  void set_pair_flag()
  { 
    pair_prod_flag++;
  }

  void SetConvertedVertex(G4ThreeVector vertex)
  {
    convX.push_back(vertex.x());
    convY.push_back(vertex.y());
    convZ.push_back(vertex.z());
  }
	
  void AddEneDep(G4int copyNo, G4double edep, G4String vol);
  void AddTrackPos(G4ThreeVector pos)
  {
    TrackPosX.push_back(pos.x());
    TrackPosY.push_back(pos.y());
    TrackPosZ.push_back(pos.z());
  }
  
  G4double GetEmEne()
  {
    return totalEmE;
  }

  G4double GetHCalEne()
  {
    return totalHCalE;
  }

private:
  G4int fEmCalBarrel;
  G4int fEmCalHCID_l;
  G4int fEmCalHCID_r;

  G4int fHCalBarAbs;
  G4int fHCalBarScintillator;
  G4int HCalBar;

  G4int HCalECAbs_r1;
  G4int HCalECScn_r1;
  G4int HCalEC_r1;

  G4int HCalECAbs_l1;
  G4int HCalECScn_l1;
  G4int HCalEC_l1;

  G4int HCalECAbs_r2;
  G4int HCalECScn_r2;
  G4int HCalEC_r2;

  G4int HCalECAbs_l2;
  G4int HCalECScn_l2;
  G4int HCalEC_l2;

  G4int fWorldHCID;
  G4int ftrackerHCID;
  G4int ftrackerEnvHCID;
  G4int pair_prod_flag;
  G4int totalEmHit;
  G4double totalEmE;

  G4int totalHCalHit;
  G4double totalHCalE;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
