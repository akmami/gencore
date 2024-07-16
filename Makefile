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
LIBS := -llcptools -lz

$(TARGET): $(OBJS)
	$(GXX) $(CXXFLAGS) -o $@ $^ $(LIBS) 

%.o: %.cpp
	$(GXX) $(CXXFLAGS) -c $< -o $@ $(LIBS) 

# dependencies
gencore.o: gencore.cpp init.o rfasta.o rfastq.o
init.o: init.cpp
helper.o: helper.cpp
rfasta.o: rfasta.cpp
rfastq.o: rfastq.cpp helper.o
similarity_metrics.o: similarity_metrics.cpp

clean:
	rm -f $(OBJS)