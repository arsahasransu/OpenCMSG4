#include "ECalEmCalorimeterSD.hh"
#include "ECalEmCalorimeterHit.hh"
#include "ECalConstants.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalEmCalorimeterSD::ECalEmCalorimeterSD(G4String name)
: G4VSensitiveDetector(name), 
  fHitsCollection(nullptr), fHCID(-1)
{
  collectionName.insert("EMcalorimeterColl");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalEmCalorimeterSD::~ECalEmCalorimeterSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalEmCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection 
    = new ECalEmCalorimeterHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); 
  }
  hce->AddHitsCollection(fHCID,fHitsCollection);
  
  // fill calorimeter hits with zero energy deposition
  for (auto i=0;i<kNofEmCells;i++) {
    fHitsCollection->insert(new ECalEmCalorimeterHit(i));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ECalEmCalorimeterSD::ProcessHits(G4Step*step, G4TouchableHistory*)
{
  auto edep = step->GetTotalEnergyDeposit();
  if (edep==0.) return true;
  
  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto physical = touchable->GetVolume();
  auto copyNo = physical->GetCopyNo();
  
  auto hit = (*fHitsCollection)[copyNo];
  // check if it is first touch
  if (!(hit->GetLogV())) {
    // fill volume information
    hit->SetLogV(physical->GetLogicalVolume());
    G4AffineTransform transform = touchable->GetHistory()->GetTopTransform();
    transform.Invert();
    hit->SetRot(transform.NetRotation());
    hit->SetPos(transform.TransformPoint(step->GetPreStepPoint()->GetPosition()));
  }
  // add energy deposition
  hit->AddEdep(edep);
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
