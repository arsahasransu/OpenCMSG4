#include "HepMCAsciiReader.hh"
#include "HepMCAsciiReaderMessenger.hh"

#include "G4RunManager.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>

HepMCAsciiReader::HepMCAsciiReader():
  filename("xxx.dat"), verbose(0), hepmcevt(0)
{
	asciiInput = new HepMC::IO_GenEvent("example_MyPythia.dat", std::ios::in);
	messenger= new HepMCAsciiReaderMessenger(this);
}

HepMCAsciiReader::~HepMCAsciiReader()
{
	delete asciiInput;
	delete hepmcevt;
	delete messenger;
}

void HepMCAsciiReader::Initialize()
{
	delete asciiInput;

	asciiInput= new HepMC::IO_GenEvent(filename.c_str(), std::ios::in);
}

HepMC::GenEvent* HepMCAsciiReader::GenerateHepMCEvent()
{
	auto hepmcEvt = asciiInput->read_next_event();
	if(!hepmcEvt) return 0;
//	else hepmcEvt->print();

	if(verbose>0) hepmcEvt-> print();

	return hepmcEvt;
}

G4bool HepMCAsciiReader::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
	auto navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
	auto world = navigator->GetWorldVolume();
	auto solid = world->GetLogicalVolume()->GetSolid();

	EInside qinside = solid->Inside(pos);
	if(qinside != kInside) return false;
	else return true;
}

void HepMCAsciiReader::HepMC2G4(const HepMC::GenEvent* hepmcEvt, G4Event* event)
{
	for(HepMC::GenEvent::vertex_const_iterator vitr = hepmcEvt->vertices_begin();
				vitr != hepmcEvt->vertices_end(); ++vitr)
	{
		G4bool qvtx = false;
		for(HepMC::GenVertex::particle_iterator pitr = (*vitr)->particles_begin(HepMC::children);
				pitr != (*vitr)->particles_end(HepMC::children); ++pitr)
		{
			if(!(*pitr)->end_vertex() && (*pitr)->status()==1)
			{
				qvtx = true;
				break;
			}
		}
		if(!qvtx) continue;
		
		HepMC::FourVector pos = (*vitr)->position();
		G4LorentzVector xvtx(pos.x(), pos.y(), pos.z(), pos.t());
		if (! CheckVertexInsideWorld(xvtx.vect()*mm)) continue;

		G4PrimaryVertex* g4vtx = 
				new G4PrimaryVertex(xvtx.x()*mm, xvtx.y()*mm, xvtx.z()*mm, xvtx.t()*mm/c_light);
		
		for (HepMC::GenVertex::particle_iterator vpitr= (*vitr)->particles_begin(HepMC::children);
						vpitr != (*vitr)->particles_end(HepMC::children); ++vpitr)
		{
			if( (*vpitr)->status() != 1 ) continue;

			G4int pdgcode= (*vpitr)-> pdg_id();
			pos= (*vpitr)-> momentum();
			G4LorentzVector p(pos.px(), pos.py(), pos.pz(), pos.e());
			G4PrimaryParticle* g4prim = 
						new G4PrimaryParticle(pdgcode, p.x()*GeV, p.y()*GeV, p.z()*GeV);
//std::cout<<g4prim->GetParticleDefinition()->GetParticleName()<<std::endl;
			g4vtx-> SetPrimary(g4prim);
		}
		event->AddPrimaryVertex(g4vtx);
	}
}

void HepMCAsciiReader::GeneratePrimaryVertex(G4Event * event)
{
	delete hepmcevt;
G4cout<<"Check coming here"<<G4endl;
	hepmcevt = GenerateHepMCEvent();
	if(! hepmcevt) 
	{
		G4cout << "HepMCInterface: no generated particles. run terminated..."<< G4endl;
		G4RunManager::GetRunManager()-> AbortRun();
		return;
	}
	HepMC2G4(hepmcevt, event);
}

