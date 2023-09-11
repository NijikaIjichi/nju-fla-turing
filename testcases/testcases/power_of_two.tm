; the finite set of states
#Q = {q0,q1,q2,q3,q4,q5,acc1,acc2,acc3,halt_acc,rej1,rej2,rej3,halt_rej}

; the finite set of input symbols
#S = {0}

; the finite set of tape symbols
#G = {0,1,_,a,c,r,e,j}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {halt_acc}

; the number of tapes
#N = 1

; the transition functions

q0 _ _ * q1
q0 * * * q1

q1 _ _ * rej1
q1 0 _ r q2
q1 1 _ * rej1

q2 _ _ * acc1
q2 0 1 r q3
q2 1 1 r q2

q3 _ _ l q5
q3 0 0 r q4
q3 1 1 r q3

q4 _ _ * rej1
q4 0 1 r q3
q4 1 1 r q4

q5 _ _ r q2
q5 0 0 l q5
q5 1 1 l q5

acc1 _ a r acc2
acc2 _ c r acc3
acc3 _ c * halt_acc

rej1 _ r r rej2
rej2 _ e r rej3
rej3 _ j * halt_rej
