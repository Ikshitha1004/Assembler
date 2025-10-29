
.code
.method main
.limit stack 4
.limit locals 8
NEW TestClass ; Create new object of class TestClass
PUSH 10
INVOKESPECIAL 1 ; Call constructor for TestClass
STORE 2 ; Store new object to 'obj'
LOAD 2  ; Load local var obj
INVOKEVIRTUAL 0 ; Call TestClass.publicMethod
RET
.endmethod
