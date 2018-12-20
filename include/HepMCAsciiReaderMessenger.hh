#ifndef HEPMC_G4_ASCII_READER_MESSENGER_H
#define HEPMC_G4_ASCII_READER_MESSENGER_H

#include "G4UImessenger.hh"

class HepMCAsciiReader;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class HepMCAsciiReaderMessenger : public G4UImessenger {
public:
  HepMCAsciiReaderMessenger(HepMCAsciiReader* agen);
  ~HepMCAsciiReaderMessenger();

  void SetNewValue(G4UIcommand* command, G4String newValues);
  G4String GetCurrentValue(G4UIcommand* command);

private:
  HepMCAsciiReader* gen;

  G4UIdirectory* dir;
  G4UIcmdWithAnInteger* verbose;
  G4UIcmdWithAString* open;

};

#endif
