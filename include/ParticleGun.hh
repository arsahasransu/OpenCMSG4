#ifndef ParticleGun_h
#define ParticleGun_h 1

#include "G4VPrimaryGenerator.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;

/// Primary generator
///
/// A single particle is generated.
/// User can select 
/// - the initial momentum and angle
/// - the momentum and angle spreads
/// - random selection of a particle type from proton, kaon+, pi+, muon+, e+ 


class ParticleGun : public G4VPrimaryGenerator
{
  public:
    ParticleGun();
    virtual ~ParticleGun();
    
    virtual void GeneratePrimaryVertex(G4Event*);
    
    void SetMomentum(G4double val) { fMomentum = val; }
    G4double GetMomentum() const { return fMomentum; }

    void SetSigmaMomentum(G4double val) { fSigmaMomentum = val; }
    G4double GetSigmaMomentum() const { return fSigmaMomentum; }
    
  private:
    void DefineCommands();

    G4GenericMessenger* fMessenger;
    G4String fType;
    G4double fMomentum;
    G4double fSigmaMomentum;
    G4double fEta;
    G4double fSigmaEta;
    G4double fPhi;
    G4double fSigmaPhi;
    G4bool fSetGunSide;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
