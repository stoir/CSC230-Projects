#!/bin/bash
FAIL=0

# Function to run a test.  Expects the arguments in the variable, args, which
# is probably bad.
runTest() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt stderr.txt
  
  echo "Test $TESTNO: ./reading ${args[@]} < input-$TESTNO.txt > output.txt 2> stderr.txt"
  ./reading ${args[@]} < input-$TESTNO.txt > output.txt 2> stderr.txt
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** FAILED - Expected an exit status toast $ESTATUS, but got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure output matches expected output.
  if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
      echo "**** FAILED - output to stdout didn't match expected."
      FAIL=1
      return 1
  fi

  # Make sure the output to stderr matches expected.
  if [ -f estderr-$TESTNO.txt ] &&
	 ! diff -q estderr-$TESTNO.txt stderr.txt >/dev/null 2>&1 ; then
      echo "**** FAILED - output to stderr didn't match expected"
      FAIL=1
      return 1
  fi

  echo "PASS"
  return 0
}

# Try to get a fresh compile of the project.
make clean
make
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your program."
    FAIL=1
fi

# Run individual tests.
if [ -x reading ] ; then
    args=(list-a.txt)
    runTest 01 0
    
    args=(list-b.txt)
    runTest 02 0

    args=(list-c.txt)
    runTest 03 0

    args=(list-d.txt)
    runTest 04 0

    args=(list-a.txt list-b.txt list-c.txt)
    runTest 05 0

    args=(list-c.txt list-d.txt)
    runTest 06 0

    args=(list-c.txt list-d.txt)
    runTest 07 0

    args=(list-c.txt list-d.txt)
    runTest 08 0

    args=(list-d.txt)
    runTest 09 0

    args=(list-a.txt list-b.txt list-c.txt list-d.txt)
    runTest 10 0

    args=(list-a.txt list-b.txt list-c.txt list-d.txt)
    runTest 11 0
    
    args=(list-d.txt)
    runTest 12 0

    args=(list-a.txt list-b.txt list-c.txt list-d.txt list-e.txt)
    runTest 13 0
    
    args=(list-z.txt)
    runTest 14 1
    
    args=()
    runTest 15 1
    
    args=(list-b.txt)
    runTest 16 0
    
    args=(list-d.txt)
    runTest 17 0
    
    args=(list-c.txt list-f.txt)
    runTest 18 1
    
    args=(list-g.txt)
    runTest 19 1
    
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
