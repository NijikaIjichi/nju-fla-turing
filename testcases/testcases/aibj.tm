; The turing machine that accepts strings in the format (a^i)(b^j)(a^i)(b^j), where i and j is not zero

; the finite set of states
#Q = {q0,0,copy_a,copy_b,return,compare,accept1,accept2,accept3,accept4,erase,reject1,reject2,reject3,reject4,reject5,halt_accept,halt_reject}

; the finite set of input symbols
#S = {a,b}

; the finite set of tape symbols
#G = {a,b,_,t,r,u,e,f,l,s}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2
; tape0: i/o and comparison
; tape1: store the left part of input,used for comparison

; the transition functions
; start state,do nothing
q0 *_ *_ ** 0

0 a_ a_ ** copy_a
0 b_ b_ ** erase        ; i==0 is not allowed
0 __ __ ** reject1      ; empty string

; copy the 'a's on tape0 to tape1 until met something other than 'a'
copy_a a_ _a rr copy_a
copy_a b_ b_ ** copy_b
copy_a __ __ ** reject1 ; j==0 is not allowed

; copy the 'b's on tape0 to tape1 until met something other than 'b'
copy_b a_ a_ *l return
copy_b b_ _b rr copy_b
copy_b __ __ ** reject1

; copy finished,return to tape1's beginning,get ready for comparison
return aa aa *l return
return ab ab *l return
return a_ a_ *r compare

; compare tape0 and tape1, reject on finding difference,accept otherwise
compare aa _a rr compare
compare bb _b rr compare
compare ab _b rr erase
compare ba _a rr erase
compare a_ __ rr erase
compare b_ __ rr erase
compare _a _a rr erase
compare _b _b rr erase
compare __ __ ** accept1

; erase the rest part of tape 0 and output reject after
erase aa _a r* erase
erase ab _b r* erase
erase a_ __ r* erase
erase ba _a r* erase
erase bb _b r* erase
erase b_ __ r* erase
erase _a __ ** reject1
erase _b __ ** reject1
erase __ __ ** reject1

; output 'true' and then accept
accept1 __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; output 'false' and then reject
reject1 __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject
