.PHONY: all check verify run run_valgrind clean

CXX = g++
CXX_FLAG = -Wall -std=c++11 -g
INCLUDE_PATH = -Iheader

CHECKCC = valgrind
CHECKCC_FLAG = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

CPPCHECK = ../../cppcheck-2.13.0/cppcheck
CPPCHECK_FLAG = --enable=all --inconclusive --suppress=missingIncludeSystem

MEMORY_DIRECTORY = memory
OBJECT_DIRECTORY = object
OUTPUT_DIRECTORY = output
VERIFICATION_DIRECTORY = verification

SOURCE_DIRECTORY = source
TESTCASE_DIRECTORY = testcase
TESTCASE_BLOCK_DIRECTORY = $(TESTCASE_DIRECTORY)/block
TESTCASE_NET_DIRECTORY = $(TESTCASE_DIRECTORY)/net

SOURCE = $(wildcard $(SOURCE_DIRECTORY)/*.cpp)
OBJECT = $(patsubst $(SOURCE_DIRECTORY)/%.cpp, $(OBJECT_DIRECTORY)/%.o, $(SOURCE))

TESTCASE = $(wildcard $(TESTCASE_BLOCK_DIRECTORY)/*)

TARGET = Lab2
VERIFIER = script/verifier
ALPHA = 0.5

all: $(TARGET)

$(MEMORY_DIRECTORY):
	mkdir $(MEMORY_DIRECTORY)

$(OBJECT_DIRECTORY):
	mkdir $(OBJECT_DIRECTORY)

$(OUTPUT_DIRECTORY):
	mkdir $(OUTPUT_DIRECTORY)

$(VERIFICATION_DIRECTORY):
	mkdir $(VERIFICATION_DIRECTORY)

$(TARGET): main.cpp $(OBJECT)
	$(CXX) $(CXX_FLAG) $(INCLUDE_PATH) $^ -o $@

$(OBJECT_DIRECTORY)/%.o: $(SOURCE_DIRECTORY)/%.cpp | $(OBJECT_DIRECTORY)
	$(CXX) $(CXX_FLAG) $(INCLUDE_PATH) -c $< -o $@

check:
	./$(CPPCHECK) $(CPPCHECK_FLAG) $(INCLUDE_PATH) .

verify: run | $(VERIFICATION_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		./$(VERIFIER) $(ALPHA) "$$testcase" $(TESTCASE_NET_DIRECTORY)/"$$testcase_base".net $(OUTPUT_DIRECTORY)/"$$testcase_base".out | tee $(VERIFICATION_DIRECTORY)/"$$testcase_base".log; \
	done

run: $(TARGET) | $(OUTPUT_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		./$(TARGET) $(ALPHA) "$$testcase" $(TESTCASE_NET_DIRECTORY)/"$$testcase_base".net $(OUTPUT_DIRECTORY)/"$$testcase_base".out; \
	done

run_valgrind: $(TARGET) | $(MEMORY_DIRECTORY) $(OUTPUT_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		$(CHECKCC) $(CHECKCC_FLAG) --log-file=$(MEMORY_DIRECTORY)/"$$testcase_base.log" ./$(TARGET) $(ALPHA) "$$testcase" $(TESTCASE_DIRECTORY)/net/"$$testcase_base".net $(OUTPUT_DIRECTORY)/"$$testcase_base".out; \
	done

clean:
	rm -rf $(TARGET) $(MEMORY_DIRECTORY) $(OBJECT_DIRECTORY) $(OUTPUT_DIRECTORY) $(VERIFICATION_DIRECTORY)
