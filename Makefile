# ===== GGEngine Makefile =====
# make              demo app (src/Main.cpp) linked against the static library
# make lib          static library -> bin/lib/libGGEngine.a
# make dll          shared library -> bin/GGEngine.dll + bin/lib/libGGEngine.dll.a
# make libs         both libraries
# make app-dll      demo app linked against the DLL -> bin/GGEngine_dll.exe
# make run          build and run the demo
# make clean        delete bin/
# make RELEASE=1    optimized build (add to any of the above)

CXX := g++
CC  := gcc
OBJDIR := bin/obj

TARGET     := bin/GGEngine.exe
TARGET_DLL := bin/GGEngine_dll.exe
STATIC_LIB := bin/lib/libGGEngine.a
SHARED_DLL := bin/GGEngine.dll
IMPORT_LIB := bin/lib/libGGEngine.dll.a
APP_SRC    := src/Main.cpp

CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -Isrc -MMD -MP
CFLAGS   := -Wall -Iinclude -Iinclude/glad -MMD -MP
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
  RM      = if exist "$(call fixpath,$1)" del /Q "$(call fixpath,$1)"
  RMDIR   = if exist "$(call fixpath,$1)" rmdir /S /Q "$(call fixpath,$1)"
  RUNCMD  = $(call fixpath,$(TARGET))
else
  MKDIR   = mkdir -p "$1"
  CP      = cp "$1" "$2"
  RM      = rm -f "$1"
  RMDIR   = rm -rf "$1"
  RUNCMD  = ./$(TARGET)
endif

# ---- Sources: every .cpp and .c under src/ (any depth) ----
rwildcard = $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC_CPP := $(call rwildcard,src,*.cpp)
SRC_C   := $(call rwildcard,src,*.c)

# The engine = everything except the demo's Main.cpp
LIB_SRC_CPP := $(filter-out $(APP_SRC),$(SRC_CPP))
LIB_OBJ := $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(LIB_SRC_CPP)) \
           $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC_C))
APP_OBJ := $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(APP_SRC))

# ---- DLLs: any .dll in dll/ or its subfolders is copied into bin/ ----
DLL_SRC := $(wildcard dll/*.dll dll/*/*.dll)
DLL_OUT := $(addprefix bin/,$(notdir $(DLL_SRC)))
vpath %.dll $(sort $(dir $(DLL_SRC)))

# ---- Goals ----
all: $(TARGET) $(DLL_OUT)
lib: $(STATIC_LIB)
dll: $(SHARED_DLL) $(IMPORT_LIB) $(DLL_OUT)
libs: lib dll
app-dll: $(TARGET_DLL) $(DLL_OUT)

# ---- Static library (.a) ----
$(STATIC_LIB): $(LIB_OBJ)
	@$(call MKDIR,bin/lib)
	@$(call RM,$@)
	ar rcs $@ $^

# ---- Shared library (.dll + import library .dll.a) ----
$(SHARED_DLL): $(LIB_OBJ)
	@$(call MKDIR,bin/lib)
	$(CXX) -shared $^ -o $@ -Wl,--out-implib,$(IMPORT_LIB) $(LDFLAGS)

$(IMPORT_LIB): $(SHARED_DLL)

# ---- Demo app ----
$(TARGET): $(APP_OBJ) $(STATIC_LIB)
	@$(call MKDIR,bin)
	$(CXX) $(APP_OBJ) $(STATIC_LIB) -o $@ $(LDFLAGS)

$(TARGET_DLL): $(APP_OBJ) $(SHARED_DLL) $(IMPORT_LIB)
	@$(call MKDIR,bin)
	$(CXX) $(APP_OBJ) $(IMPORT_LIB) -o $@

# ---- Compiling ----
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

-include $(LIB_OBJ:.o=.d) $(APP_OBJ:.o=.d)

.PHONY: all lib dll libs app-dll run clean