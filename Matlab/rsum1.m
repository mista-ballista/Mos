function value=rsum1(a,b,n)
%RSUM1: Computes a Riemann Sum for the function f on
%the interval [a,b] with a regular partition of n points.
%The points on the intervals are chosen as the right endpoints.
value = 0;
dx = (b-a)/n;
dx
for k=1:n
c = a+k*dx;
value = value + kraft(c);
end
value = dx*value;