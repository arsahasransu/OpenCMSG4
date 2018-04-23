#ifndef ECalSteppingAction_H
#define ECalSteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class ECalEventAction;

class ECalSteppingAction : public G4UserSteppingAction
{
  public:
    ECalSteppingAction(ECalEventAction* eventAction);
    virtual ~ECalSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
	    ECalEventAction* fEventAction;

};

#endif
