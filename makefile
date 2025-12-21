CFLAGS = -Wall -Wextra -Ilibs

tests: tests_logger

LOGGER_TEST_SOURCE_FILES = libs/logger/logger.c
tests_logger:
	gcc -o bin/tests/t_logger $(CFLAGS) $(LOGGER_TEST_SOURCE_FILES) tests/t_logger.c

clean:
	rm bin/tests/t_logger