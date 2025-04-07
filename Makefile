#=============================================================================#
#                                                                             #
# Kokeshi makefile                                                            #
#                                                                             #
#=============================================================================#



#=============================================================================#
# Configuration                                                               #
#=============================================================================#

# Game(s) that your mod should build for.
# By default, we try building for Wii Sports.
#
# You can build for multiple games by writing multiple pack names, separated
# by spaces.
#
# To handle this in your code, expect preprocessor definitions of the format
# PACK_{NAME}, (i.e. PACK_SPORTS, PACK_RESORT). 
PACK ?= resort

# Pass NDEBUG=1 when running make to disable debug features.
#
# By default, we build for debug. There is not much benefit to building for
# release except maybe security, or if you have something REALLY slowing down
# debug builds.
#
# NOTE: This will disable some features in libkiwi such as the mapfile and
# exception handler.
#
# (This also will define "NDEBUG" when compiling your code too.)
NDEBUG ?= 0

# Extra compiler flags to apply across all code.
# NOTE: See 'compile.py' (CFLAGS_COMMON) to see what is already being applied. 
CFLAGS := -maxerrors 1

# Pass CI=1 when running through GitHub CI to perform only code compilation.
CI ?= 0

#=============================================================================#
# Variables                                                                   #
#=============================================================================#
# Default to debug target unless NDEBUG is set
TARGET := debug
ifeq ($(NDEBUG), 1)
	TARGET := release
endif

# Verbose output for debugging (optional)
VERBOSE ?= 0
ifeq ($(VERBOSE), 0)
	QUIET = @
endif


#==============================================================================#
# Tools                                                                        #
#==============================================================================#
PYTHON  := python

ASSETSCRIPT := tools/asset.py
BUILDSCRIPT := tools/compile.py
CLEANSCRIPT := tools/clean.py

#==============================================================================#
# Default Targets                                                              #
#==============================================================================#
default: all
all: build


#==============================================================================#
# Build                                                                        #
#==============================================================================#
.PHONY: build
build:
# Build the mod for each game
#
# NOTE: If you want to add custom preprocessor definitions, supply each using
# the following format: --define=KEY VALUE
	$(QUIET) $(foreach game, $(PACK), \
		$(PYTHON) $(BUILDSCRIPT) --game=$(game) --target=$(TARGET) --cflags="$(CFLAGS)" --define="" --ci=$(CI); \
		$(PYTHON) $(ASSETSCRIPT) --game=$(game) --ci=$(CI); \
	)

#==============================================================================#
# Clean                                                                        #
#==============================================================================#
.PHONY: clean
clean:
	$(QUIET) $(PYTHON) $(CLEANSCRIPT)
