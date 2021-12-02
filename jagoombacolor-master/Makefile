MAKEFILE	:=	makefile

#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif


include $(DEVKITARM)/gba_rules

#--------
# Overrides for default rules

#%.o: %.s
#	@echo $(notdir $<)
#	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -x assembler-with-cpp $(ASFLAGS) -c $< -o $@
#
#%.o: %.S
#	@echo $(notdir $<)
#	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -x assembler-with-cpp $(ASFLAGS) -c $< -o $@

#OBJCOPY		:=	$(OBJCOPY) -R.pad

# We override the specs file because the default crt0 always clears EWRAM, and we don't want this.
# when DevKitArm changes the specs files, change the copy in the project too!

%_mb.elf:
	@echo linking multiboot CUSTOM
	@$(LD) $(LDFLAGS) -specs=../src/gba_mb_my.specs $(OFILES) $(LIBPATHS) $(LIBS) -o $@
%.elf:
	@echo linking cartridge CUSTOM
	@$(LD)  $(LDFLAGS) -specs=../src/gba_my.specs $(OFILES) $(LIBPATHS) $(LIBS) -o $@

#-------



#---------------------------------------------------------------------------------
# TARGET is the name of the output, if this ends with _mb generates a multiboot image
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	jagoombacolor
BUILD		:=	build
SOURCES		:=	src
INCLUDES	:=

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	:=	-g -Wall -Os\
			-mcpu=arm7tdmi -mtune=arm7tdmi\
 			-fomit-frame-pointer\
			-ffast-math \
			-std=c99 \
			$(ARCH)

CFLAGS	+=	$(INCLUDE)

CXXFLAGS := $(CFLAGS) \
 			-fno-rtti -fno-exceptions

ASFLAGS	:=	$(ARCH)
LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $@).map

#---------------------------------------------------------------------------------
# path to tools - this can be deleted if you set the path in windows
#---------------------------------------------------------------------------------
export PATH		:=	$(DEVKITARM)/bin:$(PATH)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:=	-lgba

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBGBA)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export PATH	:=	$(DEVKITARM)/bin:$(PATH)
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	all.s
DATA1		:=	font.lz77
DATA2		:=	fontpal.bin
DATA3		:=	#../goomba_mb.gba

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:= $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o) $(DATA1:.lz77=.o) $(DATA2:.bin=.o) $(DATA3:.gba=.o)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/$(MAKEFILE)

all	: $(BUILD)
#---------------------------------------------------------------------------------
semiclean:
	@echo deleting intermediate files...
	@rm -fr $(BUILD) $(TARGET).elf

clean: semiclean
	@echo deleting main binary
	@rm -f $(TARGET).gba

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

%.o	:	%.lz77
	@echo $(notdir $<)
	@$(bin2o)

%.o	:	%.bin
	@echo $(notdir $<)
	@$(bin2o)

%.o	:	%.gba
	@echo $(notdir $<)
	@$(bin2o)


#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).gba	:	$(OUTPUT).elf

$(OUTPUT).elf	:	$(OFILES)	gba_crt0_my.o

-include $(DEPENDS)

#replacement rule for gbafix
#---------------------------------------------------------------------------------
%.gba: %.elf
	@$(OBJCOPY) -O binary $< $@
	@echo CUSTOM built ... $(notdir $@)
	@echo gbafix $@ -t GOOMBA COLOR -c GMBC
	@gbafix $@ "-tGOOMBA COLOR" "-cGMBC"



#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
