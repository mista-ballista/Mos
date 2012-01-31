L = 0.75;
L0 = 0.95;
y0 = 0.25;
x = 1;

A = (L/(2*sqrt(x^2+L0^2)));
B = (cosd(atand(L0/x)));
C = (sind(atand(L0/x)));

hej = 2*((x^2 - L0*(L0 + y0^2 +2*y0))/(2*L*(sqrt(x^2 + L0^2))))^2 ;
apa = hej - A^2 - C^2;