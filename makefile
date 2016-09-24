CC        := gcc
CXX       := g++
FORMAT    := clang-format

WARNINGS  := -Wall -Wextra 
FULLWARN  := -Wall -Wextra -Wpedantic
STD       := -std=c++14 
OPTIMIZE  := -O3
DEBUG     := 
INCLUDES  := 
LINK      := 

MODULES   := 
SRC_DIR   := src $(addprefix src/,$(MODULES))
BUILD_DIR := obj $(addprefix obj/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,obj/%.o,$(SRC))
#INCLUDES  := $(addprefix -I,$(SRC_DIR))

ALLFILES  = $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
ALLFILES += $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.hpp))
ALLFILES += $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.tpp))

PROGNAME := cas

.PHONY: all checkdirs clean

all: checkdirs $(PROGNAME)

debug: DEBUG = -g -DDEBUG
debug: OPTIMIZE = -O0
debug: checkdirs $(PROGNAME)

release: OPTIMIZE = -flto -fwhole-program 
release: checkdirs $(PROGNAME)

profile: DEBUG = -pg
profile: checkdirs $(PROGNAME)

warn: WARNINGS = $(FULLWARN)
warn: checkdirs $(PROGNAME)

sanitize: DEBUG = -g -fsanitize=address
sanitize: checkdirs $(PROGNAME)

asm: DEBUG = -S -masm=intel
asm: checkdirs $(OBJ)

format: $(addsuffix -format, $(ALLFILES))

define make-format
$1:
	@echo $(FORMAT) $$(patsubst %-format, %, $$@)
	@$(FORMAT) -style=file -i $$(patsubst %-format, %, $$@)
endef

$(foreach file,$(ALLFILES),$(eval $(call make-format,$(addsuffix -format, $(file)))))

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	@echo $(CXX) $$<
	@$$(CXX) $$(WARNINGS) $$(STD) $$(OPTIMIZE) $$(DEBUG) $$(INCLUDES) -c $$< -o $$@

$1/%-sse.o: %-sse.cpp
	@echo $(CXX) $$< -msse4.1
	@$$(CXX) $$(WARNINGS) $$(STD) $$(OPTIMIZE) -msse4.1 $$(DEBUG) $$(INCLUDES) -c $$< -o $$@

$1/%-sse2.o: %-sse2.cpp
	@echo $(CXX) $$< -msse2
	@$$(CXX) $$(WARNINGS) $$(STD) $$(OPTIMIZE) -msse2 $$(DEBUG) $$(INCLUDES) -c $$< -o $$@

$1/%-sse3.o: %-sse3.cpp
	@echo $(CXX) $$< -mssse3
	@$$(CXX) $$(WARNINGS) $$(STD) $$(OPTIMIZE) -mssse3 $$(DEBUG) $$(INCLUDES) -c $$< -o $$@

$1/%-sse4.o: %-sse4.cpp
	@echo $(CXX) $$< -msse4.1
	@$$(CXX) $$(WARNINGS) $$(STD) $$(OPTIMIZE) -msse4.1 $$(DEBUG) $$(INCLUDES) -c $$< -o $$@

$1/%-sse42.o: %-sse42.cpp
	@echo $(CXX) $$< -msse4.2
	@$$(CXX) $$(WARNINGS) $$(STD) $$(OPTIMIZE) -msse4.2 $$(DEBUG) $$(INCLUDES) -c $$< -o $$@

$1/%-avx.o: %-avx.cpp
	@echo $(CXX) $$< -mavx
	@$$(CXX) $$(WARNINGS) $$(STD) $$(OPTIMIZE) -mavx $$(DEBUG) $$(INCLUDES) -c $$< -o $$@

$1/%-avx2.o: %-avx2.cpp
	@echo $(CXX) $$< -mavx2 -mfma
	@$$(CXX) $$(WARNINGS) $$(STD) $$(OPTIMIZE) -mavx2 -mfma $$(DEBUG) $$(INCLUDES) -c $$< -o $$@
endef



$(PROGNAME): $(OBJ)
	@echo Linking $@
	@$(CXX) $(DEBUG) $(OPTIMIZE) $^ -o bin/$@ $(LINK)

checkdirs: $(BUILD_DIR) bin

MAKEDIR = mkdir -p
RM  = rm -f
RMR = rm -rf
ifdef SYSTEMROOT
	MAKEDIR = mkdir
	RM = del
	RMR = del
endif

$(BUILD_DIR):
	@$(MAKEDIR) $@

bin:
	@$(MAKEDIR) -p $@

clean:
	@$(RMR) bin/*
	@$(RMR) obj/*
	@$(RM) $(PROGNAME)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
