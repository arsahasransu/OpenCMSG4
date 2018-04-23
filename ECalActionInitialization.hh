#ifndef ECalActionInitialization_h
#define ECalActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class ECalActionInitialization : public G4VUserActionInitialization
{
  public:
    ECalActionInitialization();
    virtual ~ECalActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
