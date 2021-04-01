#include <fstream>
#include <string>
#include <cmath>
#include <vector>

//#include "Analysis.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "Constants.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4VHitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventAction::EventAction():
  G4UserEventAction(),
  totalEmHit(0), totalEmE(0.),
  totalHCalHit(0), totalHCalE(0.),
  ecalBarEdep(), ecalECEdep_r(), ecalECEdep_l(),
  HCalBarAbsEdep(), HCalBarScintillatorEdep(), HCalBarEdep(),
  HCalECAbs_r1Edep(), HCalECScn_r1Edep(), HCalEC_r1Edep(),
  HCalECAbs_l1Edep(), HCalECScn_l1Edep(), HCalEC_l1Edep(),
  HCalECAbs_r2Edep(), HCalECScn_r2Edep(), HCalEC_r2Edep(),
  HCalECAbs_l2Edep(), HCalECScn_l2Edep(), HCalEC_l2Edep(),
  convX(), convY(), convZ(), 
  EmBarCrysNum(), EmECCrysNum_r(), EmECCrysNum_l(),
  HCalBarAbsCrysNum(), HCalBarScintillatorCrysNum(), HCalBarCrysNum(),
  HCalECAbs_r1CrysNum(), HCalECScn_r1CrysNum(), HCalEC_r1CrysNum(),
  HCalECAbs_l1CrysNum(), HCalECScn_l1CrysNum(), HCalEC_l1CrysNum(),
  HCalECAbs_r2CrysNum(), HCalECScn_r2CrysNum(), HCalEC_r2CrysNum(),
  HCalECAbs_l2CrysNum(), HCalECScn_l2CrysNum(), HCalEC_l2CrysNum(),
  TrackPosX(), TrackPosY(), TrackPosZ(),
  trackerHits(), trackerEdep(),
  trackHitCollector(),
  muonHitX(), muonHitY(), muonHitZ(), /*muonHits(),*/ muonEdep(){

  pair_prod_flag = 0;
  
  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*){
  
  pair_prod_flag = 0;
  totalEmHit = 0;
  totalEmE = 0.;
  totalHCalHit = 0;
  totalHCalE = 0.;

  eventTree->Branch("CrysEdep", &ecalBarEdep);
  eventTree->Branch("CrysNum", &EmBarCrysNum);
  eventTree->Branch("CrysEdepEC_r", &ecalECEdep_r);
  eventTree->Branch("CrysNumEC_r", &EmECCrysNum_r);
  eventTree->Branch("CrysEdepEC_l", &ecalECEdep_l);
  eventTree->Branch("CrysNumEC_l", &EmECCrysNum_l);
  eventTree->Branch("EventEdep", &totalEmE);
  eventTree->Branch("HitNum", &totalEmHit);

  eventTree->Branch("ConvertedFlag", &pair_prod_flag);
  eventTree->Branch("ConvertedX", &convX);
  eventTree->Branch("ConvertedY", &convY);
  eventTree->Branch("ConvertedZ", &convZ);
  eventTree->Branch("TrackerHitPositionX", &TrackPosX);
  eventTree->Branch("TrackerHitPositionY", &TrackPosY);
  eventTree->Branch("TrackerHitPositionZ", &TrackPosZ);
  eventTree->Branch("trackerHits", &trackerHits);
  eventTree->Branch("trackerEdep", &trackerEdep);

  eventTree->Branch("CrysHCalBarAbsEdep", &HCalBarAbsEdep);
  eventTree->Branch("CrysHCalBarAbsNum", &HCalBarAbsCrysNum);
  eventTree->Branch("CrysHCalBarScintillatorEdep", &HCalBarScintillatorEdep);
  eventTree->Branch("CrysHCalBarScintillatorNum", &HCalBarScintillatorCrysNum);
  eventTree->Branch("CrysHCalBarEdep", &HCalBarEdep);
  eventTree->Branch("CrysHCalBarNum", &HCalBarCrysNum);
  eventTree->Branch("CrysHCalECAbs_r1Edep", &HCalECAbs_r1Edep);
  eventTree->Branch("CrysHCalECAbs_r1Num", &HCalECAbs_r1CrysNum);
  eventTree->Branch("CrysHCalECScn_r1Edep", &HCalECScn_r1Edep);
  eventTree->Branch("CrysHCalECScn_r1Num", &HCalECScn_r1CrysNum);
  eventTree->Branch("CrysHCalEC_r1Edep", &HCalEC_r1Edep);
  eventTree->Branch("CrysHCalEC_r1Num", &HCalEC_r1CrysNum);
  eventTree->Branch("CrysHCalECAbs_l1Edep", &HCalECAbs_l1Edep);
  eventTree->Branch("CrysHCalECAbs_l1Num", &HCalECAbs_l1CrysNum);
  eventTree->Branch("CrysHCalECScn_l1Edep", &HCalECScn_l1Edep);
  eventTree->Branch("CrysHCalECScn_l1Num", &HCalECScn_l1CrysNum);
  eventTree->Branch("CrysHCalEC_l1Edep", &HCalEC_l1Edep);
  eventTree->Branch("CrysHCalEC_l1Num", &HCalEC_l1CrysNum);
  eventTree->Branch("CrysHCalECAbs_r2Edep", &HCalECAbs_r2Edep);
  eventTree->Branch("CrysHCalECAbs_r2Num", &HCalECAbs_r2CrysNum);
  eventTree->Branch("CrysHCalECScn_r2Edep", &HCalECScn_r2Edep);
  eventTree->Branch("CrysHCalECScn_r2Num", &HCalECScn_r2CrysNum);
  eventTree->Branch("CrysHCalEC_r2Edep", &HCalEC_r2Edep);
  eventTree->Branch("CrysHCalEC_r2Num", &HCalEC_r2CrysNum);
  eventTree->Branch("CrysHCalECAbs_l2Edep", &HCalECAbs_l2Edep);
  eventTree->Branch("CrysHCalECAbs_l2Num", &HCalECAbs_l2CrysNum);
  eventTree->Branch("CrysHCalECScn_l2Edep", &HCalECScn_l2Edep);
  eventTree->Branch("CrysHCalECScn_l2Num", &HCalECScn_l2CrysNum);
  eventTree->Branch("CrysHCalEC_l2Edep", &HCalEC_l2Edep);
  eventTree->Branch("CrysHCalEC_l2Num", &HCalEC_l2CrysNum);
  
  eventTree->Branch("EventHCalEdep", &totalHCalE);
  eventTree->Branch("HCalHitNum", &totalHCalHit);

  eventTree->Branch("MuonHitX", &muonHitX);
  eventTree->Branch("MuonHitY", &muonHitY);
  eventTree->Branch("MuonHitZ", &muonHitZ);
  //eventTree->Branch("MuonHits", &muonHits);
  eventTree->Branch("MuonEdep", &muonEdep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{

  G4cout<<"Flag for pair production: "<<pair_prod_flag<<G4endl;
  
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( printModulo==0 || event->GetEventID() % printModulo != 0) return;
  
  auto primary = event->GetPrimaryVertex(0)->GetPrimary(0);
  G4cout 
    << G4endl
    << ">>> Event " << event->GetEventID() << " >>> Simulation truth : "
    << primary->GetG4code()->GetParticleName()
    << " " << primary->GetMomentum() << G4endl;
  
  // EM calorimeter
  G4cout << "EM Calorimeter has " << totalEmHit << " hits. Total Edep is "
    << GetEmEne()/MeV << " (MeV)" << G4endl;

  G4cout << "Hadron Calorimeter has " << totalHCalHit << " hits. Total Edep is "
    << GetHCalEne()/MeV << " (MeV)" << G4endl;

  sortAndSaveTrackHit();
  eventTree->Fill();

  ecalBarEdep.erase(ecalBarEdep.begin(),ecalBarEdep.begin()+ecalBarEdep.size());
  ecalECEdep_r.erase(ecalECEdep_r.begin(),ecalECEdep_r.begin()+ecalECEdep_r.size());
  ecalECEdep_l.erase(ecalECEdep_l.begin(),ecalECEdep_l.begin()+ecalECEdep_l.size());
  
  HCalBarAbsEdep.erase(HCalBarAbsEdep.begin(),HCalBarAbsEdep.begin()+HCalBarAbsEdep.size());
  HCalBarScintillatorEdep.erase(HCalBarScintillatorEdep.begin(),HCalBarScintillatorEdep.begin()+HCalBarScintillatorEdep.size());
  HCalBarEdep.erase(HCalBarEdep.begin(),HCalBarEdep.begin()+HCalBarEdep.size());

  HCalECAbs_r1Edep.erase(HCalECAbs_r1Edep.begin(),HCalECAbs_r1Edep.begin()+HCalECAbs_r1Edep.size());
  HCalECScn_r1Edep.erase(HCalECScn_r1Edep.begin(),HCalECScn_r1Edep.begin()+HCalECScn_r1Edep.size());
  HCalEC_r1Edep.erase(HCalEC_r1Edep.begin(),HCalEC_r1Edep.begin()+HCalEC_r1Edep.size());

  HCalECAbs_l1Edep.erase(HCalECAbs_l1Edep.begin(),HCalECAbs_l1Edep.begin()+HCalECAbs_l1Edep.size());
  HCalECScn_l1Edep.erase(HCalECScn_l1Edep.begin(),HCalECScn_l1Edep.begin()+HCalECScn_l1Edep.size());
  HCalEC_l1Edep.erase(HCalEC_l1Edep.begin(),HCalEC_l1Edep.begin()+HCalEC_l1Edep.size());

  HCalECAbs_r2Edep.erase(HCalECAbs_r2Edep.begin(),HCalECAbs_r2Edep.begin()+HCalECAbs_r2Edep.size());
  HCalECScn_r2Edep.erase(HCalECScn_r2Edep.begin(),HCalECScn_r2Edep.begin()+HCalECScn_r2Edep.size());
  HCalEC_r2Edep.erase(HCalEC_r2Edep.begin(),HCalEC_r2Edep.begin()+HCalEC_r2Edep.size());

  HCalECAbs_l2Edep.erase(HCalECAbs_l2Edep.begin(),HCalECAbs_l2Edep.begin()+HCalECAbs_l2Edep.size());
  HCalECScn_l2Edep.erase(HCalECScn_l2Edep.begin(),HCalECScn_l2Edep.begin()+HCalECScn_l2Edep.size());
  HCalEC_l2Edep.erase(HCalEC_l2Edep.begin(),HCalEC_l2Edep.begin()+HCalEC_l2Edep.size());
 
  convX.erase(convX.begin(),convX.begin()+convX.size());
  convY.erase(convY.begin(),convY.begin()+convY.size());
  convZ.erase(convZ.begin(),convZ.begin()+convZ.size());
  EmBarCrysNum.erase(EmBarCrysNum.begin(),EmBarCrysNum.begin()+EmBarCrysNum.size());
  EmECCrysNum_r.erase(EmECCrysNum_r.begin(),EmECCrysNum_r.begin()+EmECCrysNum_r.size());
  EmECCrysNum_l.erase(EmECCrysNum_l.begin(),EmECCrysNum_l.begin()+EmECCrysNum_l.size());

  HCalBarAbsCrysNum.erase(HCalBarAbsCrysNum.begin(),HCalBarAbsCrysNum.begin()+HCalBarAbsCrysNum.size());
  HCalBarScintillatorCrysNum.erase(HCalBarScintillatorCrysNum.begin(),HCalBarScintillatorCrysNum.begin()+HCalBarScintillatorCrysNum.size());
  HCalBarCrysNum.erase(HCalBarCrysNum.begin(),HCalBarCrysNum.begin()+HCalBarCrysNum.size());

  HCalECAbs_r1CrysNum.erase(HCalECAbs_r1CrysNum.begin(),HCalECAbs_r1CrysNum.begin()+HCalECAbs_r1CrysNum.size());
  HCalECScn_r1CrysNum.erase(HCalECScn_r1CrysNum.begin(),HCalECScn_r1CrysNum.begin()+HCalECScn_r1CrysNum.size());
  HCalEC_r1CrysNum.erase(HCalEC_r1CrysNum.begin(),HCalEC_r1CrysNum.begin()+HCalEC_r1CrysNum.size());

  HCalECAbs_l1CrysNum.erase(HCalECAbs_l1CrysNum.begin(),HCalECAbs_l1CrysNum.begin()+HCalECAbs_l1CrysNum.size());
  HCalECScn_l1CrysNum.erase(HCalECScn_l1CrysNum.begin(),HCalECScn_l1CrysNum.begin()+HCalECScn_l1CrysNum.size());
  HCalEC_l1CrysNum.erase(HCalEC_l1CrysNum.begin(),HCalEC_l1CrysNum.begin()+HCalEC_l1CrysNum.size());

  HCalECAbs_r2CrysNum.erase(HCalECAbs_r2CrysNum.begin(),HCalECAbs_r2CrysNum.begin()+HCalECAbs_r2CrysNum.size());
  HCalECScn_r2CrysNum.erase(HCalECScn_r2CrysNum.begin(),HCalECScn_r2CrysNum.begin()+HCalECScn_r2CrysNum.size());
  HCalEC_r2CrysNum.erase(HCalEC_r2CrysNum.begin(),HCalEC_r2CrysNum.begin()+HCalEC_r2CrysNum.size());

  HCalECAbs_l2CrysNum.erase(HCalECAbs_l2CrysNum.begin(),HCalECAbs_l2CrysNum.begin()+HCalECAbs_l2CrysNum.size());
  HCalECScn_l2CrysNum.erase(HCalECScn_l2CrysNum.begin(),HCalECScn_l2CrysNum.begin()+HCalECScn_l2CrysNum.size());
  HCalEC_l2CrysNum.erase(HCalEC_l2CrysNum.begin(),HCalEC_l2CrysNum.begin()+HCalEC_l2CrysNum.size());  
  TrackPosX.erase(TrackPosX.begin(),TrackPosX.begin()+TrackPosX.size());
  TrackPosY.erase(TrackPosY.begin(),TrackPosY.begin()+TrackPosY.size());
  TrackPosZ.erase(TrackPosZ.begin(),TrackPosZ.begin()+TrackPosZ.size());
  trackerHits.erase(trackerHits.begin(),trackerHits.begin()+trackerHits.size());
  trackerEdep.erase(trackerEdep.begin(),trackerEdep.begin()+trackerEdep.size());
  trackHitCollector.erase(trackHitCollector.begin(),trackHitCollector.begin()+trackHitCollector.size());

  muonHitX.erase(muonHitX.begin(),muonHitX.begin()+muonHitX.size());
  muonHitY.erase(muonHitY.begin(),muonHitY.begin()+muonHitY.size());
  muonHitZ.erase(muonHitZ.begin(),muonHitZ.begin()+muonHitZ.size());
  //muonHits.erase(muonHits.begin(),muonHits.begin()+muonHits.size());
  muonEdep.erase(muonEdep.begin(),muonEdep.begin()+muonEdep.size());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::AddEneDep(G4int copyNo, G4double edep, G4String vol) {

  if(vol=="ECalCell") {
    totalEmE += edep;
    totalEmHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<EmBarCrysNum.size(); i++) {
      if(copyNo==EmBarCrysNum.at(i)) {
	ecalBarEdep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      ecalBarEdep.push_back(edep);
      EmBarCrysNum.push_back(copyNo);
    }
  }
  if(vol=="CellEcalEC_r") {
    totalEmE += edep;
    totalEmHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<EmECCrysNum_r.size(); i++) {
      if(copyNo==EmECCrysNum_r.at(i)) {
	ecalECEdep_r[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      ecalECEdep_r.push_back(edep);
      EmECCrysNum_r.push_back(copyNo);
    }
  }
  if(vol=="CellEcalEC_l") {
    totalEmE += edep;
    totalEmHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<EmECCrysNum_l.size(); i++) {
      if(copyNo==EmECCrysNum_l.at(i)) {
	ecalECEdep_l[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      ecalECEdep_l.push_back(edep);
      EmECCrysNum_l.push_back(copyNo);
    }
  }

  //************************Starting of HCal*****************************

  if(vol=="HCalAbsorberCell") {
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalBarAbsCrysNum.size(); i++) {
      if(copyNo==HCalBarAbsCrysNum.at(i)) {
	HCalBarAbsEdep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalBarAbsEdep.push_back(edep);
      HCalBarAbsCrysNum.push_back(copyNo);
    }
  }
	
	
  if(vol=="HCalScintillatorCell") {
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalBarScintillatorCrysNum.size(); i++) {
      if(copyNo==HCalBarScintillatorCrysNum.at(i)) {
	HCalBarScintillatorEdep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalBarScintillatorEdep.push_back(edep);
      HCalBarScintillatorCrysNum.push_back(copyNo);
    }
  }
	
  if(vol=="HCalCell") {
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalBarCrysNum.size(); i++) {
      if(copyNo==HCalBarCrysNum.at(i)) {
	HCalBarEdep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalBarEdep.push_back(edep);
      HCalBarCrysNum.push_back(copyNo);
    }
  }
  
  //*****************************End of Barrel*******************************
  
  
  if(vol=="HcalECAbsCell_r1") {
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalECAbs_r1CrysNum.size(); i++) {
      if(copyNo==HCalECAbs_r1CrysNum.at(i)) {
	HCalECAbs_r1Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalECAbs_r1Edep.push_back(edep);
      HCalECAbs_r1CrysNum.push_back(copyNo);
    }
  }
	
	
  if(vol=="HcalECScnCell_r1") {
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalECScn_r1CrysNum.size(); i++) {
      if(copyNo==HCalECScn_r1CrysNum.at(i)) {
	HCalECScn_r1Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalECScn_r1Edep.push_back(edep);
      HCalECScn_r1CrysNum.push_back(copyNo);
    }
  }
	
  if(vol=="HCalCell_r1"){
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalEC_r1CrysNum.size(); i++) {
      if(copyNo==HCalEC_r1CrysNum.at(i)) {
	HCalEC_r1Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalEC_r1Edep.push_back(edep);
      HCalEC_r1CrysNum.push_back(copyNo);
    }
  }

  //************************End of EndCap_r1*******************************

  if(vol=="HcalECAbsCell_l1") {
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalECAbs_l1CrysNum.size(); i++) {
      if(copyNo==HCalECAbs_l1CrysNum.at(i)) {
	HCalECAbs_l1Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalECAbs_l1Edep.push_back(edep);
      HCalECAbs_l1CrysNum.push_back(copyNo);
    }
  }
	
	
  if(vol=="HcalECScnCell_l1"){
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalECScn_l1CrysNum.size(); i++) {
      if(copyNo==HCalECScn_l1CrysNum.at(i)) {
	HCalECScn_l1Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalECScn_l1Edep.push_back(edep);
      HCalECScn_l1CrysNum.push_back(copyNo);
    }
  }
	
  if(vol=="HCalCell_l1") {
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalEC_l1CrysNum.size(); i++) {
      if(copyNo==HCalEC_l1CrysNum.at(i)) {
	HCalEC_l1Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalEC_l1Edep.push_back(edep);
      HCalEC_l1CrysNum.push_back(copyNo);
    }
  }

  //************************End of EndCap_l1*******************************

  if(vol=="HcalECAbsCell_r2"){
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalECAbs_r2CrysNum.size(); i++) {
      if(copyNo==HCalECAbs_r2CrysNum.at(i)) {
	HCalECAbs_r2Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalECAbs_r2Edep.push_back(edep);
      HCalECAbs_r2CrysNum.push_back(copyNo);
    }
  }
	
	
  if(vol=="HcalECScnCell_r2"){
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalECScn_r2CrysNum.size(); i++) {
      if(copyNo==HCalECScn_r2CrysNum.at(i)) {
	HCalECScn_r2Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalECScn_r2Edep.push_back(edep);
      HCalECScn_r2CrysNum.push_back(copyNo);
    }
  }
	
  if(vol=="HCalCell_r2"){
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalEC_r2CrysNum.size(); i++) {
      if(copyNo==HCalEC_r2CrysNum.at(i)) {
	HCalEC_r2Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalEC_r2Edep.push_back(edep);
      HCalEC_r2CrysNum.push_back(copyNo);
    }
  }
  //************************End of EndCap_r2*******************************

  if(vol=="HcalECAbsCell_l2"){
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalECAbs_l2CrysNum.size(); i++) {
      if(copyNo==HCalECAbs_l2CrysNum.at(i)) {
	HCalECAbs_l2Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalECAbs_l2Edep.push_back(edep);
      HCalECAbs_l2CrysNum.push_back(copyNo);
    }
  }
	
	
  if(vol=="HcalECScnCell_l2"){
    totalHCalE	+= edep;
    totalHCalHit += 1;
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalECScn_l2CrysNum.size(); i++) {
      if(copyNo==HCalECScn_l2CrysNum.at(i)) {
	HCalECScn_l2Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalECScn_l2Edep.push_back(edep);
      HCalECScn_l2CrysNum.push_back(copyNo);
    }
  }
	
  if(vol=="HCalCell_l2"){
    G4bool isNew = true;
    for(unsigned int i=0; i<HCalEC_l2CrysNum.size(); i++) {
      if(copyNo==HCalEC_l2CrysNum.at(i)) {
	HCalEC_l2Edep[i] += edep;
	isNew = false;
	break;
      }
    }
    if(isNew) {
      HCalEC_l2Edep.push_back(edep);
      HCalEC_l2CrysNum.push_back(copyNo);
    }
  }
  //************************End of EndCap_l2*******************************	

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::fillTrackHit(long eta, long phi, long rho, long hitfactor, double edep) {

  std::vector<
    std::pair< long,std::vector<
      std::pair< long,std::vector<
	std::pair< long,std::pair<long,double> >
			> >
		      > >
    >::iterator epr_it;
  
  G4bool found_pos = false;
  
  for(epr_it=trackHitCollector.begin(); epr_it<trackHitCollector.end(); ++epr_it) {

    if(found_pos == true) break; // Ensure no infinite loop

    // Sort along eta
    if(eta>((*epr_it).first)) continue;
    
    else if(eta==((*epr_it).first)) {  // equal then sort along phi

      std::vector<
	std::pair< long,std::vector<
	  std::pair< long,std::pair<long,double> >
			  > >
	>::iterator pr_it;
      for(pr_it=((*epr_it).second).begin(); pr_it<((*epr_it).second).end(); ++pr_it) {

	if(found_pos == true) break; // Ensure no infinite loop

	// Sort along phi
	if(phi>((*pr_it).first)) continue;
    
	else if(phi==((*pr_it).first)) {  // equal then sort along rho

	  std::vector<
	    std::pair< long,std::pair<long,double> >
	    >::iterator r_it;

	  for(r_it=((*pr_it).second).begin(); r_it<((*pr_it).second).end(); ++r_it) {
	    
	    if(found_pos == true) break; // Ensure no infinite loop

	    // Sort along rho
	    if(rho>((*r_it).first)) continue;
	    else if(rho==((*r_it).first)) {  // equal then add energy
	      ((*r_it).second).second += edep;
	      found_pos = true;
	      break;
	    }
	    else { // less then create another branch
	      ((*pr_it).second).insert(r_it,std::make_pair(rho,std::make_pair(hitfactor,edep)));
	      found_pos = true;
	      break;
	    }
	  }
	  if(found_pos==false) {
	    ((*pr_it).second).push_back(std::make_pair(rho,std::make_pair(hitfactor,edep)));
	    found_pos = true;
	    break;
	  }
	  
	}

	else { // less then create another branch

	  std::vector<
	    std::pair< long,std::pair<long,double> >
	    > firstVec;
	  firstVec.push_back(std::make_pair(rho,std::make_pair(hitfactor,edep)));
    
	  ((*epr_it).second).insert(pr_it,std::make_pair(phi,firstVec));
	  
	  found_pos = true;
	  break;
	  
	}
	
      }
      if(found_pos==false) {
	std::vector<
	  std::pair< long,std::pair<long,double> >
	  > firstVec;
	firstVec.push_back(std::make_pair(rho,std::make_pair(hitfactor,edep)));
	((*epr_it).second).push_back(std::make_pair(phi,firstVec));
	found_pos = true;
	break;
      }
      
    }
    
    else { // less then create another branch
	
    std::vector<
      std::pair< long,std::pair<long,double> >
      > firstVec;
    firstVec.push_back(std::make_pair(rho,std::make_pair(hitfactor,edep)));
    std::vector<
      std::pair< long,std::vector<
	std::pair< long,std::pair<long,double> >
			> >
      > secondVec;
    secondVec.push_back(std::make_pair(phi,firstVec));
    
    trackHitCollector.insert(epr_it,std::make_pair(eta,secondVec));

    found_pos = true;
    break;

    }
    
  }
    
  if(found_pos == false) {

    std::vector<
      std::pair< long,std::pair<long,double> >
      > firstVec;
    firstVec.push_back(std::make_pair(rho,std::make_pair(hitfactor,edep)));
    std::vector<
      std::pair< long,std::vector<
	std::pair< long,std::pair<long,double> >
			> >
      > secondVec;
    secondVec.push_back(std::make_pair(phi,firstVec));

    trackHitCollector.push_back(std::make_pair(eta,secondVec));
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::sortAndSaveTrackHit() {

  std::vector<
    std::pair< long,std::vector<
      std::pair< long,std::vector<
	std::pair< long,std::pair<long,double> >
			> >
		      > >
    >::iterator epr_it;
  
  for(epr_it=trackHitCollector.begin(); epr_it<trackHitCollector.end(); ++epr_it) {

        std::vector<
	std::pair< long,std::vector<
	  std::pair< long,std::pair<long,double> >
			  > >
	>::iterator pr_it;
      for(pr_it=((*epr_it).second).begin(); pr_it<((*epr_it).second).end(); ++pr_it) {
	
	  std::vector<
	    std::pair< long,std::pair<long,double> >
	    >::iterator r_it;
	  for(r_it=((*pr_it).second).begin(); r_it<((*pr_it).second).end(); ++r_it) {
	    std::pair<long,double> storeVal = (*r_it).second;
	    trackerHits.push_back(storeVal.first);
	    trackerEdep.push_back(storeVal.second);
	    //std::cout<<(*epr_it).first<<"\t"<<(*pr_it).first<<"\t"<<(*r_it).first<<"\t"<<storeVal.first<<"\t"<<storeVal.second<<std::endl;
	  }
      }
  }
}

void EventAction::fillMuonHit(G4String physName, G4ThreeVector pos, G4double edep) {

    muonHitX.push_back(pos.getX());
    muonHitY.push_back(pos.getY());
    muonHitZ.push_back(pos.getZ());
    //TVector3* vec3 = new TVector3(pos.getX(), pos.getY(), pos.getZ());
    //muonHits.push_back(vec3);
    muonEdep.push_back(edep);
  
  
}
