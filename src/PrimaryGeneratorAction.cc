#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "ParticleGun.hh"
#include "HepMCAsciiReader.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  // default generator is particle gun.
  currentGenerator = particleGun= new ParticleGun();
  currentGeneratorName = "particleGun";
  hepmcAscii = new HepMCAsciiReader();

  gentypeMap["particle"] = particleGun;
  gentypeMap["hepmc"] = hepmcAscii;

  messenger= new PrimaryGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  if(currentGenerator)
    currentGenerator-> GeneratePrimaryVertex(anEvent);
  else
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
		"PrimaryGeneratorAction001", FatalException,
		"generator is not instanciated." );
}
