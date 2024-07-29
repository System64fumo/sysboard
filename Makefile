EXEC = sysboard
LIB = libsysboard.so
PKGS = gtkmm-4.0 gtk4-layer-shell-0	
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DESTDIR = $(HOME)/.local

CXXFLAGS = -march=native -mtune=native -Os -s -Wall -flto=auto -fno-exceptions -fPIC
CXXFLAGS += $(shell pkg-config --cflags $(PKGS))
LDFLAGS = $(shell pkg-config --libs $(PKGS))

PROTOS = virtual-keyboard-unstable-v1
PROTO_DIR = proto
PROTO_HDRS = $(addprefix src/, $(addsuffix .h, $(notdir $(PROTOS))))
PROTO_SRCS = $(addprefix src/, $(addsuffix .c, $(notdir $(PROTOS))))
PROTO_OBJS = $(PROTO_SRCS:.c=.o)

all: $(EXEC) $(LIB)

install: $(all)
	mkdir -p $(DESTDIR)/bin $(DESTDIR)/lib
	install $(EXEC) $(DESTDIR)/bin/$(EXEC)
	install $(LIB) $(DESTDIR)/lib/$(LIB)

clean:
	rm $(EXEC) $(LIB) $(OBJS) $(PROTO_OBJS) $(PROTO_HDRS) $(PROTO_SRCS)

$(EXEC): src/main.o
	$(CXX) -o $(EXEC) \
	src/main.o \
	$(CXXFLAGS) \
	$(LDFLAGS)

$(LIB): $(OBJS) $(PROTO_OBJS)
	$(CXX) -o $(LIB) \
	$(filter-out src/main.o, $(OBJS)) \
	$(PROTO_OBJS) \
	$(CXXFLAGS) \
	-shared

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ \
	$(CXXFLAGS)

$(PROTO_HDRS) $(PROTO_SRCS): $(PROTO_DIR)/$(PROTOS).xml
	wayland-scanner client-header $< src/$(notdir $(basename $<)).h
	wayland-scanner public-code $< src/$(notdir $(basename $<)).c
