#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include "EventAction.hh"
#include "Constants.hh"
#include "Analysis.hh"
#include "SteppingAction.hh"

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
  ecalBarEdep(), ecalECEdep_r(), ecalECEdep_l(),
  convX(), convY(), convZ(), 
  EmBarCrysNum(), EmECCrysNum_r(), EmECCrysNum_l(),
  TrackPosX(), TrackPosY(), TrackPosZ(),
  totalHCalHit(0), totalHCalE(0.),
  trackerHits(),trackerEdep(),
  trackHitCollector(){

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

  eventTree->Branch("CrysHCalBarAbsEdep", fEventAction->GetHCalBarAbsEdep());
  eventTree->Branch("CrysHCalBarAbsNum", fEventAction->GetHCalBarAbsCrysNum());                 
  eventTree->Branch("CrysHCalBarScintillatorEdep", fEventAction->GetHCalBarScintillatorEdep());
  eventTree->Branch("CrysHCalBarScintillatorNum", fEventAction->GetHCalBarScintillatorCrysNum());   
  eventTree->Branch("CrysHCalBarEdep", fEventAction->GetHCalBarEdep());
  eventTree->Branch("CrysHCalBarNum", fEventAction->GetHCalBarCrysNum());  
  eventTree->Branch("CrysHCalECAbs_r1Edep", fEventAction->GetHCalECAbs_r1Edep());
  eventTree->Branch("CrysHCalECAbs_r1Num", fEventAction->GetHCalECAbs_r1CrysNum());             
  eventTree->Branch("CrysHCalECScn_r1Edep", fEventAction->GetHCalECScn_r1Edep());
  eventTree->Branch("CrysHCalECScn_r1Num", fEventAction->GetHCalECScn_r1CrysNum());             
  eventTree->Branch("CrysHCalEC_r1Edep", fEventAction->GetHCalEC_r1Edep());
  eventTree->Branch("CrysHCalEC_r1Num", fEventAction->GetHCalEC_r1CrysNum());
  eventTree->Branch("CrysHCalECAbs_l1Edep", fEventAction->GetHCalECAbs_l1Edep());
  eventTree->Branch("CrysHCalECAbs_l1Num", fEventAction->GetHCalECAbs_l1CrysNum());           
  eventTree->Branch("CrysHCalECScn_l1Edep", fEventAction->GetHCalECScn_l1Edep());       
  eventTree->Branch("CrysHCalECScn_l1Num", fEventAction->GetHCalECScn_l1CrysNum());    
  eventTree->Branch("CrysHCalEC_l1Edep", fEventAction->GetHCalEC_l1Edep());                 
  eventTree->Branch("CrysHCalEC_l1Num", fEventAction->GetHCalEC_l1CrysNum());   
  eventTree->Branch("CrysHCalECAbs_r2Edep", fEventAction->GetHCalECAbs_r2Edep());      
  eventTree->Branch("CrysHCalECAbs_r2Num", fEventAction->GetHCalECAbs_r2CrysNum());     
  eventTree->Branch("CrysHCalECScn_r2Edep", fEventAction->GetHCalECScn_r2Edep());  
  eventTree->Branch("CrysHCalECScn_r2Num", fEventAction->GetHCalECScn_r2CrysNum()); 
  eventTree->Branch("CrysHCalEC_r2Edep", fEventAction->GetHCalEC_r2Edep());        
  eventTree->Branch("CrysHCalEC_r2Num", fEventAction->GetHCalEC_r2CrysNum());    
  eventTree->Branch("CrysHCalECAbs_l2Edep", fEventAction->GetHCalECAbs_l2Edep());  
  eventTree->Branch("CrysHCalECAbs_l2Num", fEventAction->GetHCalECAbs_l2CrysNum()); 
  eventTree->Branch("CrysHCalECScn_l2Edep", fEventAction->GetHCalECScn_l2Edep());       
  eventTree->Branch("CrysHCalECScn_l2Num", fEventAction->GetHCalECScn_l2CrysNum());      
  eventTree->Branch("CrysHCalEC_l2Edep", fEventAction->GetHCalEC_l2Edep());           
  eventTree->Branch("CrysHCalEC_l2Num", fEventAction->GetHCalEC_l2CrysNum());
  
  eventTree->Branch("EventHCalEdep");
  eventTree->Branch("HCalHitNum");                                             
    
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
  convX.erase(convX.begin(),convX.begin()+convX.size());
  convY.erase(convY.begin(),convY.begin()+convY.size());
  convZ.erase(convZ.begin(),convZ.begin()+convZ.size());
  EmBarCrysNum.erase(EmBarCrysNum.begin(),EmBarCrysNum.begin()+EmBarCrysNum.size());
  EmECCrysNum_r.erase(EmECCrysNum_r.begin(),EmECCrysNum_r.begin()+EmECCrysNum_r.size());
  EmECCrysNum_l.erase(EmECCrysNum_l.begin(),EmECCrysNum_l.begin()+EmECCrysNum_l.size());
  TrackPosX.erase(TrackPosX.begin(),TrackPosX.begin()+TrackPosX.size());
  TrackPosY.erase(TrackPosY.begin(),TrackPosY.begin()+TrackPosY.size());
  TrackPosZ.erase(TrackPosZ.begin(),TrackPosZ.begin()+TrackPosZ.size());
  trackerHits.erase(trackerHits.begin(),trackerHits.begin()+trackerHits.size());
  trackerEdep.erase(trackerEdep.begin(),trackerEdep.begin()+trackerEdep.size());
  trackHitCollector.erase(trackHitCollector.begin(),trackHitCollector.begin()+trackHitCollector.size());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::AddEneDep(G4int copyNo, G4double edep, G4String vol)
{
	totalEmE += edep;
	if(vol=="ECalCell")
	{
		G4bool isNew = true;
		for(unsigned int i=0; i<EmBarCrysNum.size(); i++)
		{
			if(copyNo==EmBarCrysNum.at(i))
			{
				ecalBarEdep[i] += edep;
				isNew = false;
				break;
			}
		}
		if(isNew)
		{
			ecalBarEdep.push_back(edep);
			EmBarCrysNum.push_back(copyNo);
		}
	}
	if(vol=="CellEcalEC_r")
	{
		G4bool isNew = true;
		for(unsigned int i=0; i<EmECCrysNum_r.size(); i++)
		{
			if(copyNo==EmECCrysNum_r.at(i))
			{
				ecalECEdep_r[i] += edep;
				isNew = false;
				break;
			}
		}
		if(isNew)
		{
			ecalECEdep_r.push_back(edep);
			EmECCrysNum_r.push_back(copyNo);
		}
	}
	if(vol=="CellEcalEC_l")
	{
		G4bool isNew = true;
		for(unsigned int i=0; i<EmECCrysNum_l.size(); i++)
		{
			if(copyNo==EmECCrysNum_l.at(i))
			{
				ecalECEdep_l[i] += edep;
				isNew = false;
				break;
			}
		}
		if(isNew)
		{
			ecalECEdep_l.push_back(edep);
			EmECCrysNum_l.push_back(copyNo);
		}
	}
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
	    std::cout<<(*epr_it).first<<"\t"<<(*pr_it).first<<"\t"<<(*r_it).first<<"\t"<<storeVal.first<<"\t"<<storeVal.second<<std::endl;
	  }
      }
  }
}
