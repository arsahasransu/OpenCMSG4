#ifndef ECalRunAction_h
#define ECalRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include <ctime>
#include "fstream"

class ECalEventAction;

class G4Run;

/// Run action class

class ECalRunAction : public G4UserRunAction
{
  public:
    ECalRunAction(ECalEventAction* eventAction);
    virtual ~ECalRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

		std::vector<std::vector<G4double>> fEventEdep;

  private:
    ECalEventAction* fEventAction;
		time_t start, end;
		std::fstream myfile;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
