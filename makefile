NAME := tulac

SOURCE_DIR := src
BINARY_DIR := bin
OBJECT_DIR := obj
EXE := $(BINARY_DIR)/$(NAME)


CC := gcc
CFLAGS := -std=c99 							# Conform to the ISO 1999 C standard
CFLAGS += -Wall	 							# Enable most warning messages
CFLAGS += -Wextra							# Print extra (possibly unwanted) warnings

CFLAGS += -Wno-declaration-after-statement	# Ignored warnings when a declaration is found after a statement.

CFLAGS += -Werror=undef						# Error if an undefined macro is used in an #if directive.
CFLAGS += -Werror=shadow					# Error when one variable shadows another.
CFLAGS += -Werror=div-by-zero				# Error about compile-time integer division by zero.

CFLAGS += -Waggressive-loop-optimizations	# Warn if a loop with constant number of iterations triggers undefined behavior.

ifeq ($(TULA_MODE),DEBUG)
	CFLAGS += -DTULA_BUILD=0

	CFLAGS += -Wno-error=unused-but-set-parameter	  	# Warn when a function parameter is only set, otherwise unused
	CFLAGS += -Wno-error=unused-but-set-variable	  	# Warn when a variable is only set, otherwise unused.
	CFLAGS += -Wno-error=unused-const-variable	      	# Warn when a const variable is unused.
	CFLAGS += -Wno-error=unused-function	           	# Warn when a function is unused.
	CFLAGS += -Wno-error=unused-label	              	# Warn when a label is unused.
	CFLAGS += -Wno-error=unused-local-typedefs	     	# Warn when typedefs locally defined in a function are not used.
	CFLAGS += -Wno-error=unused-macros	             	# Warn about macros defined in the main file that are not used.
	CFLAGS += -Wno-error=unused-parameter	          	# Warn when a function parameter is unused.
	CFLAGS += -Wno-error=unused-value	              	# Warn when an expression value is unused.
	CFLAGS += -Wno-error=unused-variable	           	# Warn when a variable is unused.
	CFLAGS += -Wno-error=logical-op						# Warn when a logical operator is suspiciously always evaluating to true or false.
	CFLAGS += -Wno-error=absolute-value					# Warn on suspicious calls of standard functions computing absolute values.
	
endif

ifeq ($(TULA_MODE),RELEASE)
	CFLAGS += -DTULA_BUILD=1
	CFLAGS += -Werror						# Treat all warnings as errors
	
	CFLAGS += -Werror=unused-but-set-parameter	  	# Error when a function parameter is only set, otherwise unused
	CFLAGS += -Werror=unused-but-set-variable	  	# Error when a variable is only set, otherwise unused.
	CFLAGS += -Werror=unused-const-variable	      	# Error when a const variable is unused.
	CFLAGS += -Werror=unused-function	           	# Error when a function is unused.
	CFLAGS += -Werror=unused-label	              	# Error when a label is unused.
	CFLAGS += -Werror=unused-local-typedefs	     	# Error when typedefs locally defined in a function are not used.
	CFLAGS += -Werror=unused-macros	             	# Error about macros defined in the main file that are not used.
	CFLAGS += -Werror=unused-parameter	          	# Error when a function parameter is unused.
	CFLAGS += -Werror=unused-value	              	# Error when an expression value is unused.
	CFLAGS += -Werror=unused-variable	           	# Error when a variable is unused.
	CFLAGS += -Werror=logical-op					# Error when a logical operator is suspiciously always evaluating to true or false.
	CFLAGS += -Werror=absolute-value				# Error on suspicious calls of standard functions computing absolute values.
endif


# Files
SOURCES := $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(SOURCE_DIR)/**/*.c)
HEADERS := $(wildcard $(SOURCE_DIR)/*.h) $(wildcard $(SOURCE_DIR)/**/*.h)
OBJECTS := $(addprefix $(OBJECT_DIR)/, $(subst $(SOURCE_DIR)/, , $(SOURCES:%.c=%.o)))


# Targets ---------------------------------------------------------------------

default: all

all: clean $(EXE)

build: $(EXE)

# Remove all of the build files
clean:
	@ rm -rf $(BINARY_DIR)
	@ rm -rf $(OBJECT_DIR)

# Link the interpreter.
$(EXE): $(OBJECTS)
	@ echo "*" $(CC) -o $@ $(OBJECTS) $(CFLAGS)
	@ mkdir -p "./$(BINARY_DIR)"
	@ $(CC) -o $@ $(OBJECTS) $(CFLAGS)

# Compile object files.
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@ echo "*" $(CC) -c -o $@ $< $(CFLAGS)
	@ mkdir -p $(addprefix ./, $(dir $(OBJECTS)))
	@ $(CC) -c -o $@ $< $(CFLAGS)