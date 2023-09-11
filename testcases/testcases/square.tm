; The turing machine that accepts a string in 1*, and checks whether the length of the string is square of some integer

; the finite set of states
#Q = {0,init,return0,return1,return2,compare,add1by2,add1by2_half,append1to2,accept1,accept2,accept3,accept4,reject1,reject2,reject3,reject4,reject5,erase_acc,erase_rej,halt_accept,halt_reject}

; the finite set of input symbols
#S = {1}

; the finite set of tape symbols
#G = {1,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3

; the transition functions

0 *__ *__ *** init
0 ___ ___ *** init

init 1__ 111 *r* return2
init ___ ___ *** accept1

return2 1_1 1_1 **l return2
return2 1__ 1__ **r compare

compare 1_1 1_1 r*r compare
compare 1__ 1__ *** return0
compare __1 __1 l** erase_rej
compare ___ ___ l** erase_acc

return0 1__ 1__ l** return0
return0 ___ ___ r** add1by2

add1by2 11_ 11_ *r* add1by2
add1by2 1__ 11_ *r* add1by2_half
add1by2_half 1__ 11_ *** return1

return1 11_ 11_ *l* return1
return1 1__ 1__ *r* append1to2

append1to2 11_ 111 *rr append1to2
append1to2 1__ 1__ **l return2

erase_acc 1__ ___ l** erase_acc
erase_acc ___ ___ *** accept1

erase_rej 1_1 __1 l** erase_rej
erase_rej __1 __1 *** reject1

; output 'true' and then accept
accept1 ___ t__ r** accept2
accept2 ___ r__ r** accept3
accept3 ___ u__ r** accept4
accept4 ___ e__ *** halt_accept

; output 'false' and then reject
reject1 __1 f_1 r** reject2
reject2 __1 a_1 r** reject3
reject3 __1 l_1 r** reject4
reject4 __1 s_1 r** reject5
reject5 __1 e_1 *** halt_reject
