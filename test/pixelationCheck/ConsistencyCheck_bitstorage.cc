#include <iostream>
#include <cmath>
#include <vector>
#include <bitset>

using namespace std;

int main() {

  long rhbit = 0b10;
  long phbit = 0b100;
  long etbit = 0b100;

  std::vector<long> facvec;
  long i=0;
  for(double eta=0; eta<=3; eta=eta+1) {
    for(double phi=0; phi<=3; phi=phi+1) {
      for(double rho=0; rho<=1; rho=rho+1) {

	long factor = rho+rhbit*(phi+phbit*eta);
	std::bitset<64> tobits(factor);
	cout<<eta<<"\t"<<phi<<"\t"<<rho<<"\t"<<factor<<"\t"<<tobits<<endl;
	//if(i%1000000000==0) cout<<"Print: "<<i<<endl;
	//i++;
	//facvec.push_back(factor);
      }
    }
  }
  
  return -1;
}

