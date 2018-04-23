#ifndef ECalEmCalorimeterHit_h
#define ECalEmCalorimeterHit_h 1

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

class ECalEmCalorimeterHit : public G4VHit
{
  public:
    ECalEmCalorimeterHit();
    ECalEmCalorimeterHit(G4int cellID);
    ECalEmCalorimeterHit(const ECalEmCalorimeterHit &right);
    virtual ~ECalEmCalorimeterHit();

    const ECalEmCalorimeterHit& operator=(const ECalEmCalorimeterHit &right);
    int operator==(const ECalEmCalorimeterHit &right) const;
    
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

using ECalEmCalorimeterHitsCollection = G4THitsCollection<ECalEmCalorimeterHit>;

extern G4ThreadLocal G4Allocator<ECalEmCalorimeterHit>* ECalEmCalorimeterHitAllocator;

inline void* ECalEmCalorimeterHit::operator new(size_t)
{
  if (!ECalEmCalorimeterHitAllocator) {
       ECalEmCalorimeterHitAllocator = new G4Allocator<ECalEmCalorimeterHit>;
  }
  return (void*)ECalEmCalorimeterHitAllocator->MallocSingle();
}

inline void ECalEmCalorimeterHit::operator delete(void* aHit)
{
  ECalEmCalorimeterHitAllocator->FreeSingle((ECalEmCalorimeterHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
