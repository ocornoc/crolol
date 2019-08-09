WARNS = -Wall -Wextra -Wpedantic -Wno-unknown-pragmas
VERSION = -std=c++11
OPTLEVEL = -O3
DEBUG = -O0 -g -ftrapv

CXXFLAGS = $(WARNS) $(VERSION) $(OPTLEVEL)

BUILD = build
SRC = src
TESTS = tests

SRCCPPS = $(SRC)/num.cpp $(SRC)/losslessops.cpp
COREBUILDOBJS = $(BUILD)/num_src.o $(BUILD)/losslessops_src.o
TESTBUILDEXECS = $(BUILD)/basic_arith_test
BUILDOBJS = $(COREBUILDOBJS) $(TESTBUILDOBJS)
TESTEXEC = basic_arith_EXEC

default: compile
	
compile: $(BUILD) $(BUILDOBJS) $(TESTBUILDEXECS)
	
test: debug $(TESTEXEC)

debug: CXXFLAGS += $(DEBUG)
debug: compile

clean: $(BUILD)
	rm -r $(BUILD)

.PHONY: default compile test debug clean

$(BUILD):
	mkdir $(BUILD)

$(BUILD)/%_src.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BUILD)/%_test.o: $(TESTS)/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BUILD)/%_test: $(BUILD)/%_test.o $(COREBUILDOBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%_EXEC: $(BUILD)/%_test
	./$<
