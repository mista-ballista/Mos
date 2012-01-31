L = 0.75;
L0 = 0.95;
y0 = 0.25;
x = 1;

A = (L/(2*sqrt(x^2+L0^2)));
B = (cosd(atand(L0/x)));
C = (sind(atand(L0/x)));

hej = 2*((x^2 - L0*(L0 + y0^2 +2*y0))/(2*L*(sqrt(x^2 + L0^2))))^2 ;
apa = hej - A^2 - C^2;


Q = 0.75;
W = 0.95;
Y = 0.25;
L = 1;

% Q*sin(X)+Wcos(arcsin((Y+Q*cos(X))/W))=L find X
% 0.75*sin(X)+0.95cos(arcsin((0.25+0.75*cos(X))/0.95))=1

X = acos((-L^2* Q * Y-sqrt(L^6 *(-Q^2)+2* L^4* Q^4+2* L^4* Q^2* W^2-2* L^4* Q^2* Y^2-L^2* Q^6+2* L^2* Q^4* W^2+2* L^2* Q^4* Y^2-L^2* Q^2* W^4+2* L^2* Q^2* W^2* Y^2-L^2* Q^2* Y^4)+Q^3 *(-Y)+Q* W^2* Y-Q* Y^3)/(2* (L^2* Q^2+Q^2* Y^2)));