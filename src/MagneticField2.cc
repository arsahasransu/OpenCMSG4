#include "MagneticField2.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Constants.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField2::MagneticField2()
: G4MagneticField(), 
  fMessenger(nullptr), fBy(kBz)
{
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField2::~MagneticField2()
{ 
  delete fMessenger; 
}

void MagneticField2::GetFieldValue(const G4double point[4],double *bField) const
{
  // To get a linearly dropping field strength with radius
  double radius = std::sqrt(point[0]*point[0]+point[1]*point[1]);
  double constantfactor = 0.5*fBy*solenoidHalfZ*solenoidHalfZ;
  double r1 = std::sqrt(radius*radius+(point[3]-solenoidHalfZ)*(point[3]-solenoidHalfZ));
  double r2 = std::sqrt(radius*radius+(point[3]+solenoidHalfZ)*(point[3]+solenoidHalfZ));
  double theta1 = std::atan2(radius,point[3]+solenoidHalfZ);
  double theta2 = std::atan2(radius,point[3]-solenoidHalfZ);
  double phi = std::atan2(point[1],point[0]);
  double B_r = constantfactor*((std::sin(theta1)/(r1*r1))-(std::sin(theta2)/(r2*r2)));
  double B_z = constantfactor*((std::cos(theta1)/(r1*r1))-(std::cos(theta2)/(r2*r2)));
  bField[0] = B_r*std::cos(phi);
  bField[1] = B_r*std::sin(phi);
  bField[2] = B_z;
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
  valueCmd.SetDefaultValue("4.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
