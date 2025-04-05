CC := gcc
CFLAGS := -g
BINARY_DIR := $(BINARY_DIR_ROOT)/linux
OBJECT_DIR := $(OBJECTS_DIR_ROOT)/linux
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
	@ rm -rf $(BINARY_DIR_ROOT)
	@ rm -rf $(OBJECTS_DIR_ROOT)

# Link the interpreter.
$(EXE): $(OBJECTS)
	@ echo "* $(CC) $(CFLAGS) -o $@ $(OBJECTS)"
	@ mkdir -p "./$(BINARY_DIR)"
	@ $(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile object files.
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@ echo "* $(CC) $(CFLAGS) -c -o $@ $<"
	@ mkdir -p "./$(OBJECT_DIR)"
	@ $(CC) $(CFLAGS) -c -o $@ $<