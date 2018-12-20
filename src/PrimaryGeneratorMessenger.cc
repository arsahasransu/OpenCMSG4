#include "G4UIcommand.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorMessenger::PrimaryGeneratorMessenger
                                (PrimaryGeneratorAction* genaction)
  : primaryAction(genaction)
{
//  mydetdir = new G4UIdirectory("//");
//  mydetdir-> SetGuidance(" detector control commands.");

  dir= new G4UIdirectory("/generator/");
  dir-> SetGuidance("Control commands for primary generator");

  select= new G4UIcmdWithAString("/generator", this);
  select-> SetGuidance("select generator type");
  select-> SetParameterName("generator_type", false, false);
  select-> SetCandidates("particle hepmc");
  select-> SetDefaultValue("particle");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete select;
  delete mydetdir;
  delete dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                              G4String newValues)
{
  if ( command == select ) {
    primaryAction-> SetGenerator(newValues);
    G4cout << "current generator type: "
            << primaryAction-> GetGeneratorName() << G4endl;
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String PrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv, st;
  if ( command == select ) {
    cv = primaryAction-> GetGeneratorName();
  }

 return cv;
}
