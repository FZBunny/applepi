

BIN=../bin
OBJ=../obj
MAKE=make
MKDIR=mkdir -p
QTLIBS=`./getQt5LibDir`
QTINCDIR=`./getQt5IncDir`
CXXFLAGS=-fPIC -I ../src

#   For debugging with gdb, use "export DEBUG=1" before running 'make'.
ifdef DEBUG
  override CXXFLAGS += -O0 -ggdb3
endif


EXECS = $(BIN)/7bits  $(BIN)/create-printer-pins-xpm  $(BIN)/disassemble  $(BIN)/prodos  $(BIN)/video-40col-rom-to-xpm
SRC = .
VPATH=$(SRC)


all:
	$(MAKE) $(EXECS)

7bits:
	$(MAKE) 7bits.cpp $(BIN)/7bits

create-printer-pins-xpm:
	$(MAKE) $(BIN)/create-printer-pins-xpm

disassemble:
	$(MAKE) $(BIN)/disassemble

video:
	$(MAKE) $(BIN)/video-40col-rom-to-xpm

prodos:
	$(MAKE) $(BIN)/prodos


#$(OBJ)/%.o: %.cpp
#	$(COMPILE.cpp)  $(CXXFLAGS) $(QTINCS) $<
#	$(COMPILE.cpp)   $(QTINCS) $<

$(BIN)/%:
	$(LINK.cpp) $^ $(LDLIBS) -o $@
	@echo


# -- project targets -- #



$(BIN)/7bits: 7bits.cpp
	$(CXX) $(CXXFLAGS)  7bits.cpp  -o $(BIN)/7bits$(EXE)

$(BIN)/create-printer-pins-xpm: create-printer-pins-xpm.cpp
	$(CXX) $(CXXFLAGS)   create-printer-pins-xpm.cpp -o $(BIN)/create-printer-pins-xpm$(EXE)


$(BIN)/disassemble: $(OBJ)/disassembler.o disassemble.cpp
	$(CXX) $(CXXFLAGS)  $(SRC)/disassemble.cpp -I../src -I$(QTINCDIR) -I$(QTINCDIR)/QtGui -I$(QTINCDIR)/QtWidgets -I$(QTINCDIR)/QtGamepad -I$(QTINCDIR)/QtCore $(LDFLAGS) $(OBJ)/disassembler.o  -L$(QTLIBS) -lQt5Core -o $(BIN)/disassemble$(EXE)

$(BIN)/video-40col-rom-to-xpm: video-40col-rom-to-xpm.o
	$(CXX) $(CXXFLAGS)  $(SRC)/video-40col-rom-to-xpm.cpp  $(LDFLAGS)  -o $(BIN)/video-40col-rom-to-xpm$(EXE)

$(BIN)/prodos: prodos.o prodos.cpp prodos.h
	$(CXX) -Wno-address-of-packed-member $(CXXFLAGS) $(SRC)/prodos.cpp  $(LDFLAGS) $(WX_LDLIBS) $(OBJ)/debugging_dumps.o  -o $(BIN)/prodos$(EXE)



.PHONY: clean
clean:
	$(RM)  *.o $(EXECS) 

