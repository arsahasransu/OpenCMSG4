#include "ECaltrackerHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<ECaltrackerHit>* ECaltrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECaltrackerHit::ECaltrackerHit()
: G4VHit(), 
  fLayerID(-1), fTime(0.), fLocalPos(0), fWorldPos(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECaltrackerHit::ECaltrackerHit(G4int layerID)
: G4VHit(), 
  fLayerID(layerID), fTime(0.), fLocalPos(0), fWorldPos(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECaltrackerHit::~ECaltrackerHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECaltrackerHit::ECaltrackerHit(const ECaltrackerHit &right)
: G4VHit(),
  fLayerID(right.fLayerID),
  fTime(right.fTime),
  fLocalPos(right.fLocalPos),
  fWorldPos(right.fWorldPos)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ECaltrackerHit& ECaltrackerHit::operator=(const ECaltrackerHit &right)
{
  fLayerID = right.fLayerID;
  fTime = right.fTime;
  fLocalPos = right.fLocalPos;
  fWorldPos = right.fWorldPos;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int ECaltrackerHit::operator==(const ECaltrackerHit &/*right*/) const
{
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECaltrackerHit::Draw()
{
  auto visManager = G4VVisManager::GetConcreteInstance();
  if (! visManager) return;

  G4Circle circle(fWorldPos);
  circle.SetScreenSize(2);
  circle.SetFillStyle(G4Circle::filled);
  G4Colour colour(1.,1.,0.);
  G4VisAttributes attribs(colour);
  circle.SetVisAttributes(attribs);
  visManager->Draw(circle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* ECaltrackerHit::GetAttDefs() const
{
  G4bool isNew;
  auto store = G4AttDefStore::GetInstance("ECaltrackerHit",isNew);

  if (isNew) {
      (*store)["HitType"] 
        = G4AttDef("HitType","Hit Type","Physics","","G4String");
      
      (*store)["ID"] 
        = G4AttDef("ID","ID","Physics","","G4int");
      
      (*store)["Time"] 
        = G4AttDef("Time","Time","Physics","G4BestUnit","G4double");
      
      (*store)["Pos"] 
        = G4AttDef("Pos", "Position", "Physics","G4BestUnit","G4ThreeVector");
  }
  
  return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* ECaltrackerHit::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;
  
  values
    ->push_back(G4AttValue("HitType","trackerHit",""));
  values
    ->push_back(G4AttValue("ID",G4UIcommand::ConvertToString(fLayerID),""));
  values
    ->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));
  values
    ->push_back(G4AttValue("Pos",G4BestUnit(fWorldPos,"Length"),""));
  
  return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECaltrackerHit::Print()
{
  G4cout << "  Layer[" << fLayerID << "] : time " << fTime/ns
  << " (nsec) --- local (x,y) " << fLocalPos.x()
  << ", " << fLocalPos.y() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
