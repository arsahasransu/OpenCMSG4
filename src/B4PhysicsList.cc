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
/// \file B4PhysicsList.cc
/// \brief Implementation of the B4PhysicsList class

#include "B4PhysicsList.hh"

#include <iomanip>
#include "globals.hh"
#include "G4ios.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"
//#include "G4DataQuestionaire.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "B4CustomPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4PhysicsList::B4PhysicsList() : G4VModularPhysicsList() {
  
  defaultCutValue = 0.7*CLHEP::mm;
  SetVerboseLevel(1);
  
    //G4DataQuestionaire it(photon);
  G4cout << "<<< Geant4 Physics List simulation engine: QGSP_BERT 2.0"<<G4endl;
  G4cout <<G4endl;
  
 // EM Physics
  RegisterPhysics( new G4EmStandardPhysics());

  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics() );

  // Decays 
  RegisterPhysics( new G4DecayPhysics() );

   // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics() );

   // Hadron Physics
  RegisterPhysics(  new G4HadronPhysicsQGSP_BERT());

  // Stopping Physics
  RegisterPhysics( new G4StoppingPhysics() );

  // Ion Physics
  RegisterPhysics( new G4IonPhysics());
  
  // Neutron tracking cut
  RegisterPhysics( new G4NeutronTrackingCut());
  
  //Custom Physics
  RegisterPhysics( new B4CustomPhysics());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4PhysicsList::~B4PhysicsList()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void B4PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCutsWithDefault();
}  
