#include "RunAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"
#include "Constants.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "TFile.h"
#include "TTree.h"

#include <ctime>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction* eventAction):
	G4UserRunAction(),
	fEventAction(eventAction)
{

  // Create a ROOT Tree to write the output
  if ( fEventAction ) {
    outRootFile = TFile::Open("OpenCMSG4_root.root","RECREATE");
    std::cout<<"Root file created"<<std::endl;
  }

  
  // Book histograms, ntuple
  //
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*){ 

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
  
  std::cout<<"Root file closing"<<std::endl;
  outRootFile->Write();
  outRootFile->Close();
  std::cout<<"Root file closed!!!"<<std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
