#ifndef CustomPDGParser_h
#define CustomPDGParser_h

#include <vector>


class CustomPDGParser
{
public:
 static bool s_isR1Hadron(int pdg);
 static bool s_isS1Hadron(int pdg);
 static bool s_isR1Meson(int pdg);
 static bool s_isAntiR1Meson(int pdg);
 static bool s_isR1Baryon(int pdg);
 static bool s_isAntiR1Baryon(int pdg);
 static bool s_isS1Meson(int pdg);
 static bool s_isAntiS1Meson(int pdg);
 static bool s_isS1Baryon(int pdg);
 static bool s_isAntiS1Baryon(int pdg);
 static double s_charge(int pdg);
 static double s_spin(int pdg);
 static std::vector<int> s_containedQuarks(int pdg);
 static int s_containedQuarksCode(int pdg);

};

#endif
