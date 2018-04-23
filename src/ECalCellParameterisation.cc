#include "ECalCellParameterisation.hh"
#include "ECalConstants.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

#define PI 3.1415926

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalCellParameterisation::ECalCellParameterisation()
: G4VPVParameterisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalCellParameterisation::~ECalCellParameterisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalCellParameterisation::ComputeTransformation(
       const G4int, G4VPhysicalVolume*) const
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalCellParameterisation::ComputeDimensions (G4Sphere& trackerChamber, const G4int copyNo,
                            const G4VPhysicalVolume*) const
{
	// Note: copyNo will start with zero!
	G4double eta_max = 1.479;
	G4double eta_step = eta_max/85;
	G4double y = 1290.0*mm;
	G4double dy = 220*mm;

	G4int etaNum = copyNo/kNofEmRows;
	G4int phiNum = copyNo%kNofEmRows;
	G4double eta_b = -eta_max+etaNum*eta_step;
	G4double eta_e = -eta_max+(etaNum+1)*eta_step;
	G4double theta_b = 2*atan(exp(-eta_b))*180/PI;
	G4double theta_e = 2*atan(exp(-eta_e))*180/PI;
	G4double theta_avg = 0.5*(theta_b+theta_e);
  G4double rMin = y/sin(theta_avg*PI/180);
	G4double rMax = rMin+dy;
  
	trackerChamber.SetInnerRadius(rMin);
  trackerChamber.SetOuterRadius(rMax);
  trackerChamber.SetStartPhiAngle(phiNum*deg);
  trackerChamber.SetDeltaPhiAngle(1.*deg);
  trackerChamber.SetStartThetaAngle(theta_e*deg);
  trackerChamber.SetDeltaThetaAngle((theta_b-theta_e)*deg);

//	if(copyNo%3000 == 0)
//	std::cout<<copyNo<<"\t"<<etaNum<<"\t"<<phiNum<<"\t"<<"Visited"<<std::endl;

}

