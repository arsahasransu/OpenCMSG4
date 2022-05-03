#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Constants.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::MagneticField()
: G4MagneticField(), 
  fMessenger(nullptr), fBy(kBz)
{
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::~MagneticField()
{ 
  delete fMessenger; 
}

void MagneticField::GetFieldValue(const G4double [4],double *bField) const
{
  bField[0] = 0.;
  bField[1] = 0.;
  bField[2] = fBy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MagneticField::DefineCommands()
{
  // Define /field command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, 
                                      "/field/", 
                                      "Field control");

  // fieldValue command 
  auto& valueCmd
    = fMessenger->DeclareMethodWithUnit("value","tesla",
                                &MagneticField::SetField, 
                                "Set field strength.");
  valueCmd.SetParameterName("field", true);
  valueCmd.SetDefaultValue("4.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
