#ifndef ECalECSD_left_h
#define ECalECSD_left_h 1

#include "G4VSensitiveDetector.hh"

#include "ECalECHit_left.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// EM calorimeter sensitive detector

class ECalECSD_left : public G4VSensitiveDetector
{   
  public:
    ECalECSD_left(G4String name);
    virtual ~ECalECSD_left();
    
    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
    
  private:
    ECalECHit_leftsCollection* fHitsCollection;
    G4int fHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
