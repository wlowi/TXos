
CXX = $(shell wx-config --cxx)
PROGRAM = TXosTest

# wx-config --libs
WX_LIBS = $(shell wx-config --libs)

# wx-config --cxxflags
WX_CXXFLAGS_GEN = $(shell wx-config --cxxflags)
WX_CXXFLAGS = $(subst -I,-isystem,$(WX_CXXFLAGS_GEN))

EMU_OBJ = emu/EEPROM.o emu/InputImpl.o emu/OutputImpl.o emu/PortsImpl.o emu/BuzzerImpl.o emu/LcdWidget.o emu/DisplayImpl.o

CONTROLS_OBJ = controls/Controls.o

UI_OBJ = ui/UserInterface.o ui/SelectList.o ui/Cell.o ui/Menu.o

OUTPUT_OBJ = output/Output.o output/Ports.o output/Buzzer.o

MODULE_OBJ = modules/ModelSelect.o modules/Model.o modules/EngineCut.o modules/ServoReverse.o modules/ServoSubtrim.o modules/ServoLimit.o \
  modules/SwitchMonitor.o modules/ServoMonitor.o modules/SystemSetup.o modules/CalibrateSticks.o modules/CalibrateTrim.o \
  modules/Bind.o modules/RangeTest.o modules/DualExpo.o modules/Phases.o modules/Timer.o modules/VccMonitor.o modules/PhasesTrim.o \
  modules/SwitchedChannels.o

CXXFLAGS += -g -O -Wall -Wpedantic -Wextra -Wno-unused-parameter

CXXINC += -I. -Iemu -Icontrols -Iui -Ioutput -Imodules

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
