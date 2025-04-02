BINS = sysboard
LIBS = libsysboard.so
PKGS = gtkmm-4.0 gtk4-layer-shell-0
SRCS = $(wildcard src/*.cpp)

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
LIBDIR ?= $(PREFIX)/lib
DATADIR ?= $(PREFIX)/share
BUILDDIR = build

CXXFLAGS += -Os -s -Wall -flto=auto -fno-exceptions -fPIC
LDFLAGS += -Wl,-O1,--no-as-needed,-z,now,-z,pack-relative-relocs

CXXFLAGS += $(shell pkg-config --cflags $(PKGS))
LDFLAGS += $(shell pkg-config --libs $(PKGS))

OBJS = $(patsubst src/%.cpp, $(BUILDDIR)/%.o, $(SRCS))

PROTOS = $(wildcard proto/*.xml)
PROTO_HDRS = $(patsubst proto/%.xml, src/%.h, $(PROTOS))
PROTO_SRCS = $(patsubst proto/%.xml, src/%.c, $(PROTOS))
PROTO_OBJS = $(patsubst src/%.c, $(BUILDDIR)/%.o, $(PROTO_SRCS))

JOB_COUNT := $(BINS) $(LIBS) $(OBJS) $(PROTO_HDRS) $(PROTO_SRCS) $(PROTO_OBJS) $(BUILDDIR)/os-compatibility.o src/git_info.hpp
JOBS_DONE := $(shell ls -l $(JOB_COUNT) 2> /dev/null | wc -l)

define progress
	$(eval JOBS_DONE := $(shell echo $$(($(JOBS_DONE) + 1))))
	@printf "[$(JOBS_DONE)/$(shell echo $(JOB_COUNT) | wc -w)] %s %s\n" $(1) $(2)
endef

all: $(BINS) $(LIBS)

install: all
	@echo "Installing..."
	@install -D -t $(DESTDIR)$(BINDIR) $(BUILDDIR)/$(BINS)
	@install -D -t $(DESTDIR)$(LIBDIR) $(BUILDDIR)/$(LIBS)
	@install -D -t $(DESTDIR)$(DATADIR)/sys64/board config.conf style.css

clean:
	@echo "Cleaning up"
	@rm -rf $(BUILDDIR) $(BINS) $(LIBS) $(PROTO_HDRS) $(PROTO_SRCS) src/git_info.hpp

$(BINS): src/git_info.hpp $(BUILDDIR)/main.o $(BUILDDIR)/config_parser.o
	$(call progress, Linking $@)
	@$(CXX) -o $(BUILDDIR)/$@ \
	$(BUILDDIR)/main.o \
	$(BUILDDIR)/config_parser.o \
	$(CXXFLAGS) \
	$(LDFLAGS) -lwayland-client

$(LIBS): $(PROTO_HDRS) $(PROTO_OBJS) $(OBJS) $(BUILDDIR)/os-compatibility.o
	$(call progress, Linking $@)
	@$(CXX) -o $(BUILDDIR)/$@ \
	$(filter-out $(BUILDDIR)/main.o $(BUILDDIR)/config_parser.o, $(OBJS)) \
	$(PROTO_OBJS) \
	$(BUILDDIR)/os-compatibility.o \
	$(CXXFLAGS) \
	$(LDFLAGS) \
	-shared

$(BUILDDIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(call progress, Compiling $@)
	@$(CXX) -c $< -o $@ \
	$(CXXFLAGS)

$(BUILDDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(call progress, Compiling $@)
	@$(CC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/os-compatibility.o: src/os-compatibility.c
	@mkdir -p $(dir $@)
	$(call progress, Compiling $@)
	@$(CC) -c $< -o $@ $(CFLAGS)

$(PROTO_HDRS): src/%.h : proto/%.xml
	$(call progress, Creating $@)
	@wayland-scanner client-header $< $@

$(PROTO_SRCS): src/%.c : proto/%.xml
	$(call progress, Creating $@)
	@wayland-scanner public-code $< $@

src/git_info.hpp:
	$(call progress, Creating $@)
	@commit_hash=$$(git rev-parse HEAD); \
	commit_date=$$(git show -s --format=%cd --date=short $$commit_hash); \
	commit_message=$$(git show -s --format="%s" $$commit_hash | sed 's/"/\\\"/g'); \
	echo "#define GIT_COMMIT_MESSAGE \"$$commit_message\"" > src/git_info.hpp; \
	echo "#define GIT_COMMIT_DATE \"$$commit_date\"" >> src/git_info.hpp
