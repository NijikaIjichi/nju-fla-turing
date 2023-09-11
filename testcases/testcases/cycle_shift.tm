; The turing machine that performs cycle shift rightward by 1 char on its input
; for example, input 01010 gets 00101

; The finite set of states
#Q = {q0,copy0to1_0,copy0to1_1,copy1to0,return_0,return_1,write_0,write_1,halt_accept}

; the finite set of input symbols
#S = {0,1}

; the finite set of tape symbols
#G = {_,0,1,t,r,u,e,f,a,l,s}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

q0 __ __ ** halt_accept
q0 *_ *_ ** copy0to1_0

copy0to1_0 0_ 00 rr copy0to1_0
copy0to1_0 1_ 11 rr copy0to1_1
copy0to1_1 0_ 00 rr copy0to1_0
copy0to1_1 1_ 11 rr copy0to1_1
copy0to1_0 __ __ ll return_0
copy0to1_1 __ __ ll return_1

return_0 ** ** ll return_0
return_0 __ __ rr write_0
return_1 ** ** ll return_1
return_1 __ __ rr write_1

write_0 ** 0* r* copy1to0
write_1 ** 1* r* copy1to0

copy1to0 *0 00 rr copy1to0
copy1to0 *1 11 rr copy1to0
copy1to0 _* _* ll halt_accept
