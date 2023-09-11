; 4-state busy beaver
; When run with blank input, prints a number of 1's then halts.
; For background on the busy beaver problem, see wikipedia.

; the finite set of states
#Q = {0,a,b,c,d,halt}

; the finite set of input symbols
#S = {1}

; the complete set of tape symbols
#G = {1,_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 1

0 _ _ * a

a _ 1 r b
a 1 1 l b
b _ 1 l a
b 1 _ l c
c _ 1 r halt
c 1 1 l d
d _ 1 r d
d 1 _ r a
