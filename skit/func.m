
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
% m=5;
% L=10;
% b=0.3;
% h=0.5;
% E= 11e9;
% omega0= 2.5;
% omega=omega0+4;
% I = b*h^3/12;
% 
% P = omega*48*E*I/L^3;
% 
% V = P*t/m;
% l=0.75;
% kappa=620;
% L=0.95;
% y=0.25;

b=0.3;
h=0.5;
I = b*h^3/12;

L = 0.75;
L0 = 0.95;
Y0 = 0.25;
% x = 1;

kappa =620;
m = 0.6;

theta0=acosd((L-Y0)/L0);
x0=L*sind(theta0);

x = [1:-0.001:x0];

% L*sin(X)+L0cos(arcsin((Y0+L*cos(X))/L0))=x find X
% 0.75*sin(X)+0.95cos(arcsin((0.25+0.75*cos(X))/0.95))=1

theta = zeros(size(x,2),1);

a = zeros(size(x,2),1);

for i=1:size(x,2)

theta(i) = acos((-x(i)^2* L * Y0+sqrt(x(i)^6 *(-L^2)+2* x(i)^4* L^4+2* x(i)^4* L^2* L0^2-2* x(i)^4* L^2* Y0^2-x(i)^2* L^6+2* x(i)^2* L^4* L0^2+2* x(i)^2* L^4* Y0^2-x(i)^2* L^2* L0^4+2* x(i)^2* L^2* L0^2* Y0^2-x(i)^2* L^2* Y0^4)+L^3 *(-Y0)+L* L0^2* Y0-L* Y0^3)/(2* (x(i)^2* L^2+L^2* Y0^2)));


a(i)=asin((Y0+L*cos(theta(i))/L0));

xprim = L*(cos(a(i)-theta(i))/cos(a(i)));
xbiss = L*(sin(a(i)-theta(i)/cos(a(i))))-(L^2/L0)*(sin(theta(i))^2/cos(a(i))^3);

end
k=(L*L0*sin(theta0)*kappa)/2*theta0;


% F=2*(k/L)*theta*(cos(a)/cos(a-theta));
% 
% P = (k/L)*(theta/cos(a-theta));


% (m*xprim^2+2*I)*thetabiss + m*xprim*xbiss*thetaprim^2+2*k*theta=0;

E =(0.5*m*xprim^2+I)*thetaprim+k*(theta^2-theta0^2);



