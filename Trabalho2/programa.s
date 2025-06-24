.data
.globl foo

foo:
    pushq %rbp
    movq  %rsp, %rbp

    movl $0x10203040, %ecx
    movl $0x10203040, %r8d
    movl $0x10203040, %r9d
    movl $0x10203040, %r10d
    movl $0x10203040, %r11d
    
    movl %ecx, %r8d
    movl %ecx, %r9d
    movl %ecx, %r10d
    movl %ecx, %r11d

    movl %ecx, %eax
    movl %r8d, %eax
    movl %r9d, %eax
    movl %r10d, %eax
    movl %r11d, %eax
    
    leave
    ret
