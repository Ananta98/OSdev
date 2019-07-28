.section .init
_init:
    popl %ebp
    ret

.section .fini
_fini:
    popl %ebp
    ret