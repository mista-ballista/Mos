function [t,Z] = eulers(fun,a,Z0,n)

h = (a(2)-a(1))/n;
Z(:,1) = Z0;
t(1) = a(1);
for k = 1:n
    Z(:, k+1) = Z(:,k) + h*feval(fun,t(k),Z(:,k));
    t(k+1) = a(1) + h*k;
end
