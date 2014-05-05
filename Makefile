#################################
#	Compiler options	#
#################################

CC := gcc

mode ?= debug
ifeq ($(mode), release)
	# Release settings
	CFLAGS := -O2 -Wall -Wextra -pedantic
	LFLAGS := 
	INCDIR := -I./src/
	LIBDIR :=
	INCLIB :=
else
	# Debug settings
	CFLAGS := -O0 -g -Wall -Wextra -pedantic -D_DEBUG
	LFLAGS := 
	INCDIR := -I./src/
	LIBDIR :=
	INCLIB :=
endif

# Debug settings

#################################
#	Project options		#
#################################

PROJNAME := CowShell
SRCDIR := ./src
OUTDIR := ./bin
INTDIR := ./tmp


#################################
#	Target generation	#
#################################

# Recursive wildcard func
rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))

# List of source files for compilation
SRCFILES := $(call rwildcard, $(SRCDIR)/, *.c)

# List of compile targets
TARGETS := $(basename $(notdir $(SRCFILES)))


.PHONY : clean

all: info $(TARGETS)
	@echo Linking $(PROJNAME) . . .
	@$(CC) $(CFLAGS) $(LFLAGS) $(INCDIR) $(LIBDIR) -o $(OUTDIR)/$(PROJNAME) $(INTDIR)/*.o $(INCLIB)
	
% :
	@echo Compiling $@ . . .
	@$(CC) $(CFLAGS) $(INCDIR) -c $(shell echo $(SRCFILES) | grep -o -P "\S+/$@\.\S+") -o $(INTDIR)/$@.o

info:
	@echo Building in $(mode) mode . . .

list :
	@echo $(SRCFILES)

clean:
	@echo Cleaning project from intermediate and binary files . . .
	@rm -rf $(INTDIR)/* $(OUTDIR)/* 
	@echo Cleaning done.
