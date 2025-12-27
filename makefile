CFLAGS = -Wall -Wextra -Ilibs
LIB_FILES = libs/logger/logger.c         \
			libs/nrf24/nrf24.c           \
			libs/app_layer/app_layer.c   \
			libs/pres_layer/pres_layer.c

all: tests

tests: tests_logger tests_nrf24 tests_app_layer tests_pres_layer

tests_logger:
	mkdir -p bin/tests/logger
	gcc -o bin/tests/logger/messages $(CFLAGS) $(LIB_FILES) tests/logger/messages.c
	gcc -o bin/tests/logger/logger_init $(CFLAGS) $(LIB_FILES) tests/logger/logger_init.c

tests_nrf24:
	mkdir -p bin/tests
	gcc -o bin/tests/nrf24 $(CFLAGS) $(LIB_FILES) tests/nrf24.c

tests_app_layer:
	mkdir -p bin/tests/app_layer
	gcc -o bin/tests/app_layer/parser $(CFLAGS) $(LIB_FILES) tests/app_layer/parser.c

tests_pres_layer:
	mkdir -p bin/tests
	gcc -o bin/tests/pres_layer $(CFLAGS) $(LIB_FILES) tests/pres_layer.c

clean:
	rm -rf bin

clean_tests_logger:
	rm -rf bin/tests/logger

clean_tests_nrf25:
	rm -rf bin/tests/nrf24

clean_tests_app_layer:
	rm -rf bin/tests/app_layer

clean_tests_pres_layer:
	rm -rf bin/tests/pres_layer
