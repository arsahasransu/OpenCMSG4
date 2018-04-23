#ifndef ECaltrackerHit_h
#define ECaltrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

/// Drift chamber hit
///
/// It records:
/// - the layer ID
/// - the particle time
/// - the particle local and global positions

class ECaltrackerHit : public G4VHit
{
  public:
    ECaltrackerHit();
    ECaltrackerHit(G4int layerID);
    ECaltrackerHit(const ECaltrackerHit &right);
    virtual ~ECaltrackerHit();

    const ECaltrackerHit& operator=(const ECaltrackerHit &right);
    int operator==(const ECaltrackerHit &right) const;
    
    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    
    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    void SetLayerID(G4int z) { fLayerID = z; }
    G4int GetLayerID() const { return fLayerID; }

    void SetTime(G4double t) { fTime = t; }
    G4double GetTime() const { return fTime; }

    void SetLocalPos(G4ThreeVector xyz) { fLocalPos = xyz; }
    G4ThreeVector GetLocalPos() const { return fLocalPos; }

    void SetWorldPos(G4ThreeVector xyz) { fWorldPos = xyz; }
    G4ThreeVector GetWorldPos() const { return fWorldPos; }
    
  private:
    G4int fLayerID;
    G4double fTime;
    G4ThreeVector fLocalPos;
    G4ThreeVector fWorldPos;
};

using ECaltrackerHitsCollection = G4THitsCollection<ECaltrackerHit>;

extern G4ThreadLocal G4Allocator<ECaltrackerHit>* ECaltrackerHitAllocator;

inline void* ECaltrackerHit::operator new(size_t)
{
  if (!ECaltrackerHitAllocator) {
       ECaltrackerHitAllocator = new G4Allocator<ECaltrackerHit>;
  }
  return (void*)ECaltrackerHitAllocator->MallocSingle();
}

inline void ECaltrackerHit::operator delete(void* aHit)
{
  ECaltrackerHitAllocator->FreeSingle((ECaltrackerHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
