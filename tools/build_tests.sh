#!/bin/bash
TEST_FUNCTIONS=$(grep -oP '(?<=void )test_[a-zA-Z0-9_]+(?=\()' src/test/test.c)

for func in $TEST_FUNCTIONS
do
    echo "------------------------------------------"
    echo "Building Test: $func"

    make TEST=$func clean all
    if [ $? -ne 0 ]; then
        echo "Build FAILED for $func"
        exit 1
    fi
done
echo "All tests built successfully!"