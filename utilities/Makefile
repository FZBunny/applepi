

BIN=../bin
OBJ=../obj
MAKE=make
MKDIR=mkdir -p
QTLIBS=`./getQtLibDir`
QTINCDIR=-I `./getQt5IncDir`
QTINCS=$(QTINCDIR)/QtCore
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
	$(MAKE) $(BIN)/7bits

create-printer-pins-xpm:
	$(MAKE) $(BIN)/create-printer-pins-xpm

disassemble:
	$(MAKE) $(BIN)/disassemble

video:
	$(MAKE) $(BIN)/video-40col-rom-to-xpm

prodos:
	$(MAKE) $(BIN)/prodos


$(OBJ)/%.o: %.cpp
#	$(COMPILE.cpp)  $(CXXFLAGS) $(QTINCS) $<
	$(COMPILE.cpp)   $(QTINCS) $<

$(BIN)/%:
	$(LINK.cpp) $^ $(LDLIBS) -o $@
	@echo


# -- project targets -- #


$(BIN)/disassemble: $(OBJ)/disassemble.o $(OBJ)/disassembler.o
	$(CXX) $(CXXFLAGS)  $(SRC)/disassemble.cpp $(QTINCS) $(LDFLAGS) $(OBJ)/disassembler.o  $(LDLIBS) -o $(BIN)/disassemble$(EXE)

$(BIN)/video-40col-rom-to-xpm: $(OBJ)/video-40col-rom-to-xpm.o
	$(CXX) $(CXXFLAGS)  $(SRC)/video-40col-rom-to-xpm.cpp  $(LDFLAGS)  -o $(BIN)/video-40col-rom-to-xpm$(EXE)

$(BIN)/prodos: $(OBJ)/prodos.o $(SRC)/prodos.cpp $(SRC)/prodos.h
	$(CXX) -Wno-address-of-packed-member $(CXXFLAGS) $(SRC)/prodos.cpp  $(LDFLAGS) $(WX_LDLIBS) $(OBJ)/debugging_dumps.o  -o $(BIN)/prodos$(EXE)



.PHONY: clean
clean:
	$(RM)  $(OBJ)/*.d $(OBJ)/*.o $(BIN)/*

