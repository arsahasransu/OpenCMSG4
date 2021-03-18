#include "MagneticField2.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Constants.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField2::MagneticField2()
: G4MagneticField(), 
  fMessenger(nullptr), fBy(-kBz)
{
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField2::~MagneticField2()
{ 
  delete fMessenger; 
}

void MagneticField2::GetFieldValue(const G4double [4],double *bField) const
{
  bField[0] = 0.;
  bField[1] = 0.;
  bField[2] = fBy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MagneticField2::DefineCommands()
{
  // Define /field command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, 
                                      "/field/", 
                                      "Field control");

  // fieldValue command 
  auto& valueCmd
    = fMessenger->DeclareMethodWithUnit("value","tesla",
                                &MagneticField2::SetField, 
                                "Set field strength.");
  valueCmd.SetParameterName("field", true);
  valueCmd.SetDefaultValue("1.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
