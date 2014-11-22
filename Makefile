#Set this to @ to keep the makefile quiet
SILENCE = @
RANLIB = ranlib
#---- Outputs ----#
COMPONENT_NAME = CSlim
TARGET_LIB = \
	lib/lib$(COMPONENT_NAME).a

TEST_TARGET = \
	$(COMPONENT_NAME)_tests

#--- Inputs ----#
PROJECT_HOME_DIR = .
ifeq "$(CPPUTEST_HOME)" ""
    CPPUTEST_HOME = ../CppUTest
endif
ifeq "$(CSLIM_HOME)" ""
    CSLIM_HOME = ../cslim
endif
CPP_PLATFORM = Gcc

# set OS_PLATFROM to win to build with winsock
# OS_PLATFORM = win

#CFLAGS are set to override malloc, free, etc to get memory leak detection in C programs
CPPUTEST_CFLAGS = -Dmalloc=cpputest_malloc -Dfree=cpputest_free -Drealloc=cpputest_realloc
CPPFLAGS = -Wall
#GCOVFLAGS = -fprofile-arcs -ftest-coverage

#SRC_DIRS is a list of source directories that make up the target library
#If test files are in these directories, their IMPORT_TEST_GROUPs need
#to be included in main to force them to be linked in.  By convention
#put them into an AllTests.h file in each directory
SRC_DIRS = \
	src/CSlim\
	src/ExecutorC\

ifeq ($(OS_PLATFORM), win)
  SRC_DIRS += src/ComWin32
else
  SRC_DIRS += src/Com
endif

#TEST_SRC_DIRS is a list of directories including 
# - A test main (AllTests.cpp by convention)
# - OBJ files in these directories are included in the TEST_TARGET
# - Consequently - AllTests.h containing the IMPORT_TEST_GROUPS is not needed
# - 
TEST_SRC_DIRS = \
	tests \
	tests/Com \
	tests/CSlim \

MOCKS_SRC_DIRS = \
	
CSLIM_SRC_DIRS = \
	fixtures

#includes for all compiles	
INCLUDES =\
  -Iinclude/Com\
  -Iinclude/CSlim\
  -Iinclude/ExecutorC\
  -Itests/CSlim\
  -I$(CPPUTEST_HOME)/include/\

#Flags to pass to ld

ifeq ($(OS_PLATFORM), win)
  LDFLAGS += -lWS2_32
else
  LDFLAGS += 
endif

LD_LIBRARIES += -lstdc++


OTHER_MAKEFILE_TO_INCLUDE = $(CSLIM_HOME)/build/CSlimServerMakefile
	
include $(CPPUTEST_HOME)/build/ComponentMakefile

all: $(TEST_TARGET) $(CSLIM_TARGET) 
