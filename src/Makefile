#
#  TXos. A remote control transmitter OS.
#
#  MIT License
#
#  Copyright (c) 2023 wlowi
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#

CXX = $(shell wx-config --cxx)
PROGRAM = TXosTest
UNITTEST = TXosUnitTest

# wx-config --libs
WX_LIBS = $(shell wx-config --libs)

# wx-config --cxxflags
WX_CXXFLAGS_GEN = $(shell wx-config --cxxflags)
WX_CXXFLAGS = $(subst -I,-isystem,$(WX_CXXFLAGS_GEN))

## EMU_OBJ = emu/EEPROM.o emu/InputImpl.o emu/OutputImpl.o emu/PortsImpl.o emu/BuzzerImpl.o emu/LcdWidget.o emu/DisplayImpl.o
EMU_OBJ = emu/EEPROM.o emu/EmuSerial.o emu/SerialConsole.o emu/InputImpl.o emu/OutputImpl.o emu/PortsImpl.o emu/BuzzerImpl.o emu/EmuTextUILcdST7735.o emu/EmuTextUISimpleKbd.o emu/DisplayImpl.o

CONTROLS_OBJ = controls/Controls.o

UI_OBJ = TextUI/Cell.o TextUI/TextUI.o TextUI/TextUIHandler.o TextUI/TextUILcd.o TextUI/TextUIMenu.o

OUTPUT_OBJ = output/Output.o output/Ports.o output/Buzzer.o

MODULE_OBJ = modules/ModelSelect.o modules/Model.o modules/EngineCut.o modules/ServoReverse.o modules/ServoSubtrim.o modules/ServoLimit.o \
  modules/SwitchMonitor.o modules/ServoMonitor.o modules/CalibrateSticks.o modules/CalibrateTrim.o \
  modules/Bind.o modules/RangeTest.o modules/DualExpo.o modules/Phases.o modules/Timer.o modules/VccMonitor.o modules/PhasesTrim.o \
  modules/SwitchedChannels.o modules/AssignInput.o modules/ServoRemap.o modules/AnalogSwitch.o modules/ChannelRange.o \
  modules/ChannelReverse.o modules/ChannelDelay.o modules/Mixer.o modules/Statistics.o modules/LogicSwitch.o modules/ModeAssign.o \
  modules/AnalogTrim.o modules/ServoTest.o modules/ImportExport.o

CXXFLAGS += -g -O0 -Wall -Wpedantic -Wextra -Wno-unused-parameter

CXXINC += -I. -Icontrols -ITextUI -Ioutput -Imodules -Iemu

OBJECTS = TXos.o Module.o Comm.o ModuleManager.o ConfigBlock.o SystemConfig.o HomeScreen.o $(CONTROLS_OBJ) $(UI_OBJ) $(OUTPUT_OBJ) $(MODULE_OBJ)

# Unittest
UTOBJECTS = unittest/UtModules.o
UTEMU_OBJ = unittest/emu/EEPROM.o unittest/emu/EmuSerial.o unittest/emu/SerialConsole.o unittest/emu/InputImpl.o unittest/emu/OutputImpl.o unittest/emu/PortsImpl.o \
  unittest/emu/BuzzerImpl.o unittest/emu/EmuTextUILcdST7735.o unittest/emu/EmuTextUISimpleKbd.o unittest/emu/DisplayImpl.o
UTCXXINC += -I. -Icontrols -ITextUI -Ioutput -Imodules -Iunittest -Iunittest/emu

# implementation

.SUFFIXES:      .o .cpp

ifeq ($(MAKECMDGOALS),unittest)
.cpp.o:
	$(CXX) -DUNITTEST $(CXXFLAGS) $(UTCXXINC) -c  -o $@ $<
else
.cpp.o:
	$(CXX) -DEMULATION $(CXXFLAGS) $(CXXINC) $(WX_CXXFLAGS) -c  -o $@ $<
endif

all: $(PROGRAM)

unittest: $(UNITTEST) 

run:
	./$(PROGRAM)

$(PROGRAM):$(OBJECTS) $(EMU_OBJ) $(PROGRAM).o
	$(CXX) -g $(CXXINC) -o $(PROGRAM) $(PROGRAM).o $(OBJECTS) $(EMU_OBJ) $(WX_LIBS) 

$(UNITTEST):$(OBJECTS) $(UTOBJECTS) $(UTEMU_OBJ) $(UNITTEST).o 
	$(CXX) -g -DUNITTEST $(CXXINC) -o $(UNITTEST) $(UNITTEST).o $(OBJECTS) $(UTEMU_OBJ) $(UTOBJECTS)

clean:
	rm -f $(PROGRAM) $(PROGRAM).o $(UNITTEST) $(UNITTEST).o $(OBJECTS) $(EMU_OBJ) $(UTOBJECTS) $(UTEMU_OBJ)

$(UNITTEST).o: unittest/*.h

unittest/UtModules.o: unittest/*.h
