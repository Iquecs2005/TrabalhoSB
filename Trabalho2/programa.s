.data
.globl foo

foo:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $20, %rsp

    jle 0x10203040
    
    leave
    ret

test:
    ret
