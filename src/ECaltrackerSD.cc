#include "ECaltrackerSD.hh"
#include "ECaltrackerHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECaltrackerSD::ECaltrackerSD(G4String name)
: G4VSensitiveDetector(name), 
  fHitsCollection(nullptr), fHCID(-1)
{
  collectionName.insert("trackerColl");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECaltrackerSD::~ECaltrackerSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECaltrackerSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection 
    = new ECaltrackerHitsCollection(SensitiveDetectorName,collectionName[0]);

  if (fHCID<0) { 
     fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); 
  }
  hce->AddHitsCollection(fHCID,fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ECaltrackerSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  auto charge = step->GetTrack()->GetDefinition()->GetPDGCharge();
  if (charge==0.) return true;
  
  auto preStepPoint = step->GetPreStepPoint();

  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto motherPhysical = touchable->GetVolume(1); // mother
  auto copyNo = motherPhysical->GetCopyNo();

  auto worldPos = preStepPoint->GetPosition();
  auto localPos 
    = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
  
  auto hit = new ECaltrackerHit(copyNo);
  hit->SetWorldPos(worldPos);
  hit->SetLocalPos(localPos);
  hit->SetTime(preStepPoint->GetGlobalTime());
  
  fHitsCollection->insert(hit);
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
