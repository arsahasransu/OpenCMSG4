# $Id: GNUmakefile 76474 2013-11-11 10:36:34Z gcosmo $
# --------------------------------------------------------------
# GNUmakefile for examples module
# --------------------------------------------------------------

name := OpenCMSG4
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../../../..
endif


.PHONY: all


ifdef HEPMC_DIR
all : lib bin

  include $(G4INSTALL)/config/binmake.gmk

  INCFLAGS  += -I$(HEPMC_DIR)/include

  LDLIBS1  += -L$(HEPMC_DIR)/lib -lHepMC

FCFLAGS += -c


else

all:
	@echo 'ERROR - HEPMC_DIR not defined in the environment !'
	@echo '        Requires HepMC release 1.27.'
endif


visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
