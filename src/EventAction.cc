#include <fstream>
#include <string>
#include <cmath>

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
  totalEmHit(0), totalEmE(0.),
  totalHCalHit(0), totalHCalE(0.)
{

  pair_prod_flag = 0;

  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{

  pair_prod_flag = 0;
  totalEmHit = 0;
  totalEmE = 0.;
  totalHCalHit = 0;
  totalHCalE = 0.;

}     

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  //G4cout<<"Flag for pair production: "<<pair_prod_flag<<G4endl;
  analysisManager->FillNtupleIColumn(8,pair_prod_flag);
  
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

  analysisManager->FillNtupleDColumn(6,totalEmE/MeV);
  analysisManager->FillNtupleIColumn(7,totalEmHit);

  analysisManager->FillNtupleDColumn(45,totalHCalE/MeV);
  analysisManager->FillNtupleIColumn(46,totalHCalHit);
  
  analysisManager->AddNtupleRow();

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

	//************************Starting of HCal*****************************
	if(vol=="HCalAbsorberCell"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalBarAbsCrysNum.size(); i++)
	    {
	      if(copyNo==HCalBarAbsCrysNum.at(i))
		{
		  HCalBarAbsEdep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalBarAbsEdep.push_back(edep);
	      HCalBarAbsCrysNum.push_back(copyNo);
	    }
	}
	
	
	if(vol=="HCalScintillatorCell"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalBarScintillatorCrysNum.size(); i++)
	    {
	      if(copyNo==HCalBarScintillatorCrysNum.at(i))
		{
		  HCalBarScintillatorEdep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalBarScintillatorEdep.push_back(edep);
	      HCalBarScintillatorCrysNum.push_back(copyNo);
	    }
	}
	
	if(vol=="HCalCell"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalBarCrysNum.size(); i++)
	    {
	      if(copyNo==HCalBarCrysNum.at(i))
		{
		  HCalBarEdep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalBarEdep.push_back(edep);
	      HCalBarCrysNum.push_back(copyNo);
	    }
	}

	//*****************************End of Barrel*******************************


	if(vol=="HcalECAbsCell_r1"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalECAbs_r1CrysNum.size(); i++)
	    {
	      if(copyNo==HCalECAbs_r1CrysNum.at(i))
		{
		  HCalECAbs_r1Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalECAbs_r1Edep.push_back(edep);
	      HCalECAbs_r1CrysNum.push_back(copyNo);
	    }
	}
	
	
	if(vol=="HcalECScnCell_r1"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalECScn_r1CrysNum.size(); i++)
	    {
	      if(copyNo==HCalECScn_r1CrysNum.at(i))
		{
		  HCalECScn_r1Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalECScn_r1Edep.push_back(edep);
	      HCalECScn_r1CrysNum.push_back(copyNo);
	    }
	}
	
	if(vol=="HCalCell_r1"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalEC_r1CrysNum.size(); i++)
	    {
	      if(copyNo==HCalEC_r1CrysNum.at(i))
		{
		  HCalEC_r1Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalEC_r1Edep.push_back(edep);
	      HCalEC_r1CrysNum.push_back(copyNo);
	    }
	}
	//************************End of EndCap_r1*******************************

	if(vol=="HcalECAbsCell_l1"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalECAbs_l1CrysNum.size(); i++)
	    {
	      if(copyNo==HCalECAbs_l1CrysNum.at(i))
		{
		  HCalECAbs_l1Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalECAbs_l1Edep.push_back(edep);
	      HCalECAbs_l1CrysNum.push_back(copyNo);
	    }
	}
	
	
	if(vol=="HcalECScnCell_l1"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalECScn_l1CrysNum.size(); i++)
	    {
	      if(copyNo==HCalECScn_l1CrysNum.at(i))
		{
		  HCalECScn_l1Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalECScn_l1Edep.push_back(edep);
	      HCalECScn_l1CrysNum.push_back(copyNo);
	    }
	}
	
	if(vol=="HCalCell_l1"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalEC_l1CrysNum.size(); i++)
	    {
	      if(copyNo==HCalEC_l1CrysNum.at(i))
		{
		  HCalEC_l1Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalEC_l1Edep.push_back(edep);
	      HCalEC_l1CrysNum.push_back(copyNo);
	    }
	}
	//************************End of EndCap_l1*******************************

	if(vol=="HcalECAbsCell_r2"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalECAbs_r2CrysNum.size(); i++)
	    {
	      if(copyNo==HCalECAbs_r2CrysNum.at(i))
		{
		  HCalECAbs_r2Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalECAbs_r2Edep.push_back(edep);
	      HCalECAbs_r2CrysNum.push_back(copyNo);
	    }
	}
	
	
	if(vol=="HcalECScnCell_r2"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalECScn_r2CrysNum.size(); i++)
	    {
	      if(copyNo==HCalECScn_r2CrysNum.at(i))
		{
		  HCalECScn_r2Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalECScn_r2Edep.push_back(edep);
	      HCalECScn_r2CrysNum.push_back(copyNo);
	    }
	}
	
	if(vol=="HCalCell_r2"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalEC_r2CrysNum.size(); i++)
	    {
	      if(copyNo==HCalEC_r2CrysNum.at(i))
		{
		  HCalEC_r2Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalEC_r2Edep.push_back(edep);
	      HCalEC_r2CrysNum.push_back(copyNo);
	    }
	}
	//************************End of EndCap_r2*******************************

	if(vol=="HcalECAbsCell_l2"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalECAbs_l2CrysNum.size(); i++)
	    {
	      if(copyNo==HCalECAbs_l2CrysNum.at(i))
		{
		  HCalECAbs_l2Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalECAbs_l2Edep.push_back(edep);
	      HCalECAbs_l2CrysNum.push_back(copyNo);
	    }
	}
	
	
	if(vol=="HcalECScnCell_l2"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalECScn_l2CrysNum.size(); i++)
	    {
	      if(copyNo==HCalECScn_l2CrysNum.at(i))
		{
		  HCalECScn_l2Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalECScn_l2Edep.push_back(edep);
	      HCalECScn_l2CrysNum.push_back(copyNo);
	    }
	}
	
	if(vol=="HCalCell_l2"){
	  G4bool isNew = true;
	  for(unsigned int i=0; i<HCalEC_l2CrysNum.size(); i++)
	    {
	      if(copyNo==HCalEC_l2CrysNum.at(i))
		{
		  HCalEC_l2Edep[i] += edep;
		  isNew = false;
		  break;
		}
	    }
	  if(isNew)
	    {
	      HCalEC_l2Edep.push_back(edep);
	      HCalEC_l2CrysNum.push_back(copyNo);
	    }
	}
	//************************End of EndCap_l2*******************************
	
	

}

