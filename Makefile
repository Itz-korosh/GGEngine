# ===== GGEngine Makefile =====
# make              build (debug)
# make run          build and run
# make clean        delete bin/
# make RELEASE=1    optimized build

CXX    := g++
CC     := gcc
TARGET := bin/GGEngine.exe
OBJDIR := bin/obj

CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -Isrc -MMD -MP
CFLAGS   := -Wall -Iinclude -MMD -MP
LDFLAGS  := -Llib -lSDL3

ifdef RELEASE
  CXXFLAGS += -O2 -DNDEBUG
  CFLAGS   += -O2
else
  CXXFLAGS += -g
  CFLAGS   += -g
endif

# ---- Shell commands: plain Windows cmd vs. Unix-style shell ----
ifeq ($(OS),Windows_NT)
  ifeq ($(shell echo %OS%),Windows_NT)
    CMD_SHELL := 1
  endif
endif

ifdef CMD_SHELL
  fixpath = $(subst /,\,$1)
  MKDIR   = if not exist "$(call fixpath,$1)" mkdir "$(call fixpath,$1)"
  CP      = copy /Y "$(call fixpath,$1)" "$(call fixpath,$2)" >nul
  RMDIR   = if exist "$(call fixpath,$1)" rmdir /S /Q "$(call fixpath,$1)"
  RUNCMD  = $(call fixpath,$(TARGET))
else
  MKDIR   = mkdir -p "$1"
  CP      = cp "$1" "$2"
  RMDIR   = rm -rf "$1"
  RUNCMD  = ./$(TARGET)
endif

# ---- Sources: every .cpp and .c under src/ (any depth) ----
rwildcard = $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC_CPP := $(call rwildcard,src,*.cpp)
SRC_C   := $(call rwildcard,src,*.c)
OBJ     := $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRC_CPP)) \
           $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC_C))

# ---- DLLs: any .dll in dll/ or its subfolders is copied into bin/ ----
DLL_SRC := $(wildcard dll/*.dll dll/*/*.dll)
DLL_OUT := $(addprefix bin/,$(notdir $(DLL_SRC)))
vpath %.dll $(sort $(dir $(DLL_SRC)))

# ---- Rules ----
all: $(TARGET) $(DLL_OUT)

$(TARGET): $(OBJ)
	@$(call MKDIR,$(patsubst %/,%,$(dir $@)))
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: src/%.cpp
	@$(call MKDIR,$(patsubst %/,%,$(dir $@)))
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: src/%.c
	@$(call MKDIR,$(patsubst %/,%,$(dir $@)))
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.dll: %.dll
	@$(call MKDIR,bin)
	@$(call CP,$<,$@)

run: all
	$(RUNCMD)

clean:
	@$(call RMDIR,bin)

-include $(OBJ:.o=.d)

.PHONY: all run clean