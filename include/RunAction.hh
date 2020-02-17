#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include <vector>
#include <ctime>
#include "fstream"

class EventAction;

class G4Run;

/// Run action class

class RunAction : public G4UserRunAction
{
  public:
    RunAction(EventAction* eventAction);
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

		std::vector<std::vector<G4double>> fEventEdep;
  		std::vector<std::vector<G4double>> fEventHCalEdep;

  private:
    EventAction* fEventAction;
		time_t start, end;
		std::fstream myfile;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
