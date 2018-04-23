#include <fstream>
#include <string>
#include <cmath>

#include "ECalEventAction.hh"
#include "ECalEmCalorimeterHit.hh"
#include "ECalECHit.hh"
#include "ECalECHit_left.hh"
#include "ECalWorldHit.hh"
#include "ECaltrackerHit.hh"
//#include "ECalHadCalorimeterHit.hh"
#include "ECalConstants.hh"
#include "ECalAnalysis.hh"
#include "ECalSteppingAction.hh"
#include "ECalECParameterisation.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalEventAction::ECalEventAction()
: G4UserEventAction(), 
  fEmCalEdep(), 
  fConvertedX(), fConvertedY(), fConvertedZ(), 
  fEmCalCrysNum(), 
  fCellNoEcalEC_r(), fEdepEcalEC_r(), fCellNoEcalEC_l(), fEdepEcalEC_l()
{

	pair_prod_flag = 0;

  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalEventAction::~ECalEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalEventAction::BeginOfEventAction(const G4Event*)
{
    auto sdManager = G4SDManager::GetSDMpointer();
    fEmCalHCID = sdManager->GetCollectionID("EMcalorimeter/EMcalorimeterColl");
    fEmCalECHCID = sdManager->GetCollectionID("EMcalorimeterEC/EMcalorimeterECColl");
    fEmCalECHCID_left = sdManager->GetCollectionID("EMcalorimeterEC_left/EMcalorimeterECColl");
    fWorldHCID = sdManager->GetCollectionID("World/WorldColl");
    ftrackerHCID = sdManager->GetCollectionID("Tracker/trackerColl");
    ftrackerEnvHCID = sdManager->GetCollectionID("TrackerEnv/trackerColl");

	pair_prod_flag = 0;
}     

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalEventAction::EndOfEventAction(const G4Event* event)
{
  auto hce = event->GetHCofThisEvent();
  if (!hce) {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl; 
      G4Exception("ECalEventAction::EndOfEventAction()",
                  "ECalCode001", JustWarning, msg);
      return;
  }

   auto ecHC 
    = static_cast<ECalEmCalorimeterHitsCollection*>(hce->GetHC(fEmCalHCID));
   auto ecECHC 
    = static_cast<ECalECHitsCollection*>(hce->GetHC(fEmCalECHCID));
   auto ecECHC_left 
    = static_cast<ECalECHit_leftsCollection*>(hce->GetHC(fEmCalECHCID_left));
   auto wHC 
    = static_cast<ECalWorldHitsCollection*>(hce->GetHC(fWorldHCID));
   auto tHC 
    = static_cast<ECaltrackerHitsCollection*>(hce->GetHC(ftrackerHCID));
   auto tHCE 
    = static_cast<ECaltrackerHitsCollection*>(hce->GetHC(ftrackerHCID));

//  if ( (!hHC1) || (!hHC2) || (!dHC1) || (!dHC2) || (!ecHC) || (!hcHC) ) {
  if ( !ecHC || !ecECHC || !ecECHC_left || !wHC || !tHC || !tHCE) {
      G4ExceptionDescription msg;
      msg << "Some of hits collections of this event not found." << G4endl; 
      G4Exception("ECalEventAction::EndOfEventAction()",
                  "ECalCode001", JustWarning, msg);
      return;
  }  

  //
  // Fill histograms & ntuple
  // 
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // ecalEnergy
  G4int totalEmHit = 0;
  G4double totalEmE = 0.;
	auto j = 0;
  for (auto i=0;i<kNofEmCells;i++) {
    auto hit = (*ecHC)[i];
    auto edep = hit->GetEdep();
    if (edep>1*keV) {
      totalEmHit++;
      totalEmE += edep;

			fEmCalEdep.push_back(edep);
			fEmCalCrysNum.push_back(i);
			j++;
//	auto lp = hit->GetPos();
//	std::cout<<sqrt(lp.x()*lp.x()+lp.y()*lp.y())<<"\t"<<lp.z()<<"\n";
    }
  }
    
  ECalECParameterisation ecalECparam;
  for(auto num=0; num<kNofEmECCells; num++)
  {
	auto hit = (*ecECHC)[num];
	auto hit_left = (*ecECHC_left)[num];
	auto edep = hit->GetEdep();
	auto edep_left = hit_left->GetEdep();
	totalEmE += edep;
	totalEmE += edep_left;
	if(edep>0.)
	{
		totalEmHit++;
		fCellNoEcalEC_r.push_back(num);
		fEdepEcalEC_r.push_back(edep);
		G4cout<<"("<<ecalECparam.cell_pos[num][0]<<","<<ecalECparam.cell_pos[num][1]<<")\t"<<edep<<"\t!!"<<G4endl;
	}
	if(edep_left>0.)
	{
		totalEmHit++;
		fCellNoEcalEC_l.push_back(num);
		fEdepEcalEC_l.push_back(edep_left);
		G4cout<<"("<<ecalECparam.cell_pos[num][0]<<","<<ecalECparam.cell_pos[num][1]<<")\t left endcap \t"<<edep_left<<"\t$$"<<G4endl;
	}
  }

//	datfile.close();
  //
  // Print diagnostics
  // 

	G4cout<<"Flag for pair production: "<<pair_prod_flag<<G4endl;
	analysisManager->FillNtupleIColumn(4,pair_prod_flag);
  
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
    << totalEmE/MeV << " (MeV)" << G4endl;

	analysisManager->FillNtupleDColumn(2,totalEmE/MeV);
	analysisManager->FillNtupleIColumn(3,totalEmHit);
	analysisManager->AddNtupleRow();

	fEmCalEdep.erase(fEmCalEdep.begin(),fEmCalEdep.begin()+fEmCalEdep.size());
	fConvertedX.erase(fConvertedX.begin(),fConvertedX.begin()+fConvertedX.size());
	fConvertedY.erase(fConvertedY.begin(),fConvertedY.begin()+fConvertedY.size());
	fConvertedZ.erase(fConvertedZ.begin(),fConvertedZ.begin()+fConvertedZ.size());
	fEmCalCrysNum.erase(fEmCalCrysNum.begin(),fEmCalCrysNum.begin()+fEmCalCrysNum.size());
	fCellNoEcalEC_r.erase(fCellNoEcalEC_r.begin(),fCellNoEcalEC_r.begin()+fCellNoEcalEC_r.size());
	fEdepEcalEC_r.erase(fEdepEcalEC_r.begin(),fEdepEcalEC_r.begin()+fEdepEcalEC_r.size());
	fCellNoEcalEC_l.erase(fCellNoEcalEC_l.begin(),fCellNoEcalEC_l.begin()+fCellNoEcalEC_l.size());
	fEdepEcalEC_l.erase(fEdepEcalEC_l.begin(),fEdepEcalEC_l.begin()+fEdepEcalEC_l.size());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
