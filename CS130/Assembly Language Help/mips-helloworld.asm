# Hello World?
	.data
TXT:    .asciiz "Hello world"

	.text
PFW:    la $t0, TXT
	li $v0, 4
	syscall