CC = g++

ORIGINAL_FLAGS := $(CPPFLAGS)
CPPFLAGS = -Wall -pedantic
ifdef OPTIMIZE
  ifdef DEBUG
    CPPFLAGS += -g
  endif
  CPPFLAGS += -O3
else
  CPPFLAGS += -g
endif
CPPFLAGS += -MD -MP -MF .dep/$(subst /,-,$@).d
CPPFLAGS += $(shell pkg-config --cflags gl glu glut glew)
CPPFLAGS += $(ORIGINAL_FLAGS)

LDFLAGS += $(shell pkg-config --libs gl glu glut glew)

###############################################################################

C_FILES =  $(wildcard src/*.cpp)
C_FILES += $(wildcard src/*/*.cpp)
OBJECTS = $(C_FILES:.cpp=.o)

###############################################################################

all: build .dep basic
rebuild: clean all


basic: $(OBJECTS:src/%=build/%)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)


build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -rf build/*
	rm -rf .dep/*


build:
	mkdir $@


.PRECIOUS: build/%.o
.PHONY: all rebuild clean

# Include the dependency files, should be the last of the makefile
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
