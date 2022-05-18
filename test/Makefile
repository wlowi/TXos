
CXX = $(shell wx-config --cxx)
PROGRAM = TXosTest
# wx-config --libs
WX_LIBS = $(shell wx-config --libs)
# wx-config --cxxflags
WX_CXXFLAGS = $(shell wx-config --cxxflags)

EMU_OBJ = emu/EEPROM.o

CONTROLS_OBJ = controls/InputBox.o controls/Controls.o

UI_OBJ = ui/LcdWidget.o ui/DisplayBox.o ui/UserInterface.o ui/SelectList.o

OUTPUT_OBJ = output/OutputBox.o output/Output.o

MODULE_OBJ = modules/ServoReverse.o modules/ServoSubtrim.o modules/ServoLimit.o

CXX += -I. -Iemu -Icontrols -Iui -Ioutput -Imodules

OBJECTS = $(PROGRAM).o TXos.o Module.o ModuleManager.o ConfigBlock.o SystemConfig.o $(EMU_OBJ) $(CONTROLS_OBJ) $(UI_OBJ) $(OUTPUT_OBJ) $(MODULE_OBJ)

# implementation

.SUFFIXES:      .o .cpp

.cpp.o:
	$(CXX) $(WX_CXXFLAGS) -c  -o $@ $<

all: $(PROGRAM)

$(PROGRAM):$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) $(WX_LIBS) 

clean:
	rm -f $(PROGRAM) $(OBJECTS)
