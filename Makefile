INSTDIR=/usr/opt/spectcl/previous_current
#INSTDIR=/usr/opt/spectcl/4.0-007
include $(INSTDIR)/etc/SpecTcl_Makefile.include

#If you have any switches you need to add to the link add them below:
USERCXXFLAGS=-std=c++11 -I/usr/opt/ddas/3.0/include
USERCCFLAGS=$(USERCXXFLAGS)

USERLDFLAGS=-L/usr/opt/ddas/3.0/lib -lDDASUnpacker -lddasformat -Wl,-rpath=/usr/opt/ddas/3.0/lib

SOURCES=SpecTcl_ddas.cpp Unpacker_ddas.cpp ddaschannel.cpp Parameters_ddas.cpp

OBJECTS=$(SOURCES:%.cpp=%.o)

.PHONY: clean install depend help

%.o: ../src/%.cpp
	$(CXXLD) $(CXXFLAGS) $(USERCXXFLAGS) -c -o $@ $< 

SpecTcl: $(OBJECTS)
	$(CXXLD)  -o SpecTcl $(OBJECTS) $(USERLDFLAGS) \
	$(LDFLAGS)

clean:
	rm -f $(OBJECTS) SpecTcl 
depend:
	makedepend $(USERCXXFLAGS) *.cpp 
help:
	echo "make                 - Build customized SpecTcl"
	echo "make clean           - Remove objects from previous builds"
	echo "make depend          - Add dependencies to the Makefile. "
