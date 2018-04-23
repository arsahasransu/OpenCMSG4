#include "ECalCellParameterisation_tracker.hh"
#include "ECalConstants.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

#define PI 3.1415926

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalCellParameterisation_tracker::ECalCellParameterisation_tracker()
: G4VPVParameterisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalCellParameterisation_tracker::~ECalCellParameterisation_tracker()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalCellParameterisation_tracker::ComputeTransformation(
       const G4int, G4VPhysicalVolume*) const
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalCellParameterisation_tracker::ComputeDimensions (G4Tubs& trackerChamber, const G4int copyNo,
                            const G4VPhysicalVolume*) const
{
	// Note: copyNo will start with zero!
  G4double rMin = tr_brl_r[copyNo]*mm;
	G4double rMax = tr_brl_r[copyNo]*mm+tr_brl_dr[copyNo]*um;
  
	trackerChamber.SetInnerRadius(rMin);
  trackerChamber.SetOuterRadius(rMax);
  trackerChamber.SetStartPhiAngle(0*deg);
  trackerChamber.SetDeltaPhiAngle(360.*deg);
	trackerChamber.SetZHalfLength(tr_brl_halfz[copyNo]*mm);

//	if(copyNo%3000 == 0)
//	std::cout<<copyNo<<"\t"<<etaNum<<"\t"<<phiNum<<"\t"<<"Visited"<<std::endl;

}

