# ===== USER SETTINGS =========================================================

PLATFORM = guess

C_STANDARD = 89

CC = gcc
CFLAGS = -Wall -Wextra -pedantic $(SYSCFLAGS)

UNAME = uname

RM = rm -rf
MKDIR = mkdir -p


# ===== MAKE VARS =============================================================

OUTPUT_NAME = tulac

PLATFORMS = guess windows macosx linux
CSTD = c$(C_STANDARD)

SOURCE_DIR = src
BINARY_DIR = bin
OBJECT_DIR = obj

SOURCES = $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(SOURCE_DIR)/**/*.c)
HEADERS = $(wildcard $(SOURCE_DIR)/*.h) $(wildcard $(SOURCE_DIR)/**/*.h)
OBJECTS = $(addprefix $(OBJECT_DIR)/, $(subst $(SOURCE_DIR)/, , $(SOURCES:%.c=%.o)))

OUTPUT_FILE = $(BINARY_DIR)/$(OUTPUT_NAME)

ALL = all

# ===== TARGETS ===============================================================

default: $(PLATFORM)

echo:
	@ echo "MAKE = $(MAKE)"
	@ echo "UNAME = $(UNAME)"
	@ echo "PLATFORM = $(PLATFORM)"
	@ echo "PLATFORMS = $(PLATFORMS)"
	@ echo "RM = $(RM)"
	@ echo "MKDIR = $(MKDIR)"
	@ echo "C_STANDARD = $(C_STANDARD)"
	@ echo "CSTD = $(CSTD)"
	@ echo "CC = $(CC)"


all: clean $(OUTPUT_FILE)


# Remove all of the build files
clean:
	@ $(RM) $(BINARY_DIR)
	@ $(RM) $(OBJECT_DIR)


# Link the interpreter.
$(OUTPUT_FILE): $(OBJECTS)
	@ echo "*" $(CC) -o $@ $(OBJECTS) $(CFLAGS)
	@ $(MKDIR) "./$(BINARY_DIR)"
	@ $(CC) -o $@ $(OBJECTS) $(CFLAGS)


# Compile object files.
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@ echo "*" $(CC) -c -o $@ $< $(CFLAGS)
	@ $(MKDIR) $(addprefix ./, $(dir $(OBJECTS)))
	@ $(CC) -c -o $@ $< $(CFLAGS)


guess:
	@ echo "*" Guessing `$(UNAME)`
	@ $(MAKE) `$(UNAME)`


CYGWIN_NT-10.0-22631 windows:
	@ echo "*" $(MAKE) $(ALL) SYSCFLAGS=-DTULA_OS_WINDOWS SYSCFLAGS+=-DCSTD=89
	@ $(MAKE) $(ALL) SYSCFLAGS=-DTULA_OS_WINDOWS SYSCFLAGS+=-DCSTD=89


Darwin macos macosx:
	@ echo "*" $(MAKE) $(MAKE) $(ALL) SYSCFLAGS="-DTULA_OS_MAC"
	@ $(MAKE) $(ALL) SYSCFLAGS="-DTULA_OS_MAC"


Linux linux:
	@ echo "*" $(MAKE) $(ALL) SYSCFLAGS="-DTULA_OS_LINUX"
	@ $(MAKE) $(ALL) SYSCFLAGS="-DTULA_OS_LINUX"
