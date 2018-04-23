#ifndef ECalEventAction_h
#define ECalEventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include <vector>

/// Event action

class ECalEventAction : public G4UserEventAction
{
public:
    ECalEventAction();
    virtual ~ECalEventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    std::vector<G4double>& GetEmCalEdep() { return fEmCalEdep; }
    std::vector<G4double>& GetConvertedX() { return fConvertedX; }
    std::vector<G4double>& GetConvertedY() { return fConvertedY; }
    std::vector<G4double>& GetConvertedZ() { return fConvertedZ; }
    std::vector<G4int>& GetEmCalCrysNum() { return fEmCalCrysNum; }
    std::vector<G4int>& GetCellNoEcalEC_r() { return fCellNoEcalEC_r; }
    std::vector<G4double>& GetEdepEcalEC_r() { return fEdepEcalEC_r; }
    std::vector<G4int>& GetCellNoEcalEC_l() { return fCellNoEcalEC_l; }
    std::vector<G4double>& GetEdepEcalEC_l() { return fEdepEcalEC_l; }
    
		std::vector<G4double> fEmCalEdep;
		std::vector<G4double> fConvertedX;
		std::vector<G4double> fConvertedY;
		std::vector<G4double> fConvertedZ;
		std::vector<G4int> fEmCalCrysNum;
		std::vector<G4int> fCellNoEcalEC_r;
		std::vector<G4double> fEdepEcalEC_r;
		std::vector<G4int> fCellNoEcalEC_l;
		std::vector<G4double> fEdepEcalEC_l;


	void set_pair_flag()
	{ 
		pair_prod_flag++;
	}

	void SetConvertedVertex(G4ThreeVector vertex)
	{
		fConvertedX.push_back(vertex.x());
		fConvertedY.push_back(vertex.y());
		fConvertedZ.push_back(vertex.z());
	}

private:
    G4int fEmCalHCID;
    G4int fEmCalECHCID;
    G4int fEmCalECHCID_left;
    G4int fWorldHCID;
    G4int ftrackerHCID;
    G4int ftrackerEnvHCID;
    G4int pair_prod_flag;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
