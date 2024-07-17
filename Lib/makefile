INCFLAGS = -I.
DEBUG_CC_FLAGS = $(INCFLAGS) -c -g -D_DEBUG
RELEASE_CC_FLAGS = $(INCFLAGS) -c -O2 -DNDEBUG
DEBUG_BIN_DIR = ../Debug/Bin
RELEASE_BIN_DIR = ../Release/Bin
DEBUG_OBJ_DIR = ../Debug/o
RELEASE_OBJ_DIR = ../Release/o
DEBUG_TARGET = $(DEBUG_BIN_DIR)/libBMLib.a
RELEASE_TARGET = $(RELEASE_BIN_DIR)/libBMLib.a
DEBUG_OBJECTS = $(DEBUG_OBJ_DIR)/BMLock.o $(DEBUG_OBJ_DIR)/BMEv.o \
	$(DEBUG_OBJ_DIR)/BMDLNode.o $(DEBUG_OBJ_DIR)/BMFSM.o \
	$(DEBUG_OBJ_DIR)/BMSubtimer.o
RELEASE_OBJECTS = $(RELEASE_OBJ_DIR)/BMLock.o $(RELEASE_OBJ_DIR)/BMEv.o \
	$(RELEASE_OBJ_DIR)/BMDLNode.o $(RELEASE_OBJ_DIR)/BMFSM.o \
	$(RELEASE_OBJ_DIR)/BMSubtimer.o

.PHONY	:	debug
.PHONY	:	clean_debug
.PHONY	:	release
.PHONY	:	clean_release

debug	:	$(DEBUG_TARGET)
$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_BIN_DIR)
	$(AR) r $@ $^

$(DEBUG_OBJ_DIR)/BMLock.o	:	BMLock.c BMLock.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMDLNode.o	:	BMDLNode.c BMDLNode.h BMLock.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMEv.o	:	BMEv.c BMEv.h BMDLNode.h BMLock.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMFSM.o	:	BMFSM.c BMFSM.h BMEv.h BMDLNode.h BMLock.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMSubtimer.o	:	BMSubtimer.c BMSubtimer.h BMEv.h BMDLNode.h BMLock.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_TARGET) $(DEBUG_OBJECTS)

