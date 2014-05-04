#################################
#	Compiler options	#
#################################

CC := gcc
CFLAGS := -O2 -Wall -Wextra -pedantic
LFLAGS := 
INCDIR := -I./src/
LIBDIR :=
INCLIB :=


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

all: $(TARGETS)
	@echo Linking $(PROJNAME) . . .
	@$(CC) $(CFLAGS) $(LFLAGS) $(INCDIR) $(LIBDIR) -o $(OUTDIR)/$(PROJNAME) $(INTDIR)/*.o $(INCLIB)

% :
	@echo Compiling $@ . . .
	@$(CC) $(CFLAGS) $(INCDIR) -c $(shell echo $(SRCFILES) | grep -o -P "\S+/$@\.\S+") -o $(INTDIR)/$@.o

list :
	@echo $(SRCFILES)

clean:
	@echo Cleaning project from intermediate and binary files . . .
	@rm -rf $(INTDIR)/* $(OUTDIR)/* 
	@echo Cleaning done.
