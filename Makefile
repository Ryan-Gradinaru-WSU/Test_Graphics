# Compiler
CXX := g++

# SDL3 flags
SDL_CFLAGS := $(shell pkg-config sdl3 --cflags)
SDL_LIBS   := $(shell pkg-config sdl3 --libs)

# Source & include roots
SRC_ROOT := src
INC_ROOT := headers

# Recursively find all include directories
INC_DIRS := $(shell find $(INC_ROOT) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Recursively find all .cpp files
SRC_FILES := $(shell find $(SRC_ROOT) -name '*.cpp')

# Object files (mirror directory structure)
OBJDIR := obj
OBJ_FILES := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC_FILES))

# Output
BINDIR := bin
TARGET := $(BINDIR)/app

# Compile flags (NO libraries here)
CXXFLAGS := -std=c++17 -Wall -Wextra $(SDL_CFLAGS) $(INC_FLAGS)

# Link flags
LDFLAGS := $(SDL_LIBS) -lGL -ldl

# Default target
all: setup $(TARGET)

# Create directories
setup:
	mkdir -p $(OBJDIR) $(BINDIR)

# Link
$(TARGET): $(OBJ_FILES)
	@echo "Linking $(TARGET)"
	@$(CXX) $^ -o $@ $(LDFLAGS)

# Compile rule (auto-create subdirs)
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean setup
