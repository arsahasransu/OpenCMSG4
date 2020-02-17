#include "RunAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"
#include "Constants.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <ctime>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction* eventAction):
	G4UserRunAction(),
	fEventAction(eventAction)
{ 
	// Create analysis manager
	// The choice of analysis technology is done via selectin of a namespace
	// in Analysis.hh
	auto analysisManager = G4AnalysisManager::Instance();
	G4cout << "Using " << analysisManager->GetType() << G4endl;

	// Default settings
	analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetFileName("OpenCMSG4");

	// Book histograms, ntuple
	//
  

	// Creating ntuple
	//
	if ( fEventAction ) {
	  analysisManager->CreateNtuple("Events", "Events");		// tuple ID = 0
	  analysisManager->CreateNtupleDColumn("CrysEdep", fEventAction->GetEmCalEdep());                   // column Id = 0
	  analysisManager->CreateNtupleIColumn("CrysNum", fEventAction->GetEmCalCrysNum());                 // column Id = 1

	  analysisManager->CreateNtupleDColumn("CrysEdepEC_r", fEventAction->GetEmCalECEdep_r());           // column Id = 2
 	  analysisManager->CreateNtupleIColumn("CrysNumEC_r", fEventAction->GetEmCalECCrysNum_r());         // column Id = 3

	  analysisManager->CreateNtupleDColumn("CrysEdepEC_l", fEventAction->GetEmCalECEdep_l());           // column Id = 4
	  analysisManager->CreateNtupleIColumn("CrysNumEC_l", fEventAction->GetEmCalECCrysNum_l());         // column Id = 5

	  analysisManager->CreateNtupleDColumn("EventEdep");                                                // column Id = 6
	  analysisManager->CreateNtupleIColumn("HitNum");                                                   // column Id = 7

	  analysisManager->CreateNtupleIColumn("ConvertedFlag");                                            // column Id = 8
	  
	  analysisManager->CreateNtupleDColumn("ConvertedX", fEventAction->GetConvertedX());                // column Id = 9
	  analysisManager->CreateNtupleDColumn("ConvertedY", fEventAction->GetConvertedY());                // column Id = 10
	  analysisManager->CreateNtupleDColumn("ConvertedZ", fEventAction->GetConvertedZ());                // column Id = 11
	  
	  analysisManager->CreateNtupleDColumn("TrackerHitPositionX", fEventAction->GetTrackPosX());        // column Id = 12
	  analysisManager->CreateNtupleDColumn("TrackerHitPositionY", fEventAction->GetTrackPosY());        // column Id = 13	      
	  analysisManager->CreateNtupleDColumn("TrackerHitPositionZ", fEventAction->GetTrackPosZ());        // column Id = 14

	  analysisManager->CreateNtupleDColumn("CrysHCalBarAbsEdep", fEventAction->GetHCalBarAbsEdep());                   // column Id = 15
	  analysisManager->CreateNtupleIColumn("CrysHCalBarAbsNum", fEventAction->GetHCalBarAbsCrysNum());                 // column Id = 16

	  analysisManager->CreateNtupleDColumn("CrysHCalBarScintillatorEdep", fEventAction->GetHCalBarScintillatorEdep());                   // column Id = 17
	  analysisManager->CreateNtupleIColumn("CrysHCalBarScintillatorNum", fEventAction->GetHCalBarScintillatorCrysNum());                 // column Id = 18

	  analysisManager->CreateNtupleDColumn("CrysHCalBarEdep", fEventAction->GetHCalBarEdep());                   // column Id = 19
	  analysisManager->CreateNtupleIColumn("CrysHCalBarNum", fEventAction->GetHCalBarCrysNum());                 // column Id = 20


	  analysisManager->CreateNtupleDColumn("CrysHCalECAbs_r1Edep", fEventAction->GetHCalECAbs_r1Edep());                   // column Id = 21
	  analysisManager->CreateNtupleIColumn("CrysHCalECAbs_r1Num", fEventAction->GetHCalECAbs_r1CrysNum());                 // column Id = 22

	  analysisManager->CreateNtupleDColumn("CrysHCalECScn_r1Edep", fEventAction->GetHCalECScn_r1Edep());                   // column Id = 23
	  analysisManager->CreateNtupleIColumn("CrysHCalECScn_r1Num", fEventAction->GetHCalECScn_r1CrysNum());                 // column Id = 24

	  analysisManager->CreateNtupleDColumn("CrysHCalEC_r1Edep", fEventAction->GetHCalEC_r1Edep());                   // column Id = 25
	  analysisManager->CreateNtupleIColumn("CrysHCalEC_r1Num", fEventAction->GetHCalEC_r1CrysNum());                 // column Id = 26


	  analysisManager->CreateNtupleDColumn("CrysHCalECAbs_l1Edep", fEventAction->GetHCalECAbs_l1Edep());                   // column Id = 27
	  analysisManager->CreateNtupleIColumn("CrysHCalECAbs_l1Num", fEventAction->GetHCalECAbs_l1CrysNum());                 // column Id = 28

	  analysisManager->CreateNtupleDColumn("CrysHCalECScn_l1Edep", fEventAction->GetHCalECScn_l1Edep());                   // column Id = 29
	  analysisManager->CreateNtupleIColumn("CrysHCalECScn_l1Num", fEventAction->GetHCalECScn_l1CrysNum());                 // column Id = 30

	  analysisManager->CreateNtupleDColumn("CrysHCalEC_l1Edep", fEventAction->GetHCalEC_l1Edep());                   // column Id = 31
	  analysisManager->CreateNtupleIColumn("CrysHCalEC_l1Num", fEventAction->GetHCalEC_l1CrysNum());                 // column Id = 32


	  analysisManager->CreateNtupleDColumn("CrysHCalECAbs_r2Edep", fEventAction->GetHCalECAbs_r2Edep());                   // column Id = 33
	  analysisManager->CreateNtupleIColumn("CrysHCalECAbs_r2Num", fEventAction->GetHCalECAbs_r2CrysNum());                 // column Id = 34

	  analysisManager->CreateNtupleDColumn("CrysHCalECScn_r2Edep", fEventAction->GetHCalECScn_r2Edep());                   // column Id = 35
	  analysisManager->CreateNtupleIColumn("CrysHCalECScn_r2Num", fEventAction->GetHCalECScn_r2CrysNum());                 // column Id = 236

	  analysisManager->CreateNtupleDColumn("CrysHCalEC_r2Edep", fEventAction->GetHCalEC_r2Edep());                   // column Id = 37
	  analysisManager->CreateNtupleIColumn("CrysHCalEC_r2Num", fEventAction->GetHCalEC_r2CrysNum());                 // column Id = 38


	  analysisManager->CreateNtupleDColumn("CrysHCalECAbs_l2Edep", fEventAction->GetHCalECAbs_l2Edep());                   // column Id = 39
	  analysisManager->CreateNtupleIColumn("CrysHCalECAbs_l2Num", fEventAction->GetHCalECAbs_l2CrysNum());                 // column Id = 40

	  analysisManager->CreateNtupleDColumn("CrysHCalECScn_l2Edep", fEventAction->GetHCalECScn_l2Edep());                   // column Id = 41
	  analysisManager->CreateNtupleIColumn("CrysHCalECScn_l2Num", fEventAction->GetHCalECScn_l2CrysNum());                 // column Id = 42

	  analysisManager->CreateNtupleDColumn("CrysHCalEC_l2Edep", fEventAction->GetHCalEC_l2Edep());                   // column Id = 43
	  analysisManager->CreateNtupleIColumn("CrysHCalEC_l2Num", fEventAction->GetHCalEC_l2CrysNum());                 // column Id = 44


	  analysisManager->CreateNtupleDColumn("EventHCalEdep");                                            // column Id = 45
	  analysisManager->CreateNtupleIColumn("HCalHitNum");                                               // column Id = 46
	    

	  analysisManager->FinishNtuple();
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
	delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{ 

	// Remember the start time to store the duration of a run.
	start = time(NULL);
	myfile.open("time_keeper.txt",std::ios::app);
  
	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();

	// Open an output file 
	// The default file name is set in RunAction::RunAction(),
	// it can be overwritten in a macro
	analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{

	// save histograms & ntuple
	//
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();

	// Append the run time to a file
	end = time(NULL);
	std::cout<<"The time of the run was "<<difftime(end,start)
				<<" seconds."<<std::endl;
	myfile<<"The time of the run was "<<difftime(end,start)
				<<" seconds."<<std::endl;
	myfile<<"-------------------------------------------------------------"
				<<std::endl;
	myfile.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
