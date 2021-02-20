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
  outRootName("OpenCMSG4_root.root"){
  
  // Define Commands for this class
  DefineCommands();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*){ 
  
  outRootFile = TFile::Open(outRootName,"RECREATE");
  
  // Book histogram for ROOT Tree
  auto tree = new TTree("Events", "Events");
  fEventAction->EventTree(tree);
  
  // Remember the start time to store the duration of a run.
  start = time(NULL);
  myfile.open("time_keeper.txt",std::ios::app);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*){
  
  // Append the run time to a file
  end = time(NULL);
  std::cout<<"The time of the run was "<<difftime(end,start)
	   <<" seconds."<<std::endl;
  myfile<<"The time of the run was "<<difftime(end,start)
	<<" seconds."<<std::endl;
  myfile<<"-------------------------------------------------------------"
	<<std::endl;
  myfile.close();
  
  outRootFile->Write();
  outRootFile->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::DefineCommands() {
  
  runMessenger = new G4GenericMessenger(this,
					"/root/",
					"Analysis Settings");
  
  // Control for the output file name
  auto& outRootNameCmd = runMessenger->DeclareProperty("setFileName", outRootName, "Name of the output ROOT file");
  outRootNameCmd.SetParameterName("setFileName", true);
  outRootNameCmd.SetDefaultValue("OpenCMSG4_root.root");
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
