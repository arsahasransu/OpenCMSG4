#include <iostream>
#include <cmath>
#include <TFile.h>
#include <TTreeReader.h>

int main() {

  TFile f1("OpenCMSG4_root.root");
  TTreeReader myReader("Events",&f1);
  TTreeReaderValue<std::vector<long>> trackHit(myReader,"trackerHits");
  unsigned int evtCounter = 0;
  
  constexpr long rhbit = 0b1000000000000000000;
  constexpr long phbit = 0b1000000000000000000000;
  constexpr long etbit = 0b10000000000000000000000;
  constexpr double rhbitres = 10;
  constexpr double phbitres = 4*pow(10,-6);
  constexpr double etbitres = pow(10,-6);
  constexpr double etashift = 4;
  constexpr double phishift = 1.571;

  while (myReader.Next()){
    std::cout << "Event " << evtCounter++ << std::endl;
    for (auto&& value : *trackHit){

      auto factor = value;
      long etfactor = factor/(phbit*rhbit);
      double eta = etfactor*etbitres-etashift;
      factor -= etfactor*phbit*rhbit;
      long phfactor = factor/rhbit;
      double phi = phfactor*phbitres-phishift;
      factor -= phfactor*rhbit;
      long rhfactor = factor;
      double rho = rhfactor*rhbitres;
      rho /= 1000;
      
      std::cout << value << "\t" << rhfactor<< "\t" << phfactor << "\t" << etfactor << std::endl;
      std::cout << eta << "\t" << phi << "\t" << rho << "cm" <<std::endl;
    }
  }
  
  return -1;
}
