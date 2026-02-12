#include "HepMCAsciiReader.hh"

#include "HepMCAsciiReaderMessenger.hh"

#include <fstream>
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCAsciiReader::HepMCAsciiReader() : filename("xxx.dat"), verbose(0)
{
  asciiInput = new HepMC::IO_GenEvent(filename.c_str(), std::ios::in);

  messenger = new HepMCAsciiReaderMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCAsciiReader::~HepMCAsciiReader()
{
  delete asciiInput;
  delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCAsciiReader::Initialize()
{
  delete asciiInput;

  asciiInput = new HepMC::IO_GenEvent(filename.c_str(), std::ios::in);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMC::GenEvent* HepMCAsciiReader::GenerateHepMCEvent()
{
  HepMC::GenEvent* evt = asciiInput->read_next_event();
  if (!evt) return 0;  // no more event

  if (verbose > 0) evt->print();

  return evt;
}
