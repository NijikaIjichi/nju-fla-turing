#Q = {0,right0,right1,left0,left1,halt}
#S = {0,1}
#G = {0,1,_}
#q0 = 0
#B = _
#F = {halt}
#N = 1

0 _ _ * halt
0 0 _ r right0
0 1 _ r right1
right0 0 0 r right0
right0 1 0 r right1
right0 _ _ l left0
right1 0 1 r right0
right1 1 1 r right1
right1 _ _ l left1
left0 * * l left0
left0 _ 0 * halt
left1 * * l left1
left1 _ 1 * halt
