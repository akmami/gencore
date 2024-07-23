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
LCPTOOLS_CXXFLAGS := -I$(CURRENT_DIR)/lcptools/include
LCPTOOLS_LDFLAGS := -L$(CURRENT_DIR)/lcptools/lib -llcptools -Wl,-rpath,$(CURRENT_DIR)/lcptools/lib -lz
HTSLIB_CXXFLAGS := -I$(CURRENT_DIR)/htslib/include
HTSLIB_LDFLAGS := -L$(CURRENT_DIR)/htslib/lib -lhts -Wl,-rpath,$(CURRENT_DIR)/htslib/lib

$(TARGET): $(OBJS)
	$(GXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LCPTOOLS_LDFLAGS) $(HTSLIB_LDFLAGS)
	rm *.o

chtslib.o: chtslib.cpp
	$(GXX) $(CXXFLAGS) $(HTSLIB_CXXFLAGS) -c $< -o $@

rbam.o: rbam.cpp
	$(GXX) $(CXXFLAGS) $(HTSLIB_CXXFLAGS) $(LCPTOOLS_CXXFLAGS) -c $< -o $@

gencore.o: gencore.cpp
	$(GXX) $(CXXFLAGS) $(HTSLIB_CXXFLAGS) $(LCPTOOLS_CXXFLAGS) -c $< -o $@

fileio.o: fileio.cpp
	$(GXX) $(CXXFLAGS) $(LCPTOOLS_CXXFLAGS) -c $< -o $@

rfasta.o: rfasta.cpp
	$(GXX) $(CXXFLAGS) $(LCPTOOLS_CXXFLAGS) -c $< -o $@

rfastq.o: rfastq.cpp
	$(GXX) $(CXXFLAGS) $(LCPTOOLS_CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(GXX) $(CXXFLAGS) -c $< -o $@

# dependencies
gencore.o: init.o rbam.o rfasta.o rfastq.o similarity_metrics.o
chtslib.o:
fileio.o: 
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

install: clean install-htslib install-lcptools $(TARGET)

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

install-lcptools:
	@echo "Installing lcptool"
	cd lcptools && \
	make && \
	make install PREFIX=$(CURRENT_DIR)/lcptools

reinstall-lcptools:
	@echo "Re-installing lcptools"
	git submodule deinit -f -- lcptools
	rm -rf lcptools
	git submodule update --init --recursive

