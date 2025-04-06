CC := gcc
CFLAGS := -g -Wall -Wextra -Werror -Wno-unused-parameter
BINARY_DIR := $(BINARY_DIR_ROOT)/windows
OBJECT_DIR := $(OBJECTS_DIR_ROOT)/windows
EXE := $(BINARY_DIR)/$(NAME)


# Files
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
OBJECTS := $(addprefix $(OBJECT_DIR)/, $(notdir $(SOURCES:%.c=%.o)))

# Targets ---------------------------------------------------------------------

default: all

all: clean $(EXE)

build: $(EXE)

# Remove all of the build files
clean:
	@ if exist "./$(BINARY_DIR_ROOT)" rd /s /q $(BINARY_DIR_ROOT)
	@ if exist "./$(OBJECTS_DIR_ROOT)" rd /s /q $(OBJECTS_DIR_ROOT)

# Link the interpreter.
$(EXE): $(OBJECTS)
	@ echo * $(CC) $@ $(CFLAGS)
	@ if not exist "./$(BINARY_DIR)" mkdir "./$(BINARY_DIR)"
	@ $(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile object files.
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@ echo * $(CC) $< $(CFLAGS)
	@ if not exist "./$(OBJECT_DIR)" mkdir "./$(OBJECT_DIR)"
	@ $(CC) $(CFLAGS) -c -o $@ $<