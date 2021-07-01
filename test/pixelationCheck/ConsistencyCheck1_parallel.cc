#include <iostream>
#include <cmath>
#include <vector>
#include <bitset>
#include <chrono>
#include <omp.h>

using namespace std;

int main() { // Check for rho

  double lPix = 50e-06; // in m - Here it translates to step value
  double wPix = 50e-06; // in m

  double etashift = 4;
  double phishift = 3.2; // Add before pixelation and then discretize
  long rhbit = 0b1000000000000000000;
  long phbit = 0b1000000000000000000000;
  long etbit = 0b10000000000000000000000;
  double rhbitres = 10e-06;
  double phbitres = 4e-06;
  double etbitres = 1e-06;

  std::vector<long> facvec;
  std::vector<double> etavec;
  std::vector<double> phivec;
  std::vector<double> rhovec;
  long i=0;
  for(double eta=0; eta<=4.194304; eta=eta+etbitres) { // 4.194304 = ((2^(23-1))-1) * (1e-6))
    //for(double phi=0; phi<=8.388604; phi=phi+phbitres) { // 8.388604 = ((2^(22-1))-1) * (4e-6))
    for(double phi=0; phi<=0; phi=phi+phbitres) { 
      //for(double rho=0; rho<=2.62143; rho+=rhbitres) { // 2.62143 = ((2^(19-1))-1) * (10e-5)
      for(double rho=0; rho<=0; rho+=rhbitres) {
	//double pass = false;
	//pass = rho==0.00412||rho==0.00413?true:false;
	//pass = pass||(rho==0.08434||rho==0.08435)?true:false;
	//pass = pass||(rho==0.57893||rho==0.57894)?true:false;
	//if(!pass) continue;
	//if(eta>10*etbitres) continue;
	  
	double etashifted = eta+etashift;
	double phishifted = phi+phishift;
	long rhfactor = round(rho/rhbitres);
	long phfactor = round(phi/phbitres);
	long etfactor = round(eta/etbitres);
	long factor = rhfactor+rhbit*(phfactor+phbit*etfactor);

	//std::bitset<64> tobits(factor);
	//cout<<eta<<"\t"<<etfactor<<"\t"<<phi<<"\t"<<phfactor<<"\t"<<rho<<"\t"<<rhfactor<<"\t"<<factor<<"\t"<<tobits<<endl;
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
  auto start = std::chrono::steady_clock::now();
#pragma omp parallel
  {
#pragma omp for
    for(int ctr=0; ctr<facvec.size(); ctr++) {
      if(ctr%1000==0) cout<<ctr<<" done."<<endl;
      for(int j=ctr+1; j<facvec.size(); j++) {
	// Check if they are equal
	if(facvec[ctr]==facvec[j]) {
	  std::bitset<64> tobitsi(facvec[ctr]);
	  cout<<"flag: "<<facvec[ctr]<<"\t"<<etavec[ctr]<<"\t"<<phivec[ctr]<<"\t"<<rhovec[ctr]<<"\t"<<tobitsi<<endl;
	  std::bitset<64> tobitsj(facvec[j]);
	  cout<<"flag: "<<facvec[j]<<"\t"<<etavec[j]<<"\t"<<phivec[j]<<"\t"<<rhovec[j]<<"\t"<<tobitsj<<endl;
	}
      }
    }
  }
  auto end = std::chrono::steady_clock::now();
  auto diff = end-start;
  
  cout<<"Clock time: "<<std::chrono::duration<double,milli> (diff).count()<<endl;
  
  return -1;
}

