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
#include "Constants.hh"

#include <vector>
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
  : G4UserSteppingAction(),
    fEventAction(eventAction) {
    }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step * theStep) {

  // Access track information and physical volume
  G4Track * theTrack = theStep->GetTrack();
  G4ThreeVector pos = theTrack->GetPosition();
  G4VPhysicalVolume* prPhVol = theTrack->GetVolume();
  G4double prTrackID = theTrack->GetTrackID();

  // Secondaries related to the track
  G4int num_secondary = theStep->GetNumberOfSecondariesInCurrentStep();
  //const std::vector<const G4Track*> *secondaries_track;
  //G4StepPoint* preStep;

  // Condition to select converted photons
  if(num_secondary>=1 && 					// more than one daughter
     (!(prPhVol->GetName()!="ECalCell") || 	// outside the ECal vol
      !(prPhVol->GetName()!="CellEcalEC_r") || 
      !(prPhVol->GetName()!="CellEcalEC_l")) && 
     (prTrackID==1)	)						// primary track
    {
      // Individual secondary track
      //secondaries_track = theStep->GetSecondaryInCurrentStep();
      //preStep = theStep->GetPreStepPoint();

      fEventAction->SetConvertedVertex(pos/mm);
      /*
	for(G4int i=0; i<num_secondary; i++)
	{
	// Print the daughter tracks
	G4cout<<i<<"\t"<<preStep->GetTotalEnergy()<<"\t";
	G4cout
	<<secondaries_track->at(i)->GetParticleDefinition()->GetParticleName()
	<<"\t"<<secondaries_track->at(i)->GetTotalEnergy()/MeV
	<<" (MeV)\t"<<secondaries_track->at(i)->GetVolume()->GetName()
	<<"\t"<<prPhVol->GetName()<<"|||";
	}*/
      fEventAction->set_pair_flag();			// increment the converted flag
    }

  // Fill energy deposition to ntuples
  auto touchable = theStep->GetPreStepPoint()->GetTouchable();
  auto physical = touchable->GetVolume();
  auto copyNo = physical->GetCopyNo();
  double edep = theStep->GetTotalEnergyDeposit();
  //if(edep>1 && physical->GetName()=="HCalAbsorberCell") std::cout<<physical->GetName()<<"\t"<<copyNo<<"\t"<<edep<<std::endl;
    
  if(physical->GetName()=="ECalCell" && edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "ECalCell");
  }
  if(physical->GetName()=="CellEcalEC_r" && edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "CellEcalEC_r");
  }
  if(physical->GetName()=="CellEcalEC_l" && edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "CellEcalEC_l");
  }

  //***************Fill HCal Energy Deposits:****************************
	
  if(physical->GetName()=="HCalAbsorberCell" && edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "HCalAbsorberCell");
  }
	
  if(physical->GetName()=="HCalScintillatorCell" && edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "HCalScintillatorCell");
  }

  if((physical->GetName()=="HCalAbsorberCell" || physical->GetName()=="HCalScintillatorCell" )&& edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "HCalCell");
  }
	
  if(physical->GetName()=="HcalECAbsCell_r1" && edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "HcalECAbsCell_r1");
  }
	
  if(physical->GetName()=="HcalECScnCell_r1" && edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "HcalECScnCell_r1");
  }

  if((physical->GetName()=="HcalECAbsCell_r1" || physical->GetName()=="HcalECScnCell_r1" )&& edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "HCalCell_r1");
  }

  if(physical->GetName()=="HcalECAbsCell_l1" && edep>0.) {
    fEventAction->AddEneDep(copyNo, edep, "HcalECAbsCell_l1");
  }
	
  if(physical->GetName()=="HcalECScnCell_l1" && edep>0.){
    fEventAction->AddEneDep(copyNo, edep, "HcalECScnCell_l1");
  }

  if((physical->GetName()=="HcalECAbsCell_l1" || physical->GetName()=="HcalECScnCell_l1" )&& edep>0.){
    fEventAction->AddEneDep(copyNo, edep, "HCalCell_l1");
  }

  if(physical->GetName()=="HcalECAbsCell_r2" && edep>0.){
    fEventAction->AddEneDep(copyNo, edep, "HcalECAbsCell_r2");
  }
	
  if(physical->GetName()=="HcalECScnCell_r2" && edep>0.){
    fEventAction->AddEneDep(copyNo, edep, "HcalECScnCell_r2");
  }

  if((physical->GetName()=="HcalECAbsCell_r2" || physical->GetName()=="HcalECScnCell_r2" )&& edep>0.){
    fEventAction->AddEneDep(copyNo, edep, "HCalCell_r2");
  }

  if(physical->GetName()=="HcalECAbsCell_l2" && edep>0.){
    fEventAction->AddEneDep(copyNo, edep, "HcalECAbsCell_l2");
  }
	
  if(physical->GetName()=="HcalECScnCell_l2" && edep>0.){
    fEventAction->AddEneDep(copyNo, edep, "HcalECScnCell_l2");
  }

  if((physical->GetName()=="HcalECAbsCell_l2" || physical->GetName()=="HcalECScnCell_l2" )&& edep>0.){
    fEventAction->AddEneDep(copyNo, edep, "HCalCell_l2");
  }
  
  //************** End of methods to fill HCal energy deposit ************

  if((physical->GetName()=="PixelInnerBarrel" ||
      physical->GetName()=="PixelInnerDiscs" ||
      physical->GetName()=="TrackerStripInnerBarrel" ||
      physical->GetName()=="TrackerStripInnerDisks" ||
      physical->GetName()== "TrackerStripOuterBarrel" ||
      physical->GetName()=="TrackerEndCap")
     && (edep>0)
     ) {
    fEventAction->AddTrackPos(pos);
    //	  std::cout<<theTrack->GetParticleDefinition()->GetParticleName()<<"\t"<<pos<<"\t"<<edep<<"\t"<<theTrack->GetVolume()->GetName()<<std::endl;
  }

  if( (physical->GetName()=="PixelInnerBarrel" || physical->GetName()=="TrackerStripInnerBarrel" || physical->GetName()=="TrackerStripOuterBarrel") && edep>0) {
    double rhohit = physical->GetLogicalVolume()->GetSolid()->DistanceToIn(G4ThreeVector(0,0,0),G4ThreeVector(1,0,0))/mm;
    int zquant = (int) (pos.getZ()/lPix);
    double phiPix = wPix/rhohit;
    int phiquant = (int) (pos.getPhi()/phiPix);
    double zhit = (zquant+0.5)*lPix;
    double phihit = phiquant*phiPix;
    G4ThreeVector quantPos(0,0,0);
    quantPos.setRhoPhiZ(rhohit,phihit,zhit);
    double rhProc = quantPos.getRho();
    double phProc = quantPos.getPhi()+phishift;
    double etProc = quantPos.getEta()+etashift;
    
    long rhfactor = floor(rhProc/rhbitres);
    long phfactor = floor(phProc/phbitres);
    long etfactor = floor(etProc/etbitres);
    long factor = rhfactor+phfactor*rhbit+etfactor*phbit*rhbit;
    
    fEventAction->fillTrackHit(etfactor,phfactor,rhfactor,factor,edep/eV);
  }
  
  if( (physical->GetName()=="PixelInnerDisks" || physical->GetName()=="TrackerStripInnerDisks" || physical->GetName()=="TrackerEndCap") && edep>0) {
    double zhit = physical->GetObjectTranslation().getZ()/mm;
    
    G4ThreeVector pMin(0,0,0);
    G4ThreeVector pMax(0,0,0);
    physical->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
    double rmax = pMax.getX()/mm;
    int xquant = floor((pos.getX()+0.5*rmax)/lPix);
    int yquant = floor((pos.getY()+0.5*rmax)/wPix);
    double xhit = xquant*lPix-0.5*rmax;
    double yhit = yquant*wPix-0.5*rmax;
    G4ThreeVector quantPos(xhit,yhit,zhit);
    
    double rhProc = quantPos.getRho();
    double phProc = quantPos.getPhi()+phishift;
    double etProc = quantPos.getEta()+etashift;
    
    long rhfactor = floor(rhProc/rhbitres);
    long phfactor = floor(phProc/phbitres);
    long etfactor = floor(etProc/etbitres);
    long factor = rhfactor+phfactor*rhbit+etfactor*phbit*rhbit;
    
    fEventAction->fillTrackHit(rhfactor,phfactor,etfactor,factor,edep/eV);
    
  }

  if( physical->GetName()=="muonBarrelChamber" && edep>0) {
    //std::cout<<pos<<"\t"<<edep/GeV<<std::endl;
    fEventAction->fillMuonHit("muonBarrelChamber", pos, edep);    
  }
  if( physical->GetName()=="muonECChamber_r" && edep>0) {
    //std::cout<<pos<<"\t"<<edep/GeV<<std::endl;
    fEventAction->fillMuonHit("muonECChamber_r", pos, edep);    
  }
  if( physical->GetName()=="muonECChamber_l" && edep>0) {
    //std::cout<<pos<<"\t"<<edep/GeV<<std::endl;
    fEventAction->fillMuonHit("muonECChamber_l", pos, edep);    
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
