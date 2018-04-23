#include "ECalPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalPrimaryGeneratorAction::ECalPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),     
  fParticleGun(nullptr), fMessenger(nullptr), 
  fPositron(nullptr), fMuon(nullptr), fPion(nullptr), 
  fKaon(nullptr), fProton(nullptr),
  fMomentum(1000.*MeV),
  fSigmaMomentum(0.*MeV),
  fSigmaAngle(90.*deg),
  fRandomizePrimary(true),
	fSetGunSide(false) 		// default value is fire from center
{
  G4int nofParticles = 1;
  fParticleGun  = new G4ParticleGun(nofParticles);
  
  auto particleTable = G4ParticleTable::GetParticleTable();
  fPositron = particleTable->FindParticle("e+");
  fMuon = particleTable->FindParticle("mu+");
  fPion = particleTable->FindParticle("pi+");
  fKaon = particleTable->FindParticle("kaon+");
  fProton = particleTable->FindParticle("proton");
  
  // default particle kinematics
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*m,0.*m,0.*m));
  fParticleGun->SetParticleDefinition(fPositron);
  
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalPrimaryGeneratorAction::~ECalPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{

	double rpos=0.*m, thetapos=0.*deg, zpos=0.*m;
	if(fSetGunSide)
	{
		rpos = (1290+90*G4UniformRand())*mm;
		thetapos = 360*G4UniformRand()*deg;
		zpos = 2680*2*(G4UniformRand()-0.5)*mm;
		std::cout<<rpos/mm<<"\t"<<thetapos/deg<<"\t"<<zpos/mm<<std::endl;
	}
	else
	{
		rpos = 0.*m;
		thetapos = 0.*deg;
		zpos = 0.*m;
	}
  fParticleGun->SetParticlePosition(G4ThreeVector(rpos*std::cos(thetapos),rpos*std::sin(thetapos),zpos));

  G4ParticleDefinition* particle;  
  if (fRandomizePrimary) {
    G4int i = (int)(5.*G4UniformRand());
    switch(i) {
      case 0:
          particle = fPositron;
          break;
      case 1:
          particle = fMuon;
          break;
      case 2:
          particle = fPion;
          break;
      case 3:
          particle = fKaon;
          break;
      default:
          particle = fProton;
          break;
    }
    fParticleGun->SetParticleDefinition(particle);
  }
  else {
    particle = fParticleGun->GetParticleDefinition();
  }
  
  auto pp = fMomentum + (G4UniformRand()-0.5)*fSigmaMomentum;
  auto mass = particle->GetPDGMass();
  auto ekin = std::sqrt(pp*pp+mass*mass)-mass;
  fParticleGun->SetParticleEnergy(ekin);
  
  auto eta = 1.479+(G4UniformRand()-0.5)*2*0.05;
  auto angle = 2*std::atan(std::exp(-eta));
//  auto angle = 90*deg+(G4UniformRand()-0.5)*2*84*deg;
  auto phiangle = G4UniformRand()*360*deg;
	if(fSetGunSide)
	{
	  fParticleGun->SetParticleMomentumDirection(
  	                G4ThreeVector(0,0,1));
	}
	else
	{
	  fParticleGun->SetParticleMomentumDirection(
  	                G4ThreeVector(std::sin(angle)*std::cos(phiangle),std::sin(angle)*std::sin(phiangle),std::cos(angle)));
  }

  fParticleGun->GeneratePrimaryVertex(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalPrimaryGeneratorAction::DefineCommands()
{
  // Define /ECal/generator command directory using generic messenger class
  fMessenger 
    = new G4GenericMessenger(this, 
                             "/ECal/generator/", 
                             "Primary generator control");
            
  // momentum command
  auto& momentumCmd
    = fMessenger->DeclarePropertyWithUnit("momentum", "GeV", fMomentum, 
        "Mean momentum of primaries.");
  momentumCmd.SetParameterName("p", true);
  momentumCmd.SetRange("p>=0.");                                
  momentumCmd.SetDefaultValue("1.");
  // ok
  //momentumCmd.SetParameterName("p", true);
  //momentumCmd.SetRange("p>=0.");                                
  
  // sigmaMomentum command
  auto& sigmaMomentumCmd
    = fMessenger->DeclarePropertyWithUnit("sigmaMomentum",
        "MeV", fSigmaMomentum, "Sigma momentum of primaries.");
  sigmaMomentumCmd.SetParameterName("sp", true);
  sigmaMomentumCmd.SetRange("sp>=0.");                                
  sigmaMomentumCmd.SetDefaultValue("50.");

  // sigmaAngle command
  auto& sigmaAngleCmd
    = fMessenger->DeclarePropertyWithUnit("sigmaAngle", "deg", fSigmaAngle, 
        "Sigma angle divergence of primaries.");
  sigmaAngleCmd.SetParameterName("t", true);
  sigmaAngleCmd.SetRange("t>=0.");                                
  sigmaAngleCmd.SetDefaultValue("2.");

  // randomizePrimary command
  auto& randomCmd
    = fMessenger->DeclareProperty("randomizePrimary", fRandomizePrimary);
  G4String guidance
    = "Boolean flag for randomizing primary particle types.\n";   
  guidance
    += "In case this flag is false, you can select the primary particle\n";
  guidance += "  with /gun/particle command.";                               
  randomCmd.SetGuidance(guidance);
  randomCmd.SetParameterName("flg", true);
  randomCmd.SetDefaultValue("true");

  // set the gun position as sidewise or from center command
  auto& GunPos
    = fMessenger->DeclareProperty("SetGunSide", fSetGunSide);
  guidance
    = "Set the gun position to fire from center or from inside the crystal sidewise\n";   
  guidance
    += "false -> Default fire from center \t true-> fire sidewise from inside the crystal\n";
  GunPos.SetGuidance(guidance);
  GunPos.SetParameterName("bool_flg", true);
  GunPos.SetDefaultValue("false");
}

//..oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
