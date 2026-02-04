#ifndef HEPMC_ASCII_READER_H
#define HEPMC_ASCII_READER_H

#include "HepMC/IO_GenEvent.h"
#include "HepMCG4Interface.hh"

class HepMCAsciiReaderMessenger;

class HepMCAsciiReader : public HepMCG4Interface
{
  protected:
    G4String filename;
    HepMC::IO_GenEvent* asciiInput;

    G4int verbose;
    HepMCAsciiReaderMessenger* messenger;

    virtual HepMC::GenEvent* GenerateHepMCEvent();

  public:
    HepMCAsciiReader();
    ~HepMCAsciiReader();

    // set/get methods
    void SetFileName(G4String name);
    G4String GetFileName() const;

    void SetVerboseLevel(G4int i);
    G4int GetVerboseLevel() const;

    // methods...
    void Initialize();
};

// ====================================================================
// inline functions
// ====================================================================

inline void HepMCAsciiReader::SetFileName(G4String name)
{
  filename = name;
}

inline G4String HepMCAsciiReader::GetFileName() const
{
  return filename;
}

inline void HepMCAsciiReader::SetVerboseLevel(G4int i)
{
  verbose = i;
}

inline G4int HepMCAsciiReader::GetVerboseLevel() const
{
  return verbose;
}

#endif
