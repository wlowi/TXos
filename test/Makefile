
CXX = $(shell wx-config --cxx)
PROGRAM = TXosTest
# wx-config --libs
WX_LIBS = $(shell wx-config --libs)
# wx-config --cxxflags
WX_CXXFLAGS = $(shell wx-config --cxxflags)

CONTROLS_OBJ = controls/InputBox.o controls/Controls.o

UI_OBJ = ui/LcdWidget.o ui/DisplayBox.o

OUTPUT_OBJ = output/OutputBox.o output/Output.o

CXX += -I. -Icontrols -Iui -Ioutput

OBJECTS = $(PROGRAM).o TXos.o $(CONTROLS_OBJ) $(UI_OBJ) $(OUTPUT_OBJ)

# implementation

.SUFFIXES:      .o .cpp

.cpp.o:
	$(CXX) $(WX_CXXFLAGS) -c  -o $@ $<

all: $(PROGRAM)

$(PROGRAM):$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) $(WX_LIBS) 

clean:
	rm -f $(PROGRAM) $(OBJECTS)
