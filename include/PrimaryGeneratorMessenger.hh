#ifndef PRIMARY_GENERATOR_MESSENGER_H
#define PRIMARY_GENERATOR_MESSENGER_H

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class PrimaryGeneratorAction;

class PrimaryGeneratorMessenger : public G4UImessenger {
public:
  PrimaryGeneratorMessenger(PrimaryGeneratorAction* genaction);
  ~PrimaryGeneratorMessenger();

  void SetNewValue(G4UIcommand* command, G4String newValues);
  G4String GetCurrentValue(G4UIcommand* command);

private:
  PrimaryGeneratorAction* primaryAction;

  G4UIdirectory* dir;
  G4UIdirectory* mydetdir;
  G4UIcmdWithAString* select;

};

#endif
