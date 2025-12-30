/* Retro68/Gnu assembler */

	.text
	.globl SENDCOMPLETE
	
SENDCOMPLETE:
	# A0 has address of IOParam

    moveal %a0@(-4),%a0	/* Get busy flag address */
    clrb  %a0@(0)	/* Clear the busy flag (FALSE) */
    rts
	
