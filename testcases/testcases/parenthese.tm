; This program checks if the parentheses '(', ')' in a string are balanced.
; Input: a string containing ( and ) but no blanks or '!'
;
; The program keeps track of the number of '(' encountered on a stack to the left of the tape.
; When a new '(' is read, an extra '(' is pushed to the stack. When a ')' is read, an '(' is popped.
; If the parentheses are balanced, then the stack should be empty when the end of input is reached.
;
; Exercise: Extend this to handle other delimiters eg [], {}

; the finite set of states
#Q = {0,1,5,10,20,21,30,31,32,40,50,51,60,70,71,80,accept,accept2,reject,reject2,halt_accept,halt_reject}

; the finite set of input symbols
#S = {(,)}

; the complete set of tape symbols
#G = {(,),_,!,:}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept,halt_reject}

; the number of tapes
#N = 1

; Set up stack
0 * * l 1
0 _ _ l 1
1 _ ! r 51

; Find first non-blank
5 _ _ r 5
5 * * * 10



; Find first ( or )
10 ( ! l 20
10 ) ! l 30
10 _ _ l 40
10 * ! r 10


; Push ( onto stack
20 ! ! l 20
20 * * * 21
20 _ _ * 21
21 _ ( r 50
21 * * l 21


; Pop ( from stack
30 ! ! l 30
30 * * * 31
30 _ _ * 31
31 _ _ r 32
31 * * l 31
32 ( _ r 50
32 ! ! * 80  ; Trying to decrement below 0, unbalanced parentheses, missing (


; Reached end-of-input, check if stack is empty
40 ! ! l 40
40 _ _ r 60 ; Stack is empty, parentheses are balanced
40 * * l 70 ; Stack is not empty, unbalanced parentheses, missing )


; Finished increment/decrement, return to start
50 ! ! r 51
50 _ _ l 40 ; Reached end-of-input
50 * * r 50
51 ! ! r 51
51 * * * 10
51 _ _ * 10


; Parentheses balanced
60 ! _ r 60
60 * * * accept
60 _ _ * accept

; Parentheses unbalanced - missing )
70 _ _ r 71
70 * * l 70
71 _ _ * reject
71 * _ r 71

; Parentheses unbalanced - missing (
80 _ _ * reject
80 * _ r 80

accept * : r accept2
accept _ : r accept2
accept2 * ) * halt_accept
accept2 _ ) * halt_accept

reject * : r reject2
reject _ : r reject2
reject2 * ( * halt_reject
reject2 _ ( * halt_reject
