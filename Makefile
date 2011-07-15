ARDUINO_DIR = /Applications/Arduino.app/Contents/Resources/Java

TARGET       = CSlim
BOARD_TAG    = mega2560
ARDUINO_PORT = /dev/tty.usbmodem621
ARDUINO_LIBS = Ethernet Spi Ethernet/utility

CSLIM_SRC = src/CSlim/Slim.c src/CSlim/ListExecutor.c src/CSlim/SlimConnectionHandler.c src/CSlim/SlimList.c \
src/CSlim/SlimListDeserializer.c src/CSlim/SlimListSerializer.c src/CSlim/SlimUtil.c src/CSlim/StatementExecutor.c \
src/CSlim/SymbolTable.c

FIXTURE_C_SRC = fixtures/Fixtures.c fixtures/DecisionTableExample.c fixtures/ExceptionsExample.c fixtures/QueryTableExample.c \
fixtures/ScriptTableExample.c
FIXTURE_CPP_SRC = fixtures/FixtureInCpp.cpp

CPP_COMPILING = 1
CPPFLAGS += -Iinclude/CSlim -Iinclude/ComArduino
LOCAL_C_SRCS += $(CSLIM_SRC) $(FIXTURE_C_SRC)
LOCAL_CPP_SRCS += src/ComArduino/SerialComLink.cpp $(FIXTURE_CPP_SRC)

include Arduino.mk

