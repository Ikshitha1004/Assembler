.class_metadata
class_count 1
class_begin IOHandler None
field_count 0
method_count 10
method readChar IOHandler.readChar
method writeChar IOHandler.writeChar@C
method readString IOHandler.readString@[C@I
method stringToInt IOHandler.stringToInt@[C
method intToString IOHandler.intToString@I@[C
method doubleToString IOHandler.doubleToString@F@[C
method readInt IOHandler.readInt
method printString IOHandler.printString@[C
method printInt IOHandler.printInt@I
method printDouble IOHandler.printDouble@F
class_end
.end_metadata

.code

.method IOHandler.readChar
.limit stack 4
.limit locals 3
PUSH 1
NEWARRAY C
STORE 1 ; Store new flattened array to 'c'
PUSH 0
LOAD 1  ; Load local var c
PUSH 1
POP
LOAD 1 ; Load array variable 'c'
PUSH 0
ALOAD
STORE 2 ; Store to local 'ans'
LOAD 2  ; Load local var ans
RET
.endmethod

.method IOHandler.writeChar@C
.limit stack 4
.limit locals 4
PUSH 1
NEWARRAY C
STORE 3 ; Store new flattened array to 'arr'
LOAD 3 ; Load array variable 'arr'
PUSH 0
LOAD_ARG 1  ; Load parameter 'c'
ASTORE ; Store to array element
PUSH 1
LOAD 3  ; Load local var arr
PUSH 1
POP
.endmethod

.method IOHandler.readString@[C@I
.limit stack 4
.limit locals 6
PUSH 0
STORE 4 ; Init i
L0:
LOAD 4  ; Load local var i
LOAD_ARG 2  ; Load parameter 'size'
PUSH 1
ISUB
ICMP_LT
JNZ L1
JMP L2
L1:
LOAD_ARG 0 ; Load 'this' for method call
INVOKEVIRTUAL 0 ; Call IOHandler.readChar
STORE 5 ; Store to local 'c'
LOAD 5  ; Load local var c
PUSH 92 ; Push ASCII for char '\n'
ICMP_EQ
JNZ L3
JMP L5
L5:
LOAD 5  ; Load local var c
PUSH 92 ; Push ASCII for char '\r'
ICMP_EQ
JNZ L3
JMP L4
L3:
JMP L2 ; BREAK
L4:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 4  ; Load local var i
LOAD 5  ; Load local var c
ASTORE ; Store to array element
LOAD 4 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 4 ; Post increment
POP
JMP L0
L2:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 4  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
.endmethod

.method IOHandler.stringToInt@[C
.limit stack 4
.limit locals 9
PUSH 1
STORE 8 ; Init sign
PUSH 0
STORE 7 ; Init num
PUSH 0
STORE 6 ; Init i
LOAD_ARG 1 ; Load array parameter 'arr'
PUSH 0
ALOAD
PUSH 45 ; Push ASCII for char '-'
ICMP_EQ
JNZ L6
JMP L7
L6:
PUSH 1
INEG
STORE 8 ; Store to local 'sign'
LOAD 6 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 6 ; Post increment
POP
L7:
L8:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 6  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L9
JMP L10
L9:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 6  ; Load local var i
ALOAD
PUSH 48 ; Push ASCII for char '0'
ICMP_GE
JNZ L14
JMP L12
L14:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 6  ; Load local var i
ALOAD
PUSH 57 ; Push ASCII for char '9'
ICMP_LE
JNZ L11
JMP L12
L11:
LOAD 7  ; Load local var num
PUSH 10
IMUL
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 6  ; Load local var i
ALOAD
PUSH 48 ; Push ASCII for char '0'
ISUB
IADD
STORE 7 ; Store to local 'num'
JMP L13
L12:
JMP L10 ; BREAK
L13:
LOAD 6 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 6 ; Post increment
POP
JMP L8
L10:
LOAD 8  ; Load local var sign
LOAD 7  ; Load local var num
IMUL
RET
.endmethod

.method IOHandler.intToString@I@[C
.limit stack 4
.limit locals 13
PUSH 0
STORE 9 ; Init i
PUSH 0
STORE 10 ; Init isNeg
LOAD_ARG 1  ; Load parameter 'x'
PUSH 0
ICMP_EQ
JNZ L15
JMP L16
L15:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 9 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 9 ; Post increment
PUSH 48 ; Push ASCII for char '0'
ASTORE ; Store to array element
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 9  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
L16:
LOAD_ARG 1  ; Load parameter 'x'
PUSH 0
ICMP_LT
JNZ L17
JMP L18
L17:
PUSH 1
STORE 10 ; Store to local 'isNeg'
LOAD_ARG 1  ; Load parameter 'x'
INEG
STORE 1 ; Store to local 'x'
L18:
L19:
LOAD_ARG 1  ; Load parameter 'x'
PUSH 0
ICMP_GT
JNZ L20
JMP L21
L20:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 9 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 9 ; Post increment
LOAD_ARG 1  ; Load parameter 'x'
PUSH 10
IMOD
PUSH 48 ; Push ASCII for char '0'
IADD
ASTORE ; Store to array element
PUSH 10
STORE 1 ; Store to local 'x'
JMP L19
L21:
LOAD 10  ; Load local var isNeg
PUSH 1
ICMP_EQ
JNZ L22
JMP L23
L22:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 9 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 9 ; Post increment
PUSH 45 ; Push ASCII for char '-'
ASTORE ; Store to array element
L23:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 9  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
PUSH 0
STORE 11 ; Init j
JMP L24
L25:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 11  ; Load local var j
ALOAD
STORE 12 ; Init temp
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 11  ; Load local var j
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 9  ; Load local var i
LOAD 11  ; Load local var j
ISUB
PUSH 1
ISUB
ALOAD
ASTORE ; Store to array element
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 9  ; Load local var i
LOAD 11  ; Load local var j
ISUB
PUSH 1
ISUB
LOAD 12  ; Load local var temp
ASTORE ; Store to array element
L26:
LOAD 11 ; Load current value of j for post increment
DUP
PUSH 1
IADD
STORE 11 ; Post increment
L24:
LOAD 11  ; Load local var j
LOAD 9  ; Load local var i
PUSH 2
IDIV
ICMP_LT
JNZ L25
JMP L27
L27:
.endmethod

.method IOHandler.doubleToString@F@[C
.limit stack 4
.limit locals 20
LOAD_ARG 1  ; Load parameter 'val'
STORE 13 ; Init intPart
LOAD_ARG 1  ; Load parameter 'val'
LOAD 13  ; Load local var intPart
FSUB
STORE 14 ; Init fracPart
LOAD 14  ; Load local var fracPart
PUSH 0
ICMP_LT
JNZ L28
JMP L29
L28:
LOAD 14  ; Load local var fracPart
FNEG
STORE 14 ; Store to local 'fracPart'
L29:
PUSH 50
NEWARRAY C
STORE 15 ; Store new flattened array to 'intBuf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD 13  ; Load local var intPart
LOAD 15  ; Load local var intBuf
INVOKEVIRTUAL 4 ; Call IOHandler.intToString@I@[C
PUSH 0
STORE 17 ; Init j
PUSH 0
STORE 16 ; Init i
L30:
LOAD 15 ; Load array variable 'intBuf'
LOAD 17  ; Load local var j
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L31
JMP L32
L31:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 16  ; Load local var i
LOAD 15 ; Load array variable 'intBuf'
LOAD 17  ; Load local var j
ALOAD
ASTORE ; Store to array element
LOAD 16 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 16 ; Post increment
POP
LOAD 17 ; Load current value of j for post increment
DUP
PUSH 1
IADD
STORE 17 ; Post increment
POP
JMP L30
L32:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 16 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 16 ; Post increment
PUSH 46 ; Push ASCII for char '.'
ASTORE ; Store to array element
PUSH 0
STORE 18 ; Init k
JMP L33
L34:
LOAD 14  ; Load local var fracPart
PUSH 10
FMUL
STORE 14 ; Store to local 'fracPart'
LOAD 14  ; Load local var fracPart
STORE 19 ; Init digit
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 16 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 16 ; Post increment
PUSH 48 ; Push ASCII for char '0'
LOAD 19  ; Load local var digit
IADD
ASTORE ; Store to array element
LOAD 14  ; Load local var fracPart
LOAD 19  ; Load local var digit
FSUB
STORE 14 ; Store to local 'fracPart'
L35:
LOAD 18 ; Load current value of k for post increment
DUP
PUSH 1
IADD
STORE 18 ; Post increment
L33:
LOAD 18  ; Load local var k
PUSH 6
ICMP_LT
JNZ L34
JMP L36
L36:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 16  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
.endmethod

.method IOHandler.readInt
.limit stack 4
.limit locals 21
PUSH 50
NEWARRAY C
STORE 20 ; Store new flattened array to 'buf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD 20  ; Load local var buf
PUSH 50
INVOKEVIRTUAL 2 ; Call IOHandler.readString@[C@I
LOAD_ARG 0 ; Load 'this' for method call
LOAD 20  ; Load local var buf
INVOKEVIRTUAL 3 ; Call IOHandler.stringToInt@[C
RET
.endmethod

.method IOHandler.printString@[C
.limit stack 4
.limit locals 22
PUSH 0
STORE 21 ; Init i
L37:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 21  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L38
JMP L39
L38:
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 21  ; Load local var i
ALOAD
INVOKEVIRTUAL 1 ; Call IOHandler.writeChar@C
LOAD 21 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 21 ; Post increment
POP
JMP L37
L39:
.endmethod

.method IOHandler.printInt@I
.limit stack 4
.limit locals 23
PUSH 50
NEWARRAY C
STORE 22 ; Store new flattened array to 'buf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'x'
LOAD 22  ; Load local var buf
INVOKEVIRTUAL 4 ; Call IOHandler.intToString@I@[C
LOAD_ARG 0 ; Load 'this' for method call
LOAD 22  ; Load local var buf
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
.endmethod

.method IOHandler.printDouble@F
.limit stack 4
.limit locals 24
PUSH 100
NEWARRAY C
STORE 23 ; Store new flattened array to 'buf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'x'
LOAD 23  ; Load local var buf
INVOKEVIRTUAL 5 ; Call IOHandler.doubleToString@F@[C
LOAD_ARG 0 ; Load 'this' for method call
LOAD 23  ; Load local var buf
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
.endmethod
