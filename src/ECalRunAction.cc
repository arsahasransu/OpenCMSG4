#include "ECalRunAction.hh"
#include "ECalEventAction.hh"
#include "ECalAnalysis.hh"
#include "ECalConstants.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalRunAction::ECalRunAction(ECalEventAction* eventAction)
 : G4UserRunAction(),
   fEventAction(eventAction)
{ 
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in ECalAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetNtupleMerging(true);
     // Note: merging ntuples is available only with Root output
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("ECal");

  // Book histograms, ntuple
  //
  

  // Creating ntuple
  //
  if ( fEventAction ) {
    analysisManager->CreateNtuple("Events", "Events");		// tuple ID = 0
    analysisManager                                   // column Id = 0
      ->CreateNtupleDColumn("CrysEdep", fEventAction->GetEmCalEdep()); 
    analysisManager                                   // column Id = 1
      ->CreateNtupleIColumn("CrysNum", fEventAction->GetEmCalCrysNum()); 
    analysisManager                                   // column Id = 2
      ->CreateNtupleDColumn("EventEdep"); 
    analysisManager                                   // column Id = 3
      ->CreateNtupleIColumn("HitNum"); 
    analysisManager                                   // column Id = 4
      ->CreateNtupleIColumn("ConvertedFlag"); 
    analysisManager                                   // column Id = 5
      ->CreateNtupleDColumn("ConvertedX", fEventAction->GetConvertedX()); 
    analysisManager                                   // column Id = 6
      ->CreateNtupleDColumn("ConvertedY", fEventAction->GetConvertedY()); 
    analysisManager                                   // column Id = 7
      ->CreateNtupleDColumn("ConvertedZ", fEventAction->GetConvertedZ()); 
    analysisManager                                   // column Id = 8
      ->CreateNtupleIColumn("cellNoEcalEC_right", fEventAction->GetCellNoEcalEC_r()); 
    analysisManager                                   // column Id = 9
      ->CreateNtupleDColumn("edepEcalEC_right", fEventAction->GetEdepEcalEC_r()); 
    analysisManager                                   // column Id = 10
      ->CreateNtupleIColumn("cellNoEcalEC_left", fEventAction->GetCellNoEcalEC_l()); 
    analysisManager                                   // column Id = 11
      ->CreateNtupleDColumn("edepEcalEC_left", fEventAction->GetEdepEcalEC_l()); 
    analysisManager->FinishNtuple();
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalRunAction::~ECalRunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 

	start = time(NULL);

	myfile.open("time_keeper.txt",std::ios::app);
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file 
  // The default file name is set in ECalRunAction::ECalRunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // save histograms & ntuple
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

	end = time(NULL);

	std::cout<<"The time of the run was "<<difftime(end,start)<<" seconds."<<std::endl;
	myfile<<"The time of the run was "<<difftime(end,start)<<" seconds."<<std::endl;
	myfile<<"-------------------------------------------------------------"<<std::endl;
	myfile.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
