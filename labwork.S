  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.global delay
#.global hexasc
.global time2string
.global enable_interrupt

.data

.text
hexasc: 
    andi    $a0,$a0,0xf 
    
    slti    $t0,$a0,10  
    beq     $t0,$0,chars    
    nop
    # Numbers 0-9
nums:   addi    $v0,$a0,0x30    
    	j hexend        
    	nop
    # Characters A-F
chars:  addi    $v0,$a0,0x37    
    
hexend: jr  $ra         
    nop
	
# a0 = time to wait in milliseconds
delay: 
	move	$t0, $a0
whileloop:	
	bge	$zero, $t0, delaystop
	nop
	addi	$t0, $t0, -1
	addi	$t1, $zero, 0
forloop:
	bge	$t1, 4311, whileloop
	nop
	addi	$t1, $t1, 1
	j 	forloop		
delaystop:
	jr	$ra
	nop

# a0 = address for writing
# a1 = time information
time2string:
	PUSH	$ra
	PUSH	$s0	#for output address
	PUSH	$s1	#NBCD info
	PUSH	$s2	#for checking which bits to read
	PUSH	$s3
	move 	$s0, $a0
	move 	$s1, $a1
	li	$s2, 0xF
	sll	$s2, $s2, 12
	addi	$s3, $zero, 12
time2stringloop:
	and	$t0, $s1, $s2
	srlv	$a0, $t0, $s3
	PUSH	$s0	
	PUSH	$s1	
	PUSH	$s2
	PUSH	$s3	
	jal 	hexasc
	nop
	POP	$s3
	POP	$s2
	POP	$s1
	POP	$s0	
	sb	$v0, 0($s0)
	srl 	$s2, $s2, 4
	addi 	$s3, $s3, -4
	addi	$s0, $s0, 1
	beq	$s3, 4, time2stringcolon
	nop
	bne	$s3, -4, time2stringloop
	nop
time2stringend:
	sb	$zero, 0($s0)
	POP	$s3
	POP	$s2
	POP	$s1	
	POP	$s0	
	POP	$ra
	jr	$ra
	nop
time2stringcolon:
	addi	$t0, $zero, 0x3A
	sb 	$t0, 0($s0)
	addi	$s0, $s0, 1
	j	time2stringloop
	nop

enable_interrupt:
	ei
	jr $ra