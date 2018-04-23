#include "ECalMagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "ECalConstants.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalMagneticField::ECalMagneticField()
: G4MagneticField(), 
  fMessenger(nullptr), fBy(kBz)
{
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalMagneticField::~ECalMagneticField()
{ 
  delete fMessenger; 
}

void ECalMagneticField::GetFieldValue(const G4double [4],double *bField) const
{
  bField[0] = 0.;
  bField[1] = 0.;
  bField[2] = fBy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalMagneticField::DefineCommands()
{
  // Define /ECal/field command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, 
                                      "/ECal/field/", 
                                      "Field control");

  // fieldValue command 
  auto& valueCmd
    = fMessenger->DeclareMethodWithUnit("value","tesla",
                                &ECalMagneticField::SetField, 
                                "Set field strength.");
  valueCmd.SetParameterName("field", true);
  valueCmd.SetDefaultValue("1.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
