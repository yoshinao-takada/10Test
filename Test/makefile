INCFLAGS = -I../Lib -I.
DEBUG_CC_FLAGS = $(INCFLAGS) -c -g -D_DEBUG
RELEASE_CC_FLAGS = $(INCFLAGS) -c -O2 -DNDEBUG
DEBUG_BIN_DIR = ../Debug/Bin
RELEASE_BIN_DIR = ../Release/Bin
DEBUG_OBJ_DIR = ../Debug/o
RELEASE_OBJ_DIR = ../Release/o
DEBUG_TARGET = $(DEBUG_BIN_DIR)/Test.exe
RELEASE_TARGET = $(RELEASE_BIN_DIR)/Test.
DEBUG_LIB = $(DEBUG_BIN_DIR)/libBMLib.a
RELEASE_LIB = $(RELEASE_BIN_DIR)/libBMLib.a
DEBUG_OBJECTS = $(DEBUG_OBJ_DIR)/main.o $(DEBUG_OBJ_DIR)/BMEvUT.o \
	$(DEBUG_OBJ_DIR)/BMDLNodeUT.o
RELEASE_OBJECTS = $(RELEASE_OBJ_DIR)/main.o $(RELEASE_OBJ_DIR)/BMEvUT.o \
	$(RELEASE_OBJ_DIR)/BMDLNodeUT.o

.PHONY	:	debug
.PHONY	:	clean_debug
.PHONY	:	release
.PHONY	:	clean_release

debug	:	$(DEBUG_TARGET)
$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS) $(DEBUG_LIB)
	$(CC) -o $@ $^ -L$(DEBUG_BIN_DIR) -lBMLib -lpthread -lm

$(DEBUG_OBJ_DIR)/main.o	:	main.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMEvUT.o	:	BMEvUT.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMDLNodeUT.o	:	BMDLNodeUT.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_TARGET) $(DEBUG_OBJECTS)
