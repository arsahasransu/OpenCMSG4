#include "EventAction.hh"
#include "SteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"

#include <vector>
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction) :
	G4UserSteppingAction(),
	fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step * theStep)
{

	// Access track information and physical volume
	G4Track * theTrack = theStep->GetTrack();
	G4ThreeVector pos = theTrack->GetPosition();
	G4VPhysicalVolume* prPhVol = theTrack->GetVolume();
	G4double prTrackID = theTrack->GetTrackID();

	// Secondaries related to the track
	G4int num_secondary = theStep->GetNumberOfSecondariesInCurrentStep();
	const std::vector<const G4Track*> *secondaries_track;
	G4StepPoint* preStep;

	// Condition to select converted photons
	if(	num_secondary>=1 && 					// more than one daughter
		(!(prPhVol->GetName()!="ECalCell") || 	// outside the ECal vol
			!(prPhVol->GetName()!="CellEcalEC_r") || 
			!(prPhVol->GetName()!="CellEcalEC_l")) && 
		(prTrackID==1)	)						// primary track
	{
		// Individual secondary track
		secondaries_track = theStep->GetSecondaryInCurrentStep();
		preStep = theStep->GetPreStepPoint();

		G4cout<<pos/mm<<" ||";

		fEventAction->SetConvertedVertex(pos/mm);

		for(G4int i=0; i<num_secondary; i++)
		{
			// Print the daughter tracks
			G4cout<<i<<"\t"<<preStep->GetTotalEnergy()<<"\t";
			G4cout
				<<secondaries_track->at(i)->GetParticleDefinition()->GetParticleName()
				<<"\t"<<secondaries_track->at(i)->GetTotalEnergy()/MeV
				<<" (MeV)\t"<<secondaries_track->at(i)->GetVolume()->GetName()
				<<"\t"<<prPhVol->GetName()<<"|||";
		}
		fEventAction->set_pair_flag();			// increment the converted flag
		G4cout<<G4endl;
	}

	// Fill energy deposition to ntuples
	auto touchable = theStep->GetPreStepPoint()->GetTouchable();
	auto physical = touchable->GetVolume();
	auto copyNo = physical->GetCopyNo();
	auto edep = theStep->GetTotalEnergyDeposit();

	if(physical->GetName()=="ECalCell" && edep>0.)
	{
		fEventAction->AddEneDep(copyNo, edep, "ECalCell");
	}
	if(physical->GetName()=="CellEcalEC_r" && edep>0.)
	{
		fEventAction->AddEneDep(copyNo, edep, "CellEcalEC_r");
	}
	if(physical->GetName()=="CellEcalEC_l" && edep>0.)
	{
		fEventAction->AddEneDep(copyNo, edep, "CellEcalEC_l");
	}

	if((physical->GetName()=="PixelInnerBarrel" ||
		 physical->GetName()=="PixelInnerDiscs" ||
	         physical->GetName()=="TrackerStripInnerBarrel" ||
	         physical->GetName()=="TrackerStripInnerDisks" ||
	         physical->GetName()== "TrackerStripOuterBarrel" ||
	         physical->GetName()=="TrackerEndCap")
	   && (edep>0)
	   )
	{
	  fEventAction->AddTrackPos(pos);
	  //	  std::cout<<theTrack->GetParticleDefinition()->GetParticleName()<<"\t"<<pos<<"\t"<<edep<<"\t"<<theTrack->GetVolume()->GetName()<<std::endl;
	}

	if(physical->GetName()=="PixelInnerBarrel" && edep>0)
	{
	  double PIBPixelZ = 2*PIB_halfz*mm/PIB_nZPixel;
	  int ctrZ = floor((pos.getZ()/mm+PIB_halfz*mm/mm)/(PIBPixelZ/mm));
	  double PIBPixelPhi = 2*M_PI/PIB_nPhiPixel;
	  double phiHit = pos.getPhi();
	  phiHit = phiHit>=0?phiHit:2*M_PI+phiHit; // To put phiHit between 0 and 2*pi
	  int ctrPhi = floor(phiHit/PIBPixelPhi);
	  int PIBnum = (ctrZ+1)*PIB_nPhiPixel+ctrPhi;
	  fEventAction->PIBPixelArray[PIBnum] = true;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
