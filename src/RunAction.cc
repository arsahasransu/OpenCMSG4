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
	if ( fEventAction ) 
	{
		analysisManager->CreateNtuple("Events", "Events");		// tuple ID = 0
    	analysisManager                                   // column Id = 0
    		->CreateNtupleDColumn("CrysEdep", fEventAction->GetEmCalEdep()); 
    	analysisManager                                   // column Id = 1
    		->CreateNtupleIColumn("CrysNum", fEventAction->GetEmCalCrysNum()); 
    	analysisManager                                   // column Id = 2
    		->CreateNtupleDColumn("CrysEdepEC_r", 
									fEventAction->GetEmCalECEdep_r()); 
	    analysisManager                                   // column Id = 3
		    ->CreateNtupleIColumn("CrysNumEC_r", 
									fEventAction->GetEmCalECCrysNum_r()); 
	    analysisManager                                   // column Id = 4
		    ->CreateNtupleDColumn("CrysEdepEC_l", 
									fEventAction->GetEmCalECEdep_l()); 
	    analysisManager                                   // column Id = 5
    		->CreateNtupleIColumn("CrysNumEC_l", 
									fEventAction->GetEmCalECCrysNum_l()); 
	    analysisManager                                   // column Id = 6
		    ->CreateNtupleDColumn("EventEdep"); 
	    analysisManager                                   // column Id = 7
    		->CreateNtupleIColumn("HitNum"); 
	    analysisManager                                   // column Id = 8
    		->CreateNtupleIColumn("ConvertedFlag"); 
	    analysisManager                                   // column Id = 9
		   	->CreateNtupleDColumn("ConvertedX", fEventAction->GetConvertedX()); 
	    analysisManager                                   // column Id = 10
		    ->CreateNtupleDColumn("ConvertedY", fEventAction->GetConvertedY()); 
	    analysisManager                                   // column Id = 11
		    ->CreateNtupleDColumn("ConvertedZ", fEventAction->GetConvertedZ()); 
	    analysisManager                                  // column Id = 12
	      ->CreateNtupleDColumn("TrackerHitPositionX", fEventAction->GetTrackPosX());
	    analysisManager                                  // column Id = 13
	      ->CreateNtupleDColumn("TrackerHitPositionY", fEventAction->GetTrackPosY());
	    analysisManager                                  // column Id = 14
	      ->CreateNtupleDColumn("TrackerHitPositionZ", fEventAction->GetTrackPosZ());
	    analysisManager                                  // column Id = 14
	      ->CreateNtupleIColumn("PIBPixelNum", fEventAction->GetPIBPixelNum());
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
