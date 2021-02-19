# OpenCMSG4

We have used GEANT4 in this project to create a simplified version of the CMS calorimeters for the following purposes.
1. Fast generation of calorimetric data compared to the actual CMS build.
2. Basic tracker pixelation of data post processed.
3. Availaible to all compared to the actual simulation code which is for those inside the collaboration only.

The first version of this code contains the ECal only but very soon we will follow up with the HCal as well.

# Pre-requisites
## GEANT4
Please follow the [link](http://geant4.web.cern.ch/) to know more about the Geant4 package. The most recent download can be found [here](http://geant4.web.cern.ch/support/download) and the [installation instruction](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/index.html) as well as [user guide](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/index.html) can be found respectively. While installing Geant4 package, please keep mind to use the option **-DGEANT4_INSTALL_DATA=ON** to install pre-created data data files. More information about this can be found in the installation guide under the heading **Standard Options**.

The above code is based on the Geant4 libraries. The code has been built on version 
> 4.10.04 

and tested on 
> 4.10.05.

We will keep on testing with more versions and keep on adding them to our list.

## HepMC 2
This code only supports the version 2 implementation of HepMC reader. The tested version being 2.06.09. Please download it from [here](http://lcgapp.cern.ch/project/simu/HepMC/), unpack it and follow the instructions given in **INSTALL.cmake** to install it.

Define **HEPMC_DIR** path variable to its installation directory.

# Tested Linux Distribution
The following distributions have been tested on. If required, please mail us on the hypernews and we can test it for more distributions.

1. Ubuntu 16.04 LTS
  * GCC 5.4
  * GCC 4.8
2. Ubuntu 18.04 LTS
  * GCC 7.3
  
# Running the executable
```
git clone <branch_required>
mkdir build_DIR
cd build_DIR
cmake -DGeant4_DIR=$GEANT4 ../OpenCMSG4
make
./OpenCMSG4 (for visual mode)
./OpenCMSG4 myrun.txt (for batch mode)
```

# Pre-availaible commands

Apart the regular GEANT4 commands, the following general purpose commands have been added to facilitate use of the code with little or in cases no knowledge of the GEANT4 package. 

## Detector level
The run needs to be initialized everytime after these commands to enlist the changes you make to the detector. 
```
/detector/nonSensitiveMode <true/false>
```
This command enables or disables the non-Sensitive elements of the tracker. The non-Sensitive elements emulate the material budget of the tracker roughly along its variation through change in pseudo-rapidity.
```
/detector/trackerMode 111111
```
The tracker has six sections. Each of the six digit represents a section in the order <PIB|PID|TIB|TID|TOB|TEC>. The presence of each section can be controlled individually by using 1(0) to turn the respective section on(off). In conjunction with the above command, the absence of a tracker segment, implies the absence of its non-Senstive elements as well. 
1. PIB - Pixel Inner Barrel
2. PID - Pixel Inner Disk
3. TIB - Tracker Inner Barrel
4. TID - Tracker Inner Disk
5. TOB - Tracker Outer Barrel
6. TEC - Tracker End-Cap

```
/detector/ecalMode 111
```
The elctromagnetic calorimeter has three segments <LeftEndCap|Barrel|RightEndCap>. The presence(absence) of each segment can be controlled by using 1(0) e.g 010 would mean the presence of barrel only.

One has to use the command
```
/run/initialize
```
every time a change to the detector configuration is made.

## Gun Level
### Single Particle Gun

Change to the name of ROOT file can be made using the command

```
/root/setFileName exampleROOTfilename.root
```

### HepMC Event

## Output Level


# Acknowledgement

The authors of this code, A.R.Sahasransu and Dr. Ritesh Singh, acknowledge Prof. Satyaki Bhattacharya and Prof. Sunanda Banerjee for their support and occasional technical guidance in the project.
