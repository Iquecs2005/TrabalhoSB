.data
.globl foo

foo:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $20, %rsp

    movl $0, %eax
    movl $0, %ecx
    movl -8(%rbp), %eax
    movl -8(%rbp), %ecx
    movl %edi, -8(%rbp)
    movl %esi, -8(%rbp)
    movl %edx, -8(%rbp)
    addl %ecx, %eax
    subl %ecx, %eax
    imull %ecx, %eax
    movl %eax, -8(%rbp)

    cmpl $0, -8(%rbp)
    jg test

    leave
    ret

test:
    ret
