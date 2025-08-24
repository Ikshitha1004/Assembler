CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -g
SRCDIR := src
BINDIR := bin
TARGET := $(BINDIR)/assembler
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BINDIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR) *.o

.PHONY: all clean dirs
