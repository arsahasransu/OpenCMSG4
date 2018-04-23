#ifndef ECalDetectorConstruction_h
#define ECalDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"

#include <vector>

class ECalMagneticField;

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;

/// Detector construction

class ECalDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ECalDetectorConstruction();
    virtual ~ECalDetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    
    void ConstructMaterials();
    
  private:
    void DefineCommands();

    G4GenericMessenger* fMessenger;
    
    static G4ThreadLocal ECalMagneticField* fMagneticField;
    static G4ThreadLocal G4FieldManager* fFieldMgr;
    
    G4LogicalVolume* logicWorld;
    G4LogicalVolume* ecalLogical;
    G4LogicalVolume* ecalECLogical;
    G4LogicalVolume* ecalECLogical_left;
    G4LogicalVolume* cellEcalECLogical;
    G4LogicalVolume* cellEcalECLogical_left;
    G4LogicalVolume* fCellLogical;
    G4LogicalVolume* fMagneticLogical;
    G4LogicalVolume* trackerLogical;
    G4LogicalVolume* singleTrackerLogical;
    
    std::vector<G4VisAttributes*> fVisAttributes;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
