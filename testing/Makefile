# Extended Testing Makefile for hidefile.so
#
# This Makefile compiles hidefile.c into hidefile.so and then
# runs extended tests to verify that:
#   - The readdir interposition hides files whose names contain
#     the substrings provided by the HIDDEN environment variable.
#   - The open interposition blocks files from opening when their
#     names end with any suffix provided by the BLOCKED environment variable.
#
# Run:
#    make clean
#    make all
#    make extra_test

# Build the shared library
all: hidefile.so

hidefile.so: hidefile.c
	@gcc -shared -fPIC hidefile.c -o hidefile.so -ldl
	@echo "Compiled hidefile.so successfully."

# Extended tests for readdir and open interposition
extra_test: test-hidetest test-opentest

###########################################
# Test readdir functionality (file hiding)
###########################################
test-hidetest:
	@echo "============================================"
	@echo "=== Testing readdir (file hiding) feature ==="
	@echo "============================================"
	@echo "Cleaning up previous test files..."
	@rm -f secretA.txt secretB.log visible.txt output-hid.txt
	@echo "Creating test files:"
	@echo "This is secret A" > secretA.txt
	@echo "This is secret B" > secretB.log
	@echo "This is visible" > visible.txt
	@echo ""
	@echo "Test 1: No HIDDEN set (all files should be listed)"
	@LD_PRELOAD="$(PWD)/hidefile.so" ls -1 > output-hid.txt
	@echo "Output (expected: secretA.txt, secretB.log, visible.txt):"
	@cat output-hid.txt
	@echo ""
	@echo "Test 2: HIDDEN set to 'secretA.txt' (should hide secretA.txt)"
	@HIDDEN=secretA.txt LD_PRELOAD="$(PWD)/hidefile.so" ls -1 > output-hid.txt
	@echo "Output (expected: secretB.log, visible.txt):"
	@cat output-hid.txt
	@echo ""
	@echo "Test 3: HIDDEN set to 'secret' (should hide any file containing 'secret')"
	@HIDDEN=secret LD_PRELOAD="$(PWD)/hidefile.so" ls -1 > output-hid.txt
	@echo "Output (expected: visible.txt):"
	@cat output-hid.txt
	@echo ""
	@echo "readdir tests completed."
	@echo ""

###########################################
# Test open functionality (file blocking)
###########################################
test-opentest:
	@echo "============================================"
	@echo "=== Testing open (file blocking) feature ==="
	@echo "============================================"
	@echo "Cleaning up previous test files..."
	@rm -f file1.txt file2.csv file3.data
	@echo "Creating test files:"
	@echo "Text file content." > file1.txt
	@echo "CSV file content." > file2.csv
	@echo "Data file content." > file3.data
	@echo ""
	@echo "Test 1: No BLOCKED set (all files should open)"
	@echo "Opening file1.txt:"; LD_PRELOAD="$(PWD)/hidefile.so" cat file1.txt && echo "file1.txt opened" || echo "file1.txt open failed"
	@echo "Opening file2.csv:"; LD_PRELOAD="$(PWD)/hidefile.so" cat file2.csv && echo "file2.csv opened" || echo "file2.csv open failed"
	@echo "Opening file3.data:"; LD_PRELOAD="$(PWD)/hidefile.so" cat file3.data && echo "file3.data opened" || echo "file3.data open failed"
	@echo ""
	@echo "Test 2: BLOCKED set to '.txt' (should block file1.txt only)"
	@echo "Opening file1.txt (should be blocked):"
	@BLOCKED=.txt LD_PRELOAD="$(PWD)/hidefile.so" sh -c 'cat file1.txt && echo "file1.txt opened: test FAILED" || echo "file1.txt blocked: test PASSED"'
	@echo "Opening file2.csv (should open):"
	@LD_PRELOAD="$(PWD)/hidefile.so" sh -c 'cat file2.csv && echo "file2.csv opened: test PASSED" || echo "file2.csv open failed"'
	@echo "Opening file3.data (should open):"
	@LD_PRELOAD="$(PWD)/hidefile.so" sh -c 'cat file3.data && echo "file3.data opened: test PASSED" || echo "file3.data open failed"'
	@echo ""
	@echo "Test 3: BLOCKED set to '.csv:.data' (should block file2.csv and file3.data)"
	@echo "Opening file2.csv (should be blocked):"
	@BLOCKED=.csv:.data LD_PRELOAD="$(PWD)/hidefile.so" sh -c 'cat file2.csv && echo "file2.csv opened: test FAILED" || echo "file2.csv blocked: test PASSED"'
	@echo "Opening file3.data (should be blocked):"
	@BLOCKED=.csv:.data LD_PRELOAD="$(PWD)/hidefile.so" sh -c 'cat file3.data && echo "file3.data opened: test FAILED" || echo "file3.data blocked: test PASSED"'
	@echo "Opening file1.txt (should open):"
	@LD_PRELOAD="$(PWD)/hidefile.so" sh -c 'cat file1.txt && echo "file1.txt opened: test PASSED" || echo "file1.txt open failed"'
	@echo ""
	@echo "open() tests completed."
	@echo ""

# Clean up generated files and the shared library
clean:
	@echo "Cleaning up..."
	@rm -f hidefile.so secretA.txt secretB.log visible.txt output-hid.txt file1.txt file2.csv file3.data
	@echo "Clean complete."
