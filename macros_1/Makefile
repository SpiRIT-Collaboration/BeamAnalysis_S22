#
# Makefile for ROOT and ANAROOT environment
#

COMPILER		= g++
CXXFLAGS		= -g -O2 -pipe -Wall -Wextra -fPIC

ROOTCFLAGS	:= $(shell root-config --cflags)
ROOTLIBS		:= $(shell root-config --libs) -lMinuit
ROOTGLIBS		:= $(shell root-config --glibs) # for the use of GUI

ANAROOTDIR		= /home/s015/svn/2016SAMURAI.s022
ANAROOTFLAGS	= -I$(ANAROOTDIR)/include
ANAROOTLIBS		= -L$(ANAROOTDIR)/lib -lanaroot -lananadeko -lanabrips -lanasamurai -lanaloop -lanacore 

CXXFLAGS		+= $(ROOTCFLAGS) $(ANAROOTFLAGS)
CXXLIBS			= $(ROOTLIBS) $(ANAROOTLIBS) $(SYSLIBS) -lXMLParser


TARGET1		= RIDFtoPPACRAW
TARGET2		= RIDFtoPLARAW
SRCS1			= $(TARGET1).cc
OBJS1			:= $(SRCS1:.cc=.o)
SRCS2			= $(TARGET2).cc
OBJS2			:= $(SRCS2:.cc=.o)

all: $(TARGET1) $(TARGET2)


$(TARGET1): $(OBJS1)
		$(COMPILER) $(CXXLIBS) $(OBJS1) -o $@

$(TARGET2): $(OBJS2)
		$(COMPILER) $(CXXLIBS) $(OBJS2) -o $@

.cc.o:
		$(COMPILER) $(CXXFLAGS) -c $<

clean:
		rm -f $(TARGET1) $(OBJS1) $(TARGET2) $(OBJS2)
