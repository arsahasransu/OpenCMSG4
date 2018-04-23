#ifndef ECalECSD_h
#define ECalECSD_h 1

#include "G4VSensitiveDetector.hh"

#include "ECalECHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// EM calorimeter sensitive detector

class ECalECSD : public G4VSensitiveDetector
{   
  public:
    ECalECSD(G4String name);
    virtual ~ECalECSD();
    
    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
    
  private:
    ECalECHitsCollection* fHitsCollection;
    G4int fHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
