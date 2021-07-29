#include <CustomPDGParser.hh>
#include<iostream>

/*CustomPDGParser::CustomPDGParser(int pdgCode) : m_pdgCode(pdgCode)
{

}*/

bool CustomPDGParser::s_isR1Hadron(int pdg) 
{
 int pdgAbs=abs(pdg);
 return ((pdgAbs/1000000 % 10 == 1)); //checking isospin multiplet or not
}

bool CustomPDGParser::s_isS1Hadron(int pdg) 
{
 int pdgAbs=abs(pdg);
 return ((pdgAbs/1000000 % 10 == 2)); //checking isospin multiplet
}

bool CustomPDGParser::s_isR1Meson(int pdg)
{
 int pdgAbs=abs(pdg);
 return ((pdgAbs/1000000 % 10 == 1)&&(pdg>0)&&((pdgAbs/10)%1000>0)&&((pdgAbs/10)%1000<10));
}

bool CustomPDGParser::s_isAntiR1Meson(int pdg)
{
 int pdgAbs=abs(pdg);
 return ((pdgAbs/1000000 % 10 == 1)&&(pdg<0)&&((pdgAbs/10)%1000>0)&&((pdgAbs/10)%1000<10));
}

bool CustomPDGParser::s_isR1Baryon(int pdg)
{
 int pdgAbs=abs(pdg);
 return  ((pdgAbs/1000000 % 10 == 1)&&(pdg>0)&&((pdgAbs/10)%1000>10)&&((pdgAbs/10)%1000<100));

}

bool CustomPDGParser::s_isAntiR1Baryon(int pdg)
{
 int pdgAbs=abs(pdg);
 return  ((pdgAbs/1000000 % 10 == 1)&&(pdg<0)&&((pdgAbs/10)%1000>10)&&((pdgAbs/10)%1000<100));

}

bool CustomPDGParser::s_isS1Meson(int pdg)
{
 int pdgAbs=abs(pdg);
 return  ((pdgAbs/1000000 % 10 == 2)&&(pdg<0)&&((pdgAbs/10)%1000>0)&&((pdgAbs/10)%1000<10));

}

bool CustomPDGParser::s_isAntiS1Meson(int pdg)
{
 int pdgAbs=abs(pdg);
 return  ((pdgAbs/1000000 % 10 == 2)&&(pdg>0)&&((pdgAbs/10)%1000>0)&&((pdgAbs/10)%1000<10));

}

bool CustomPDGParser::s_isS1Baryon(int pdg)
{
 int pdgAbs=abs(pdg);
 /* std::cout<<"PDG: "<<pdgAbs<<std::endl;
 std::cout<<"% 10000: "<<pdgAbs % 10000<<std::endl;
 std::cout<<"% 10000 / 1000: "<<pdgAbs % 10000 / 1000<<std::endl;*/
 return ((pdgAbs/1000000 % 10 == 2)&&(pdg<0)&&((pdgAbs/10)%1000>10)&&((pdgAbs/10)%1000<100));
}

bool CustomPDGParser::s_isAntiS1Baryon(int pdg)
{
 int pdgAbs=abs(pdg);
 /* std::cout<<"PDG: "<<pdgAbs<<std::endl;
 std::cout<<"% 10000: "<<pdgAbs % 10000<<std::endl;
 std::cout<<"% 10000 / 1000: "<<pdgAbs % 10000 / 1000<<std::endl;*/
 return ((pdgAbs/1000000 % 10 == 2)&&(pdg>0)&&((pdgAbs/10)%1000>10)&&((pdgAbs/10)%1000<100));
}

double CustomPDGParser::s_charge(int pdg)
{
  
	//std::cout<<"Print: "<<std::endl;
  float sign=1;
  //int pdgAbs=abs(pdg);
  if(pdg > 0) sign=-1;
  int charge=sign*abs(pdg/10000%10);
	//std::cout<<"Print: "<<std::endl;
  if(s_isR1Meson(pdg) || s_isAntiS1Meson(pdg))
	{
	  
	  //std::cout<<"Print: "<<std::endl;
	  int quark = s_containedQuarks(pdg)[0];
	  //std::cout<<"Print: after charge"<<std::endl;
	  //charge = sign*abs(pdg/10000%10);
	  //std::cout<<"Print: before charge"<<charge<<std::endl;
	  if (quark==2) charge-=2; //ubar
	  if (quark%2!=0) charge+=1; //dbar, sbar
	  charge/=3;
	  //charge *= sign;
	  //std::cout<<"Print: after charge"<<charge<<std::endl;
	  return charge;
	}
	
	if(s_isS1Meson(pdg) || s_isAntiR1Meson(pdg))
	{
	  int quark = s_containedQuarks(pdg)[0];
	  //int charge = sign*abs(pdg/10000%10);
	  if (quark==2) charge+=2; //u
	  if (quark%2!=0) charge-=1; //d, s
	  charge/=3;
	  //charge *= sign;
	  return charge;
	}
	
  if(s_isR1Baryon(pdg) || s_isAntiS1Baryon(pdg))
	{
	  
	  //int quark = s_containedQuarksCode(pdg);
	  //int charge = sign*abs(pdg/10000%10); //scalar charge
	  for(int i=s_containedQuarksCode(pdg); i > 0; i /= 10) {
	    if ((i%10)%2!=0) charge-=1;
	    else charge+=2;
	  }
	  
	  charge/=3;
	  //charge*=sign;
	  return charge;
	}
	
	if(s_isS1Baryon(pdg) || s_isAntiR1Baryon(pdg))
	{
	  
	  //int quark = s_containedQuarksCode(pdg);
	  //int charge = sign*abs(pdg/10000%10); //scalar charge
	  for(int i=s_containedQuarksCode(pdg); i > 0; i /= 10) {
	    if ((i%10)%2!=0) charge+=1;
	    else charge-=2;
	  }
	  
	  charge/=3;
	  //charge*=sign;
	  return charge;
	}

return charge; 
}

double CustomPDGParser::s_spin(int pdg)
{
 int pdgAbs=abs(pdg);
 return pdgAbs % 10;    
}

 std::vector<int> CustomPDGParser::s_containedQuarks(int pdg)
{
 std::vector<int> quarks;
 //std::cout<<"Print: in containedQuarks: "<<s_containedQuarksCode(pdg)<<std::endl;
 for(int i=s_containedQuarksCode(pdg); i > 0; i /= 10) {
   quarks.push_back(i % 10);
 }
 //std::cout<<"Print: "<<quarks[0]<<std::endl;
 return quarks; 
}

 int CustomPDGParser::s_containedQuarksCode(int pdg)
{
 int pdgAbs=abs(pdg);
 //std::cout<<"Print: in QCode"<<pdgAbs<<std::endl;
 if(s_isR1Baryon(pdg) || s_isS1Baryon(pdg) || s_isAntiR1Baryon(pdg) || s_isAntiS1Baryon(pdg))
   return pdgAbs / 10 % 1000 % 100;

 if(s_isR1Meson(pdg) || s_isS1Meson(pdg) || s_isAntiR1Meson(pdg) || s_isAntiS1Meson(pdg))
   return pdgAbs / 10 % 1000 % 10;
  

return 0;
}
