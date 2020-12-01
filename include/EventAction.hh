#ifndef EventAction_h
#define EventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "Constants.hh"

#include "TTree.h"

#include <vector>

/// Event action

class EventAction : public G4UserEventAction{
  
public:
  EventAction();
  virtual ~EventAction();
  
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
  
  bool PIBPixelArray[PIB_nZPixel*PIB_nPhiPixel];

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

  void EventTree(TTree* tree){
    eventTree = tree;

  }

private:
  G4int fEmCalBarrel;
  G4int fEmCalHCID_l;
  G4int fEmCalHCID_r;
  G4int fWorldHCID;
  G4int ftrackerHCID;
  G4int ftrackerEnvHCID;
  G4int pair_prod_flag;
  G4int totalEmHit;
  G4double totalEmE;

  std::vector<G4double> ecalBarEdep;
  std::vector<G4double> ecalECEdep_r;
  std::vector<G4double> ecalECEdep_l;
  std::vector<G4double> convX;
  std::vector<G4double> convY;
  std::vector<G4double> convZ;
  std::vector<G4int> EmBarCrysNum;
  std::vector<G4int> EmECCrysNum_r;
  std::vector<G4int> EmECCrysNum_l;
  std::vector<G4double> TrackPosX;
  std::vector<G4double> TrackPosY;
  std::vector<G4double> TrackPosZ;
  std::vector<G4int> PIBPixelNum;

  TTree* eventTree;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
