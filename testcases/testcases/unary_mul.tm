; The turing machine that accepts a right multiplication equation

; the finite set of states
#Q = {0,copy_0to1,mul,return1,copy_1to2,return2,compare,accept1,accept2,accept3,accept4,erase,reject1,reject2,reject3,reject4,reject5,halt_accept,halt_reject}

; the finite set of input symbols
#S = {1,x,=}

; the finite set of tape symbols
#G = {1,x,=,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3
; tape0: i/o and comparison
; tape1: store the first number,used for computing the result
; tape3: store the result of multiplication computation for comparison

; the transition functions
0 1__ 1__ *** copy_0to1
0 x__ x__ *** erase
0 =__ =__ *** erase
0 ___ ___ *** reject1

copy_0to1 1__ _1_ rr* copy_0to1
copy_0to1 x__ ___ r** mul
copy_0to1 =__ =__ *** erase
copy_0to1 ___ ___ *** reject1

mul 1__ 1__ *l* return1
mul x__ x__ *** erase
mul =__ =__ **l return2
mul ___ ___ *** reject1

return1 11_ 11_ *l* return1
return1 1__ 1__ *r* copy_1to2

copy_1to2 11_ 111 *rr copy_1to2
copy_1to2 1__ ___ r** mul

return2 =_1 =_1 **l return2
return2 =__ ___ r*r compare

compare 1_1 __1 r*r compare
compare 1__ 1__ *** erase
compare __1 ___ *** reject1
compare ___ ___ *** accept1
compare x__ ___ r** erase
compare x_1 ___ r** erase
compare =__ ___ r** erase
compare =_1 ___ r** erase

; erase the rest part of tape 0 and output reject after
erase 1__ ___ r** erase
erase x__ ___ r** erase
erase =__ ___ r** erase
erase ___ ___ r** reject1

; output 'true' and then accept
accept1 ___ t__ r** accept2
accept2 ___ r__ r** accept3
accept3 ___ u__ r** accept4
accept4 ___ e__ *** halt_accept

; output 'false' and then reject
reject1 ___ f__ r** reject2
reject2 ___ a__ r** reject3
reject3 ___ l__ r** reject4
reject4 ___ s__ r** reject5
reject5 ___ e__ *** halt_reject
