CFLAGS = -Wall -Wextra -Ilibs

tests: tests_logger

LOGGER_TEST_SOURCE_FILES = libs/logger/logger.c
tests_logger:
	gcc -o bin/tests/logger $(CFLAGS) $(LOGGER_TEST_SOURCE_FILES) tests/logger.c

clean:
	rm bin/tests/logger