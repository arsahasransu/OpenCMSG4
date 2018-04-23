#ifndef ECalCellParameterisation_H
#define ECalCellParameterisation_H 1

#include "ECalConstants.hh"
#include "G4Sphere.hh"

#include "globals.hh"
#include "G4VPVParameterisation.hh"

#include <array>
#include <cmath>


class G4Sphere;

// Dummy declarations to get rid of warnings ...

class G4Box;
class G4Trd;
class G4Trap;
class G4Cons;
class G4Orb;
class G4Ellipsoid;
class G4Torus;
class G4Para;
class G4Hype;
class G4Tubs;
class G4Polycone;
class G4Polyhedra;

class G4VPhysicalVolume;

/// EM Calorimeter cell parameterisation

class ECalCellParameterisation : public G4VPVParameterisation
{
  public:
    ECalCellParameterisation();
    virtual ~ECalCellParameterisation();
    
    virtual void ComputeTransformation(
                   const G4int copyNo,G4VPhysicalVolume *physVol) const;

    virtual void ComputeDimensions (G4Sphere& trackerChamber, const G4int copyNo,
                            const G4VPhysicalVolume* physVol) const;

  private:  // Dummy declarations to get rid of warnings ...

    void ComputeDimensions (G4Tubs&,const G4int,
                            const G4VPhysicalVolume*) const {};
    void ComputeDimensions (G4Box&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trd&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trap&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Cons&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Orb&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Ellipsoid&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Torus&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Para&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Hype&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polycone&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polyhedra&,const G4int,
                            const G4VPhysicalVolume*) const {}
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
