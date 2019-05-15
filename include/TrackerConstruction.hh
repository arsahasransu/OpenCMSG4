#ifndef TrackerConstruction_h
#define TrackerConstruction_h 1

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"

#include <vector>

class TrackerConstruction
{
public:
  TrackerConstruction();
  virtual ~TrackerConstruction();

  void makePIB(G4Material*, G4Material*, G4Material*, std::vector<G4Material*>, G4LogicalVolume*, bool, std::vector<G4VisAttributes*>);
  void makePID(G4Material*, G4Material*, G4Material*, std::vector<G4Material*>, G4LogicalVolume*, bool, std::vector<G4VisAttributes*>);
  void makeTIB(G4Material*, G4Material*, std::vector<G4Material*>, G4LogicalVolume*, bool, std::vector<G4VisAttributes*>);
  void makeTID(G4Material*, G4Material*, G4Material*, G4LogicalVolume*, bool, std::vector<G4VisAttributes*>);
  void makeTOB(G4Material*, G4Material*, G4Material*, std::vector<G4Material*>, G4LogicalVolume*, bool, std::vector<G4VisAttributes*>);
  void makeTEC(G4Material*, std::vector<G4Material*>, G4LogicalVolume*, bool, std::vector<G4VisAttributes*>);

private:
  std::vector<G4LogicalVolume*> PIB_Logical;
  std::vector<G4LogicalVolume*> PID_Logical;
  std::vector<G4LogicalVolume*> TIB_Logical;
  std::vector<G4LogicalVolume*> TID_Logical;
  std::vector<G4LogicalVolume*> TOB_Logical;
  std::vector<G4LogicalVolume*> TEC_Logical;
  G4VisAttributes* visAttributes; 
};

#endif
