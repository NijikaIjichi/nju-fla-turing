; the finite set of states
#Q = {q0,forward,q1,q2,q3,end}

; the finite set of input symbols
#S = {0,1}

; the finite set of tape symbols
#G = {0,1,_}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {end}

; the number of tapes
#N = 1

; the transition functions

q0 * * * forward
q0 _ _ * forward

forward 0 0 r forward
forward 1 1 r forward
forward _ _ l q1

q1 0 _ l end
q1 1 0 l q2
q1 _ _ l q1

; carry
q2 0 1 l q3
q2 1 0 l q2
q2 _ 1 r forward

; no carry
q3 0 0 l q3
q3 1 1 l q3
q3 _ _ r forward
