#
# DTW implementation
#

s1=[3,4,2]
x1 = 0:2

s2=[4,2,2,3]
x2= 0:3

D=zeros(3,4)
for i = 1:length (s1)
	for j = 1:length (s2)
		D(i,j) = abs(s1(i) - s2(j));
	endfor
endfor
D

plot(x1, s1, x2, s2)

dwt(s1, s2)

