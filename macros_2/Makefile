#
# Makefile for ROOT and ANAROOT environment
#

COMPILER		= g++
CXXFLAGS		= -g -O2 -pipe -Wall -Wextra -fPIC

ROOTCFLAGS	:= $(shell root-config --cflags)
ROOTLIBS		:= $(shell root-config --libs) -lMinuit
ROOTGLIBS		:= $(shell root-config --glibs) # for the use of GUI

ANAROOTDIR		= /home/s015/svn/2016SAMURAI.s015
ANAROOTFLAGS	= -I$(ANAROOTDIR)/include
ANAROOTLIBS		= -L$(ANAROOTDIR)/lib -lanaroot -lananadeko -lanabrips -lanasamurai -lanaloop -lanacore 

CXXFLAGS		+= $(ROOTCFLAGS) $(ANAROOTFLAGS)
CXXLIBS			= $(ROOTLIBS) $(ANAROOTLIBS) $(SYSLIBS) -lXMLParser


TARGET2		= RIDFtoRAWROOT_S22
SRCS2			= $(TARGET2).cc
OBJS2			:= $(SRCS2:.cc=.o)

all: $(TARGET2)

$(TARGET2): $(OBJS2)
		$(COMPILER) $(CXXLIBS) $(OBJS2) -o $@

.cc.o:
		$(COMPILER) $(CXXFLAGS) -c $<

clean:
		rm -f $(TARGET2) $(OBJS2)
