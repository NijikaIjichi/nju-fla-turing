#Q = {0,one,judge,left,right,accept1,accept2,accept3,accept4,reject1,reject2,reject3,reject4,reject5,halt_accept,halt_reject}
#S = {1}
#G = {1,_,t,r,u,e,f,a,l,s}
#q0 = 0
#B = _
#F = {halt_accept}
#N = 2

0 __ __ ** accept1
0 1_ 1_ ** one

one 1_ _1 rr judge
one __ __ ** reject1

judge __ __ ** accept1
judge 1_ 1_ *l left

left 1* 1* *l left
left 1_ 1_ *r right

right 11 _1 rr right
right 1_ _1 rr one
right _1 __ ** reject1
right __ __ ** reject1

accept1 __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

reject1 __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject
