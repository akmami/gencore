# programs
TARGET := gencore
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

# directories
CURRENT_DIR := $(shell pwd)
BIN_DIR := $(CURRENT_DIR)/bin

# compiler
GXX := g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++11
TIME := /usr/bin/time -v

# object files that need lcptools
LCPTOOLS_LIBS := -llcptools -lz
HTSLIB_CXXFLAGS := -I$(CURRENT_DIR)/htslib/include
HTSLIB_LDFLAGS := -L$(CURRENT_DIR)/htslib/lib -lhts -Wl,-rpath,$(CURRENT_DIR)/htslib/lib

$(TARGET): $(OBJS)
	$(GXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LCPTOOLS_LIBS) $(HTSLIB_LDFLAGS)

chtslib.o: chtslib.cpp
	$(GXX) $(CXXFLAGS) $(HTSLIB_CXXFLAGS) -c $< -o $@

rbam.o: rbam.cpp
	$(GXX) $(CXXFLAGS) $(HTSLIB_CXXFLAGS) -c $< -o $@ $(LCPTOOLS_LIBS)

gencore.o: gencore.cpp
	$(GXX) $(CXXFLAGS) $(HTSLIB_CXXFLAGS) -c $< -o $@ $(LCPTOOLS_LIBS)

rfasta.o: rfasta.cpp
	$(GXX) $(CXXFLAGS) -c $< -o $@ $(LCPTOOLS_LIBS)

rfastq.o: rfastq.cpp
	$(GXX) $(CXXFLAGS) -c $< -o $@ $(LCPTOOLS_LIBS)

%.o: %.cpp
	$(GXX) $(CXXFLAGS) -c $< -o $@

# dependencies
gencore.o: init.o rbam.o rfasta.o rfastq.o
init.o:
helper.o:
rbam.o: similarity_metrics.o
rfasta.o: similarity_metrics.o
rfastq.o: helper.o similarity_metrics.o
similarity_metrics.o:

clean: 
	@echo "Cleaning"
	rm -f $(OBJS)
	rm -f $(TARGET)

install: clean install-htslib $(TARGET)

install-htslib:
	@echo "Installing htslib"
	cd htslib && \
	autoreconf -i && \
	./configure && \
	make && \
	make prefix=$(CURRENT_DIR)/htslib install

reinstall-htslib:
	@echo "Re-installing htslib"
	git submodule deinit -f -- htslib
	rm -rf htslib
	git submodule update --init --recursive
