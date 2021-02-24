#ifndef HCalConstruction_h
#define HCalConstruction_h 1

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"

#include <vector>

class HCalConstruction
{
public:
  HCalConstruction();
  virtual ~HCalConstruction();

  void makeBarrel(G4Material*, G4Material*, G4LogicalVolume*, std::vector<G4VisAttributes*>);
  void makeEndCapOuter(G4Material*, G4Material*, G4LogicalVolume*, G4LogicalVolume*, std::vector<G4VisAttributes*>);
  void makeEndCapInner(G4Material*, G4Material*, G4LogicalVolume*, G4LogicalVolume*, std::vector<G4VisAttributes*>);
  void makeEndCapJoint(G4Material*, G4Material*, G4LogicalVolume*, std::vector<G4VisAttributes*>);

private:
// Barrel section (tower 1-15)
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalBarAbsLogical;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalBarScnLogical;
// End cap outer rim with 5 degree resolution in phi (tower 17-20)
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECAbsLogical_r1;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECAbsLogical_l1;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECScnLogical_r1;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECScnLogical_l1;
// End cap inner rim with 10 degree resolution in phi (tower 21 - 28)
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECAbsLogical_r2;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECAbsLogical_l2;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECScnLogical_r2;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECScnLogical_l2;
// End Cap to Barrel joining section (tower 16)
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECAbsLogical_rj;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECAbsLogical_lj;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECScnLogical_rj;
  std::vector< std::vector< std::vector<G4LogicalVolume*> > > cellHcalECScnLogical_lj;
  G4VisAttributes* visAttributes; 
};

#endif
