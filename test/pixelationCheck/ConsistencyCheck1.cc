#include <iostream>
#include <cmath>
#include <vector>
#include <bitset>

using namespace std;

int main() { // Check for rho

  double lPix = 50e-06; // in m - Here it translates to step value
  double wPix = 50e-06; // in m

  double etashift = 4;
  double phishift = 3.2; // Add before pixelation and then discretize
  long rhbit = 0b100000000000000000;
  long phbit = 0b100000000000000000000;
  long etbit = 0b1000000000000000000000;
  double rhbitres = 10e-06;
  double phbitres = 4e-06;
  double etbitres = 1e-06;

  std::vector<long> facvec;
  std::vector<double> etavec;
  std::vector<double> phivec;
  std::vector<double> rhovec;
  long i=0;
  for(double eta=0; eta<=0; eta=eta+(10e5)*etbitres) {
    for(double phi=0; phi<=0; phi=phi+(10e4)*phbitres) {
      for(double rho=0; rho<=2.62144; rho+=rhbitres) {
	//double pass = false;
	//pass = rho==0.00412||rho==0.00413?true:false;
	//pass = pass||(rho==0.08434||rho==0.08435)?true:false;
	//pass = pass||(rho==0.57893||rho==0.57894)?true:false;
	//if(!pass) continue;

	  
	double etashifted = eta+etashift;
	double phishifted = phi+phishift;
	long rhfactor = round(rho/rhbitres);
	long phfactor = round(phi/phbitres);
	long etfactor = round(eta/etbitres);
	long factor = rhfactor+rhbit*(phfactor+phbit*etfactor);

	//std::bitset<64> tobits(factor);
	//cout<<rho<<"\t"<<rhfactor<<"\t"<<factor<<"\t"<<tobits<<endl;
	//cout<<rho/rhbitres<<endl;
	//if(i%1000000000==0) cout<<"Print: "<<i<<endl;
	//i++;
	facvec.push_back(factor);
	etavec.push_back(eta);
	phivec.push_back(phi);
	rhovec.push_back(rho);
      }
    }
  }

  cout<<"Size of array to check: "<<facvec.size()<<endl;
  for(int i=0; i<facvec.size(); i++) {
    if(i%10000==0) cout<<i<<" done."<<endl;
    for(int j=i+1; j<facvec.size(); j++) {
      // Check if they are equal
      if(facvec[i]==facvec[j]) {
	std::bitset<64> tobitsi(facvec[i]);
	cout<<"flag: "<<facvec[i]<<"\t"<<etavec[i]<<"\t"<<phivec[i]<<"\t"<<rhovec[i]<<"\t"<<tobitsi<<endl;
	std::bitset<64> tobitsj(facvec[j]);
	cout<<"flag: "<<facvec[j]<<"\t"<<etavec[j]<<"\t"<<phivec[j]<<"\t"<<rhovec[j]<<"\t"<<tobitsj<<endl;
      }
    }
  }
  
  return -1;
}

