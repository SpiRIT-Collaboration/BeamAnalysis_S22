#----------! Written by C. Santamaria / CEA Saclay !----------
include ./Makefile.arch
OBJ =

#======= BEGIN CONFIGURATION =========

# TClonesArray variables
OBJ += BeamRaw.o
OBJ += BeamReco.o
OBJ += TBeamEnergy.o
OBJ += TBDCProjection.o

# #=======  END  CONFIGURATION =========

SOFLAG =  -shared

TARGET = libclass.so
HDR = $(OBJ:.o=.h)

DICTSRC = .libclass_dict.cc
DICTOBJ = $(DICTSRC:.cc=.o)
DICTHDR = $(DICTSRC:.cc=.h)

CXXFLAGS = -fPIC `root-config --cflags`
LDFLAGS = `root-config --libs`

all: $(TARGET)

$(TARGET): $(OBJ) $(DICTOBJ)
	@echo `uname`
	$(CXX) $(SOFLAG) -o $@ $^ $(CXFLAGS) $(LDFLAGS)

-include $(OBJ:.o=.d)

%.d: %.cc
	g++ -MM $(CXXFLAGS) $(LIBS) $*.cc > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

$(DICTSRC): $(HDR) linkdef.h
	rootcint -f $@ -c $^

clean:
	rm -f *.d *.o libclass.so $(DICTOBJ) $(DICTSRC) $(DICTHDR)
