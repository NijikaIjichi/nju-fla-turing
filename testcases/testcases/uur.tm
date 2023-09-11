; the finite set of states
#Q = {read,got_a,got_b,cmp_a,cmp_b,return,acc1,acc2,acc3,halt_acc,rej1,rej2,rej3,halt_rej}

; the finite set of input symbols
#S = {a,b}

; the finite set of tape symbols
#G = {a,b,c,e,r,j,_}

; the start state
#q0 = read

; the blank symbol
#B = _

; the set of final states
#F = {halt_acc}

; the number of tapes
#N = 1

; the transition functions
read a _ r got_a
read b _ r got_b
read _ _ r acc1

got_a a a r got_a
got_a b b r got_a
got_a _ _ l cmp_a

got_b a a r got_b
got_b b b r got_b
got_b _ _ l cmp_b

cmp_a a _ l return
cmp_a b _ l rej1
cmp_a _ _ l rej1

cmp_b a _ l rej1
cmp_b b _ l return
cmp_b _ _ l rej1

acc1 _ a r acc2
acc2 _ c r acc3
acc3 _ c * halt_acc

rej1 _ r r rej2
rej2 _ e r rej3
rej3 _ j * halt_rej

return a a l return
return b b l return
return _ _ r read
