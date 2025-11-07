.class_metadata
class_count 6
class_begin IOHandler None
field_count 0
method_count 11
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
method IOHandler IOHandler.IOHandler
class_end
class_begin StringHandler None
field_count 0
method_count 13
method length StringHandler.length@[C
method substr StringHandler.substr@[C@I@I@[C
method compare StringHandler.compare@[C@[C
method insert StringHandler.insert@[C@I@C
method erase StringHandler.erase@[C@I@I
method islower StringHandler.islower@C
method isupper StringHandler.isupper@C
method tolower StringHandler.tolower@C
method toupper StringHandler.toupper@C
method isalpha StringHandler.isalpha@C
method isalnum StringHandler.isalnum@C
method isnum StringHandler.isnum@C
method StringHandler StringHandler.StringHandler
class_end
class_begin FileHandler None
field_count 2
field fd I 0
field isOpen I 0
method_count 6
method FileHandler FileHandler.FileHandler
method fopen FileHandler.fopen@[C@I
method fclose FileHandler.fclose
method fread FileHandler.fread@[C@I
method fwrite FileHandler.fwrite@[C@I
method is_open FileHandler.is_open
class_end
class_begin Termios None
field_count 1
field clflag I 0
method_count 2
method Termios Termios.Termios
method copyFrom Termios.copyFrom@Termios
class_end
class_begin TerminalHandler None
field_count 1
field Eorigtermios Termios 3
method_count 5
method disableRawMode TerminalHandler.disableRawMode
method enableRawMode TerminalHandler.enableRawMode
method tcgetattr TerminalHandler.tcgetattr@I@Termios
method tcsetattr TerminalHandler.tcsetattr@I@I@Termios
method TerminalHandler TerminalHandler.TerminalHandler
class_end
class_begin Editor None
field_count 9
field rows [[C 3
field rowCount I 0
field filename [C 3
field cx I 0
field cy I 0
field insert_mode I 0
field command_mode I 0
field command_buffer [C 3
field status_msg [C 3
method_count 1
method Editor Editor.Editor
class_end
.end_metadata

.code

.method IOHandler.readChar
.limit stack 4
.limit locals 2
PUSH 1
NEWARRAY C
STORE 0 ; Store new flattened array to 'c'
LOAD 0  ; Load local var c
PUSH 1
PUSH 0
SYS_CALL READ ; read
POP
LOAD 0 ; Load array variable 'c'
PUSH 0
ALOAD
STORE 1 ; Store to local 'ans'
LOAD 1  ; Load local var ans
RET
.endmethod

.method IOHandler.writeChar@C
.limit stack 4
.limit locals 4
LOAD_ARG 0 ; Copy arg 'c' to local
STORE 2
PUSH 1
NEWARRAY C
STORE 3 ; Store new flattened array to 'arr'
LOAD 3 ; Load array variable 'arr'
PUSH 0
LOAD 2  ; Load parameter 'c'
ASTORE ; Store to array element
LOAD 3  ; Load local var arr
PUSH 1
PUSH 1
SYS_CALL WRITE ; write
POP
RET
.endmethod

.method IOHandler.readString@[C@I
.limit stack 4
.limit locals 8
LOAD_ARG 0 ; Copy arg 'arr' to local
STORE 4
LOAD_ARG 1 ; Copy arg 'size' to local
STORE 5
PUSH 0
STORE 6 ; Init i
L0:
LOAD 6  ; Load local var i
LOAD 5  ; Load parameter 'size'
PUSH 1
ISUB
ICMP_LT
JNZ L1
JMP L2
L1:
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 0 ; Call IOHandler.readChar
STORE 7 ; Store to local 'c'
LOAD 7  ; Load local var c
PUSH 92 ; Push ASCII for char '\n'
ICMP_EQ
JNZ L3
JMP L5
L5:
LOAD 7  ; Load local var c
PUSH 92 ; Push ASCII for char '\r'
ICMP_EQ
JNZ L3
JMP L4
L3:
JMP L2 ; BREAK
L4:
LOAD_ARG 4 ; Load array parameter 'arr'
LOAD 6  ; Load local var i
LOAD 7  ; Load local var c
ASTORE ; Store to array element
LOAD 6 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 6 ; Store local 'i'
POP
JMP L0
L2:
LOAD_ARG 4 ; Load array parameter 'arr'
LOAD 6  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
.endmethod

.method IOHandler.stringToInt@[C
.limit stack 4
.limit locals 12
LOAD_ARG 0 ; Copy arg 'arr' to local
STORE 8
PUSH 1
STORE 11 ; Init sign
PUSH 0
STORE 10 ; Init num
PUSH 0
STORE 9 ; Init i
LOAD_ARG 8 ; Load array parameter 'arr'
PUSH 0
ALOAD
PUSH 45 ; Push ASCII for char '-'
ICMP_EQ
JNZ L6
JMP L7
L6:
PUSH 1
INEG
STORE 11 ; Store to local 'sign'
LOAD 9 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 9 ; Store local 'i'
POP
L7:
L8:
LOAD_ARG 8 ; Load array parameter 'arr'
LOAD 9  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L9
JMP L10
L9:
LOAD_ARG 8 ; Load array parameter 'arr'
LOAD 9  ; Load local var i
ALOAD
PUSH 48 ; Push ASCII for char '0'
ICMP_GEQ
JNZ L14
JMP L12
L14:
LOAD_ARG 8 ; Load array parameter 'arr'
LOAD 9  ; Load local var i
ALOAD
PUSH 57 ; Push ASCII for char '9'
ICMP_LEQ
JNZ L11
JMP L12
L11:
LOAD 10  ; Load local var num
PUSH 10
IMUL
LOAD_ARG 8 ; Load array parameter 'arr'
LOAD 9  ; Load local var i
ALOAD
PUSH 48 ; Push ASCII for char '0'
ISUB
IADD
STORE 10 ; Store to local 'num'
JMP L13
L12:
JMP L10 ; BREAK
L13:
LOAD 9 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 9 ; Store local 'i'
POP
JMP L8
L10:
LOAD 11  ; Load local var sign
LOAD 10  ; Load local var num
IMUL
RET
.endmethod

.method IOHandler.intToString@I@[C
.limit stack 4
.limit locals 18
LOAD_ARG 0 ; Copy arg 'x' to local
STORE 12
LOAD_ARG 1 ; Copy arg 'arr' to local
STORE 13
PUSH 0
STORE 14 ; Init i
PUSH 0
STORE 15 ; Init isNeg
LOAD 12  ; Load parameter 'x'
PUSH 0
ICMP_EQ
JNZ L15
JMP L16
L15:
LOAD_ARG 13 ; Load array parameter 'arr'
LOAD 14 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 14 ; Store local 'i'
PUSH 48 ; Push ASCII for char '0'
ASTORE ; Store to array element
LOAD_ARG 13 ; Load array parameter 'arr'
LOAD 14  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
L16:
LOAD 12  ; Load parameter 'x'
PUSH 0
ICMP_LT
JNZ L17
JMP L18
L17:
PUSH 1
STORE 15 ; Store to local 'isNeg'
LOAD 12  ; Load parameter 'x'
INEG
STORE 12 ; Store to local 'x'
L18:
L19:
LOAD 12  ; Load parameter 'x'
PUSH 0
ICMP_GT
JNZ L20
JMP L21
L20:
LOAD_ARG 13 ; Load array parameter 'arr'
LOAD 14 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 14 ; Store local 'i'
LOAD 12  ; Load parameter 'x'
PUSH 10
IMOD
PUSH 48 ; Push ASCII for char '0'
IADD
ASTORE ; Store to array element
PUSH 10
STORE 12 ; Store to local 'x'
JMP L19
L21:
LOAD 15  ; Load local var isNeg
PUSH 1
ICMP_EQ
JNZ L22
JMP L23
L22:
LOAD_ARG 13 ; Load array parameter 'arr'
LOAD 14 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 14 ; Store local 'i'
PUSH 45 ; Push ASCII for char '-'
ASTORE ; Store to array element
L23:
LOAD_ARG 13 ; Load array parameter 'arr'
LOAD 14  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
PUSH 0
STORE 16 ; Init j
JMP L24
L25:
LOAD_ARG 13 ; Load array parameter 'arr'
LOAD 16  ; Load local var j
ALOAD
STORE 17 ; Init temp
LOAD_ARG 13 ; Load array parameter 'arr'
LOAD 16  ; Load local var j
LOAD_ARG 13 ; Load array parameter 'arr'
LOAD 14  ; Load local var i
LOAD 16  ; Load local var j
ISUB
PUSH 1
ISUB
ALOAD
ASTORE ; Store to array element
LOAD_ARG 13 ; Load array parameter 'arr'
LOAD 14  ; Load local var i
LOAD 16  ; Load local var j
ISUB
PUSH 1
ISUB
LOAD 17  ; Load local var temp
ASTORE ; Store to array element
L26:
LOAD 16 ; Load local 'j'
DUP
PUSH 1
IADD ; ++
STORE 16 ; Store local 'j'
L24:
LOAD 16  ; Load local var j
LOAD 14  ; Load local var i
PUSH 2
IDIV
ICMP_LT
JNZ L25
JMP L27
L27:
RET
.endmethod

.method IOHandler.doubleToString@F@[C
.limit stack 4
.limit locals 29
LOAD_ARG 0 ; Copy arg 'val' to local
STORE 18
LOAD_ARG 1 ; Copy arg 'arr' to local
STORE 19
PUSH 0
STORE 20 ; Init neg
LOAD 18  ; Load parameter 'val'
PUSH 0
ICMP_LT
JNZ L28
JMP L29
L28:
PUSH 1
STORE 20 ; Store to local 'neg'
LOAD 18  ; Load parameter 'val'
FNEG
STORE 18 ; Store to local 'val'
L29:
PUSH 0
STORE 21 ; Init intPart
LOAD 18  ; Load parameter 'val'
STORE 22 ; Init temp
L30:
LOAD 22  ; Load local var temp
FPUSH 1.0
FCMP_GEQ
JNZ L31
JMP L32
L31:
LOAD 22  ; Load local var temp
FPUSH 1.0
FSUB
STORE 22 ; Store to local 'temp'
LOAD 21  ; Load local var intPart
PUSH 1
IADD
STORE 21 ; Store to local 'intPart'
JMP L30
L32:
LOAD 18  ; Load parameter 'val'
LOAD 21  ; Load local var intPart
FSUB
STORE 23 ; Init frac
PUSH 50
NEWARRAY C
STORE 24 ; Store new flattened array to 'intBuf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD 21  ; Load local var intPart
LOAD 24  ; Load local var intBuf
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 4 ; Call IOHandler.intToString@I@[C
PUSH 0
STORE 26 ; Init j
PUSH 0
STORE 25 ; Init i
LOAD 20  ; Load local var neg
PUSH 1
ICMP_EQ
JNZ L33
JMP L34
L33:
LOAD_ARG 19 ; Load array parameter 'arr'
LOAD 25 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 25 ; Store local 'i'
PUSH 45 ; Push ASCII for char '-'
ASTORE ; Store to array element
L34:
L35:
LOAD 24 ; Load array variable 'intBuf'
LOAD 26  ; Load local var j
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L36
JMP L37
L36:
LOAD_ARG 19 ; Load array parameter 'arr'
LOAD 25 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 25 ; Store local 'i'
LOAD 24 ; Load array variable 'intBuf'
LOAD 26 ; Load local 'j'
DUP
PUSH 1
IADD ; ++
STORE 26 ; Store local 'j'
ALOAD
ASTORE ; Store to array element
JMP L35
L37:
LOAD_ARG 19 ; Load array parameter 'arr'
LOAD 25 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 25 ; Store local 'i'
PUSH 46 ; Push ASCII for char '.'
ASTORE ; Store to array element
PUSH 0
STORE 27 ; Init k
JMP L38
L39:
LOAD 23  ; Load local var frac
FPUSH 10.0
FMUL
STORE 23 ; Store to local 'frac'
PUSH 0
STORE 28 ; Init digit
L42:
LOAD 23  ; Load local var frac
FPUSH 1.0
FCMP_GEQ
JNZ L43
JMP L44
L43:
LOAD 23  ; Load local var frac
FPUSH 1.0
FSUB
STORE 23 ; Store to local 'frac'
LOAD 28  ; Load local var digit
PUSH 1
IADD
STORE 28 ; Store to local 'digit'
JMP L42
L44:
LOAD_ARG 19 ; Load array parameter 'arr'
LOAD 25 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 25 ; Store local 'i'
PUSH 48 ; Push ASCII for char '0'
LOAD 28  ; Load local var digit
IADD
ASTORE ; Store to array element
L40:
LOAD 27 ; Load local 'k'
DUP
PUSH 1
IADD ; ++
STORE 27 ; Store local 'k'
L38:
LOAD 27  ; Load local var k
PUSH 6
ICMP_LT
JNZ L39
JMP L41
L41:
LOAD_ARG 19 ; Load array parameter 'arr'
LOAD 25  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
.endmethod

.method IOHandler.readInt
.limit stack 4
.limit locals 30
PUSH 50
NEWARRAY C
STORE 29 ; Store new flattened array to 'buf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD 29  ; Load local var buf
PUSH 50
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 2 ; Call IOHandler.readString@[C@I
LOAD_ARG 0 ; Load 'this' for method call
LOAD 29  ; Load local var buf
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 3 ; Call IOHandler.stringToInt@[C
RET
.endmethod

.method IOHandler.printString@[C
.limit stack 4
.limit locals 32
LOAD_ARG 0 ; Copy arg 'arr' to local
STORE 30
PUSH 0
STORE 31 ; Init i
L45:
LOAD_ARG 30 ; Load array parameter 'arr'
LOAD 31  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L46
JMP L47
L46:
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 30 ; Load array parameter 'arr'
LOAD 31  ; Load local var i
ALOAD
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 1 ; Call IOHandler.writeChar@C
LOAD 31 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 31 ; Store local 'i'
POP
JMP L45
L47:
RET
.endmethod

.method IOHandler.printInt@I
.limit stack 4
.limit locals 34
LOAD_ARG 0 ; Copy arg 'x' to local
STORE 32
PUSH 50
NEWARRAY C
STORE 33 ; Store new flattened array to 'buf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD 32  ; Load parameter 'x'
LOAD 33  ; Load local var buf
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 4 ; Call IOHandler.intToString@I@[C
LOAD_ARG 0 ; Load 'this' for method call
LOAD 33  ; Load local var buf
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
RET
.endmethod

.method IOHandler.printDouble@F
.limit stack 4
.limit locals 36
LOAD_ARG 0 ; Copy arg 'x' to local
STORE 34
PUSH 100
NEWARRAY C
STORE 35 ; Store new flattened array to 'buf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD 34  ; Load parameter 'x'
LOAD 35  ; Load local var buf
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 5 ; Call IOHandler.doubleToString@F@[C
LOAD_ARG 0 ; Load 'this' for method call
LOAD 35  ; Load local var buf
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
RET
.endmethod

.method IOHandler.IOHandler
.limit stack 10
.limit locals 1
RET
.endmethod

.method StringHandler.length@[C
.limit stack 4
.limit locals 38
LOAD_ARG 0 ; Copy arg 'str' to local
STORE 36
PUSH 0
STORE 37 ; Init len
L48:
LOAD_ARG 36 ; Load array parameter 'str'
LOAD 37  ; Load local var len
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L49
JMP L50
L49:
LOAD 37 ; Load local 'len'
DUP
PUSH 1
IADD ; ++
STORE 37 ; Store local 'len'
POP
JMP L48
L50:
LOAD 37  ; Load local var len
RET
.endmethod

.method StringHandler.substr@[C@I@I@[C
.limit stack 4
.limit locals 44
LOAD_ARG 0 ; Copy arg 'str' to local
STORE 38
LOAD_ARG 1 ; Copy arg 'start' to local
STORE 39
LOAD_ARG 2 ; Copy arg 'len' to local
STORE 40
LOAD_ARG 3 ; Copy arg 'res' to local
STORE 41
LOAD_ARG 0 ; Load 'this' for method call
LOAD 38  ; Load parameter 'str'
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 42 ; Init n
LOAD 39  ; Load parameter 'start'
PUSH 0
ICMP_LT
JNZ L51
JMP L53
L53:
LOAD 39  ; Load parameter 'start'
LOAD 42  ; Load local var n
ICMP_GEQ
JNZ L51
JMP L52
L51:
LOAD_ARG 41 ; Load array parameter 'res'
PUSH 0
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
L52:
PUSH 0
STORE 43 ; Store to local 'i'
JMP L54
L55:
LOAD_ARG 41 ; Load array parameter 'res'
LOAD 43  ; Load local var i
LOAD_ARG 38 ; Load array parameter 'str'
LOAD 39  ; Load parameter 'start'
LOAD 43  ; Load local var i
IADD
ALOAD
ASTORE ; Store to array element
L56:
LOAD 43 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 43 ; Store local 'i'
L54:
LOAD 43  ; Load local var i
LOAD 40  ; Load parameter 'len'
ICMP_LT
JNZ L58
JMP L57
L58:
LOAD 39  ; Load parameter 'start'
LOAD 43  ; Load local var i
IADD
LOAD 42  ; Load local var n
ICMP_LT
JNZ L55
JMP L57
L57:
LOAD_ARG 41 ; Load array parameter 'res'
LOAD 43  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
.endmethod

.method StringHandler.compare@[C@[C
.limit stack 4
.limit locals 47
LOAD_ARG 0 ; Copy arg 's1' to local
STORE 44
LOAD_ARG 1 ; Copy arg 's2' to local
STORE 45
PUSH 0
STORE 46 ; Init i
L59:
LOAD_ARG 44 ; Load array parameter 's1'
LOAD 46  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L62
JMP L61
L62:
LOAD_ARG 45 ; Load array parameter 's2'
LOAD 46  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L60
JMP L61
L60:
LOAD_ARG 44 ; Load array parameter 's1'
LOAD 46  ; Load local var i
ALOAD
LOAD_ARG 45 ; Load array parameter 's2'
LOAD 46  ; Load local var i
ALOAD
ICMP_NEQ
JNZ L63
JMP L64
L63:
LOAD_ARG 44 ; Load array parameter 's1'
LOAD 46  ; Load local var i
ALOAD
LOAD_ARG 45 ; Load array parameter 's2'
LOAD 46  ; Load local var i
ALOAD
ISUB
RET
L64:
LOAD 46 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 46 ; Store local 'i'
POP
JMP L59
L61:
LOAD_ARG 44 ; Load array parameter 's1'
LOAD 46  ; Load local var i
ALOAD
LOAD_ARG 45 ; Load array parameter 's2'
LOAD 46  ; Load local var i
ALOAD
ISUB
RET
.endmethod

.method StringHandler.insert@[C@I@C
.limit stack 4
.limit locals 52
LOAD_ARG 0 ; Copy arg 'str' to local
STORE 47
LOAD_ARG 1 ; Copy arg 'pos' to local
STORE 48
LOAD_ARG 2 ; Copy arg 'c' to local
STORE 49
LOAD_ARG 0 ; Load 'this' for method call
LOAD 47  ; Load parameter 'str'
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 50 ; Init n
LOAD 48  ; Load parameter 'pos'
PUSH 0
ICMP_LT
JNZ L65
JMP L66
L65:
PUSH 0
STORE 48 ; Store to local 'pos'
L66:
LOAD 48  ; Load parameter 'pos'
LOAD 50  ; Load local var n
ICMP_GT
JNZ L67
JMP L68
L67:
LOAD 50  ; Load local var n
STORE 48 ; Store to local 'pos'
L68:
LOAD 50  ; Load local var n
STORE 51 ; Init i
JMP L69
L70:
LOAD_ARG 47 ; Load array parameter 'str'
LOAD 51  ; Load local var i
PUSH 1
IADD
LOAD_ARG 47 ; Load array parameter 'str'
LOAD 51  ; Load local var i
ALOAD
ASTORE ; Store to array element
L71:
LOAD 51 ; Load local 'i'
DUP
PUSH 1
ISUB ; --
STORE 51 ; Store local 'i'
L69:
LOAD 51  ; Load local var i
LOAD 48  ; Load parameter 'pos'
ICMP_GEQ
JNZ L70
JMP L72
L72:
LOAD_ARG 47 ; Load array parameter 'str'
LOAD 48  ; Load parameter 'pos'
LOAD 49  ; Load parameter 'c'
ASTORE ; Store to array element
RET
.endmethod

.method StringHandler.erase@[C@I@I
.limit stack 4
.limit locals 57
LOAD_ARG 0 ; Copy arg 'str' to local
STORE 52
LOAD_ARG 1 ; Copy arg 'pos' to local
STORE 53
LOAD_ARG 2 ; Copy arg 'len' to local
STORE 54
LOAD_ARG 0 ; Load 'this' for method call
LOAD 52  ; Load parameter 'str'
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 55 ; Init n
LOAD 53  ; Load parameter 'pos'
PUSH 0
ICMP_LT
JNZ L73
JMP L75
L75:
LOAD 53  ; Load parameter 'pos'
LOAD 55  ; Load local var n
ICMP_GEQ
JNZ L73
JMP L74
L73:
RET
L74:
LOAD 53  ; Load parameter 'pos'
STORE 56 ; Init i
JMP L76
L77:
LOAD_ARG 52 ; Load array parameter 'str'
LOAD 56  ; Load local var i
LOAD_ARG 52 ; Load array parameter 'str'
LOAD 56  ; Load local var i
LOAD 54  ; Load parameter 'len'
IADD
ALOAD
ASTORE ; Store to array element
L78:
LOAD 56 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 56 ; Store local 'i'
L76:
LOAD 56  ; Load local var i
LOAD 54  ; Load parameter 'len'
IADD
LOAD 55  ; Load local var n
ICMP_LT
JNZ L77
JMP L79
L79:
LOAD_ARG 52 ; Load array parameter 'str'
LOAD 55  ; Load local var n
LOAD 54  ; Load parameter 'len'
ISUB
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
.endmethod

.method StringHandler.islower@C
.limit stack 4
.limit locals 58
LOAD_ARG 0 ; Copy arg 'c' to local
STORE 57
LOAD 57  ; Load parameter 'c'
PUSH 97 ; Push ASCII for char 'a'
ICMP_GEQ
JNZ L83
JMP L81
L83:
LOAD 57  ; Load parameter 'c'
PUSH 122 ; Push ASCII for char 'z'
ICMP_LEQ
JNZ L80
JMP L81
L80: ; Return true
PUSH 1
JMP L82
L81: ; Return false
PUSH 0
L82:
RET
.endmethod

.method StringHandler.isupper@C
.limit stack 4
.limit locals 59
LOAD_ARG 0 ; Copy arg 'c' to local
STORE 58
LOAD 58  ; Load parameter 'c'
PUSH 65 ; Push ASCII for char 'A'
ICMP_GEQ
JNZ L87
JMP L85
L87:
LOAD 58  ; Load parameter 'c'
PUSH 90 ; Push ASCII for char 'Z'
ICMP_LEQ
JNZ L84
JMP L85
L84: ; Return true
PUSH 1
JMP L86
L85: ; Return false
PUSH 0
L86:
RET
.endmethod

.method StringHandler.tolower@C
.limit stack 4
.limit locals 60
LOAD_ARG 0 ; Copy arg 'c' to local
STORE 59
LOAD_ARG 0 ; Load 'this' for method call
LOAD 59  ; Load parameter 'c'
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 6 ; Call StringHandler.isupper@C
PUSH 1
ICMP_EQ
JNZ L88
JMP L89
L88:
LOAD 59  ; Load parameter 'c'
PUSH 97 ; Push ASCII for char 'a'
PUSH 65 ; Push ASCII for char 'A'
ISUB
IADD
RET
L89:
LOAD 59  ; Load parameter 'c'
RET
.endmethod

.method StringHandler.toupper@C
.limit stack 4
.limit locals 61
LOAD_ARG 0 ; Copy arg 'c' to local
STORE 60
LOAD_ARG 0 ; Load 'this' for method call
LOAD 60  ; Load parameter 'c'
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 5 ; Call StringHandler.islower@C
PUSH 1
ICMP_EQ
JNZ L90
JMP L91
L90:
LOAD 60  ; Load parameter 'c'
PUSH 97 ; Push ASCII for char 'a'
PUSH 65 ; Push ASCII for char 'A'
ISUB
ISUB
RET
L91:
LOAD 60  ; Load parameter 'c'
RET
.endmethod

.method StringHandler.isalpha@C
.limit stack 4
.limit locals 62
LOAD_ARG 0 ; Copy arg 'c' to local
STORE 61
LOAD_ARG 0 ; Load 'this' for method call
LOAD 61  ; Load parameter 'c'
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 5 ; Call StringHandler.islower@C
PUSH 1
ICMP_EQ
JNZ L92
JMP L95
L95:
LOAD_ARG 0 ; Load 'this' for method call
LOAD 61  ; Load parameter 'c'
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 6 ; Call StringHandler.isupper@C
PUSH 1
ICMP_EQ
JNZ L92
JMP L93
L92: ; Return true
PUSH 1
JMP L94
L93: ; Return false
PUSH 0
L94:
RET
.endmethod

.method StringHandler.isalnum@C
.limit stack 4
.limit locals 63
LOAD_ARG 0 ; Copy arg 'c' to local
STORE 62
LOAD_ARG 0 ; Load 'this' for method call
LOAD 62  ; Load parameter 'c'
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 9 ; Call StringHandler.isalpha@C
PUSH 1
ICMP_EQ
JNZ L96
JMP L99
L99:
LOAD_ARG 0 ; Load 'this' for method call
LOAD 62  ; Load parameter 'c'
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 11 ; Call StringHandler.isnum@C
PUSH 1
ICMP_EQ
JNZ L96
JMP L97
L96: ; Return true
PUSH 1
JMP L98
L97: ; Return false
PUSH 0
L98:
RET
.endmethod

.method StringHandler.isnum@C
.limit stack 4
.limit locals 64
LOAD_ARG 0 ; Copy arg 'c' to local
STORE 63
LOAD 63  ; Load parameter 'c'
PUSH 48 ; Push ASCII for char '0'
ICMP_GEQ
JNZ L103
JMP L101
L103:
LOAD 63  ; Load parameter 'c'
PUSH 57 ; Push ASCII for char '9'
ICMP_LEQ
JNZ L100
JMP L101
L100: ; Return true
PUSH 1
JMP L102
L101: ; Return false
PUSH 0
L102:
RET
.endmethod

.method StringHandler.StringHandler
.limit stack 10
.limit locals 1
RET
.endmethod

.method FileHandler.FileHandler
.limit stack 10
.limit locals 3
LOAD_ARG 0      ; Push 'this' reference for field 'fd'
PUSH 0      ; Default value for 'fd'
LOAD_ARG 0      ; Push 'this' reference for field 'isOpen'
PUSH 0      ; Default value for 'isOpen'
LOAD_ARG 0 ; 'this' for assignment to member 'fd'
PUSH 1
INEG
PUTFIELD 0
LOAD_ARG 0 ; 'this' for assignment to member 'isOpen'
PUSH 0
PUTFIELD 1
RET
.endmethod

.method FileHandler.fopen@[C@I
.limit stack 4
.limit locals 67
LOAD_ARG 0 ; Copy arg 'filename' to local
STORE 64
LOAD_ARG 1 ; Copy arg 'mode' to local
STORE 65
PUSH 0
STORE 66 ; Init flags
LOAD 65  ; Load parameter 'mode'
PUSH 0
ICMP_EQ
JNZ L104
JMP L105
L104:
PUSH 0
STORE 66 ; Store to local 'flags'
JMP L106
L105:
LOAD 65  ; Load parameter 'mode'
PUSH 1
ICMP_EQ
JNZ L107
JMP L108
L107:
PUSH 577
STORE 66 ; Store to local 'flags'
JMP L109
L108:
LOAD 65  ; Load parameter 'mode'
PUSH 2
ICMP_EQ
JNZ L110
JMP L111
L110:
PUSH 1089
STORE 66 ; Store to local 'flags'
L111:
L109:
L106:
LOAD_ARG 0 ; 'this' for assignment to member 'fd'
LOAD 64  ; Load parameter 'filename'
LOAD 66  ; Load local var flags
SYS_CALL OPEN ; open
PUTFIELD 0
LOAD_ARG 0 ; Load 'this' to access member 'fd'
GETFIELD 0
PUSH 0
ICMP_LT
JNZ L112
JMP L113
L112:
LOAD_ARG 0 ; 'this' for assignment to member 'isOpen'
PUSH 0
PUTFIELD 1
PUSH 1
INEG
RET
L113:
LOAD_ARG 0 ; 'this' for assignment to member 'isOpen'
PUSH 1
PUTFIELD 1
PUSH 0
RET
.endmethod

.method FileHandler.fclose
.limit stack 4
.limit locals 0
LOAD_ARG 0 ; Load 'this' to access member 'isOpen'
GETFIELD 1
PUSH 1
ICMP_EQ
JNZ L114
JMP L115
L114:
LOAD_ARG 0 ; Load 'this' to access member 'fd'
GETFIELD 0
SYS_CALL CLOSE ; close
POP
LOAD_ARG 0 ; 'this' for assignment to member 'fd'
PUSH 1
INEG
PUTFIELD 0
LOAD_ARG 0 ; 'this' for assignment to member 'isOpen'
PUSH 0
PUTFIELD 1
L115:
RET
.endmethod

.method FileHandler.fread@[C@I
.limit stack 4
.limit locals 70
LOAD_ARG 0 ; Copy arg 'buffer' to local
STORE 67
LOAD_ARG 1 ; Copy arg 'size' to local
STORE 68
LOAD_ARG 0 ; Load 'this' to access member 'isOpen'
GETFIELD 1
PUSH 0
ICMP_EQ
JNZ L116
JMP L117
L116:
PUSH 1
INEG
RET
L117:
LOAD 67  ; Load parameter 'buffer'
LOAD 68  ; Load parameter 'size'
LOAD_ARG 0 ; Load 'this' to access member 'fd'
GETFIELD 0
SYS_CALL READ ; read
STORE 69 ; Init bytesRead
LOAD 69  ; Load local var bytesRead
RET
.endmethod

.method FileHandler.fwrite@[C@I
.limit stack 4
.limit locals 73
LOAD_ARG 0 ; Copy arg 'buffer' to local
STORE 70
LOAD_ARG 1 ; Copy arg 'size' to local
STORE 71
LOAD_ARG 0 ; Load 'this' to access member 'isOpen'
GETFIELD 1
PUSH 0
ICMP_EQ
JNZ L118
JMP L119
L118:
PUSH 1
INEG
RET
L119:
LOAD 70  ; Load parameter 'buffer'
LOAD 71  ; Load parameter 'size'
LOAD_ARG 0 ; Load 'this' to access member 'fd'
GETFIELD 0
SYS_CALL WRITE ; write
STORE 72 ; Init bytesWritten
LOAD 72  ; Load local var bytesWritten
RET
.endmethod

.method FileHandler.is_open
.limit stack 4
.limit locals 0
LOAD_ARG 0 ; Load 'this' to access member 'isOpen'
GETFIELD 1
RET
.endmethod

.method Termios.Termios
.limit stack 10
.limit locals 2
LOAD_ARG 0      ; Push 'this' reference for field 'clflag'
PUSH 0      ; Default value for 'clflag'
LOAD_ARG 0 ; 'this' for assignment to member 'clflag'
PUSH 0
PUTFIELD 0
RET
.endmethod

.method Termios.copyFrom@Termios
.limit stack 4
.limit locals 74
LOAD_ARG 0 ; Copy arg 'src' to local
STORE 73
LOAD_ARG 0 ; 'this' for assignment to member 'clflag'
LOAD 73  ; Load parameter 'src'
GETFIELD 0 ; Get field 'clflag'
PUTFIELD 0
RET
.endmethod

.method TerminalHandler.disableRawMode
.limit stack 4
.limit locals 0
LOAD_ARG 0 ; Load 'this' for method call
PUSH 0
PUSH 2
LOAD_ARG 0 ; Load 'this' to access member object 'Eorigtermios'
GETFIELD 0
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 3 ; Call TerminalHandler.tcsetattr@I@I@Termios
RET
.endmethod

.method TerminalHandler.enableRawMode
.limit stack 4
.limit locals 75
LOAD_ARG 0 ; Load 'this' for method call
PUSH 0
LOAD_ARG 0 ; Load 'this' to access member object 'Eorigtermios'
GETFIELD 0
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 2 ; Call TerminalHandler.tcgetattr@I@Termios
PUSH 20 ; String literal length
NEWARRAY C ; Create char array for string "Raw mode enabled.\n"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 82 ; Push char 'R'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 97 ; Push char 'a'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 119 ; Push char 'w'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 109 ; Push char 'm'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 111 ; Push char 'o'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 100 ; Push char 'd'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7
PUSH 101 ; Push char 'e'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 8 ; Push index 8
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 9 ; Push index 9
PUSH 101 ; Push char 'e'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 10 ; Push index 10
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 11 ; Push index 11
PUSH 97 ; Push char 'a'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 12 ; Push index 12
PUSH 98 ; Push char 'b'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 13 ; Push index 13
PUSH 108 ; Push char 'l'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 14 ; Push index 14
PUSH 101 ; Push char 'e'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 15 ; Push index 15
PUSH 100 ; Push char 'd'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 16 ; Push index 16
PUSH 46 ; Push char '.'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 17 ; Push index 17
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 18 ; Push index 18
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 19 ; Push index 19 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
PUSH 18
PUSH 1
SYS_CALL WRITE ; write
POP
LOAD 74  ; Load local var raw
LOAD_ARG 0 ; Load 'this' to access member object 'Eorigtermios'
GETFIELD 0
LOAD 74  ; Load local var raw
INVOKEVIRTUAL 1 ; Call Termios.copyFrom@Termios
LOAD 74  ; Load local var raw
LOAD 74  ; Load local var raw
GETFIELD 0 ; Get field 'clflag'
PUSH 4
IDIV
PUSH 4
IMUL
PUTFIELD 0 ; Set field 'clflag'
LOAD_ARG 0 ; Load 'this' for method call
PUSH 0
PUSH 2
LOAD 74  ; Load local var raw
LOAD_ARG 0 ; vm identification
INVOKEVIRTUAL 3 ; Call TerminalHandler.tcsetattr@I@I@Termios
RET
.endmethod

.method TerminalHandler.tcgetattr@I@Termios
.limit stack 4
.limit locals 77
LOAD_ARG 0 ; Copy arg 'fd' to local
STORE 75
LOAD_ARG 1 ; Copy arg 't' to local
STORE 76
LOAD 76  ; Load parameter 't'
PUSH 1
PUTFIELD 0 ; Set field 'clflag'
RET
.endmethod

.method TerminalHandler.tcsetattr@I@I@Termios
.limit stack 4
.limit locals 80
LOAD_ARG 0 ; Copy arg 'fd' to local
STORE 77
LOAD_ARG 1 ; Copy arg 'flag' to local
STORE 78
LOAD_ARG 2 ; Copy arg 't' to local
STORE 79
PUSH 26 ; String literal length
NEWARRAY C ; Create char array for string "Termios attributes set.\n"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 84 ; Push char 'T'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 101 ; Push char 'e'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 109 ; Push char 'm'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 105 ; Push char 'i'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 111 ; Push char 'o'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 115 ; Push char 's'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 8 ; Push index 8
PUSH 97 ; Push char 'a'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 9 ; Push index 9
PUSH 116 ; Push char 't'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 10 ; Push index 10
PUSH 116 ; Push char 't'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 11 ; Push index 11
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 12 ; Push index 12
PUSH 105 ; Push char 'i'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 13 ; Push index 13
PUSH 98 ; Push char 'b'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 14 ; Push index 14
PUSH 117 ; Push char 'u'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 15 ; Push index 15
PUSH 116 ; Push char 't'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 16 ; Push index 16
PUSH 101 ; Push char 'e'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 17 ; Push index 17
PUSH 115 ; Push char 's'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 18 ; Push index 18
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 19 ; Push index 19
PUSH 115 ; Push char 's'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 20 ; Push index 20
PUSH 101 ; Push char 'e'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 21 ; Push index 21
PUSH 116 ; Push char 't'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 22 ; Push index 22
PUSH 46 ; Push char '.'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 23 ; Push index 23
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 24 ; Push index 24
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 25 ; Push index 25 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
PUSH 25
PUSH 1
SYS_CALL WRITE ; write
POP
RET
.endmethod

.method TerminalHandler.TerminalHandler
.limit stack 10
.limit locals 2
LOAD_ARG 0      ; Push 'this' reference for field 'Eorigtermios'
NEW Termios
DUP
DUP ; for vm identification
INVOKEVIRTUAL 0 ; Call default ctor for Termios
PUTFIELD 0 ; Store new instance to 'Eorigtermios'
RET
.endmethod

.method Editor.Editor
.limit stack 10
.limit locals 10
LOAD_ARG 0      ; Push 'this' reference for field 'rows'
PUSH 1024
PUSH 1024
IMUL ; Multiply dimensions for flattened array
NEWARRAY C
PUTFIELD 0
LOAD_ARG 0      ; Push 'this' reference for field 'rowCount'
PUSH 0      ; Default value for 'rowCount'
LOAD_ARG 0      ; Push 'this' reference for field 'filename'
PUSH 256
NEWARRAY C
PUTFIELD 2
LOAD_ARG 0      ; Push 'this' reference for field 'cx'
PUSH 0      ; Default value for 'cx'
LOAD_ARG 0      ; Push 'this' reference for field 'cy'
PUSH 0      ; Default value for 'cy'
LOAD_ARG 0      ; Push 'this' reference for field 'insert_mode'
PUSH 0      ; Default value for 'insert_mode'
LOAD_ARG 0      ; Push 'this' reference for field 'command_mode'
PUSH 0      ; Default value for 'command_mode'
LOAD_ARG 0      ; Push 'this' reference for field 'command_buffer'
PUSH 256
NEWARRAY C
PUTFIELD 7
LOAD_ARG 0      ; Push 'this' reference for field 'status_msg'
PUSH 256
NEWARRAY C
PUTFIELD 8
LOAD_ARG 0 ; 'this' for assignment to member 'rowCount'
PUSH 0
PUTFIELD 1
LOAD_ARG 0 ; 'this' for assignment to member 'cx'
PUSH 0
PUTFIELD 3
LOAD_ARG 0 ; 'this' for assignment to member 'cy'
PUSH 0
PUTFIELD 4
LOAD_ARG 0 ; 'this' for assignment to member 'insert_mode'
PUSH 0
PUTFIELD 5
LOAD_ARG 0 ; 'this' for assignment to member 'command_mode'
PUSH 0
PUTFIELD 6
RET
.endmethod
NEW Editor ; Create new object of class Editor
DUP
DUP ; for identification in vm
INVOKEVIRTUAL 0 ; Call constructor for Editor
STORE 80 ; Store new Object E
NEW TerminalHandler ; Create new object of class TerminalHandler
DUP
DUP ; for identification in vm
INVOKEVIRTUAL 4 ; Call constructor for TerminalHandler
STORE 81 ; Store new Object th
NEW IOHandler ; Create new object of class IOHandler
DUP
DUP ; for identification in vm
INVOKEVIRTUAL 10 ; Call constructor for IOHandler
STORE 82 ; Store new Object io
NEW StringHandler ; Create new object of class StringHandler
DUP
DUP ; for identification in vm
INVOKEVIRTUAL 12 ; Call constructor for StringHandler
STORE 83 ; Store new Object sh
NEW FileHandler ; Create new object of class FileHandler
DUP
DUP ; for identification in vm
INVOKEVIRTUAL 0 ; Call constructor for FileHandler
STORE 84 ; Store new Object fh

.method disableRawMode@TerminalHandler
.limit stack 4
.limit locals 86
LOAD_ARG 0 ; Copy arg 'th' to local
STORE 85
LOAD 85  ; Load parameter 'th'
LOAD 85  ; Load parameter 'th'
INVOKEVIRTUAL 0 ; Call TerminalHandler.disableRawMode
RET
.endmethod

.method enableRawMode
.limit stack 4
.limit locals 0
LOAD 81  ; Load local var th
LOAD 81  ; Load local var th
INVOKEVIRTUAL 1 ; Call TerminalHandler.enableRawMode
RET
.endmethod

.method readKey
.limit stack 4
.limit locals 0
LOAD 82  ; Load local var io
LOAD 82  ; Load local var io
INVOKEVIRTUAL 0 ; Call IOHandler.readChar
RET
.endmethod

.method openFile@[C@Editor
.limit stack 5
.limit locals 96
LOAD_ARG 0 ; Copy arg 'fname' to local
STORE 86
LOAD_ARG 1 ; Copy arg 'E' to local
STORE 87
LOAD 83  ; Load local var sh
LOAD 86  ; Load parameter 'fname'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUSH 0
ICMP_EQ
JNZ L120
JMP L121
L120:
LOAD 87  ; Load parameter 'E'
PUSH 1
PUTFIELD 1 ; Set field 'rowCount'
LOAD 87  ; Load parameter 'E'
GETFIELD 0 ; Get field 'rows'
PUSH 0
PUSH 1024
IMUL
PUSH 0
IADD
PUSH 97 ; Push ASCII for char 'a'
ASTORE ; Store to member array element 'rows'
RET
L121:
LOAD 84  ; Load local var fh
LOAD 86  ; Load parameter 'fname'
PUSH 0
LOAD 84  ; Load local var fh
INVOKEVIRTUAL 1 ; Call FileHandler.fopen@[C@I
PUSH 0
ICMP_NEQ
JNZ L122
JMP L123
L122:
LOAD 87  ; Load parameter 'E'
PUSH 1
PUTFIELD 1 ; Set field 'rowCount'
LOAD 87  ; Load parameter 'E'
GETFIELD 0 ; Get field 'rows'
PUSH 0
PUSH 1024
IMUL
PUSH 0
IADD
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'rows'
RET
L123:
PUSH 1024
PUSH 1024
IMUL
NEWARRAY C
STORE 88 ; Store new flattened array to 'buffer'
LOAD 84  ; Load local var fh
LOAD 88  ; Load local var buffer
PUSH 1024
PUSH 1024
IMUL
LOAD 84  ; Load local var fh
INVOKEVIRTUAL 3 ; Call FileHandler.fread@[C@I
STORE 89 ; Init bytesRead
PUSH 0
STORE 90 ; Init start
LOAD 87  ; Load parameter 'E'
PUSH 0
PUTFIELD 1 ; Set field 'rowCount'
PUSH 0
STORE 91 ; Init i
JMP L124
L125:
LOAD 88 ; Load array variable 'buffer'
LOAD 91  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\n'
ICMP_EQ
JNZ L128
JMP L129
L128:
LOAD 91  ; Load local var i
LOAD 90  ; Load local var start
ISUB
STORE 92 ; Init len
LOAD 92  ; Load local var len
PUSH 1024
ICMP_GEQ
JNZ L130
JMP L131
L130:
PUSH 1024
PUSH 1
ISUB
STORE 92 ; Store to local 'len'
L131:
PUSH 0
STORE 93 ; Init j
JMP L132
L133:
L134:
LOAD 93 ; Load local 'j'
DUP
PUSH 1
IADD ; ++
STORE 93 ; Store local 'j'
L132:
LOAD 93  ; Load local var j
LOAD 92  ; Load local var len
ICMP_LT
JNZ L133
JMP L135
L135:
LOAD 87  ; Load parameter 'E'
GETFIELD 1
LOAD 87  ; Load parameter 'E'
LOAD 87  ; Load parameter 'E'
GETFIELD 1
PUSH 1
IADD ; ++
PUTFIELD 1
POP
LOAD 91  ; Load local var i
PUSH 1
IADD
STORE 90 ; Store to local 'start'
L129:
L126:
LOAD 91 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 91 ; Store local 'i'
L124:
LOAD 91  ; Load local var i
LOAD 89  ; Load local var bytesRead
ICMP_LT
JNZ L125
JMP L127
L127:
LOAD 90  ; Load local var start
LOAD 89  ; Load local var bytesRead
ICMP_LT
JNZ L136
JMP L137
L136:
LOAD 89  ; Load local var bytesRead
LOAD 90  ; Load local var start
ISUB
STORE 94 ; Init len
LOAD 94  ; Load local var len
PUSH 1024
ICMP_GEQ
JNZ L138
JMP L139
L138:
PUSH 1024
PUSH 1
ISUB
STORE 94 ; Store to local 'len'
L139:
PUSH 0
STORE 95 ; Init j
JMP L140
L141:
L142:
LOAD 95 ; Load local 'j'
DUP
PUSH 1
IADD ; ++
STORE 95 ; Store local 'j'
L140:
LOAD 95  ; Load local var j
LOAD 94  ; Load local var len
ICMP_LT
JNZ L141
JMP L143
L143:
LOAD 87  ; Load parameter 'E'
GETFIELD 1
LOAD 87  ; Load parameter 'E'
LOAD 87  ; Load parameter 'E'
GETFIELD 1
PUSH 1
IADD ; ++
PUTFIELD 1
POP
L137:
LOAD 84  ; Load local var fh
LOAD 84  ; Load local var fh
INVOKEVIRTUAL 2 ; Call FileHandler.fclose
LOAD 83  ; Load local var sh
LOAD 86  ; Load parameter 'fname'
PUSH 0
LOAD 83  ; Load local var sh
LOAD 86  ; Load parameter 'fname'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 87  ; Load parameter 'E'
GETFIELD 2 ; Get field 'filename'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
RET
.endmethod

.method saveFile
.limit stack 5
.limit locals 103
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 2 ; Get field 'filename'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUSH 0
ICMP_EQ
JNZ L144
JMP L145
L144:
RET
L145:
LOAD 84  ; Load local var fh
LOAD 80  ; Load local var E
GETFIELD 2 ; Get field 'filename'
PUSH 1
LOAD 84  ; Load local var fh
INVOKEVIRTUAL 1 ; Call FileHandler.fopen@[C@I
PUSH 0
ICMP_NEQ
JNZ L146
JMP L147
L146:
LOAD 83  ; Load local var sh
PUSH 18 ; String literal length
NEWARRAY C ; Create char array for string "Error saving file"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 69 ; Push char 'E'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 111 ; Push char 'o'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 115 ; Push char 's'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7
PUSH 97 ; Push char 'a'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 8 ; Push index 8
PUSH 118 ; Push char 'v'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 9 ; Push index 9
PUSH 105 ; Push char 'i'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 10 ; Push index 10
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 11 ; Push index 11
PUSH 103 ; Push char 'g'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 12 ; Push index 12
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 13 ; Push index 13
PUSH 102 ; Push char 'f'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 14 ; Push index 14
PUSH 105 ; Push char 'i'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 15 ; Push index 15
PUSH 108 ; Push char 'l'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 16 ; Push index 16
PUSH 101 ; Push char 'e'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 17 ; Push index 17 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
PUSH 0
PUSH 17
LOAD 80  ; Load local var E
GETFIELD 8 ; Get field 'status_msg'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
RET
L147:
PUSH 0
STORE 96 ; Init i
JMP L148
L149:
PUSH 1024
NEWARRAY C
STORE 97 ; Store new flattened array to 'line'
PUSH 0
STORE 98 ; Init j1
JMP L152
L153:
LOAD 97 ; Load array variable 'line'
LOAD 98  ; Load local var j1
LOAD 80  ; Load local var E
LOAD 96  ; Load local var i
ALOAD ; Load array element
LOAD 98  ; Load local var j1
ASTORE ; Store to array element
L154:
LOAD 98 ; Load local 'j1'
DUP
PUSH 1
IADD ; ++
STORE 98 ; Store local 'j1'
L152:
LOAD 98  ; Load local var j1
PUSH 1024
ICMP_LT
JNZ L153
JMP L155
L155:
LOAD 83  ; Load local var sh
LOAD 97  ; Load local var line
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 99 ; Init len
LOAD 84  ; Load local var fh
LOAD 97  ; Load local var line
LOAD 99  ; Load local var len
LOAD 84  ; Load local var fh
INVOKEVIRTUAL 4 ; Call FileHandler.fwrite@[C@I
POP
LOAD 96  ; Load local var i
PUSH 1
IADD
LOAD 80  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
ICMP_LT
JNZ L156
JMP L157
L156:
PUSH 1
NEWARRAY C
STORE 100 ; Store new flattened array to 'nl'
LOAD 100 ; Load array variable 'nl'
PUSH 0
PUSH 92 ; Push ASCII for char '\n'
ASTORE ; Store to array element
LOAD 84  ; Load local var fh
LOAD 100  ; Load local var nl
PUSH 1
LOAD 84  ; Load local var fh
INVOKEVIRTUAL 4 ; Call FileHandler.fwrite@[C@I
POP
L157:
L150:
LOAD 96 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 96 ; Store local 'i'
L148:
LOAD 96  ; Load local var i
LOAD 80  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
ICMP_LT
JNZ L149
JMP L151
L151:
LOAD 84  ; Load local var fh
LOAD 84  ; Load local var fh
INVOKEVIRTUAL 2 ; Call FileHandler.fclose
PUSH 256
NEWARRAY C
STORE 101 ; Store new flattened array to 'msg'
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 2 ; Get field 'filename'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 102 ; Init len
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 2 ; Get field 'filename'
PUSH 0
LOAD 102  ; Load local var len
LOAD 101  ; Load local var msg
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
LOAD 83  ; Load local var sh
LOAD 101  ; Load local var msg
PUSH 0
PUSH 34 ; Push ASCII for char '"'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 3 ; Call StringHandler.insert@[C@I@C
LOAD 83  ; Load local var sh
LOAD 101  ; Load local var msg
LOAD 83  ; Load local var sh
LOAD 101  ; Load local var msg
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUSH 34 ; Push ASCII for char '"'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 3 ; Call StringHandler.insert@[C@I@C
LOAD 83  ; Load local var sh
PUSH 9 ; String literal length
NEWARRAY C ; Create char array for string " written"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 119 ; Push char 'w'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 105 ; Push char 'i'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 116 ; Push char 't'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 116 ; Push char 't'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 101 ; Push char 'e'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 8 ; Push index 8 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
PUSH 0
PUSH 8
LOAD 80  ; Load local var E
GETFIELD 8 ; Get field 'status_msg'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
RET
.endmethod

.method drawRows
.limit stack 4
.limit locals 104
LOAD 82  ; Load local var io
PUSH 8 ; String literal length
NEWARRAY C ; Create char array for string "\x1b[2J"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 120 ; Push char 'x'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 49 ; Push char '1'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 98 ; Push char 'b'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 91 ; Push char '['
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 50 ; Push char '2'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 74 ; Push char 'J'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 82  ; Load local var io
PUSH 7 ; String literal length
NEWARRAY C ; Create char array for string "\x1b[H"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 120 ; Push char 'x'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 49 ; Push char '1'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 98 ; Push char 'b'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 91 ; Push char '['
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 72 ; Push char 'H'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
PUSH 0
STORE 103 ; Init i
JMP L158
L159:
LOAD 82  ; Load local var io
LOAD 103  ; Load local var i
PUSH 1
IADD
LOAD 82  ; Load local var io
INVOKEVIRTUAL 8 ; Call IOHandler.printInt@I
LOAD 82  ; Load local var io
PUSH 2 ; String literal length
NEWARRAY C ; Create char array for string " "
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 82  ; Load local var io
LOAD 80  ; Load local var E
LOAD 103  ; Load local var i
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 82  ; Load local var io
PUSH 5 ; String literal length
NEWARRAY C ; Create char array for string "\r\n"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
L160:
LOAD 103 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 103 ; Store local 'i'
L158:
LOAD 103  ; Load local var i
LOAD 80  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
ICMP_LT
JNZ L159
JMP L161
L161:
LOAD 82  ; Load local var io
PUSH 8 ; String literal length
NEWARRAY C ; Create char array for string "\x1b[7m"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 120 ; Push char 'x'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 49 ; Push char '1'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 98 ; Push char 'b'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 91 ; Push char '['
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 55 ; Push char '7'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 109 ; Push char 'm'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 82  ; Load local var io
PUSH 7 ; String literal length
NEWARRAY C ; Create char array for string "FILE: "
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 70 ; Push char 'F'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 73 ; Push char 'I'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 76 ; Push char 'L'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 69 ; Push char 'E'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 58 ; Push char ':'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 82  ; Load local var io
LOAD 80  ; Load local var E
GETFIELD 2 ; Get field 'filename'
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 82  ; Load local var io
PUSH 10 ; String literal length
NEWARRAY C ; Create char array for string " | MODE: "
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 124 ; Push char '|'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 77 ; Push char 'M'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 79 ; Push char 'O'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 68 ; Push char 'D'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 69 ; Push char 'E'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7
PUSH 58 ; Push char ':'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 8 ; Push index 8
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 9 ; Push index 9 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 80  ; Load local var E
GETFIELD 5 ; Get field 'insert_mode'
PUSH 1
ICMP_EQ
JNZ L162
JMP L163
L162:
LOAD 82  ; Load local var io
PUSH 7 ; String literal length
NEWARRAY C ; Create char array for string "INSERT"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 73 ; Push char 'I'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 78 ; Push char 'N'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 83 ; Push char 'S'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 69 ; Push char 'E'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 82 ; Push char 'R'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 84 ; Push char 'T'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
JMP L164
L163:
LOAD 80  ; Load local var E
GETFIELD 6 ; Get field 'command_mode'
PUSH 1
ICMP_EQ
JNZ L165
JMP L166
L165:
LOAD 82  ; Load local var io
PUSH 8 ; String literal length
NEWARRAY C ; Create char array for string "COMMAND"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 67 ; Push char 'C'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 79 ; Push char 'O'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 77 ; Push char 'M'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 77 ; Push char 'M'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 65 ; Push char 'A'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 78 ; Push char 'N'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 68 ; Push char 'D'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
JMP L167
L166:
LOAD 82  ; Load local var io
PUSH 7 ; String literal length
NEWARRAY C ; Create char array for string "NORMAL"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 78 ; Push char 'N'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 79 ; Push char 'O'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 82 ; Push char 'R'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 77 ; Push char 'M'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 65 ; Push char 'A'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 76 ; Push char 'L'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
L167:
L164:
LOAD 82  ; Load local var io
PUSH 11 ; String literal length
NEWARRAY C ; Create char array for string "\x1b[m\r\n"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 120 ; Push char 'x'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 49 ; Push char '1'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 98 ; Push char 'b'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 91 ; Push char '['
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 109 ; Push char 'm'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 8 ; Push index 8
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 9 ; Push index 9
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 10 ; Push index 10 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 80  ; Load local var E
GETFIELD 6 ; Get field 'command_mode'
PUSH 1
ICMP_EQ
JNZ L168
JMP L169
L168:
LOAD 82  ; Load local var io
PUSH 2 ; String literal length
NEWARRAY C ; Create char array for string ":"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 58 ; Push char ':'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 82  ; Load local var io
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 82  ; Load local var io
PUSH 5 ; String literal length
NEWARRAY C ; Create char array for string "\r\n"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
JMP L170
L169:
LOAD 82  ; Load local var io
LOAD 80  ; Load local var E
GETFIELD 8 ; Get field 'status_msg'
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 82  ; Load local var io
PUSH 5 ; String literal length
NEWARRAY C ; Create char array for string "\r\n"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 92 ; Push char '\'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 82  ; Load local var io
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
L170:
RET
.endmethod

.method moveCursor@C
.limit stack 4
.limit locals 105
LOAD_ARG 0 ; Copy arg 'key' to local
STORE 104
LOAD 104  ; Load parameter 'key'
PUSH 104 ; Push ASCII for char 'h'
ICMP_EQ
JNZ L171
JMP L172
L171:
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
PUSH 0
ICMP_GT
JNZ L174
JMP L175
L174:
LOAD 80  ; Load local var E
GETFIELD 3
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 3
PUSH 1
ISUB ; --
PUTFIELD 3
POP
L175:
JMP L173
L172:
LOAD 104  ; Load parameter 'key'
PUSH 108 ; Push ASCII for char 'l'
ICMP_EQ
JNZ L176
JMP L177
L176:
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
ICMP_LT
JNZ L179
JMP L180
L179:
LOAD 80  ; Load local var E
GETFIELD 3
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 3
PUSH 1
IADD ; ++
PUTFIELD 3
POP
L180:
JMP L178
L177:
LOAD 104  ; Load parameter 'key'
PUSH 107 ; Push ASCII for char 'k'
ICMP_EQ
JNZ L181
JMP L182
L181:
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 0
ICMP_GT
JNZ L184
JMP L185
L184:
LOAD 80  ; Load local var E
GETFIELD 4
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4
PUSH 1
ISUB ; --
PUTFIELD 4
POP
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
ICMP_GT
JNZ L186
JMP L187
L186:
LOAD 80  ; Load local var E
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUTFIELD 3 ; Set field 'cx'
L187:
L185:
JMP L183
L182:
LOAD 104  ; Load parameter 'key'
PUSH 106 ; Push ASCII for char 'j'
ICMP_EQ
JNZ L188
JMP L189
L188:
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
LOAD 80  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
ICMP_LT
JNZ L190
JMP L191
L190:
LOAD 80  ; Load local var E
GETFIELD 4
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4
PUSH 1
IADD ; ++
PUTFIELD 4
POP
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
ICMP_GT
JNZ L192
JMP L193
L192:
LOAD 80  ; Load local var E
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUTFIELD 3 ; Set field 'cx'
L193:
L191:
L189:
L183:
L178:
L173:
RET
.endmethod

.method insertChar@C
.limit stack 4
.limit locals 106
LOAD_ARG 0 ; Copy arg 'c' to local
STORE 105
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 105  ; Load parameter 'c'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 3 ; Call StringHandler.insert@[C@I@C
LOAD 80  ; Load local var E
GETFIELD 3
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 3
PUSH 1
IADD ; ++
PUTFIELD 3
POP
RET
.endmethod

.method deleteChar
.limit stack 4
.limit locals 0
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
ICMP_LT
JNZ L194
JMP L195
L194:
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
PUSH 1
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 4 ; Call StringHandler.erase@[C@I@I
L195:
RET
.endmethod

.method processCommand
.limit stack 5
.limit locals 0
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUSH 0
ICMP_EQ
JNZ L196
JMP L197
L196:
RET
L197:
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 2 ; String literal length
NEWARRAY C ; Create char array for string "q"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 113 ; Push char 'q'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 2 ; Call StringHandler.compare@[C@[C
PUSH 0
ICMP_EQ
JNZ L198
JMP L199
L198:
PUSH 0
SYS_CALL EXIT
JMP L200
L199:
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 2 ; String literal length
NEWARRAY C ; Create char array for string "w"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 119 ; Push char 'w'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 2 ; Call StringHandler.compare@[C@[C
PUSH 0
ICMP_EQ
JNZ L201
JMP L202
L201:
CALL saveFile
JMP L203
L202:
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 3 ; String literal length
NEWARRAY C ; Create char array for string "wq"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 119 ; Push char 'w'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 113 ; Push char 'q'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 2 ; Call StringHandler.compare@[C@[C
PUSH 0
ICMP_EQ
JNZ L204
JMP L205
L204:
CALL saveFile
PUSH 0
SYS_CALL EXIT
JMP L206
L205:
LOAD 83  ; Load local var sh
PUSH 24 ; String literal length
NEWARRAY C ; Create char array for string "Not an editor command: "
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 78 ; Push char 'N'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 111 ; Push char 'o'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 2 ; Push index 2
PUSH 116 ; Push char 't'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 3 ; Push index 3
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 4 ; Push index 4
PUSH 97 ; Push char 'a'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 5 ; Push index 5
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 6 ; Push index 6
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 7 ; Push index 7
PUSH 101 ; Push char 'e'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 8 ; Push index 8
PUSH 100 ; Push char 'd'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 9 ; Push index 9
PUSH 105 ; Push char 'i'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 10 ; Push index 10
PUSH 116 ; Push char 't'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 11 ; Push index 11
PUSH 111 ; Push char 'o'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 12 ; Push index 12
PUSH 114 ; Push char 'r'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 13 ; Push index 13
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 14 ; Push index 14
PUSH 99 ; Push char 'c'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 15 ; Push index 15
PUSH 111 ; Push char 'o'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 16 ; Push index 16
PUSH 109 ; Push char 'm'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 17 ; Push index 17
PUSH 109 ; Push char 'm'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 18 ; Push index 18
PUSH 97 ; Push char 'a'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 19 ; Push index 19
PUSH 110 ; Push char 'n'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 20 ; Push index 20
PUSH 100 ; Push char 'd'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 21 ; Push index 21
PUSH 58 ; Push char ':'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 22 ; Push index 22
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 23 ; Push index 23 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
PUSH 0
PUSH 23
LOAD 80  ; Load local var E
GETFIELD 8 ; Get field 'status_msg'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
L206:
L203:
L200:
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 0
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'command_buffer'
LOAD 80  ; Load local var E
PUSH 0
PUTFIELD 6 ; Set field 'command_mode'
RET
.endmethod

.method processKeypress
.limit stack 6
.limit locals 111
LOAD 82  ; Load local var io
LOAD 82  ; Load local var io
INVOKEVIRTUAL 0 ; Call IOHandler.readChar
STORE 106 ; Init c
LOAD 80  ; Load local var E
GETFIELD 6 ; Get field 'command_mode'
PUSH 1
ICMP_EQ
JNZ L207
JMP L208
L207:
LOAD 106  ; Load local var c
PUSH 92 ; Push ASCII for char '\r'
ICMP_EQ
JNZ L209
JMP L212
L212:
LOAD 106  ; Load local var c
PUSH 92 ; Push ASCII for char '\n'
ICMP_EQ
JNZ L209
JMP L210
L209:
CALL processCommand
JMP L211
L210:
LOAD 106  ; Load local var c
PUSH 127
ICMP_EQ
JNZ L213
JMP L216
L216:
LOAD 106  ; Load local var c
PUSH 92 ; Push ASCII for char '\b'
ICMP_EQ
JNZ L213
JMP L214
L213:
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 107 ; Init len
LOAD 107  ; Load local var len
PUSH 0
ICMP_GT
JNZ L217
JMP L218
L217:
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
LOAD 107  ; Load local var len
PUSH 1
ISUB
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'command_buffer'
L218:
JMP L215
L214:
LOAD 106  ; Load local var c
PUSH 27
ICMP_EQ
JNZ L219
JMP L220
L219:
LOAD 80  ; Load local var E
PUSH 0
PUTFIELD 6 ; Set field 'command_mode'
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 0
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'command_buffer'
JMP L221
L220:
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 106  ; Load local var c
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 3 ; Call StringHandler.insert@[C@I@C
L221:
L215:
L211:
RET
L208:
LOAD 80  ; Load local var E
GETFIELD 5 ; Get field 'insert_mode'
PUSH 0
ICMP_EQ
JNZ L222
JMP L223
L222:
LOAD 106  ; Load local var c
PUSH 105 ; Push ASCII for char 'i'
ICMP_EQ
JNZ L225
JMP L226
L225:
LOAD 80  ; Load local var E
PUSH 1
PUTFIELD 5 ; Set field 'insert_mode'
JMP L227
L226:
LOAD 106  ; Load local var c
PUSH 113 ; Push ASCII for char 'q'
ICMP_EQ
JNZ L228
JMP L229
L228:
PUSH 0
SYS_CALL EXIT
JMP L230
L229:
LOAD 106  ; Load local var c
PUSH 115 ; Push ASCII for char 's'
ICMP_EQ
JNZ L231
JMP L232
L231:
CALL saveFile
JMP L233
L232:
LOAD 106  ; Load local var c
PUSH 120 ; Push ASCII for char 'x'
ICMP_EQ
JNZ L234
JMP L235
L234:
CALL deleteChar
JMP L236
L235:
LOAD 106  ; Load local var c
PUSH 111 ; Push ASCII for char 'o'
ICMP_EQ
JNZ L237
JMP L238
L237:
LOAD 80  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
STORE 108 ; Init i
JMP L240
L241:
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 108  ; Load local var i
PUSH 1
ISUB
PUSH 0
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 108  ; Load local var i
PUSH 1
ISUB
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 80  ; Load local var E
LOAD 108  ; Load local var i
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
L242:
LOAD 108 ; Load local 'i'
DUP
PUSH 1
ISUB ; --
STORE 108 ; Store local 'i'
L240:
LOAD 108  ; Load local var i
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
ICMP_GT
JNZ L241
JMP L243
L243:
LOAD 80  ; Load local var E
GETFIELD 0 ; Get field 'rows'
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
PUSH 1024
IMUL
PUSH 0
IADD
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'rows'
LOAD 80  ; Load local var E
GETFIELD 1
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 1
PUSH 1
IADD ; ++
PUTFIELD 1
POP
LOAD 80  ; Load local var E
GETFIELD 4
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4
PUSH 1
IADD ; ++
PUTFIELD 4
POP
LOAD 80  ; Load local var E
PUSH 0
PUTFIELD 3 ; Set field 'cx'
LOAD 80  ; Load local var E
PUSH 1
PUTFIELD 5 ; Set field 'insert_mode'
JMP L239
L238:
LOAD 106  ; Load local var c
PUSH 58 ; Push ASCII for char ':'
ICMP_EQ
JNZ L244
JMP L245
L244:
LOAD 80  ; Load local var E
PUSH 1
PUTFIELD 6 ; Set field 'command_mode'
JMP L246
L245:
LOAD 106  ; Load local var c
CALL moveCursor@C
L246:
L239:
L236:
L233:
L230:
L227:
JMP L224
L223:
LOAD 106  ; Load local var c
PUSH 27
ICMP_EQ
JNZ L247
JMP L248
L247:
LOAD 80  ; Load local var E
PUSH 0
PUTFIELD 5 ; Set field 'insert_mode'
JMP L249
L248:
LOAD 106  ; Load local var c
PUSH 127
ICMP_EQ
JNZ L250
JMP L253
L253:
LOAD 106  ; Load local var c
PUSH 92 ; Push ASCII for char '\b'
ICMP_EQ
JNZ L250
JMP L251
L250:
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
PUSH 0
ICMP_GT
JNZ L254
JMP L255
L254:
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
PUSH 1
ISUB
PUSH 1
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 4 ; Call StringHandler.erase@[C@I@I
LOAD 80  ; Load local var E
GETFIELD 3
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 3
PUSH 1
ISUB ; --
PUTFIELD 3
POP
L255:
JMP L252
L251:
LOAD 106  ; Load local var c
PUSH 92 ; Push ASCII for char '\r'
ICMP_EQ
JNZ L256
JMP L257
L256:
PUSH 1024
NEWARRAY C
STORE 109 ; Store new flattened array to 'newLine'
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
ISUB
LOAD 109  ; Load local var newLine
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
LOAD 80  ; Load local var E
GETFIELD 0 ; Get field 'rows'
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1024
IMUL
LOAD 80  ; Load local var E
GETFIELD 3 ; Get field 'cx'
IADD
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'rows'
LOAD 80  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
STORE 110 ; Init i
JMP L259
L260:
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 110  ; Load local var i
PUSH 1
ISUB
PUSH 0
LOAD 83  ; Load local var sh
LOAD 80  ; Load local var E
LOAD 110  ; Load local var i
PUSH 1
ISUB
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 80  ; Load local var E
LOAD 110  ; Load local var i
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
L261:
LOAD 110 ; Load local 'i'
DUP
PUSH 1
ISUB ; --
STORE 110 ; Store local 'i'
L259:
LOAD 110  ; Load local var i
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
ICMP_GT
JNZ L260
JMP L262
L262:
LOAD 83  ; Load local var sh
LOAD 109  ; Load local var newLine
PUSH 0
LOAD 83  ; Load local var sh
LOAD 109  ; Load local var newLine
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
LOAD 83  ; Load local var sh
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
LOAD 80  ; Load local var E
GETFIELD 1
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 1
PUSH 1
IADD ; ++
PUTFIELD 1
POP
LOAD 80  ; Load local var E
GETFIELD 4
LOAD 80  ; Load local var E
LOAD 80  ; Load local var E
GETFIELD 4
PUSH 1
IADD ; ++
PUTFIELD 4
POP
LOAD 80  ; Load local var E
PUSH 0
PUTFIELD 3 ; Set field 'cx'
JMP L258
L257:
LOAD 106  ; Load local var c
CALL insertChar@C
L258:
L252:
L249:
L224:
RET
.endmethod

.method main
.limit stack 4
.limit locals 0
PUSH 1 ; String literal length
NEWARRAY C ; Create char array for string ""
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0 for null terminator
PUSH 0 ; Push null terminator
ASTORE ; Store null terminator in array
CALL openFile@[C
L263:
JMP L264
L264:
CALL drawRows
CALL processKeypress
JMP L263
L265:
PUSH 0
RET
.endmethod
