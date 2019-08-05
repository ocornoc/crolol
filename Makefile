WARNS = -Wall -Wextra -Wpedantic -Wno-unknown-pragmas
VERSION = -std=c++11
OPTLEVEL = -O3 -flto
DEBUG = -O0 -g -ftrapv

CXXFLAGS = $(WARNS) $(VERSION) $(OPTLEVEL)

BUILD = build
SRC = src
TESTS = tests

SRCCPPS = $(SRC)/num.cpp $(SRC)/losslessops.cpp
BUILDOBJS = $(BUILD)/num_src.o $(BUILD)/losslessops_src.o
TESTEXECS = basic_arith_EXEC

default: compile
	
compile: $(BUILD) $(BUILDOBJS)
	
test: $(BUILD) $(TESTEXECS)

debug: CXXFLAGS += $(DEBUG)
debug: test

clean: $(BUILD)
	rm -r $(BUILD)

.PHONY: default compile test debug clean

$(BUILD):
	mkdir $(BUILD)

$(BUILD)/%_src.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BUILD)/%_test.o: $(TESTS)/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BUILD)/%_test: $(BUILD)/%_test.o $(BUILDOBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%_EXEC: $(BUILD)/%_test
	./$<
