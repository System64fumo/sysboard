EXEC = sysboard
LIB = libsysboard.so
PKGS = gtkmm-4.0 gtk4-layer-shell-0	
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DESTDIR = $(HOME)/.local

CXXFLAGS = -march=native -mtune=native -Os -s -Wall -flto=auto -fno-exceptions -fPIC
CXXFLAGS += $(shell pkg-config --cflags $(PKGS))
LDFLAGS = $(shell pkg-config --libs $(PKGS))

PROTOS = $(wildcard proto/*.xml)
PROTO_HDRS = $(patsubst proto/%.xml, src/%.h, $(PROTOS))
PROTO_SRCS = $(patsubst proto/%.xml, src/%.c, $(PROTOS))
PROTO_OBJS = $(PROTO_SRCS:.c=.o)

all: $(EXEC) $(LIB)

install: $(all)
	mkdir -p $(DESTDIR)/bin $(DESTDIR)/lib
	install $(EXEC) $(DESTDIR)/bin/$(EXEC)
	install $(LIB) $(DESTDIR)/lib/$(LIB)

clean:
	rm $(EXEC) $(LIB) $(OBJS) $(PROTO_OBJS) $(PROTO_HDRS) $(PROTO_SRCS) src/os-compatibility.o

$(EXEC): src/main.o
	$(CXX) -o $(EXEC) \
	src/main.o \
	$(CXXFLAGS) \
	$(LDFLAGS)

$(LIB): $(PROTO_HDRS) $(PROTO_OBJS) $(OBJS) src/os-compatibility.o
	$(CXX) -o $(LIB) \
	$(filter-out src/main.o, $(OBJS)) \
	$(PROTO_OBJS) \
	src/os-compatibility.o \
	$(CXXFLAGS) \
	-shared

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ \
	$(CXXFLAGS)

src/os-compatibility.o: src/os-compatibility.c
	$(CC) -c src/os-compatibility.c \
		-o src/os-compatibility.o

$(PROTO_HDRS): src/%.h : proto/%.xml
	wayland-scanner client-header $< $@

$(PROTO_SRCS): src/%.c : proto/%.xml
	wayland-scanner public-code $< $@
