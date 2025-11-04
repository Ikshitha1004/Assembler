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
.limit locals 3
PUSH 1
NEWARRAY C
STORE 2 ; Store new flattened array to 'arr'
LOAD 2 ; Load array variable 'arr'
PUSH 0
LOAD_ARG 1  ; Load parameter 'c'
ASTORE ; Store to array element
LOAD 2  ; Load local var arr
PUSH 1
PUSH 1
SYS_CALL WRITE ; write
POP
RET
.endmethod

.method IOHandler.readString@[C@I
.limit stack 4
.limit locals 5
PUSH 0
STORE 3 ; Init i
L0:
LOAD 3  ; Load local var i
LOAD_ARG 2  ; Load parameter 'size'
PUSH 1
ISUB
ICMP_LT
JNZ L1
JMP L2
L1:
LOAD_ARG 0 ; Load 'this' for method call
INVOKEVIRTUAL 0 ; Call IOHandler.readChar
STORE 4 ; Store to local 'c'
LOAD 4  ; Load local var c
PUSH 92 ; Push ASCII for char '\n'
ICMP_EQ
JNZ L3
JMP L5
L5:
LOAD 4  ; Load local var c
PUSH 92 ; Push ASCII for char '\r'
ICMP_EQ
JNZ L3
JMP L4
L3:
JMP L2 ; BREAK
L4:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 3  ; Load local var i
LOAD 4  ; Load local var c
ASTORE ; Store to array element
LOAD 3 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 3 ; Store local 'i'
POP
JMP L0
L2:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 3  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
.endmethod

.method IOHandler.stringToInt@[C
.limit stack 4
.limit locals 8
PUSH 1
STORE 7 ; Init sign
PUSH 0
STORE 6 ; Init num
PUSH 0
STORE 5 ; Init i
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
STORE 7 ; Store to local 'sign'
LOAD 5 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 5 ; Store local 'i'
POP
L7:
L8:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 5  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L9
JMP L10
L9:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 5  ; Load local var i
ALOAD
PUSH 48 ; Push ASCII for char '0'
ICMP_GEQ
JNZ L14
JMP L12
L14:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 5  ; Load local var i
ALOAD
PUSH 57 ; Push ASCII for char '9'
ICMP_LEQ
JNZ L11
JMP L12
L11:
LOAD 6  ; Load local var num
PUSH 10
IMUL
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 5  ; Load local var i
ALOAD
PUSH 48 ; Push ASCII for char '0'
ISUB
IADD
STORE 6 ; Store to local 'num'
JMP L13
L12:
JMP L10 ; BREAK
L13:
LOAD 5 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 5 ; Store local 'i'
POP
JMP L8
L10:
LOAD 7  ; Load local var sign
LOAD 6  ; Load local var num
IMUL
RET
.endmethod

.method IOHandler.intToString@I@[C
.limit stack 4
.limit locals 12
PUSH 0
STORE 8 ; Init i
PUSH 0
STORE 9 ; Init isNeg
LOAD_ARG 1  ; Load parameter 'x'
PUSH 0
ICMP_EQ
JNZ L15
JMP L16
L15:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 8 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 8 ; Store local 'i'
PUSH 48 ; Push ASCII for char '0'
ASTORE ; Store to array element
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 8  ; Load local var i
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
STORE 9 ; Store to local 'isNeg'
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
LOAD 8 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 8 ; Store local 'i'
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
LOAD 9  ; Load local var isNeg
PUSH 1
ICMP_EQ
JNZ L22
JMP L23
L22:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 8 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 8 ; Store local 'i'
PUSH 45 ; Push ASCII for char '-'
ASTORE ; Store to array element
L23:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 8  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
PUSH 0
STORE 10 ; Init j
JMP L24
L25:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 10  ; Load local var j
ALOAD
STORE 11 ; Init temp
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 10  ; Load local var j
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 8  ; Load local var i
LOAD 10  ; Load local var j
ISUB
PUSH 1
ISUB
ALOAD
ASTORE ; Store to array element
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 8  ; Load local var i
LOAD 10  ; Load local var j
ISUB
PUSH 1
ISUB
LOAD 11  ; Load local var temp
ASTORE ; Store to array element
L26:
LOAD 10 ; Load local 'j'
DUP
PUSH 1
IADD ; ++
STORE 10 ; Store local 'j'
L24:
LOAD 10  ; Load local var j
LOAD 8  ; Load local var i
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
.limit locals 21
PUSH 0
STORE 12 ; Init neg
LOAD_ARG 1  ; Load parameter 'val'
PUSH 0
ICMP_LT
JNZ L28
JMP L29
L28:
PUSH 1
STORE 12 ; Store to local 'neg'
LOAD_ARG 1  ; Load parameter 'val'
FNEG
STORE 1 ; Store to local 'val'
L29:
PUSH 0
STORE 13 ; Init intPart
LOAD_ARG 1  ; Load parameter 'val'
STORE 14 ; Init temp
L30:
LOAD 14  ; Load local var temp
FPUSH 1.0
FCMP_GEQ
JNZ L31
JMP L32
L31:
LOAD 14  ; Load local var temp
FPUSH 1.0
FSUB
STORE 14 ; Store to local 'temp'
LOAD 13  ; Load local var intPart
PUSH 1
IADD
STORE 13 ; Store to local 'intPart'
JMP L30
L32:
LOAD_ARG 1  ; Load parameter 'val'
LOAD 13  ; Load local var intPart
FSUB
STORE 15 ; Init frac
PUSH 50
NEWARRAY C
STORE 16 ; Store new flattened array to 'intBuf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD 13  ; Load local var intPart
LOAD 16  ; Load local var intBuf
INVOKEVIRTUAL 4 ; Call IOHandler.intToString@I@[C
PUSH 0
STORE 18 ; Init j
PUSH 0
STORE 17 ; Init i
LOAD 12  ; Load local var neg
PUSH 1
ICMP_EQ
JNZ L33
JMP L34
L33:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 17 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 17 ; Store local 'i'
PUSH 45 ; Push ASCII for char '-'
ASTORE ; Store to array element
L34:
L35:
LOAD 16 ; Load array variable 'intBuf'
LOAD 18  ; Load local var j
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L36
JMP L37
L36:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 17 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 17 ; Store local 'i'
LOAD 16 ; Load array variable 'intBuf'
LOAD 18 ; Load local 'j'
DUP
PUSH 1
IADD ; ++
STORE 18 ; Store local 'j'
ALOAD
ASTORE ; Store to array element
JMP L35
L37:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 17 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 17 ; Store local 'i'
PUSH 46 ; Push ASCII for char '.'
ASTORE ; Store to array element
PUSH 0
STORE 19 ; Init k
JMP L38
L39:
LOAD 15  ; Load local var frac
FPUSH 10.0
FMUL
STORE 15 ; Store to local 'frac'
PUSH 0
STORE 20 ; Init digit
L42:
LOAD 15  ; Load local var frac
FPUSH 1.0
FCMP_GEQ
JNZ L43
JMP L44
L43:
LOAD 15  ; Load local var frac
FPUSH 1.0
FSUB
STORE 15 ; Store to local 'frac'
LOAD 20  ; Load local var digit
PUSH 1
IADD
STORE 20 ; Store to local 'digit'
JMP L42
L44:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 17 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 17 ; Store local 'i'
PUSH 48 ; Push ASCII for char '0'
LOAD 20  ; Load local var digit
IADD
ASTORE ; Store to array element
L40:
LOAD 19 ; Load local 'k'
DUP
PUSH 1
IADD ; ++
STORE 19 ; Store local 'k'
L38:
LOAD 19  ; Load local var k
PUSH 6
ICMP_LT
JNZ L39
JMP L41
L41:
LOAD_ARG 2 ; Load array parameter 'arr'
LOAD 17  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
.endmethod

.method IOHandler.readInt
.limit stack 4
.limit locals 22
PUSH 50
NEWARRAY C
STORE 21 ; Store new flattened array to 'buf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD 21  ; Load local var buf
PUSH 50
INVOKEVIRTUAL 2 ; Call IOHandler.readString@[C@I
LOAD_ARG 0 ; Load 'this' for method call
LOAD 21  ; Load local var buf
INVOKEVIRTUAL 3 ; Call IOHandler.stringToInt@[C
RET
.endmethod

.method IOHandler.printString@[C
.limit stack 4
.limit locals 23
PUSH 0
STORE 22 ; Init i
L45:
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 22  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L46
JMP L47
L46:
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1 ; Load array parameter 'arr'
LOAD 22  ; Load local var i
ALOAD
INVOKEVIRTUAL 1 ; Call IOHandler.writeChar@C
LOAD 22 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 22 ; Store local 'i'
POP
JMP L45
L47:
RET
.endmethod

.method IOHandler.printInt@I
.limit stack 4
.limit locals 24
PUSH 50
NEWARRAY C
STORE 23 ; Store new flattened array to 'buf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'x'
LOAD 23  ; Load local var buf
INVOKEVIRTUAL 4 ; Call IOHandler.intToString@I@[C
LOAD_ARG 0 ; Load 'this' for method call
LOAD 23  ; Load local var buf
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
RET
.endmethod

.method IOHandler.printDouble@F
.limit stack 4
.limit locals 25
PUSH 100
NEWARRAY C
STORE 24 ; Store new flattened array to 'buf'
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'x'
LOAD 24  ; Load local var buf
INVOKEVIRTUAL 5 ; Call IOHandler.doubleToString@F@[C
LOAD_ARG 0 ; Load 'this' for method call
LOAD 24  ; Load local var buf
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
.limit locals 26
PUSH 0
STORE 25 ; Init len
L48:
LOAD_ARG 1 ; Load array parameter 'str'
LOAD 25  ; Load local var len
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L49
JMP L50
L49:
LOAD 25 ; Load local 'len'
DUP
PUSH 1
IADD ; ++
STORE 25 ; Store local 'len'
POP
JMP L48
L50:
LOAD 25  ; Load local var len
RET
.endmethod

.method StringHandler.substr@[C@I@I@[C
.limit stack 4
.limit locals 28
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'str'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 26 ; Init n
LOAD_ARG 2  ; Load parameter 'start'
PUSH 0
ICMP_LT
JNZ L51
JMP L53
L53:
LOAD_ARG 2  ; Load parameter 'start'
LOAD 26  ; Load local var n
ICMP_GEQ
JNZ L51
JMP L52
L51:
LOAD_ARG 4 ; Load array parameter 'res'
PUSH 0
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
L52:
PUSH 0
STORE 27 ; Store to local 'i'
JMP L54
L55:
LOAD_ARG 4 ; Load array parameter 'res'
LOAD 27  ; Load local var i
LOAD_ARG 1 ; Load array parameter 'str'
LOAD_ARG 2  ; Load parameter 'start'
LOAD 27  ; Load local var i
IADD
ALOAD
ASTORE ; Store to array element
L56:
LOAD 27 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 27 ; Store local 'i'
L54:
LOAD 27  ; Load local var i
LOAD_ARG 3  ; Load parameter 'len'
ICMP_LT
JNZ L58
JMP L57
L58:
LOAD_ARG 2  ; Load parameter 'start'
LOAD 27  ; Load local var i
IADD
LOAD 26  ; Load local var n
ICMP_LT
JNZ L55
JMP L57
L57:
LOAD_ARG 4 ; Load array parameter 'res'
LOAD 27  ; Load local var i
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
.endmethod

.method StringHandler.compare@[C@[C
.limit stack 4
.limit locals 29
PUSH 0
STORE 28 ; Init i
L59:
LOAD_ARG 1 ; Load array parameter 's1'
LOAD 28  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L62
JMP L61
L62:
LOAD_ARG 2 ; Load array parameter 's2'
LOAD 28  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\0'
ICMP_NEQ
JNZ L60
JMP L61
L60:
LOAD_ARG 1 ; Load array parameter 's1'
LOAD 28  ; Load local var i
ALOAD
LOAD_ARG 2 ; Load array parameter 's2'
LOAD 28  ; Load local var i
ALOAD
ICMP_NEQ
JNZ L63
JMP L64
L63:
LOAD_ARG 1 ; Load array parameter 's1'
LOAD 28  ; Load local var i
ALOAD
LOAD_ARG 2 ; Load array parameter 's2'
LOAD 28  ; Load local var i
ALOAD
ISUB
RET
L64:
LOAD 28 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 28 ; Store local 'i'
POP
JMP L59
L61:
LOAD_ARG 1 ; Load array parameter 's1'
LOAD 28  ; Load local var i
ALOAD
LOAD_ARG 2 ; Load array parameter 's2'
LOAD 28  ; Load local var i
ALOAD
ISUB
RET
.endmethod

.method StringHandler.insert@[C@I@C
.limit stack 4
.limit locals 31
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'str'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 29 ; Init n
LOAD_ARG 2  ; Load parameter 'pos'
PUSH 0
ICMP_LT
JNZ L65
JMP L66
L65:
PUSH 0
STORE 2 ; Store to local 'pos'
L66:
LOAD_ARG 2  ; Load parameter 'pos'
LOAD 29  ; Load local var n
ICMP_GT
JNZ L67
JMP L68
L67:
LOAD 29  ; Load local var n
STORE 2 ; Store to local 'pos'
L68:
LOAD 29  ; Load local var n
STORE 30 ; Init i
JMP L69
L70:
LOAD_ARG 1 ; Load array parameter 'str'
LOAD 30  ; Load local var i
PUSH 1
IADD
LOAD_ARG 1 ; Load array parameter 'str'
LOAD 30  ; Load local var i
ALOAD
ASTORE ; Store to array element
L71:
LOAD 30 ; Load local 'i'
DUP
PUSH 1
ISUB ; --
STORE 30 ; Store local 'i'
L69:
LOAD 30  ; Load local var i
LOAD_ARG 2  ; Load parameter 'pos'
ICMP_GEQ
JNZ L70
JMP L72
L72:
LOAD_ARG 1 ; Load array parameter 'str'
LOAD_ARG 2  ; Load parameter 'pos'
LOAD_ARG 3  ; Load parameter 'c'
ASTORE ; Store to array element
RET
.endmethod

.method StringHandler.erase@[C@I@I
.limit stack 4
.limit locals 33
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'str'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 31 ; Init n
LOAD_ARG 2  ; Load parameter 'pos'
PUSH 0
ICMP_LT
JNZ L73
JMP L75
L75:
LOAD_ARG 2  ; Load parameter 'pos'
LOAD 31  ; Load local var n
ICMP_GEQ
JNZ L73
JMP L74
L73:
RET
L74:
LOAD_ARG 2  ; Load parameter 'pos'
STORE 32 ; Init i
JMP L76
L77:
LOAD_ARG 1 ; Load array parameter 'str'
LOAD 32  ; Load local var i
LOAD_ARG 1 ; Load array parameter 'str'
LOAD 32  ; Load local var i
LOAD_ARG 3  ; Load parameter 'len'
IADD
ALOAD
ASTORE ; Store to array element
L78:
LOAD 32 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 32 ; Store local 'i'
L76:
LOAD 32  ; Load local var i
LOAD_ARG 3  ; Load parameter 'len'
IADD
LOAD 31  ; Load local var n
ICMP_LT
JNZ L77
JMP L79
L79:
LOAD_ARG 1 ; Load array parameter 'str'
LOAD 31  ; Load local var n
LOAD_ARG 3  ; Load parameter 'len'
ISUB
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to array element
RET
.endmethod

.method StringHandler.islower@C
.limit stack 4
.limit locals 2
LOAD_ARG 1  ; Load parameter 'c'
PUSH 97 ; Push ASCII for char 'a'
ICMP_GEQ
JNZ L83
JMP L81
L83:
LOAD_ARG 1  ; Load parameter 'c'
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
.limit locals 2
LOAD_ARG 1  ; Load parameter 'c'
PUSH 65 ; Push ASCII for char 'A'
ICMP_GEQ
JNZ L87
JMP L85
L87:
LOAD_ARG 1  ; Load parameter 'c'
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
.limit locals 2
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'c'
INVOKEVIRTUAL 6 ; Call StringHandler.isupper@C
PUSH 1
ICMP_EQ
JNZ L88
JMP L89
L88:
LOAD_ARG 1  ; Load parameter 'c'
PUSH 97 ; Push ASCII for char 'a'
PUSH 65 ; Push ASCII for char 'A'
ISUB
IADD
RET
L89:
LOAD_ARG 1  ; Load parameter 'c'
RET
.endmethod

.method StringHandler.toupper@C
.limit stack 4
.limit locals 2
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'c'
INVOKEVIRTUAL 5 ; Call StringHandler.islower@C
PUSH 1
ICMP_EQ
JNZ L90
JMP L91
L90:
LOAD_ARG 1  ; Load parameter 'c'
PUSH 97 ; Push ASCII for char 'a'
PUSH 65 ; Push ASCII for char 'A'
ISUB
ISUB
RET
L91:
LOAD_ARG 1  ; Load parameter 'c'
RET
.endmethod

.method StringHandler.isalpha@C
.limit stack 4
.limit locals 2
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'c'
INVOKEVIRTUAL 5 ; Call StringHandler.islower@C
PUSH 1
ICMP_EQ
JNZ L92
JMP L95
L95:
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'c'
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
.limit locals 2
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'c'
INVOKEVIRTUAL 9 ; Call StringHandler.isalpha@C
PUSH 1
ICMP_EQ
JNZ L96
JMP L99
L99:
LOAD_ARG 0 ; Load 'this' for method call
LOAD_ARG 1  ; Load parameter 'c'
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
.limit locals 2
LOAD_ARG 1  ; Load parameter 'c'
PUSH 48 ; Push ASCII for char '0'
ICMP_GEQ
JNZ L103
JMP L101
L103:
LOAD_ARG 1  ; Load parameter 'c'
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
.limit locals 34
PUSH 0
STORE 33 ; Init flags
LOAD_ARG 2  ; Load parameter 'mode'
PUSH 0
ICMP_EQ
JNZ L104
JMP L105
L104:
PUSH 0
STORE 33 ; Store to local 'flags'
JMP L106
L105:
LOAD_ARG 2  ; Load parameter 'mode'
PUSH 1
ICMP_EQ
JNZ L107
JMP L108
L107:
PUSH 577
STORE 33 ; Store to local 'flags'
JMP L109
L108:
LOAD_ARG 2  ; Load parameter 'mode'
PUSH 2
ICMP_EQ
JNZ L110
JMP L111
L110:
PUSH 1089
STORE 33 ; Store to local 'flags'
L111:
L109:
L106:
LOAD_ARG 0 ; 'this' for assignment to member 'fd'
LOAD_ARG 1  ; Load parameter 'filename'
LOAD 33  ; Load local var flags
PUSH 0644
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
.limit locals 35
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
LOAD_ARG 1  ; Load parameter 'buffer'
LOAD_ARG 2  ; Load parameter 'size'
LOAD_ARG 0 ; Load 'this' to access member 'fd'
GETFIELD 0
SYS_CALL READ ; read
STORE 34 ; Init bytesRead
LOAD 34  ; Load local var bytesRead
RET
.endmethod

.method FileHandler.fwrite@[C@I
.limit stack 4
.limit locals 36
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
LOAD_ARG 1  ; Load parameter 'buffer'
LOAD_ARG 2  ; Load parameter 'size'
LOAD_ARG 0 ; Load 'this' to access member 'fd'
GETFIELD 0
SYS_CALL WRITE ; write
STORE 35 ; Init bytesWritten
LOAD 35  ; Load local var bytesWritten
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
.limit locals 2
LOAD_ARG 0 ; 'this' for assignment to member 'clflag'
LOAD_ARG 1  ; Load parameter 'src'
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
INVOKEVIRTUAL 3 ; Call TerminalHandler.tcsetattr@I@I@Termios
RET
.endmethod

.method TerminalHandler.enableRawMode
.limit stack 4
.limit locals 37
LOAD_ARG 0 ; Load 'this' for method call
PUSH 0
LOAD_ARG 0 ; Load 'this' to access member object 'Eorigtermios'
GETFIELD 0
INVOKEVIRTUAL 2 ; Call TerminalHandler.tcgetattr@I@Termios
PUSH 19 ; String literal length
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
PUSH 18
PUSH 1
SYS_CALL WRITE ; write
POP
LOAD 36  ; Load local var raw
LOAD_ARG 0 ; Load 'this' to access member object 'Eorigtermios'
GETFIELD 0
INVOKEVIRTUAL 1 ; Call Termios.copyFrom@Termios
LOAD 36  ; Load local var raw
LOAD 36  ; Load local var raw
GETFIELD 0 ; Get field 'clflag'
PUSH 4
IDIV
PUSH 4
IMUL
PUTFIELD 0 ; Set field 'clflag'
LOAD_ARG 0 ; Load 'this' for method call
PUSH 0
PUSH 2
LOAD 36  ; Load local var raw
INVOKEVIRTUAL 3 ; Call TerminalHandler.tcsetattr@I@I@Termios
RET
.endmethod

.method TerminalHandler.tcgetattr@I@Termios
.limit stack 4
.limit locals 3
LOAD_ARG 2  ; Load parameter 't'
PUSH 1
PUTFIELD 0 ; Set field 'clflag'
RET
.endmethod

.method TerminalHandler.tcsetattr@I@I@Termios
.limit stack 4
.limit locals 4
PUSH 25 ; String literal length
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
INVOKEVIRTUAL 0 ; Call constructor for Editor
STORE 37 ; Store new Object E
NEW TerminalHandler ; Create new object of class TerminalHandler
DUP
INVOKEVIRTUAL 4 ; Call constructor for TerminalHandler
STORE 38 ; Store new Object th
NEW IOHandler ; Create new object of class IOHandler
DUP
INVOKEVIRTUAL 10 ; Call constructor for IOHandler
STORE 39 ; Store new Object io
NEW StringHandler ; Create new object of class StringHandler
DUP
INVOKEVIRTUAL 12 ; Call constructor for StringHandler
STORE 40 ; Store new Object sh
NEW FileHandler ; Create new object of class FileHandler
DUP
INVOKEVIRTUAL 0 ; Call constructor for FileHandler
STORE 41 ; Store new Object fh

.method disableRawMode@TerminalHandler
.limit stack 4
.limit locals 1
LOAD_ARG 0  ; Load parameter 'th'
INVOKEVIRTUAL 0 ; Call TerminalHandler.disableRawMode
RET
.endmethod

.method enableRawMode
.limit stack 4
.limit locals 0
LOAD 38  ; Load local var th
INVOKEVIRTUAL 1 ; Call TerminalHandler.enableRawMode
RET
.endmethod

.method readKey
.limit stack 4
.limit locals 0
LOAD 39  ; Load local var io
INVOKEVIRTUAL 0 ; Call IOHandler.readChar
RET
.endmethod

.method openFile@[C@Editor
.limit stack 5
.limit locals 50
LOAD 40  ; Load local var sh
LOAD_ARG 0  ; Load parameter 'fname'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUSH 0
ICMP_EQ
JNZ L120
JMP L121
L120:
LOAD_ARG 1  ; Load parameter 'E'
PUSH 1
PUTFIELD 1 ; Set field 'rowCount'
LOAD_ARG 1  ; Load parameter 'E'
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
LOAD 41  ; Load local var fh
LOAD_ARG 0  ; Load parameter 'fname'
PUSH 0
INVOKEVIRTUAL 1 ; Call FileHandler.fopen@[C@I
PUSH 0
ICMP_NEQ
JNZ L122
JMP L123
L122:
LOAD_ARG 1  ; Load parameter 'E'
PUSH 1
PUTFIELD 1 ; Set field 'rowCount'
LOAD_ARG 1  ; Load parameter 'E'
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
STORE 42 ; Store new flattened array to 'buffer'
LOAD 41  ; Load local var fh
LOAD 42  ; Load local var buffer
PUSH 1024
PUSH 1024
IMUL
INVOKEVIRTUAL 3 ; Call FileHandler.fread@[C@I
STORE 43 ; Init bytesRead
PUSH 0
STORE 44 ; Init start
LOAD_ARG 1  ; Load parameter 'E'
PUSH 0
PUTFIELD 1 ; Set field 'rowCount'
PUSH 0
STORE 45 ; Init i
JMP L124
L125:
LOAD 42 ; Load array variable 'buffer'
LOAD 45  ; Load local var i
ALOAD
PUSH 92 ; Push ASCII for char '\n'
ICMP_EQ
JNZ L128
JMP L129
L128:
LOAD 45  ; Load local var i
LOAD 44  ; Load local var start
ISUB
STORE 46 ; Init len
LOAD 46  ; Load local var len
PUSH 1024
ICMP_GEQ
JNZ L130
JMP L131
L130:
PUSH 1024
PUSH 1
ISUB
STORE 46 ; Store to local 'len'
L131:
PUSH 0
STORE 47 ; Init j
JMP L132
L133:
L134:
LOAD 47 ; Load local 'j'
DUP
PUSH 1
IADD ; ++
STORE 47 ; Store local 'j'
L132:
LOAD 47  ; Load local var j
LOAD 46  ; Load local var len
ICMP_LT
JNZ L133
JMP L135
L135:
LOAD_ARG 1  ; Load parameter 'E'
GETFIELD 1
LOAD_ARG 1  ; Load parameter 'E'
LOAD_ARG 1  ; Load parameter 'E'
GETFIELD 1
PUSH 1
IADD ; ++
PUTFIELD 1
POP
LOAD 45  ; Load local var i
PUSH 1
IADD
STORE 44 ; Store to local 'start'
L129:
L126:
LOAD 45 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 45 ; Store local 'i'
L124:
LOAD 45  ; Load local var i
LOAD 43  ; Load local var bytesRead
ICMP_LT
JNZ L125
JMP L127
L127:
LOAD 44  ; Load local var start
LOAD 43  ; Load local var bytesRead
ICMP_LT
JNZ L136
JMP L137
L136:
LOAD 43  ; Load local var bytesRead
LOAD 44  ; Load local var start
ISUB
STORE 48 ; Init len
LOAD 48  ; Load local var len
PUSH 1024
ICMP_GEQ
JNZ L138
JMP L139
L138:
PUSH 1024
PUSH 1
ISUB
STORE 48 ; Store to local 'len'
L139:
PUSH 0
STORE 49 ; Init j
JMP L140
L141:
L142:
LOAD 49 ; Load local 'j'
DUP
PUSH 1
IADD ; ++
STORE 49 ; Store local 'j'
L140:
LOAD 49  ; Load local var j
LOAD 48  ; Load local var len
ICMP_LT
JNZ L141
JMP L143
L143:
LOAD_ARG 1  ; Load parameter 'E'
GETFIELD 1
LOAD_ARG 1  ; Load parameter 'E'
LOAD_ARG 1  ; Load parameter 'E'
GETFIELD 1
PUSH 1
IADD ; ++
PUTFIELD 1
POP
L137:
LOAD 41  ; Load local var fh
INVOKEVIRTUAL 2 ; Call FileHandler.fclose
LOAD 40  ; Load local var sh
LOAD_ARG 0  ; Load parameter 'fname'
PUSH 0
LOAD 40  ; Load local var sh
LOAD_ARG 0  ; Load parameter 'fname'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD_ARG 1  ; Load parameter 'E'
GETFIELD 2 ; Get field 'filename'
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
RET
.endmethod

.method saveFile
.limit stack 5
.limit locals 57
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 2 ; Get field 'filename'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUSH 0
ICMP_EQ
JNZ L144
JMP L145
L144:
RET
L145:
LOAD 41  ; Load local var fh
LOAD 37  ; Load local var E
GETFIELD 2 ; Get field 'filename'
PUSH 1
INVOKEVIRTUAL 1 ; Call FileHandler.fopen@[C@I
PUSH 0
ICMP_NEQ
JNZ L146
JMP L147
L146:
LOAD 40  ; Load local var sh
PUSH 17 ; String literal length
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
PUSH 0
PUSH 17
LOAD 37  ; Load local var E
GETFIELD 8 ; Get field 'status_msg'
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
RET
L147:
PUSH 0
STORE 50 ; Init i
JMP L148
L149:
PUSH 1024
NEWARRAY C
STORE 51 ; Store new flattened array to 'line'
PUSH 0
STORE 52 ; Init j1
JMP L152
L153:
LOAD 51 ; Load array variable 'line'
LOAD 52  ; Load local var j1
LOAD 37  ; Load local var E
LOAD 50  ; Load local var i
ALOAD ; Load array element
LOAD 52  ; Load local var j1
ASTORE ; Store to array element
L154:
LOAD 52 ; Load local 'j1'
DUP
PUSH 1
IADD ; ++
STORE 52 ; Store local 'j1'
L152:
LOAD 52  ; Load local var j1
PUSH 1024
ICMP_LT
JNZ L153
JMP L155
L155:
LOAD 40  ; Load local var sh
LOAD 51  ; Load local var line
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 53 ; Init len
LOAD 41  ; Load local var fh
LOAD 51  ; Load local var line
LOAD 53  ; Load local var len
INVOKEVIRTUAL 4 ; Call FileHandler.fwrite@[C@I
POP
LOAD 50  ; Load local var i
PUSH 1
IADD
LOAD 37  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
ICMP_LT
JNZ L156
JMP L157
L156:
PUSH 1
NEWARRAY C
STORE 54 ; Store new flattened array to 'nl'
LOAD 54 ; Load array variable 'nl'
PUSH 0
PUSH 92 ; Push ASCII for char '\n'
ASTORE ; Store to array element
LOAD 41  ; Load local var fh
LOAD 54  ; Load local var nl
PUSH 1
INVOKEVIRTUAL 4 ; Call FileHandler.fwrite@[C@I
POP
L157:
L150:
LOAD 50 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 50 ; Store local 'i'
L148:
LOAD 50  ; Load local var i
LOAD 37  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
ICMP_LT
JNZ L149
JMP L151
L151:
LOAD 41  ; Load local var fh
INVOKEVIRTUAL 2 ; Call FileHandler.fclose
PUSH 256
NEWARRAY C
STORE 55 ; Store new flattened array to 'msg'
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 2 ; Get field 'filename'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 56 ; Init len
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 2 ; Get field 'filename'
PUSH 0
LOAD 56  ; Load local var len
LOAD 55  ; Load local var msg
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
LOAD 40  ; Load local var sh
LOAD 55  ; Load local var msg
PUSH 0
PUSH 34 ; Push ASCII for char '"'
INVOKEVIRTUAL 3 ; Call StringHandler.insert@[C@I@C
LOAD 40  ; Load local var sh
LOAD 55  ; Load local var msg
LOAD 40  ; Load local var sh
LOAD 55  ; Load local var msg
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUSH 34 ; Push ASCII for char '"'
INVOKEVIRTUAL 3 ; Call StringHandler.insert@[C@I@C
LOAD 40  ; Load local var sh
PUSH 8 ; String literal length
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
PUSH 0
PUSH 8
LOAD 37  ; Load local var E
GETFIELD 8 ; Get field 'status_msg'
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
RET
.endmethod

.method drawRows
.limit stack 4
.limit locals 58
LOAD 39  ; Load local var io
PUSH 7 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 39  ; Load local var io
PUSH 6 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
PUSH 0
STORE 57 ; Init i
JMP L158
L159:
LOAD 39  ; Load local var io
LOAD 57  ; Load local var i
PUSH 1
IADD
INVOKEVIRTUAL 8 ; Call IOHandler.printInt@I
LOAD 39  ; Load local var io
PUSH 1 ; String literal length
NEWARRAY C ; Create char array for string " "
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 32 ; Push char ' '
ASTORE ; Store char in array
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 39  ; Load local var io
LOAD 37  ; Load local var E
LOAD 57  ; Load local var i
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 39  ; Load local var io
PUSH 4 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
L160:
LOAD 57 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 57 ; Store local 'i'
L158:
LOAD 57  ; Load local var i
LOAD 37  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
ICMP_LT
JNZ L159
JMP L161
L161:
LOAD 39  ; Load local var io
PUSH 7 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 39  ; Load local var io
PUSH 6 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 39  ; Load local var io
LOAD 37  ; Load local var E
GETFIELD 2 ; Get field 'filename'
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 39  ; Load local var io
PUSH 9 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 37  ; Load local var E
GETFIELD 5 ; Get field 'insert_mode'
PUSH 1
ICMP_EQ
JNZ L162
JMP L163
L162:
LOAD 39  ; Load local var io
PUSH 6 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
JMP L164
L163:
LOAD 37  ; Load local var E
GETFIELD 6 ; Get field 'command_mode'
PUSH 1
ICMP_EQ
JNZ L165
JMP L166
L165:
LOAD 39  ; Load local var io
PUSH 7 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
JMP L167
L166:
LOAD 39  ; Load local var io
PUSH 6 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
L167:
L164:
LOAD 39  ; Load local var io
PUSH 10 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 37  ; Load local var E
GETFIELD 6 ; Get field 'command_mode'
PUSH 1
ICMP_EQ
JNZ L168
JMP L169
L168:
LOAD 39  ; Load local var io
PUSH 1 ; String literal length
NEWARRAY C ; Create char array for string ":"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 58 ; Push char ':'
ASTORE ; Store char in array
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 39  ; Load local var io
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 39  ; Load local var io
PUSH 4 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
JMP L170
L169:
LOAD 39  ; Load local var io
LOAD 37  ; Load local var E
GETFIELD 8 ; Get field 'status_msg'
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
LOAD 39  ; Load local var io
PUSH 4 ; String literal length
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
INVOKEVIRTUAL 7 ; Call IOHandler.printString@[C
L170:
RET
.endmethod

.method moveCursor@C
.limit stack 4
.limit locals 1
LOAD_ARG 0  ; Load parameter 'key'
PUSH 104 ; Push ASCII for char 'h'
ICMP_EQ
JNZ L171
JMP L172
L171:
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
PUSH 0
ICMP_GT
JNZ L174
JMP L175
L174:
LOAD 37  ; Load local var E
GETFIELD 3
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 3
PUSH 1
ISUB ; --
PUTFIELD 3
POP
L175:
JMP L173
L172:
LOAD_ARG 0  ; Load parameter 'key'
PUSH 108 ; Push ASCII for char 'l'
ICMP_EQ
JNZ L176
JMP L177
L176:
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
ICMP_LT
JNZ L179
JMP L180
L179:
LOAD 37  ; Load local var E
GETFIELD 3
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 3
PUSH 1
IADD ; ++
PUTFIELD 3
POP
L180:
JMP L178
L177:
LOAD_ARG 0  ; Load parameter 'key'
PUSH 107 ; Push ASCII for char 'k'
ICMP_EQ
JNZ L181
JMP L182
L181:
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 0
ICMP_GT
JNZ L184
JMP L185
L184:
LOAD 37  ; Load local var E
GETFIELD 4
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4
PUSH 1
ISUB ; --
PUTFIELD 4
POP
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
ICMP_GT
JNZ L186
JMP L187
L186:
LOAD 37  ; Load local var E
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUTFIELD 3 ; Set field 'cx'
L187:
L185:
JMP L183
L182:
LOAD_ARG 0  ; Load parameter 'key'
PUSH 106 ; Push ASCII for char 'j'
ICMP_EQ
JNZ L188
JMP L189
L188:
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
LOAD 37  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
ICMP_LT
JNZ L190
JMP L191
L190:
LOAD 37  ; Load local var E
GETFIELD 4
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4
PUSH 1
IADD ; ++
PUTFIELD 4
POP
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
ICMP_GT
JNZ L192
JMP L193
L192:
LOAD 37  ; Load local var E
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
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
.limit locals 1
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD_ARG 0  ; Load parameter 'c'
INVOKEVIRTUAL 3 ; Call StringHandler.insert@[C@I@C
LOAD 37  ; Load local var E
GETFIELD 3
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
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
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
ICMP_LT
JNZ L194
JMP L195
L194:
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
PUSH 1
INVOKEVIRTUAL 4 ; Call StringHandler.erase@[C@I@I
L195:
RET
.endmethod

.method processCommand
.limit stack 5
.limit locals 0
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
PUSH 0
ICMP_EQ
JNZ L196
JMP L197
L196:
RET
L197:
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 1 ; String literal length
NEWARRAY C ; Create char array for string "q"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 113 ; Push char 'q'
ASTORE ; Store char in array
INVOKEVIRTUAL 2 ; Call StringHandler.compare@[C@[C
PUSH 0
ICMP_EQ
JNZ L198
JMP L199
L198:
PUSH 0
CALL exit@I
JMP L200
L199:
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 1 ; String literal length
NEWARRAY C ; Create char array for string "w"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 119 ; Push char 'w'
ASTORE ; Store char in array
INVOKEVIRTUAL 2 ; Call StringHandler.compare@[C@[C
PUSH 0
ICMP_EQ
JNZ L201
JMP L202
L201:
CALL saveFile
JMP L203
L202:
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 2 ; String literal length
NEWARRAY C ; Create char array for string "wq"
DUP ; Duplicate array ref for ASTORE
PUSH 0 ; Push index 0
PUSH 119 ; Push char 'w'
ASTORE ; Store char in array
DUP ; Duplicate array ref for ASTORE
PUSH 1 ; Push index 1
PUSH 113 ; Push char 'q'
ASTORE ; Store char in array
INVOKEVIRTUAL 2 ; Call StringHandler.compare@[C@[C
PUSH 0
ICMP_EQ
JNZ L204
JMP L205
L204:
CALL saveFile
PUSH 0
CALL exit@I
JMP L206
L205:
LOAD 40  ; Load local var sh
PUSH 23 ; String literal length
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
PUSH 0
PUSH 23
LOAD 37  ; Load local var E
GETFIELD 8 ; Get field 'status_msg'
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
L206:
L203:
L200:
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 0
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'command_buffer'
LOAD 37  ; Load local var E
PUSH 0
PUTFIELD 6 ; Set field 'command_mode'
RET
.endmethod

.method processKeypress
.limit stack 6
.limit locals 63
LOAD 39  ; Load local var io
INVOKEVIRTUAL 0 ; Call IOHandler.readChar
STORE 58 ; Init c
LOAD 37  ; Load local var E
GETFIELD 6 ; Get field 'command_mode'
PUSH 1
ICMP_EQ
JNZ L207
JMP L208
L207:
LOAD 58  ; Load local var c
PUSH 92 ; Push ASCII for char '\r'
ICMP_EQ
JNZ L209
JMP L212
L212:
LOAD 58  ; Load local var c
PUSH 92 ; Push ASCII for char '\n'
ICMP_EQ
JNZ L209
JMP L210
L209:
CALL processCommand
JMP L211
L210:
LOAD 58  ; Load local var c
PUSH 127
ICMP_EQ
JNZ L213
JMP L216
L216:
LOAD 58  ; Load local var c
PUSH 92 ; Push ASCII for char '\b'
ICMP_EQ
JNZ L213
JMP L214
L213:
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
STORE 59 ; Init len
LOAD 59  ; Load local var len
PUSH 0
ICMP_GT
JNZ L217
JMP L218
L217:
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
LOAD 59  ; Load local var len
PUSH 1
ISUB
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'command_buffer'
L218:
JMP L215
L214:
LOAD 58  ; Load local var c
PUSH 27
ICMP_EQ
JNZ L219
JMP L220
L219:
LOAD 37  ; Load local var E
PUSH 0
PUTFIELD 6 ; Set field 'command_mode'
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
PUSH 0
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'command_buffer'
JMP L221
L220:
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
GETFIELD 7 ; Get field 'command_buffer'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 58  ; Load local var c
INVOKEVIRTUAL 3 ; Call StringHandler.insert@[C@I@C
L221:
L215:
L211:
RET
L208:
LOAD 37  ; Load local var E
GETFIELD 5 ; Get field 'insert_mode'
PUSH 0
ICMP_EQ
JNZ L222
JMP L223
L222:
LOAD 58  ; Load local var c
PUSH 105 ; Push ASCII for char 'i'
ICMP_EQ
JNZ L225
JMP L226
L225:
LOAD 37  ; Load local var E
PUSH 1
PUTFIELD 5 ; Set field 'insert_mode'
JMP L227
L226:
LOAD 58  ; Load local var c
PUSH 113 ; Push ASCII for char 'q'
ICMP_EQ
JNZ L228
JMP L229
L228:
PUSH 0
CALL exit@I
JMP L230
L229:
LOAD 58  ; Load local var c
PUSH 115 ; Push ASCII for char 's'
ICMP_EQ
JNZ L231
JMP L232
L231:
CALL saveFile
JMP L233
L232:
LOAD 58  ; Load local var c
PUSH 120 ; Push ASCII for char 'x'
ICMP_EQ
JNZ L234
JMP L235
L234:
CALL deleteChar
JMP L236
L235:
LOAD 58  ; Load local var c
PUSH 111 ; Push ASCII for char 'o'
ICMP_EQ
JNZ L237
JMP L238
L237:
LOAD 37  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
STORE 60 ; Init i
JMP L240
L241:
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 60  ; Load local var i
PUSH 1
ISUB
PUSH 0
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 60  ; Load local var i
PUSH 1
ISUB
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 37  ; Load local var E
LOAD 60  ; Load local var i
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
L242:
LOAD 60 ; Load local 'i'
DUP
PUSH 1
ISUB ; --
STORE 60 ; Store local 'i'
L240:
LOAD 60  ; Load local var i
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
ICMP_GT
JNZ L241
JMP L243
L243:
LOAD 37  ; Load local var E
GETFIELD 0 ; Get field 'rows'
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
PUSH 1024
IMUL
PUSH 0
IADD
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'rows'
LOAD 37  ; Load local var E
GETFIELD 1
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 1
PUSH 1
IADD ; ++
PUTFIELD 1
POP
LOAD 37  ; Load local var E
GETFIELD 4
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4
PUSH 1
IADD ; ++
PUTFIELD 4
POP
LOAD 37  ; Load local var E
PUSH 0
PUTFIELD 3 ; Set field 'cx'
LOAD 37  ; Load local var E
PUSH 1
PUTFIELD 5 ; Set field 'insert_mode'
JMP L239
L238:
LOAD 58  ; Load local var c
PUSH 58 ; Push ASCII for char ':'
ICMP_EQ
JNZ L244
JMP L245
L244:
LOAD 37  ; Load local var E
PUSH 1
PUTFIELD 6 ; Set field 'command_mode'
JMP L246
L245:
LOAD 58  ; Load local var c
CALL moveCursor@C
L246:
L239:
L236:
L233:
L230:
L227:
JMP L224
L223:
LOAD 58  ; Load local var c
PUSH 27
ICMP_EQ
JNZ L247
JMP L248
L247:
LOAD 37  ; Load local var E
PUSH 0
PUTFIELD 5 ; Set field 'insert_mode'
JMP L249
L248:
LOAD 58  ; Load local var c
PUSH 127
ICMP_EQ
JNZ L250
JMP L253
L253:
LOAD 58  ; Load local var c
PUSH 92 ; Push ASCII for char '\b'
ICMP_EQ
JNZ L250
JMP L251
L250:
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
PUSH 0
ICMP_GT
JNZ L254
JMP L255
L254:
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
PUSH 1
ISUB
PUSH 1
INVOKEVIRTUAL 4 ; Call StringHandler.erase@[C@I@I
LOAD 37  ; Load local var E
GETFIELD 3
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 3
PUSH 1
ISUB ; --
PUTFIELD 3
POP
L255:
JMP L252
L251:
LOAD 58  ; Load local var c
PUSH 92 ; Push ASCII for char '\r'
ICMP_EQ
JNZ L256
JMP L257
L256:
PUSH 1024
NEWARRAY C
STORE 61 ; Store new flattened array to 'newLine'
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
ISUB
LOAD 61  ; Load local var newLine
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
LOAD 37  ; Load local var E
GETFIELD 0 ; Get field 'rows'
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1024
IMUL
LOAD 37  ; Load local var E
GETFIELD 3 ; Get field 'cx'
IADD
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store to member array element 'rows'
LOAD 37  ; Load local var E
GETFIELD 1 ; Get field 'rowCount'
STORE 62 ; Init i
JMP L259
L260:
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 62  ; Load local var i
PUSH 1
ISUB
PUSH 0
LOAD 40  ; Load local var sh
LOAD 37  ; Load local var E
LOAD 62  ; Load local var i
PUSH 1
ISUB
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 37  ; Load local var E
LOAD 62  ; Load local var i
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
L261:
LOAD 62 ; Load local 'i'
DUP
PUSH 1
ISUB ; --
STORE 62 ; Store local 'i'
L259:
LOAD 62  ; Load local var i
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
ICMP_GT
JNZ L260
JMP L262
L262:
LOAD 40  ; Load local var sh
LOAD 61  ; Load local var newLine
PUSH 0
LOAD 40  ; Load local var sh
LOAD 61  ; Load local var newLine
INVOKEVIRTUAL 0 ; Call StringHandler.length@[C
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4 ; Get field 'cy'
PUSH 1
IADD
INVOKEVIRTUAL 1 ; Call StringHandler.substr@[C@I@I@[C
LOAD 37  ; Load local var E
GETFIELD 1
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 1
PUSH 1
IADD ; ++
PUTFIELD 1
POP
LOAD 37  ; Load local var E
GETFIELD 4
LOAD 37  ; Load local var E
LOAD 37  ; Load local var E
GETFIELD 4
PUSH 1
IADD ; ++
PUTFIELD 4
POP
LOAD 37  ; Load local var E
PUSH 0
PUTFIELD 3 ; Set field 'cx'
JMP L258
L257:
LOAD 58  ; Load local var c
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
PUSH 0 ; String literal length
NEWARRAY C ; Create char array for string ""
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
