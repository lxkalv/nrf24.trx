CFLAGS = -Wall -Wextra -Ilibs
LIB_FILES = libs/logger/logger.c       \
			libs/nrf24/nrf24.c         \
			libs/app_layer/app_layer.c

all: tests

tests: tests_logger tests_nrf24 tests_app_layer

tests_logger:
	mkdir -p bin/tests
	gcc -o bin/tests/logger $(CFLAGS) $(LIB_FILES) tests/logger.c

tests_nrf24:
	mkdir -p bin/tests
	gcc -o bin/tests/nrf24 $(CFLAGS) $(LIB_FILES) tests/nrf24.c

tests_app_layer:
	mkdir -p bin/tests
	gcc -o bin/tests/app_layer $(CFLAGS) $(LIB_FILES) tests/app_layer.c

clean:
	rm -rf bin

clean_tests_logger:
	rm -rf bin/tests/logger

clean_tests_nrf25:
	rm -rf bin/tests/nrf24

clean_tests_app_layer:
	rm -rf bin/tests/app_layer
