; The turing machine that traverse the input and do nothing

; the finite set of states
#Q = {q0,halt_accept}

; the finite set of input symbols
#S = {a,b}

; the finite set of tape symbols
#G = {_,a,b}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions
q0 ** *a ** halt_accept
