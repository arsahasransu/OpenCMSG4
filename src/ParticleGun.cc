#include "ParticleGun.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryVertex.hh"

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ParticleGun::ParticleGun() : 
	G4VPrimaryGenerator(),     
	fMessenger(nullptr), 
	fType("e+"),
	fMomentum(1000.*MeV), fSigmaMomentum(0.*MeV),
	fEta(0), fSigmaEta(1.479),
	fPhi(0*deg), fSigmaPhi(90*deg),
	fSetGunSide(false) 		// default value is fire from center
{  
	// define commands for this class
	DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ParticleGun::~ParticleGun()
{
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ParticleGun::GeneratePrimaryVertex(G4Event* event)
{

	double rpos=0.*m, thetapos=0.*deg, zpos=0.*m;
	if(fSetGunSide)								// position gun in the crystals
	{
		rpos = (1290+90*G4UniformRand())*mm;
		thetapos = 360*G4UniformRand()*deg;
		zpos = 2680*2*(G4UniformRand()-0.5)*mm;
		std::cout<<rpos/mm<<"\t"<<thetapos/deg<<"\t"<<zpos/mm<<std::endl;
	}
	else										// position gun at center
	{
		rpos = 0.*m;
		thetapos = 0.*deg;
		zpos = 0.*m;
	}

	G4PrimaryVertex* vtx = new G4PrimaryVertex(rpos*std::cos(thetapos),
												rpos*std::sin(thetapos),zpos,0);
	G4PrimaryParticle* ptx = new G4PrimaryParticle();

	G4ParticleDefinition* particle;  

	auto particleTable = G4ParticleTable::GetParticleTable();
	particle = particleTable->FindParticle(fType);
	ptx->SetG4code(particle);
  
	auto pp = fMomentum + (G4UniformRand()-0.5)*2*fSigmaMomentum;
	auto mass = particle->GetPDGMass();
	auto ekin = std::sqrt(pp*pp+mass*mass)-mass;
	ptx->SetKineticEnergy(ekin);

  auto eta = fEta+(G4UniformRand()-0.5)*2*fSigmaEta;
  auto angle = 2*std::atan(std::exp(-eta));
  auto phiangle = fPhi+G4UniformRand()*fSigmaPhi;
	if(fSetGunSide)
	{
		ptx->SetMomentumDirection(G4ThreeVector(0,0,1));
//	  fParticleGun->SetParticleMomentumDirection(
//  	                G4ThreeVector(0,0,1));
	}
	else
	{
		ptx->SetMomentumDirection(G4ThreeVector(std::sin(angle)*std::cos(phiangle),std::sin(angle)*std::sin(phiangle),std::cos(angle)));
//	  fParticleGun->SetParticleMomentumDirection(
//  	                G4ThreeVector(std::sin(angle)*std::cos(phiangle),std::sin(angle)*std::sin(phiangle),std::cos(angle)));
  }

	vtx->SetPrimary(ptx);
	event->AddPrimaryVertex(vtx);
//  fParticleGun->GeneratePrimaryVertex(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ParticleGun::DefineCommands()
{
  // Define //generator command directory using generic messenger class
  fMessenger 
    = new G4GenericMessenger(this, 
                             "/generator/particle/", 
                             "Primary generator control");
            
  // set the particle type command
  auto& typeCmd
    = fMessenger->DeclareProperty("type", fType, 
        "Type of Particle to fire");
  typeCmd.SetParameterName("type", true);
  typeCmd.SetDefaultValue("e+");

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
  sigmaMomentumCmd.SetDefaultValue("0.");

  // eta command
  auto& etaCmd
    = fMessenger->DeclareProperty("eta", fEta, 
        "Pseudorapidity of primaries.");
  etaCmd.SetParameterName("eta", true);
  etaCmd.SetDefaultValue("0.");

  // sigmaEta command
  auto& sigmaEtaCmd
    = fMessenger->DeclareProperty("sigmaEta", fSigmaEta, 
        "Sigma angle divergence of pseudorapidity of primaries.");
  sigmaEtaCmd.SetParameterName("sEta", true);
  sigmaEtaCmd.SetRange("sEta>=0.");                                
  sigmaEtaCmd.SetDefaultValue("0.1");

  // phi command
  auto& phiCmd
    = fMessenger->DeclarePropertyWithUnit("phi", "deg", fPhi, 
        "Azimuth of primaries.");
  phiCmd.SetParameterName("phi", true);
  phiCmd.SetRange("phi>=0. && phi<360");                                
  phiCmd.SetDefaultValue("0.");

  // sigmaPhi command
  auto& sigmaPhiCmd
    = fMessenger->DeclarePropertyWithUnit("sigmaPhi", "deg", fSigmaPhi, 
        "Sigma angle divergence of azimuth of primaries.");
  sigmaPhiCmd.SetParameterName("sPhi", true);
  sigmaPhiCmd.SetRange("sPhi>=0. && sPhi<360");                                
  sigmaPhiCmd.SetDefaultValue("90");

  // set the gun position as sidewise or from center command
  auto& GunPos
    = fMessenger->DeclareProperty("SetGunSide", fSetGunSide);
  G4String guidance
    = "Set the gun position to fire from center or from inside the crystal sidewise\n";   
  guidance
    += "false -> Default fire from center \t true-> fire sidewise from inside the crystal\n";
  GunPos.SetGuidance(guidance);
  GunPos.SetParameterName("bool_flg", true);
  GunPos.SetDefaultValue("false");
}

//..oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
