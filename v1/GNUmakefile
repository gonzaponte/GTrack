# SoftRelTools GNUmakefile for GTrack top level

# Compile the files in xml subdirectory
SUBDIRS=xml

LIB_TYPE := shared
LIB := lib$(PACKAGE)
LIBCXXFILES := $(wildcard *.cxx)

################################

include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_fmwk.mk
include SoftRelTools/arch_spec_xercesc.mk
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_bhep.mk

