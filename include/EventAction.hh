#ifndef EventAction_h
#define EventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include "TTree.h"
//#include "TVector3.h"
#include <vector>

/// Event action

class EventAction : public G4UserEventAction{
  
public:
  EventAction();
  virtual ~EventAction();
  
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
  
  void set_pair_flag(){ 
    pair_prod_flag++;
  }
  
  void SetConvertedVertex(G4ThreeVector vertex){
    convX.push_back(vertex.x());
    convY.push_back(vertex.y());
    convZ.push_back(vertex.z());
  }
  
  void AddEneDep(G4int copyNo, G4double edep, G4String vol);
  void AddTrackPos(G4ThreeVector pos){
    TrackPosX.push_back(pos.x());
    TrackPosY.push_back(pos.y());
    TrackPosZ.push_back(pos.z());
  }
  
  G4double GetEmEne(){
    return totalEmE;
  }

  G4double GetHCalEne(){
    return totalHCalE;
  }

  void EventTree(TTree* tree){
    eventTree = tree;

  }

  void fillTrackHit(long, long, long, long, double);
  void fillMuonHit(G4String, G4ThreeVector, G4double);

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

  std::vector<long> trackerHits;
  std::vector<long> trackerEdep;
  std::vector<
    std::pair< long,std::vector<
		      std::pair< long,std::vector<
					std::pair< long,std::pair<long,double> >
					> >
		      > >
    > trackHitCollector;

  std::vector<G4double> muonHitX;
  std::vector<G4double> muonHitY;
  std::vector<G4double> muonHitZ;
  //std::vector<TVector3*> muonHits;
  std::vector<G4double> muonEdep;

  TTree* eventTree;
  void sortAndSaveTrackHit();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
