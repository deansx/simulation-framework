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
TXTEV=../../examples/text_event/

INCLUDES=-I . -I $(UTIL) -I $(DSIM) -I $(TXTEV)

SOURCES=stim_loader_test.cc \
	$(UTIL)common_strings.cc \
	$(UTIL)common_messages.cc \
	$(UTIL)common_utilities.cc \
	$(UTIL)config_mgr.cc \
	$(UTIL)log_mgr.cc \
	$(UTIL)sim_time.cc \
	$(UTIL)arg_parser.cc \
	$(DSIM)sim_exec.cc \
	$(DSIM)sim_base_event.cc \
	$(DSIM)stim_loader.cc \
	$(TXTEV)sim_text_event.cc \
	$(TXTEV)log_text_event.cc \
	$(TXTEV)stim_text_event_loader.cc

OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=stim_loader

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
