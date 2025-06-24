.data
.globl foo

foo:
    pushq %rbp
    movq  %rsp, %rbp

    movl  $100, %eax
    
    leave
    ret
