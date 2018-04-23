#ifndef ECaltrackerSD_h
#define ECaltrackerSD_h 1

#include "G4VSensitiveDetector.hh"

#include "ECaltrackerHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// Drift chamber sensitive detector

class ECaltrackerSD : public G4VSensitiveDetector
{
  public:
    ECaltrackerSD(G4String name);
    virtual ~ECaltrackerSD();
    
    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
    
  private:
    ECaltrackerHitsCollection* fHitsCollection;
    G4int fHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
