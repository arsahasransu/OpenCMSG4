#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "HepMCAsciiReaderMessenger.hh"
#include "HepMCAsciiReader.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCAsciiReaderMessenger::HepMCAsciiReaderMessenger
                             (HepMCAsciiReader* agen)
  : gen(agen)
{
  dir= new G4UIdirectory("/hepmcAscii/");
  dir-> SetGuidance("Reading HepMC event from an Ascii file");

  verbose=
    new G4UIcmdWithAnInteger("/hepmcAscii/verbose", this);
  verbose-> SetGuidance("Set verbose level");
  verbose-> SetParameterName("verboseLevel", false, false);
  verbose-> SetRange("verboseLevel>=0 && verboseLevel<=1");

  open= new G4UIcmdWithAString("/hepmcAscii/open", this);
  open-> SetGuidance("(re)open data file (HepMC Ascii format)");
  open-> SetParameterName("input ascii file", true, true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCAsciiReaderMessenger::~HepMCAsciiReaderMessenger()
{
  delete verbose;
  delete open;

  delete dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCAsciiReaderMessenger::SetNewValue(G4UIcommand* command,
                                              G4String newValues)
{
  if (command==verbose) {
    int level= verbose-> GetNewIntValue(newValues);
    gen-> SetVerboseLevel(level);
  } else if (command==open) {
    gen-> SetFileName(newValues);
    G4cout << "HepMC Ascii inputfile: "
           << gen-> GetFileName() << G4endl;
    gen-> Initialize();
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String HepMCAsciiReaderMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;

  if (command == verbose) {
    cv= verbose-> ConvertToString(gen-> GetVerboseLevel());
  } else  if (command == open) {
    cv= gen-> GetFileName();
  }
  return cv;
}
