//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "B4CustomPhysics.hh"
#include "CustomParticleFactory.hh"
#include "G4DecayTable.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4CustomPhysics::B4CustomPhysics(const G4String& name):G4VPhysicsConstructor(name){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4CustomPhysics::~B4CustomPhysics(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4CustomPhysics::ConstructParticle(){
    
    ConstructExotics();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4CustomPhysics::ConstructProcess()
{
    addCustomPhysics();
}

void B4CustomPhysics::ConstructExotics()
{
  CustomParticleFactory::loadCustomParticles();     
}

#include "G4HadronElasticProcess.hh"
#include "FullModelHadronicProcess.hh"
//#include "ToyModelHadronicProcess.hh"

#include "G4PionMinusInelasticProcess.hh"
//#include "G4LEPionMinusInelastic.hh"
//#include "G4HEPionMinusInelastic.hh"

#include "G4PionPlusInelasticProcess.hh"
//#include "G4LEPionPlusInelastic.hh"
//#include "G4HEPionPlusInelastic.hh"
#include "G4DecayTable.hh"
#include "G4hhIonisation.hh"
#include "G4hMultipleScattering.hh"

void B4CustomPhysics::addCustomPhysics()
{
  auto theParticleIterator = GetParticleIterator();
  G4cout << " CustomPhysics: adding CustomPhysics processes  " <<G4endl;
  theParticleIterator->reset();

  while((*theParticleIterator)())
    {	
      int i = 0;
      G4ParticleDefinition* particle = theParticleIterator->value();
      //      G4cout<<particle->GetParticleName()<<G4endl;
      CustomParticle* cp = dynamic_cast<CustomParticle*>(particle);
      if(CustomParticleFactory::isCustomParticle(particle))
	{
	  G4cout << particle->GetParticleName()<<", "<<particle->GetPDGEncoding() 
		 << " is Custom. Mass is "
		 <<particle->GetPDGMass()/GeV
		 <<" GeV."<<G4endl;

	  G4DecayTable* table = particle->GetDecayTable();
	  G4cout<<"table: "<<table<<G4endl;
	  if (table!=0) table->DumpInfo();

	  if(cp->GetCloud()!=0)
	    {
	      G4cout<<"Cloud mass is "
		    <<cp->GetCloud()->GetPDGMass()/GeV
		    <<" GeV. Spectator mass is "
		    <<static_cast<CustomParticle*>(particle)->GetSpectator()->GetPDGMass()/GeV
		    <<" GeV." 
		    << G4endl;
	    }
	  G4ProcessManager* pmanager = particle->GetProcessManager();
	  if(pmanager)
	    { 
              if(cp!=0) {
		pmanager->AddDiscreteProcess(new FullModelHadronicProcess()); // Full parametrised model
		//		pmanager->AddDiscreteProcess(new FullModelHadronicProcess()); // Toy-model
	      }
              if(particle->GetPDGCharge()/eplus != 0)
		{ 
		  pmanager->AddProcess(new G4hMultipleScattering(),-1, 1,i+1); //todo:-AddProcess function arguements???
		  pmanager->AddProcess(new G4hhIonisation(),       -1, 2,i+2);
		}
	      pmanager->DumpInfo();
            }
	  else
	    G4cout << "   No pmanager" << G4endl;
	}


    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Decay.hh"

void B4CustomPhysics::ConstructGeneral() //todo:-Decay added for custom particles???
{
  auto theParticleIterator = GetParticleIterator();
  // Add Decay Process
   G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) { 
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}
