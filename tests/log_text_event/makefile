# makefile for the stim_loader test fixture

# compiler args:
CC=g++
CVERS=-std=c++11
LDFLAGS=-g
TESTS=-DSIM_TST -DTEST_HARNESS -DTIME_VERBOSE
DEFS=$(TESTS) -DLINUX
#WARNS=-Wno-deprecated -Wno-write-strings 
CFLAGS=$(CVERS) $(WARNS) $(DEFS) $(LDFLAGS)

# directories
UTIL=../../util/
DSIM=../../desim/
TXEV=../../examples/text_event/

INCLUDES=-I . -I $(UTIL) -I $(DSIM) -I $(TXEV)

SOURCES=log_text_event_main.cc \
	$(TXEV)log_text_event.cc \
	$(UTIL)log_mgr.cc \
	$(UTIL)sim_time.cc \
	$(UTIL)common_messages.cc \
	$(UTIL)common_utilities.cc \
	$(UTIL)common_strings.cc

OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=log_text_event

all: $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM -MF $3 -MP -MT $2 $(INCLUDES) $(CFLAGS) $1
endef

%.o: %.cc
	$(call make-depend,$<,$@,$(subst .o,.d,$@))
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(subst .cc,.d,$(SOURCES))
endif

clean:
	rm -vf $(OBJECTS)
	rm -vf $(EXECUTABLE).exe
	rm -vf $(subst .cc,.d,$(SOURCES))
