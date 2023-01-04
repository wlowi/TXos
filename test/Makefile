
CXX = $(shell wx-config --cxx)
PROGRAM = TXosTest
UNITTEST = TXosUnitTest

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
  modules/SwitchedChannels.o modules/AssignInput.o modules/ServoRemap.o modules/AnalogSwitch.o modules/ChannelRange.o \
  modules/ChannelReverse.o modules/ChannelDelay.o modules/Mixer.o modules/Statistics.o modules/LogicSwitch.o modules/ModeAssign.o

CXXFLAGS += -g -O -Wall -Wpedantic -Wextra -Wno-unused-parameter

CXXINC += -I. -Icontrols -Iui -Ioutput -Imodules -Iemu

OBJECTS = TXos.o Module.o ModuleManager.o ConfigBlock.o SystemConfig.o $(CONTROLS_OBJ) $(UI_OBJ) $(OUTPUT_OBJ) $(MODULE_OBJ)

# Unittest
UTOBJECTS = unittest/UtModules.o
UTEMU_OBJ = unittest/emu/EEPROM.o unittest/emu/InputImpl.o unittest/emu/OutputImpl.o unittest/emu/PortsImpl.o \
  unittest/emu/BuzzerImpl.o unittest/emu/LcdWidget.o unittest/emu/DisplayImpl.o
UTCXXINC += -I. -Icontrols -Iui -Ioutput -Imodules -Iunittest -Iunittest/emu


# implementation

.SUFFIXES:      .o .cpp

ifeq ($(MAKECMDGOALS),unittest)
.cpp.o:
	$(CXX) -DUNITTEST $(CXXFLAGS) $(UTCXXINC) -c  -o $@ $<
else
.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINC) $(WX_CXXFLAGS) -c  -o $@ $<
endif

all: $(PROGRAM)

unittest: $(UNITTEST) 

$(PROGRAM):$(OBJECTS) $(EMU_OBJ) $(PROGRAM).o
	$(CXX) -g $(CXXINC) -o $(PROGRAM) $(PROGRAM).o $(OBJECTS) $(EMU_OBJ) $(WX_LIBS) 

$(UNITTEST):$(OBJECTS) $(UTOBJECTS) $(UTEMU_OBJ) $(UNITTEST).o 
	$(CXX) -g -DUNITTEST $(CXXINC) -o $(UNITTEST) $(UNITTEST).o $(OBJECTS) $(UTEMU_OBJ) $(UTOBJECTS)

clean:
	rm -f $(PROGRAM) $(PROGRAM).o $(UNITTEST) $(UNITTEST).o $(OBJECTS) $(EMU_OBJ) $(UTOBJECTS) $(UTEMU_OBJ)

$(UNITTEST).o: unittest/*.h

unittest/UtModules.o: unittest/*.h