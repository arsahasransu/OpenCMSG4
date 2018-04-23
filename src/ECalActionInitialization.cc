#include "ECalActionInitialization.hh"
#include "ECalPrimaryGeneratorAction.hh"
#include "ECalRunAction.hh"
#include "ECalEventAction.hh"
#include "ECalSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalActionInitialization::ECalActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalActionInitialization::~ECalActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalActionInitialization::BuildForMaster() const
{
  ECalEventAction* eventAction = new ECalEventAction;
  SetUserAction(new ECalRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalActionInitialization::Build() const
{

  SetUserAction(new ECalPrimaryGeneratorAction);

  auto eventAction = new ECalEventAction;
  SetUserAction(eventAction);

  G4UserSteppingAction* stepping_action = new ECalSteppingAction(eventAction);
  SetUserAction(stepping_action);

  SetUserAction(new ECalRunAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
