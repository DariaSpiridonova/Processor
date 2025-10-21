;----------------------------------------------------------------------------------------------------------
; Main function                                                                                           -
;----------------------------------------------------------------------------------------------------------
PRINT_RAM 
CALL :10 ; Get parameters
CALL :1
PRINT_RAM
HLT



:1 ; while y < 10
:4
PUSHR SP
PUSH 10
JA :2
RET

:2
:7
PUSHR DX
PUSH 10
JA :3
SUB
POPR DX
PUSH 0
POPR DX
PUSHR SP
PUSH 1
ADD
POPR SP
JMP :4


:3
PUSHR DX
PUSHR AX
SUB
POPR BP
PUSHR BP
PUSHR BP
MUL
POPR BP
PUSHR SP
PUSHR BX
SUB
POPR SI
PUSHR SI
PUSHR SI
MUL
POPR SI
PUSHR BP
PUSHR SI
ADD
PUSHR CX
JAE :5
SUB
POPR BP
PUSH 0
:6
POPM [DI]
PUSHR DX
PUSH 1
ADD
POPR DX
PUSH 1
PUSHR DI
ADD
POPR DI
JMP :7

:5
PUSH 1
JMP :6


;----------------------------------------------------------------------------------------------------------
; Getting the circle parameters and init registers                                                     -
;----------------------------------------------------------------------------------------------------------
:10
IN
IN
IN
POPR CX
POPR BX 
POPR AX
PUSHR CX
PUSHR CX
MUL
POPR CX

PUSH 0
PUSH 0
POPR DX
POPR SP
PUSH 100
POPR DI
RET