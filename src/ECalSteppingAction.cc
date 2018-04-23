#include <vector>
#include <cmath>

#include "ECalEventAction.hh"
#include "ECalSteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalSteppingAction::ECalSteppingAction(ECalEventAction* eventAction)
: G4UserSteppingAction(),
fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalSteppingAction::~ECalSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalSteppingAction::UserSteppingAction(const G4Step * theStep)
{

//  G4cout<<theStep->GetNumberOfSecondariesInCurrentStep();

     G4Track * theTrack = theStep->GetTrack();
	G4ThreeVector pos = theTrack->GetPosition();
//	G4double r = sqrt(pos.x()*pos.x()+pos.y()*pos.y());
	G4VPhysicalVolume* prPhVol = theTrack->GetVolume();
	G4double prTrackID = theTrack->GetTrackID();
//	const G4ParticleDefinition* prParticle = theTrack->GetParticleDefinition();

	G4int num_secondary = theStep->GetNumberOfSecondariesInCurrentStep();
	const std::vector<const G4Track*> *secondaries_track;
	G4StepPoint* preStep;

	if(num_secondary>=1 && prPhVol->GetName()!="cellPhysical" && (prTrackID==1 || prTrackID==2 || prTrackID==3))
	{
		secondaries_track = theStep->GetSecondaryInCurrentStep();
		preStep = theStep->GetPreStepPoint();
		G4cout<<pos/mm<<" ||";
		fEventAction->SetConvertedVertex(pos/mm);

		for(G4int i=0; i<num_secondary; i++)
		{
			G4cout<<i<<"\t"<<preStep->GetTotalEnergy()<<"\t";
			G4cout<<secondaries_track->at(i)->GetParticleDefinition()->GetParticleName()<<"\t"<<secondaries_track->at(i)->GetTotalEnergy()/MeV
																	<<" (MeV)\t"<<secondaries_track->at(i)->GetVolume()->GetName();
		}
		fEventAction->set_pair_flag();
		G4cout<<G4endl;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
