#include <iostream>
#include <cmath>
#include <vector>

#include <TVector3.h>

using namespace std;

long barrel(double rho, double eta, double phi) {

  double lPix = 50e-06; // in m - Here it translates to step value
  double wPix = 50e-06; // in m

  double etashift = 4;
  double phishift = 3.2;
  long rhbit = 0b10000000000000000000;
  long phbit = 0b10000000000000000000000;
  long etbit = 0b100000000000000000000000;
  double rhbitres = 10e-06;
  double phbitres = 4e-06;
  double etbitres = 1e-06;

  TVector3 pos;
  pos.SetPtEtaPhi(rho,eta,phi);
  double rhohit = rho;
  int zquant = (int) (pos.Z()/lPix);
  double phiPix = wPix/rhohit;
  int phiquant = (int) (pos.Phi()/phiPix);
  double zhit = (zquant+0.5)*lPix;
  double phihit = phiquant*phiPix;

  TVector3 quantPos(0,0,0);
  quantPos.SetXYZ(rhohit*cos(phihit),rhohit*sin(phihit),zhit);
  double rhProc = quantPos.Pt();
  double phProc = quantPos.Phi()+phishift;
  double etProc = quantPos.Eta()+etashift;
    
  long rhfactor = floor(rhProc/rhbitres);
  long phfactor = floor(phProc/phbitres);
  long etfactor = floor(etProc/etbitres);
  long factor = rhfactor+phfactor*rhbit+etfactor*phbit*rhbit;

  return factor;
}

int main() {

  // Generate a million random points in the barrel region.

  double barR[] = {0.044,0.073,0.102,0.255,0.339,0.4185,0.498,0.608,0.692,0.780,0.868,0.965,1.080};
  int barRlen = sizeof(barR)/sizeof(barR[0]);
  double etamax = 1.8;
  double phimax = 3.2;

  std::vector<long> trackBit;
  std::vector<double> rhoBit;
  std::vector<double> etaBit;
  std::vector<double> phiBit;
  
  for(unsigned int ctr=0; ctr<100000; ctr++) {
    int r1 = rand() % barRlen;
    float r2 = -etamax + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2*etamax)));
    float r3 = -phimax + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2*phimax)));

    long factor = barrel(barR[r1],r2,r3);
    trackBit.push_back(factor);
    rhoBit.push_back(barR[r1]);
    etaBit.push_back(r2);
    phiBit.push_back(r3);
    
    //cout<<r1<<" "<<r2<<" "<<r3<<" "<<factor<<endl;
  }

  cout<<trackBit.size()<<endl;

  for(unsigned int ctr1=0; ctr1<trackBit.size(); ctr1++) {
    if(ctr1%10000==0) cout<<ctr1<<" done"<<endl;
    for(unsigned int ctr2=ctr1+1; ctr2<trackBit.size(); ctr2++) {

      if(trackBit[ctr1]==trackBit[ctr2]) {
	cout<<"flag"<<endl;
	cout<<trackBit[ctr1]<<"\t"<<rhoBit[ctr1]<<"\t"<<etaBit[ctr1]<<"\t"<<phiBit[ctr1]<<endl;
	cout<<trackBit[ctr2]<<"\t"<<rhoBit[ctr2]<<"\t"<<etaBit[ctr2]<<"\t"<<phiBit[ctr2]<<endl;
      }
    }
  }
  
  return -1;
}

