#!/bin/bash
FAIL=0

# Function to check the output of the program after it's run, including checking
# an output file, standard output, standard error and exit status.
checkResults() {
  TESTNO=$1
  ESTATUS=$2
  STATUS=$3

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test $TESTNO FAILED - Expected an exit status of $ESTATUS, bug got: $STATUS"
      FAIL=1
      return 1
  fi

  # if there's an expected output file, make sure the program also has one
  # that matches.  If there's no expected output file, then the program
  # shouldn't write an output file.
  if [ -f expected-$TESTNO.txt ]; then
      # Make sure the output file contains correct output.
      if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
	  echo "**** Test $TESTNO FAILED - output file didn't match expected."
	  FAIL=1
	  return 1
      fi
  else
      if [ -f output.txt ]; then
	  echo "**** Test $TESTNO FAILED - the program wrote an output file but it shouldn't for this test case."
	  FAIL=1
	  return 1
      fi
  fi

  # Make sure the output to stdout matches expected.
  if ! diff -q estdout-$TESTNO.txt stdout.txt >/dev/null 2>&1 ; then
      echo "**** Test $TESTNO FAILED - output to stdout didn't match expected"
      FAIL=1
      return 1
  fi

  # Make sure the output to stderr matches expected.
  if ! diff -q estderr-$TESTNO.txt stderr.txt >/dev/null 2>&1 ; then
      echo "**** Test $TESTNO FAILED - output to stderr didn't match expected"
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Try to get a fresh compile of the project.
make clean
make
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your program."
    FAIL=1
fi

# Run individual tests.  Since they each require different arguments, it didn't make
# as much sense to write a single test function for this.
if [ -x chop ] ; then
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 01: ./chop input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 01 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 02: ./chop line 3 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop line 3 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 02 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 03: ./chop lines 2 25 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop lines 2 25 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 03 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 04: ./chop line 1 line 1 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop line 1 line 1 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 04 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 05: ./chop line 1 lines 6 7 lines 9 10 input-b.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop line 1 lines 6 7 lines 9 10 input-b.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 05 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 06: ./chop col 3 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop col 3 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 06 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 07: ./chop cols 2 25 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop cols 2 25 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 07 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 08: ./chop col 1 col 1 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop col 1 col 1 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 08 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 09: ./chop cols 19 22 cols 27 33 col 32 input-b.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop cols 19 22 cols 27 33 col 32 input-b.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 09 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 10: ./chop line 1 cols 13 17 lines 3 5 col 2 input-b.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop line 1 cols 13 17 lines 3 5 col 2 input-b.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 10 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 11: ./chop lines 22 38 lines 49 61 cols 22 31 lines 55 63 input-c.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop lines 22 38 lines 49 61 cols 22 31 lines 55 63 input-c.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 11 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 12: ./chop lines 21 41 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop lines 21 41 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 12 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 13: ./chop cols 16 35 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop cols 16 35 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 13 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 14: ./chop col 2 col 11 cols 18 21 input-d.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop col 2 col 11 cols 18 21 input-d.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 14 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 15: ./chop cols 5 10 lines 10 15 - output.txt < input-a.txt > stdout.txt 2> stderr.txt"
    ./chop cols 5 10 lines 10 15 - output.txt < input-a.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 15 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 16: ./chop cols 5 10 lines 10 15 input-a.txt - > stdout.txt 2> stderr.txt"
    ./chop cols 5 10 lines 10 15 input-a.txt - > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 16 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 17: ./chop col 1 line 1 input-e.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop col 1 line 1 input-e.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 17 1 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 18: ./chop col 1 line 1 input-f.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop col 1 line 1 input-f.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 18 1 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 19: ./chop col 1 line 1 not-a-file.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop col 1 line 1 not-a-file.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 19 1 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 20: ./chop garbage 1 2 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop garbage 1 2 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 20 1 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 21: ./chop cols 10 5 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop cols 10 5 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 21 1 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 22: ./chop cols 10 5 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./chop lines 5 seven input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 22 1 $STATUS
else
    echo "**** Your program couldn't be tested since it didn't compile successfully."
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
