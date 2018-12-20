#ifndef _HEPMC_ASCII_READER_H
#define _HEPMC_ASCII_READER_H

#include "G4VPrimaryGenerator.hh"
#include "HepMC/IO_GenEvent.h"

class HepMCAsciiReaderMessenger;

class HepMCAsciiReader : public G4VPrimaryGenerator
{
	public:
		HepMCAsciiReader();
		~HepMCAsciiReader();

	protected:
		G4String filename;
		G4int verbose;
		HepMCAsciiReaderMessenger* messenger;

		HepMC::IO_GenEvent* asciiInput;
		HepMC::GenEvent* hepmcevt;
		virtual G4bool CheckVertexInsideWorld(const G4ThreeVector& pos) const;
		virtual HepMC::GenEvent* GenerateHepMCEvent();
		void HepMC2G4(const HepMC::GenEvent* hepmcevt, G4Event* event);

	public:
		virtual void GeneratePrimaryVertex(G4Event * event);

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
  filename= name;
}

inline G4String HepMCAsciiReader::GetFileName() const
{
  return filename;
}

inline void HepMCAsciiReader::SetVerboseLevel(G4int i)
{
  verbose= i;
}

inline G4int HepMCAsciiReader::GetVerboseLevel() const
{
  return verbose;
}

#endif
