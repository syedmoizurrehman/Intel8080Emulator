MVI D, 0
l1:
MVI E, 0
l2:
	MVI A, 2
	CMP E	; A - E
	PUSH PSW
	INR E
	POP PSW
	JNZ l2
MVI A, 2
CMP D	; A - D
PUSH PSW
INR D
POP PSW
JNZ l1
HLT