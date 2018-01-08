.globl zfunction

zfunction:
	push %ebp
	movl %esp, %ebp
	movl 0x8(%ebp), %eax
	andl $0xff803fff, %eax
	shll $0x4, %eax	
	andl $0xfffffff0, %eax
	leave
	ret

