.PHONY: all check verify run run_valgrind clean

CXX = g++
CXX_FLAG = -Wall -Wextra -std=c++11 -g -O3
INCLUDE_PATH = -Iheader

CHECKCC = valgrind
CHECKCC_FLAG = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

CPPCHECK = ../../cppcheck-2.13.0/cppcheck
CPPCHECK_FLAG = --enable=all --inconclusive --suppress=missingIncludeSystem

MEMORY_DIRECTORY = memory
OBJECT_DIRECTORY = object
VERIFICATION_DIRECTORY = verification

SOURCE_DIRECTORY = source
TESTCASE_DIRECTORY = testcase

SOURCE = $(wildcard $(SOURCE_DIRECTORY)/*.cpp)
OBJECT = $(patsubst $(SOURCE_DIRECTORY)/%.cpp, $(OBJECT_DIRECTORY)/%.o, $(SOURCE))

TESTCASE = $(wildcard $(TESTCASE_DIRECTORY)/*.gmp)

TARGET = D2DGRter
EVALUATOR = script/evaluator

all: $(TARGET)

$(MEMORY_DIRECTORY):
	mkdir $(MEMORY_DIRECTORY)

$(OBJECT_DIRECTORY):
	mkdir $(OBJECT_DIRECTORY)

$(VERIFICATION_DIRECTORY):
	mkdir $(VERIFICATION_DIRECTORY)

$(TARGET): main.cpp $(OBJECT)
	$(CXX) $(CXX_FLAG) $(INCLUDE_PATH) $^ -o $@

$(OBJECT_DIRECTORY)/%.o: $(SOURCE_DIRECTORY)/%.cpp | $(OBJECT_DIRECTORY)
	$(CXX) $(CXX_FLAG) $(INCLUDE_PATH) -c $< -o $@

check: $(CPPCHECK)
	./$(CPPCHECK) $(CPPCHECK_FLAG) $(INCLUDE_PATH) .

verify: run $(EVALUATOR) | $(VERIFICATION_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		./$(EVALUATOR) $(TESTCASE_DIRECTORY) "$$testcase_base" | tee $(VERIFICATION_DIRECTORY)/"$$testcase_base".log; \
	done

run: $(TARGET)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		./$(TARGET) "$$testcase" $(TESTCASE_DIRECTORY)/"$$testcase_base".gcl $(TESTCASE_DIRECTORY)/"$$testcase_base".cst $(TESTCASE_DIRECTORY)/"$$testcase_base".lg; \
	done

run_valgrind: $(TARGET) | $(MEMORY_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		$(CHECKCC) $(CHECKCC_FLAG) --log-file=$(MEMORY_DIRECTORY)/"$$testcase_base.log" ./$(TARGET) "$$testcase" $(TESTCASE_DIRECTORY)/"$$testcase_base".gcl $(TESTCASE_DIRECTORY)/"$$testcase_base".cst $(TESTCASE_DIRECTORY)/"$$testcase_base".lg; \
	done

clean:
	rm -rf $(TARGET) $(MEMORY_DIRECTORY) $(OBJECT_DIRECTORY) $(TESTCASE_DIRECTORY)/*.lg $(VERIFICATION_DIRECTORY)
