#!/bin/bash
FAIL=0

# Function to run a test and check its output and exit status against expected.
checkResults() {
  TESTNO=$1
  ESTATUS=$2
  STATUS=$3

  # Make sffure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test $TESTNO FAILED - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure the output matches the expected output.
  if ! diff -q expected-$TESTNO.ppm output.ppm >/dev/null 2>&1 ; then
      echo "**** Test $TESTNO FAILED - output didn't match the expected output"
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Build and test each of the programs.
make clean

make brighten

# Run tests for just the brighten program
if [ -x brighten ] ; then
    echo "Test 01 : ./brighten < image-1.ppm > output.ppm"
    ./brighten < image-1.ppm > output.ppm
    checkResults 01 0 $?

    echo "Test 02 : ./brighten < image-2.ppm > output.ppm"
    ./brighten < image-2.ppm > output.ppm
    checkResults 02 0 $?

    echo "Test 03 : ./brighten < image-4.ppm | ./brighten | ./brighten > output.ppm"
    ./brighten < image-4.ppm | ./brighten | ./brighten > output.ppm
    checkResults 03 0 $?

    echo "Test 04 : ./brighten < image-6.ppm > output.ppm"
    ./brighten < image-6.ppm > output.ppm
    checkResults 04 100 $?

else
    echo "**** The brighten program didn't compile successfully"
    FAIL=1
fi

make border

# Run tests for just the border program
if [ -x border ] ; then
    echo "Test 05 : ./border < image-3.ppm > output.ppm"
    ./border < image-3.ppm > output.ppm
    checkResults 05 0 $?

    echo "Test 06 : ./border < image-4.ppm > output.ppm"
    ./border < image-4.ppm > output.ppm
    checkResults 06 0 $?

    echo "Test 07 : ./border < image-4.ppm | ./border | ./border > output.ppm"
    ./border < image-4.ppm | ./border | ./border > output.ppm
    checkResults 07 0 $?

    echo "Test 08 : ./border < image-7.ppm > output.ppm"
    ./border < image-7.ppm > output.ppm
    checkResults 08 100 $?

else
    echo "**** The border program didn't compile successfully"
    FAIL=1
fi

make blur

# Run tests for just the border program
if [ -x blur ] ; then
    echo "Test 09 : ./blur < image-4.ppm > output.ppm"
    ./blur < image-4.ppm > output.ppm
    checkResults 09 0 $?

    echo "Test 10 : ./blur < image-5.ppm > output.ppm"
    ./blur < image-5.ppm > output.ppm
    checkResults 10 0 $?

    echo "Test 11 : ./blur < image-4.ppm | ./blur | ./blur > output.ppm"
    ./blur < image-4.ppm | ./blur | ./blur > output.ppm
    checkResults 11 0 $?

    echo "Test 12 : ./blur < image-8.ppm > output.ppm"
    ./blur < image-8.ppm > output.ppm
    checkResults 12 101 $?

    echo "Test 13 : ./blur < image-9.ppm > output.ppm"
    ./blur < image-9.ppm > output.ppm
    checkResults 13 101 $?

else
    echo "**** The blur program didn't compile successfully"
    FAIL=1
fi

# Run tests for just the border program
if [ -x brighten ] && [ -x border ] && [ -x blur ] ; then
    echo "Test 14 : ./brighten < image-5.ppm | ./blur | ./border > output.ppm"
    ./brighten < image-5.ppm | ./blur | ./border > output.ppm
    checkResults 14 0 $?

    echo "Test 15 : ./border < image-5.ppm | ./blur | ./brighten > output.ppm"
    ./border < image-5.ppm | ./blur | ./brighten > output.ppm
    checkResults 15 0 $?

else
    echo "**** At least one of your programs didn't compile successfully."
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
