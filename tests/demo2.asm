.class ObjectExample
.method public static main
    .limit stack 4
    .limit locals 1

    NEW MyClass          ; allocate new object → obj_ref
    DUP                  ; duplicate obj_ref (for field set)
    LOAD 0               ; push some value
    PUTFIELD MyClass/label I

    RET
.end
