# Test file for "Bit Manipulation"


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

#------------------
test "init"
setPINA 0x00
continue 2
expectPORTC 0x00
expectPORTB 0x00
checkResult

#-----------------
test "first state in sequence"
setPINA 0x04
continue 2
expectPORTC 0x01
expectPORTB 0x00
checkResult

#-------------------
test "release button"
setPINA 0x00
continue 2
expectPORTC 0x02
expectPORTB 0x00
checkResult

#-----------------
test "second state in sequence"
setPINA 0x02
continue 2
expectPORTC 0x03
expectPORTB 0x01
checkResult

#------------------
test "hold second state"
setPINA 0x02
continue 2
expectPORTC 0x03
expectPORTB 0x01
checkResult

#-------------------
test "hold completed state after button release"
setPINA 0x00
continue 2 
expectPORTC 0x04
expectPORTB 0x01
checkResult

#-------------------------------------------------------------
#to lock

#-------------------
test "hold pound"
setPINA 0x04
continue 2
expectPORTC 0x05
expectPORTB 0x01
checkResult

#-------------------
test "release pound"
setPINA 0x00
continue 2
expectPORTC 0x06
expectPORTB 0x01
checkResult

#--------------------
test "hold y"
setPINA 0x02
continue 2 
expectPORTC 0x07
expectPORTB 0x00
checkResult

#--------------------
test "should be back to the start"
setPINA 0x00
continue 2 
expectPORTC 0x00
expectPORTB 0x00
checkResult




# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
