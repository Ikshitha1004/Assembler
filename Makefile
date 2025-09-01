##Makefile for Linux/Mac
# CXX := g++
# CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -g
# SRCDIR := src
# BINDIR := bin
# TARGET := $(BINDIR)/assembler
# SOURCES := $(wildcard $(SRCDIR)/*.cpp)
# OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

# all: dirs $(TARGET)

# dirs:
# 	@mkdir -p $(BINDIR)

# $(TARGET): $(OBJECTS)
# 	$(CXX) $(CXXFLAGS) -o $@ $^

# $(BINDIR)/%.o: $(SRCDIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -rf $(BINDIR) *.o

# .PHONY: all clean dirs

##Windows Like Makefile

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -g
SRCDIR := src
BINDIR := bin
TARGET := $(BINDIR)/assembler.exe
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

all: dirs $(TARGET)

dirs:
	@if not exist $(BINDIR) mkdir $(BINDIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@if exist $(BINDIR) rmdir /S /Q $(BINDIR)
	@if exist *.o del /Q *.o

.PHONY: all clean dirs