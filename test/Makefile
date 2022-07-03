
CXX = $(shell wx-config --cxx)
PROGRAM = TXosTest

# wx-config --libs
WX_LIBS = $(shell wx-config --libs)

# wx-config --cxxflags
WX_CXXFLAGS_GEN = $(shell wx-config --cxxflags)
WX_CXXFLAGS = $(subst -I,-isystem,$(WX_CXXFLAGS_GEN))

EMU_OBJ = emu/EEPROM.o emu/InputImpl.o emu/OutputImpl.o emu/LcdWidget.o emu/DisplayImpl.o

CONTROLS_OBJ = controls/Controls.o

UI_OBJ = ui/UserInterface.o ui/SelectList.o ui/Cell.o

OUTPUT_OBJ = output/Output.o

MODULE_OBJ = modules/ModelSelect.o modules/Model.o modules/EngineCut.o modules/ServoReverse.o modules/ServoSubtrim.o modules/ServoLimit.o

CXXFLAGS += -g -O -Wall -Wpedantic -Wextra -Wno-unused-parameter

CXXINC += -I. -Iemu -Icontrols -Iui -Ioutput -Imodules

#-Weffc++ -pedantic  \
#-pedantic-errors -Wextra -Waggregate-return -Wcast-align \
#-Wcast-qual -Wconversion \
#-Wdisabled-optimization \
#-Werror -Wfloat-equal -Wformat=2 \
#-Wformat-nonliteral -Wformat-security  \
#-Wformat-y2k \
#-Wimplicit  -Wimport  -Winit-self  -Winline \
#-Winvalid-pch   \
#-Wlong-long \
#-Wmissing-field-initializers -Wmissing-format-attribute   \
#-Wmissing-include-dirs -Wmissing-noreturn \
#-Wpacked  -Wpadded -Wpointer-arith \
#-Wredundant-decls \
#-Wshadow -Wstack-protector \
#-Wstrict-aliasing=2 -Wswitch-default \
#-Wswitch-enum \
#-Wunreachable-code -Wunused \
#-Wunused-parameter \
#-Wvariadic-macros \
#-Wwrite-strings

OBJECTS = $(PROGRAM).o TXos.o Module.o ModuleManager.o ConfigBlock.o SystemConfig.o $(EMU_OBJ) $(CONTROLS_OBJ) $(UI_OBJ) $(OUTPUT_OBJ) $(MODULE_OBJ)

# implementation

.SUFFIXES:      .o .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINC) $(WX_CXXFLAGS) -c  -o $@ $<

all: $(PROGRAM)

$(PROGRAM):$(OBJECTS)
	$(CXX) -g $(CXXINC) -o $(PROGRAM) $(OBJECTS) $(WX_LIBS) 

clean:
	rm -f $(PROGRAM) $(OBJECTS)
