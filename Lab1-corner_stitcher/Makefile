.PHONY: all check verify run run_valgrind generate debug clean

CXX = g++
CXX_FLAG = -Wall -std=c++11 -g
INCLUDE_PATH = -Iheader

CHECKCC = valgrind
CHECKCC_FLAG = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

CPPCHECK = ../../cppcheck-2.13.0/cppcheck
CPPCHECK_FLAG = --enable=all --inconclusive --suppress=missingIncludeSystem

PYTHON = python3
DRAWING = draw_block_layout.py

GOLDEN_DIRECTORY = golden
MEMORY_DIRECTORY = memory
OBJECT_DIRECTORY = object
OUTPUT_DIRECTORY = output
LAYOUT_DIRECTORY = layout
VERIFICATION_DIRECTORY = verification

SOURCE_DIRECTORY = source

SOURCE = $(wildcard $(SOURCE_DIRECTORY)/*.cpp)
OBJECT = $(patsubst $(SOURCE_DIRECTORY)/%.cpp, $(OBJECT_DIRECTORY)/%.o, $(SOURCE))

TESTCASE = $(wildcard testcase/*)

TARGET = Lab1
GENERATOR = script/golden_generate

all: $(TARGET)

$(GOLDEN_DIRECTORY):
	mkdir $(GOLDEN_DIRECTORY)

$(MEMORY_DIRECTORY):
	mkdir $(MEMORY_DIRECTORY)

$(OBJECT_DIRECTORY):
	mkdir $(OBJECT_DIRECTORY)

$(OUTPUT_DIRECTORY):
	mkdir $(OUTPUT_DIRECTORY)

$(LAYOUT_DIRECTORY):
	mkdir $(LAYOUT_DIRECTORY)

$(VERIFICATION_DIRECTORY):
	mkdir $(VERIFICATION_DIRECTORY)

$(TARGET): main.cpp $(OBJECT)
	$(CXX) $(CXX_FLAG) $(INCLUDE_PATH) $^ -o $@

$(OBJECT_DIRECTORY)/%.o: $(SOURCE_DIRECTORY)/%.cpp | $(OBJECT_DIRECTORY)
	$(CXX) $(CXX_FLAG) $(INCLUDE_PATH) -c $< -o $@

check:
	./$(CPPCHECK) $(CPPCHECK_FLAG) $(INCLUDE_PATH) .

verify: run generate | $(VERIFICATION_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		diff $(OUTPUT_DIRECTORY)/"$$testcase_base".out $(GOLDEN_DIRECTORY)/"$$testcase_base".gold | tee $(VERIFICATION_DIRECTORY)/"$$testcase_base".log; \
	done

run: $(TARGET) | $(OUTPUT_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		./$(TARGET) "$$testcase" $(OUTPUT_DIRECTORY)/"$$testcase_base".out; \
	done

run_valgrind: $(TARGET) | $(MEMORY_DIRECTORY) $(OUTPUT_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		$(CHECKCC) $(CHECKCC_FLAG) --log-file=$(MEMORY_DIRECTORY)/"$$testcase_base.log" ./$(TARGET) "$$testcase" $(OUTPUT_DIRECTORY)/"$$testcase_base".out; \
	done

generate: | $(GOLDEN_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		./$(GENERATOR) "$$testcase" $(GOLDEN_DIRECTORY)/"$$testcase_base".gold; \
	done

debug: $(TARGET) $(DRAWING) | $(OUTPUT_DIRECTORY) $(LAYOUT_DIRECTORY)
	for testcase in $(TESTCASE); do \
		testcase_base=$$(basename "$$testcase"); \
		testcase_base=$${testcase_base%.*}; \
		./$(TARGET) "$$testcase" $(OUTPUT_DIRECTORY)/"$$testcase_base".out $(LAYOUT_DIRECTORY)/"$$testcase_base".layout; \
		$(PYTHON) $(DRAWING) $(LAYOUT_DIRECTORY)/"$$testcase_base".layout $(LAYOUT_DIRECTORY)/"$$testcase_base".png; \
	done

clean:
	rm -rf $(TARGET) $(GOLDEN_DIRECTORY) $(MEMORY_DIRECTORY) $(OBJECT_DIRECTORY) $(OUTPUT_DIRECTORY) $(LAYOUT_DIRECTORY) $(VERIFICATION_DIRECTORY)
