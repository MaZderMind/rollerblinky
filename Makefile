# Hey Emacs, this is a -*- makefile -*-

# AVR-GCC Makefile template, derived from the WinAVR template (which
# is public domain), believed to be neutral to any flavor of "make"
# (GNU make, BSD make, SysV make) - adapted to c++

MCU = atmega1280
#MCU = atmega328p
FORMAT = ihex
TARGET = rollerblinky
SRC = $(wildcard *.cpp)
ASRC =
OPT = 2

# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CXXSTANDARD = -std=c++11

# Place -D or -U options here
CXXDEFS = -DF_CPU=16000000

# Place -I options here
CXXINCS =


CXXDEBUG = -g$(DEBUG)
CXXWARN = -Wall -Wextra -Wpedantic
CXXTUNING = -fdiagnostics-color -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -finline-limit=3 -fno-inline-small-functions -fno-tree-scev-cprop -fno-split-wide-types -fno-jump-tables -fno-exceptions
LDTUNING = -Wl,--gc-sections,--relax,--print-gc-sections
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
CXXFLAGS = $(CXXTUNING) $(CXXDEBUG) $(CXXDEFS) $(CXXINCS) -O$(OPT) $(CXXWARN) $(CXXSTANDARD) $(CXXEXTRA)


#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 


#Additional libraries.

# Minimalistic printf version
#PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
#PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt
PRINTF_LIB =

# Minimalistic scanf version
#SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
#SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt
SCANF_LIB =

#MATH_LIB = -lm
MATH_LIB =

LDEXTRALIBS =
LDLIBS = $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB) $(LDEXTRALIBS)

# External memory options

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--defsym=__heap_start=0x801100,--defsym=__heap_end=0x80ffff
EXTMEMOPTS =

#LDMAP = $(LDFLAGS) -Wl,-Map=$(TARGET).map,--cref
LDFLAGS = $(EXTMEMOPTS) $(LDMAP) $(LDLIBS)


# Programming support using avrdude. Settings and variables.

AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = /dev/ttyUSB0

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep
AVRDUDE_WRITE_FUSE = -U lfuse:w:0xc0:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

ifeq ($(MCU), atmega1280)
  AVRDUDE_OPTIONS = -b 57600
else
  AVRDUDE_OPTIONS = -b 115200
endif

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE_COUNTER = -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_NO_VERIFY = -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude> 
# to submit bug reports.
#AVRDUDE_VERBOSE = -v -v

AVRDUDE_BASIC = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS = $(AVRDUDE_BASIC) $(AVRDUDE_NO_VERIFY) $(AVRDUDE_VERBOSE) $(AVRDUDE_ERASE_COUNTER) $(AVRDUDE_OPTIONS)


CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
MV = mv -f

# Define all object files.
OBJ = $(SRC:.cpp=.o) $(ASRC:.S=.o) 

# Define all listing files.
LST = $(ASRC:.S=.lst) $(SRC:.cpp=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)


# Default target.
#all: build doc
all: build

build: elf lss hex size

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym


# Program the device.
program: build
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

program-eep: $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)

program-fuse:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FUSE)


# Print the devices space usage
size: $(TARGET).elf
	$(AVRSIZE) --mcu=$(MCU) --format=avr $(TARGET).elf



terminal:
	picocom --baud 57600 --echo $(AVRDUDE_PORT)

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000 


coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).cof


.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@



# Link: create ELF output file from object files.
$(TARGET).elf: $(OBJ)
	$(CXX) $(ALL_CXXFLAGS) $(LDTUNING) $(OBJ) --output $@ $(LDFLAGS)


# Compile: create object files from C source files.
.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
.c.s:
	$(CXX) -S $(ALL_CXXFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CXX) -c $(ALL_ASFLAGS) $< -o $@



# Target: clean project.
clean:
	$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf \
	$(TARGET).map $(TARGET).sym $(TARGET).lss \
	$(OBJ) $(LST) $(SRC:.cpp=.s) $(SRC:.cpp=.d)
	$(REMOVEDIR) doc

depend:
	if grep '^# DO NOT DELETE' $(MAKEFILE) >/dev/null; \
	then \
		sed -e '/^# DO NOT DELETE/,$$d' $(MAKEFILE) > \
			$(MAKEFILE).$$$$ && \
		$(MV) $(MAKEFILE).$$$$ $(MAKEFILE); \
	fi
	echo '# DO NOT DELETE THIS LINE -- make depend depends on it.' \
		>> $(MAKEFILE); \
	$(CXX) -M -mmcu=$(MCU) $(CXXDEFS) $(CXXINCS) $(SRC) $(ASRC) >> $(MAKEFILE)

spaces:
	dos2unix *.cpp *.h
	sed -i 's/[ \t]*$$//g' *.cpp *.h

doc: *.cxx *.h
	doxygen >/dev/null

.PHONY:	all build elf hex eep lss sym program coff extcoff clean depend size spaces
