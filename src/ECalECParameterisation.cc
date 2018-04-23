#include "ECalECParameterisation.hh"
#include "ECalConstants.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>
#include <fstream>

#define PI 3.1415926

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalECParameterisation::ECalECParameterisation()
: G4VPVParameterisation()
{
	G4int i,j,k = 0;
	G4double rmin = 3205*tan(2*atan(exp(-3)));
	G4double rmax = 3205*tan(2*atan(exp(-1.479)));
//	std::array<std::array<G4bool, 10>, 10> flag;
/*	for(i=0; i<10; i++)
	for(j=0; j<10; j++)
	{
		G4double dbb = sqrt(pow(25.9*6*(i),2)+pow(25.9*6*(j),2));
		G4double dbt = sqrt(pow(25.9*6*(i),2)+pow(25.9*6*(j+1),2));
		G4double dtb = sqrt(pow(25.9*6*(i+1),2)+pow(25.9*6*(j),2));
		G4double dtt = sqrt(pow(25.9*6*(i+1),2)+pow(25.9*6*(j+1),2));
		if(dbb<rmin||dbt<rmin||dtb<rmin||dtt<rmin||dbb>rmax||dbt>rmax||dtb>rmax||dtt>rmax)
		flag[i][j] = true;
		else
		flag[i][j] = false;
	}
*/
//	std::ofstream out;
//	out.open("Cell_Data.dat",std::ofstream::out|std::ofstream::app);

	for(i=-60; i<60; i++)
	for(j=-60; j<60; j++)
	if(k<kNofEmECCells)
	{
		G4double d1 = sqrt(pow(25.9*(i+0.5),2)+pow(25.9*(j+0.5),2));
//		G4double d2 = sqrt(pow(25.9*(i),2)+pow(25.9*(j),2));
		if(d1>rmin && d1<rmax)
		{
			xpos[k] = 25.9*(i+0.5);
			ypos[k] = 25.9*(j+0.5);
			cell_pos[k] = {i,j};
//			out<<k<<"\t"<<i<<"\t"<<j<<std::endl;
			k++;
		}//std::cout<<i<<" "<<j<<" "<<k<<" "<<rmin<<" "<<"1560.0"<<" "<<d1<<std::endl;
	}
//	out.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalECParameterisation::~ECalECParameterisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalECParameterisation::ComputeTransformation(
       const G4int copyNo, G4VPhysicalVolume* cellEcalEC) const
{	
	cellEcalEC->SetTranslation(G4ThreeVector(xpos[copyNo]*mm, ypos[copyNo]*mm, 0));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ECalECParameterisation::ComputeDimensions (G4Trap& cellEcalEC, const G4int copyNo,
                            const G4VPhysicalVolume* phys) const
{
	G4double y1=24.7, y2=25.9, len=220.0;
	G4ThreeVector* d = new G4ThreeVector(xpos[copyNo], ypos[copyNo], (3205+110+1300));

	if(phys->GetName()=="ecalECPhysical")
	{
		cellEcalEC.SetAllParameters(0.5*len*mm, d->getTheta()*rad,
	             d->getPhi()*rad,  0.5*y1*mm,
	             0.5*y1*mm, 0.5*y1*mm,
	             0*deg, 0.5*y2*mm,
	             0.5*y2*mm, 0.5*y2*mm,
	             0*deg);
	}
	else
	{
		d = new G4ThreeVector(xpos[copyNo], ypos[copyNo], -(3205+110+1300));
		cellEcalEC.SetAllParameters(0.5*len*mm, d->getTheta()*rad,
	             d->getPhi()*rad,  0.5*y2*mm,
	             0.5*y2*mm, 0.5*y2*mm,
	             0*deg, 0.5*y1*mm,
	             0.5*y1*mm, 0.5*y1*mm,
	             0*deg);
	}
}
