.RECIPEPREFIX != ps

CXX := g++ # This is the main compiler
SRCDIR := src
BUILDDIR := build
TARGET := bin/template

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
IGNORE_WARNINGS := #-Wint-to-pointer-cast -Wunused-variable -Wreturn-type
CFLAGS := -g -std=c++11 $(IGNORE_WARNINGS) # -Wall
LIB := -L lib -lSDL2 -lGL
INC := -I include

$(TARGET): $(OBJECTS)
    @echo " Linking..."
    @echo " $(CXX) $^ -o $(TARGET) $(LIB)"; $(CXX) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
    @mkdir -p $(BUILDDIR)
    @echo " $(CXX) $(CFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CFLAGS) $(INC) -c -o $@ $<

switch: scripts/generateSwitch.py
    python scripts/generateSwitch.py code > include/opcodeSwitch.cpp
    python scripts/generateSwitch.py state > include/stateInfoSwitch.cpp
    python scripts/generateSwitch.py disassemble > include/disassembleSwitch.cpp

clean:
    @echo " Cleaning..."
    @echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester:
    $(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

.PHONY: clean
