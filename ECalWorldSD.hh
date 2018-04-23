#ifndef ECalWorldSD_h
#define ECalWorldSD_h 1

#include "G4VSensitiveDetector.hh"

#include "ECalWorldHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// EM calorimeter sensitive detector

class ECalWorldSD : public G4VSensitiveDetector
{   
  public:
    ECalWorldSD(G4String name);
    virtual ~ECalWorldSD();
    
    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
    
  private:
    ECalWorldHitsCollection* fHitsCollection;
    G4int fHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
