#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int main() {

  double lPix = 50e-06; // in m - Here it translates to step value
  double wPix = 50e-06; // in m

  double etashift = 4;
  double phishift = 1.571;
  long rhbit = 0b1000000000000000000;
  long phbit = 0b1000000000000000000000;
  long etbit = 0b10000000000000000000000;
  double rhbitres = 10e-06;
  double phbitres = 4e-06;
  double etbitres = 1e-06;

  long i=0;
  for(double rho=0; rho<2; rho=rho+1000*rhbitres) {

    long rhfactor = floor(rho/rhbitres);
    long factor = rhfactor;
    cout<<factor<<endl;
    if(i%100000000==0) cout<<i<<endl;
    i++;
    //facvec.push_back(factor);
  }
  
  return -1;
}

