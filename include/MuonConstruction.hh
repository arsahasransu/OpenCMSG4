#ifndef MuonConstruction_h
#define MuonConstruction_h 1

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"

#include <vector>

class MuonConstruction
{
public:
  MuonConstruction();
  virtual ~MuonConstruction();

  void makeBarrel(G4Material*, G4LogicalVolume*, std::vector<G4VisAttributes*>);
  void makeEndCap_negz(G4Material*, G4LogicalVolume*, bool, std::vector<G4VisAttributes*>);
  void makeEndCap_posz(G4Material*, G4LogicalVolume*, bool, std::vector<G4VisAttributes*>);

private:
  std::vector<G4LogicalVolume*> chamberMuonBarLogical;
  std::vector<G4LogicalVolume*> chamberMuonECLogical_r;
  std::vector<G4LogicalVolume*> chamberMuonECLogical_l;
  G4VisAttributes* visAttributes; 
};

#endif
