#Q = {q0,got_a_1,got_b_1,got_a_2,got_b_2,return,check,halt_accept,return_acc,erase_acc,return_rej,erase_rej,acc1,acc2,acc3,acc4,rej1,rej2,rej3,rej4,rej5,halt_reject}

#S = {a,b,c}

#G = {_,a,b,c,x,t,r,u,e,f,l,s}

#q0 = q0

#B = _

#F = {halt_accept}

#N = 1

q0 x x r q0
q0 a x r got_a_1
q0 b x r got_b_1
q0 c c r check

got_a_1 a a r got_a_1
got_a_1 b b r got_a_1
got_a_1 c c r got_a_2
got_a_1 _ _ l return_rej

got_a_2 x x r got_a_2
got_a_2 a x l return
got_a_2 b b * return_rej
got_a_2 c c * return_rej
got_a_2 _ _ l return_rej

got_b_1 a a r got_b_1
got_b_1 b b r got_b_1
got_b_1 c c r got_b_2
got_b_1 _ _ l return_rej

got_b_2 x x r got_b_2
got_b_2 b x l return
got_b_2 a a * return_rej
got_b_2 c c * return_rej
got_b_2 _ _ l return_rej

return * * l return
return _ _ r q0

check x x r check
check a a * return_rej
check b b * return_rej
check c c * return_rej
check _ _ l return_acc

return_acc * * l return_acc
return_acc _ _ r erase_acc

return_rej * * l return_rej
return_rej _ _ r erase_rej

erase_acc * _ r erase_acc
erase_acc _ _ * acc1

erase_rej * _ r erase_rej
erase_rej _ _ * rej1

acc1 _ t r acc2
acc2 _ r r acc3
acc3 _ u r acc4
acc4 _ e * halt_accept

rej1 _ f r rej2
rej2 _ a r rej3
rej3 _ l r rej4
rej4 _ s r rej5
rej5 _ e * halt_reject