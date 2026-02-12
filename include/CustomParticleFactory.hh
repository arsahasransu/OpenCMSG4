#ifndef CustomParticleFactory_h
#define CustomParticleFactory_h 1

#include "CustomParticle.hh"
#include <set>


// ######################################################################
// ###                          CustomParticle                                ###
// ######################################################################

class CustomParticleFactory 
{
 private:
  static thread_local bool loaded;
  static thread_local std::set<G4ParticleDefinition *> m_particles;

 public:
  static void loadCustomParticles();
  static bool isCustomParticle(G4ParticleDefinition *particle);
  
};

#endif
