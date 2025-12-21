#include <stdio.h>

#include "logger/logger.h"

int main() {
    
    if (logger_init("test.log") < 0) {
        printf("An error occurred!\n");
    } else {
        printf("We're good my guy\n");
    }

    return 0;
}