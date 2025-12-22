CFLAGS = -Wall -Wextra -Ilibs

all: tests

tests: tests_logger

LOGGER_TEST_SOURCE_FILES = libs/logger/logger.c
tests_logger:
	mkdir -p bin/tests
	gcc -o bin/tests/logger $(CFLAGS) $(LOGGER_TEST_SOURCE_FILES) tests/logger.c

clean:
	rm -rf bin

clean_tests_logger:
	rm -rf bin/tests/logger