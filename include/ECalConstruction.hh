#ifndef ECalConstruction_h
#define ECalConstruction_h 1

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"

#include <vector>

class ECalConstruction
{
public:
  ECalConstruction();
  virtual ~ECalConstruction();

  void makeBarrel(G4Material*, G4LogicalVolume*, std::vector<G4VisAttributes*>);
  void makeEndCap(G4Material*, G4LogicalVolume*, G4LogicalVolume*, bool, bool, std::vector<G4VisAttributes*>);

private:
  std::vector< std::vector<G4LogicalVolume*> > cellEcalBarLogical;
  std::vector<G4LogicalVolume*> cellEcalECLogical_r;
  std::vector<G4LogicalVolume*> cellEcalECLogical_l;
  G4VisAttributes* visAttributes; 
};

#endif
