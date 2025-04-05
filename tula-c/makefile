NAME := tulac
SOURCE_DIR := src
BINARY_DIR_ROOT := bin
OBJECTS_DIR_ROOT := obj

ifeq ($(MAKE),make)
	include utils/linux.mk
else ifeq ($(findstring mingw32-make,$(MAKE)),mingw32-make)
	include utils/windows.mk
endif
