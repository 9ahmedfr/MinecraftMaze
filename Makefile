# Linker flags (specifying the location of the mcpp library)
LDFLAGS = -L/usr/local/lib -lmcpp -Wl,-rpath,/usr/local/lib

.default: all

all: mazeRunner

clean:
	rm -f mazeRunner *.o

run:
	./mazeRunner

test:
	./mazeRunner -testmode

# Variables for test file locations
TEST_DIR_INPUT = Tests/Input
TEST_DIR_OUTPUT = Tests/Output
TEST_DIR_EXPOUT = Tests/ExpOutput
TEST_CASES = $(shell ls $(TEST_DIR_INPUT) | sed 's/\.input//')

runTests:
	@echo "========================================"
	@echo "             RUNNING TESTS              "
	@echo "========================================\n"
	@for test in $(TEST_CASES); do \
		echo "RUNNING TEST: $$test"; \
		./mazeRunner -testmode < $(TEST_DIR_INPUT)/$$test.input > $(TEST_DIR_OUTPUT)/$$test.out; \
		total_lines=$$(wc -l < $(TEST_DIR_EXPOUT)/$$test.expout); \
		diff_lines=$$(diff -w $(TEST_DIR_OUTPUT)/$$test.out $(TEST_DIR_EXPOUT)/$$test.expout | wc -l); \
		similarity=$$((100 * (total_lines - diff_lines) / total_lines)); \
		if [ $$similarity -eq 100 ]; then \
			echo "$$test Passed (100%)\n"; \
		else \
			echo "$$test Failed ($$similarity%)\n"; \
		fi; \
	done

testOut:
	./mazeRunner -testmode < Tests/Input/WallFollowerExitFound.input > Tests/Output/WallFollowerExitFound.out
	diff -w Tests/Output/WallFollowerExitFound.out Tests/ExpOutput/WallFollowerExitFound.expout

mazeRunner: Maze.o Agent.o mazeRunner.o
	g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	g++ -Wall -Werror -std=c++17 -g -O -c $^