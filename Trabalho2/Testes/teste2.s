.data
.globl foo

foo:
    pushq %rbp
    movq  %rsp, %rbp

    movl  $100, %ecx
    movl  %ecx, %eax
    
    leave
    ret
