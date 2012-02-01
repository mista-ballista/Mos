% L = 0.75;
% L0 = 0.95;
% y0 = 0.25;
% x = 1;
% 
% A = (L/(2*sqrt(x^2+L0^2)));
% B = (cosd(atand(L0/x)));
% C = (sind(atand(L0/x)));
% 
% hej = 2*((x^2 - L0*(L0 + y0^2 +2*y0))/(2*L*(sqrt(x^2 + L0^2))))^2 ;
% apa = hej - A^2 - C^2;


L = 0.75;
L0 = 0.95;
Y0 = 0.25;
x = 1;

% L*sin(X)+L0cos(arcsin((Y0+L*cos(X))/L0))=x find X
% 0.75*sin(X)+0.95cos(arcsin((0.25+0.75*cos(X))/0.95))=1

theta = acosd((-x^2* L * Y0+sqrt(x^6 *(-L^2)+2* x^4* L^4+2* x^4* L^2* L0^2-2* x^4* L^2* Y0^2-x^2* L^6+2* x^2* L^4* L0^2+2* x^2* L^4* Y0^2-x^2* L^2* L0^4+2* x^2* L^2* L0^2* Y0^2-x^2* L^2* Y0^4)+L^3 *(-Y0)+L* L0^2* Y0-L* Y0^3)/(2* (x^2* L^2+L^2* Y0^2)));