EXEC = sysboard
LIB = libsysboard.so
PKGS = gtkmm-4.0 gtk4-layer-shell-0	
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DESTDIR = $(HOME)/.local

CXXFLAGS += -Os -s -Wall -flto=auto -fno-exceptions -fPIC
LDFLAGS += -Wl,-O1,--as-needed,-z,now,-z,pack-relative-relocs

CXXFLAGS += $(shell pkg-config --cflags $(PKGS))
LDFLAGS += $(shell pkg-config --libs $(PKGS))

PROTOS = $(wildcard proto/*.xml)
PROTO_HDRS = $(patsubst proto/%.xml, src/%.h, $(PROTOS))
PROTO_SRCS = $(patsubst proto/%.xml, src/%.c, $(PROTOS))
PROTO_OBJS = $(PROTO_SRCS:.c=.o)

JOB_COUNT := $(EXEC) $(LIB) $(OBJS) $(PROTO_HDRS) $(PROTO_SRCS) $(PROTO_OBJS) src/os-compatibility.o src/git_info.hpp
JOBS_DONE := $(shell ls -l $(JOB_COUNT) 2> /dev/null | wc -l)

define progress
	$(eval JOBS_DONE := $(shell echo $$(($(JOBS_DONE) + 1))))
	@printf "[$(JOBS_DONE)/$(shell echo $(JOB_COUNT) | wc -w)] %s %s\n" $(1) $(2)
endef


all: $(EXEC) $(LIB)

install: $(all)
	mkdir -p $(DESTDIR)/bin $(DESTDIR)/lib
	install $(EXEC) $(DESTDIR)/bin/$(EXEC)
	install $(LIB) $(DESTDIR)/lib/$(LIB)

clean:
	@echo "Cleaning up"
	@rm $(EXEC) $(LIB) $(OBJS) $(PROTO_OBJS) $(PROTO_HDRS) $(PROTO_SRCS) src/os-compatibility.o src/git_info.hpp

$(EXEC): src/git_info.hpp src/main.o src/config_parser.o
	$(call progress, Linking $@)
	@$(CXX) -o $(EXEC) \
	src/main.o \
	src/config_parser.o \
	$(CXXFLAGS) \
	$(shell pkg-config --libs gtkmm-4.0 gtk4-layer-shell-0)

$(LIB): $(PROTO_HDRS) $(PROTO_OBJS) $(OBJS) src/os-compatibility.o
	$(call progress, Linking $@)
	@$(CXX) -o $(LIB) \
	$(filter-out src/main.o src/config_parser.o, $(OBJS)) \
	$(PROTO_OBJS) \
	src/os-compatibility.o \
	$(CXXFLAGS) \
	$(LDFLAGS) \
	-shared

%.o: %.cpp
	$(call progress, Compiling $@)
	@$(CXX) $(CFLAGS) -c $< -o $@ \
	$(CXXFLAGS)

%.o: %.c
	$(call progress, Compiling $@)
	@$(CC) -c $< -o $@ $(CFLAGS)

src/os-compatibility.o: src/os-compatibility.c
	$(call progress, Compiling $@)
	@$(CC) -c src/os-compatibility.c \
		-o src/os-compatibility.o

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
	commit_message=$$(git show -s --format=%s $$commit_hash); \
	echo "#define GIT_COMMIT_MESSAGE \"$$commit_message\"" > src/git_info.hpp; \
	echo "#define GIT_COMMIT_DATE \"$$commit_date\"" >> src/git_info.hpp
