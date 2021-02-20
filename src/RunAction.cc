#include "RunAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"
#include "Constants.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include "TFile.h"
#include "TTree.h"

#include <ctime>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction* eventAction):
  G4UserRunAction(),
  runMessenger(nullptr),
  fEventAction(eventAction),
  outRootName("OpenCMSG4_root.root")
{ 

  // Define Commands for this class
  DefineCommands();

  // Book histograms, ntuple
  //
  

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
