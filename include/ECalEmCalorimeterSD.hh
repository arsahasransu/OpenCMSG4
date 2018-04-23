#ifndef ECalEmCalorimeterSD_h
#define ECalEmCalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"

#include "ECalEmCalorimeterHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// EM calorimeter sensitive detector

class ECalEmCalorimeterSD : public G4VSensitiveDetector
{   
  public:
    ECalEmCalorimeterSD(G4String name);
    virtual ~ECalEmCalorimeterSD();
    
    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
    
  private:
    ECalEmCalorimeterHitsCollection* fHitsCollection;
    G4int fHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
