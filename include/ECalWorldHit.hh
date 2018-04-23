#ifndef ECalWorldHit_h
#define ECalWorldHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

/// EM Calorimeter hit
///
/// It records:
/// - the cell ID
/// - the energy deposit 
/// - the cell logical volume, its position and rotation

class ECalWorldHit : public G4VHit
{
  public:
    ECalWorldHit();
    ECalWorldHit(G4int cellID);
    ECalWorldHit(const ECalWorldHit &right);
    virtual ~ECalWorldHit();

    const ECalWorldHit& operator=(const ECalWorldHit &right);
    int operator==(const ECalWorldHit &right) const;
    
    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    
    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();
    
    void SetCellID(G4int z) { fCellID = z; }
    G4int GetCellID() const { return fCellID; }

    void SetEdep(G4double de) { fEdep = de; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }

    void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    G4ThreeVector GetPos() const { return fPos; }

    void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
    G4RotationMatrix GetRot() const { return fRot; }

    void SetLogV(G4LogicalVolume* val) { fPLogV = val; }
    const G4LogicalVolume* GetLogV() const { return fPLogV; }
    
  private:
    G4int fCellID;
    G4double fEdep;
    G4ThreeVector fPos;
    G4RotationMatrix fRot;
    const G4LogicalVolume* fPLogV;
};

using ECalWorldHitsCollection = G4THitsCollection<ECalWorldHit>;

extern G4ThreadLocal G4Allocator<ECalWorldHit>* ECalWorldHitAllocator;

inline void* ECalWorldHit::operator new(size_t)
{
  if (!ECalWorldHitAllocator) {
       ECalWorldHitAllocator = new G4Allocator<ECalWorldHit>;
  }
  return (void*)ECalWorldHitAllocator->MallocSingle();
}

inline void ECalWorldHit::operator delete(void* aHit)
{
  ECalWorldHitAllocator->FreeSingle((ECalWorldHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
