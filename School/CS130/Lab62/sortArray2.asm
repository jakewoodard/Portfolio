	.data
FIN:	.asciiz "/Users/woodardkj/Desktop/CS130/Lab62/ints_210.dat"
FOUT: 	.asciiz "/Users/woodardkj/Desktop/CS130/Lab62/ints_62.dat"
E1:	.asciiz "Error: input file open error"
E2:	.asciiz "Error: output file open error"
E3:	.asciiz "Error: input file read error"
E4: 	.asciiz "Error: output file write error"
NUM:    .asciiz "Processed     ints"
BUFF:	.word 0:200
MAXX:	.word 200
	.text
	
MAIN:	li $v0, 13         # open input file for reading
	la $a0, FIN        # load address of file name
	li $a1, 0          # 0 for reading
	li $a2, 0      
	syscall            # open file
	move $s0, $v0      # store file descriptor
	beq $s0, -1, ERR1  # conditional branch for error checking
	
 	li $v0, 14         # read input file into buffer
	move $a0, $s0      # put file descriptor into argument 
	la $a1, BUFF       # load address of buffer
 	la $a2,	MAXX       # load max value
	syscall            # read file
	move $s2, $v0      # store total characters read
	blt $s2, 1, ERR3
	
	li $v0, 13         # open output file for writing 
	la $a0, FOUT       # load address of file name
	li $a1, 1          # 1 for writing
	li $a2, 0
	syscall            # open file
	move $s1, $v0      # store file descriptor
	beq $s1, -1, ERR2  # conditional statement error checking
	
	li $v0, 15         # write contents of buffer into the output file
	move $a0, $s1      # put file descriptor into argument 0
	la $a1, BUFF       # load address of word buffer
	la $a2, 800        # specify max
	syscall            # write
	move $s3, $v0      # store number of bytes written
	bne $s3, $s2, ERR4 # error check number of byte written
	
	li $v0, 16         # load instruction to close input file
	move $a0, $s0      # move file descriptor
	syscall            # close file
	
	li $v0, 16         # load instruction to close output file
	move $a0, $s1      # move file descriptor
	syscall            # close file

	div $s2, $s2, 4    # divide bytes read by 4 to get total 
	li $t0, '0'        # load '0' into temp register	
	li $t1, 10         # load 10 into temp register for division
	li $t2, 100        # load 100 into temp register for division
 	div $s2, $t2       # divide total characters read by 100
	mfhi $t3           # store remainder in temp register
	mflo $s3           # store quotient (100 dig)
	div $t3, $t1       # divide by 10
	mfhi $s5           # store remainder (1 dig)
	mflo $s4           # store quotient (10 dig)
	
	add $s3, $s3, $t0  # turn each int into a char 
	add $s4, $s4, $t0  # to insert into string
	add $s5, $s5, $t0  # by adding '0'
	
	la $t4, NUM        # load address of unmodified text
        sb $s3, 10($t4)    # load 100 digit
	sb $s4, 11($t4)	   # load 10 digit
	sb $s5, 12($t4)    # load 1 digit
	li $v0, 4          # load instruction to print string
	move $a0, $t4      # load address of string
	syscall            # print
	b EXIT             # branch to exit
	
ERR1:	li $v0, 4      	   # load instruction to print string
	la $a0, E1         # load address of error message 1
	syscall            # print 
	b EXIT             # branch to exit
	
ERR2: 	li $v0, 4          # load instruction to print string
	la $a0, E2         # load address of error message 2
	syscall            # print
	b EXIT             # branch to exit

ERR3: 	li $v0, 4          # load instruction to print string
	la $a0, E3         # load address of error message 3
	syscall            # print
	b EXIT             # branch to exit	

ERR4:   li $v0, 4          # load instruction to print string
	la $a0, E4         # load address of error message 4
	syscall            # print
	b EXIT             # branch to exit
	
	
EXIT:   li   $v0, 10       # specify Exit service
        syscall            # 
