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
  convX(), convY(), convZ(), 
  EmBarCrysNum(), EmECCrysNum_r(), EmECCrysNum_l(),
  TrackPosX(), TrackPosY(), TrackPosZ(),
  totalEmHit(0), totalEmE(0.)
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

}     

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  G4cout<<"Flag for pair production: "<<pair_prod_flag<<G4endl;
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

  analysisManager->FillNtupleDColumn(6,totalEmE/MeV);
  analysisManager->FillNtupleIColumn(7,totalEmHit);
  analysisManager->AddNtupleRow();

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

