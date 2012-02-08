function [ F ] = kraft( draglangd )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here



L = 0.5; %Lemmarnas längd
Y0 = 0.1; %Längd på handtag
M = 0.093; %Massan på lemmar

kappa =620; %Fjäder konstant för bågen
m = 0.025; %Massan på pilen

theta0= 0.3491;

x0=L*sin(theta0); %Stränghöjd i startläge
L0 = Y0+L*cos(theta0);
% a0= asin((Y0+L*cos(theta0))/L0);
k=(L*L0*sin(theta0)*kappa)/(2*theta0);
x=x0+draglangd;

%Moment of inertia från pdf
I = 0.3*M*L^2;

theta = acos((-x^2* L * Y0+sqrt(x^6 *(-L^2)+2* x^4* L^4+2* x^4* L^2* L0^2-2* x^4* L^2* Y0^2-x^2* L^6+2* x^2*...
        L^4* L0^2+2* x^2* L^4* Y0^2-x^2* L^2* L0^4+2* x^2* L^2* L0^2* Y0^2-x^2* L^2* Y0^4)+L^3 *(-Y0)+L* L0^2*...
        Y0-L* Y0^3)/(2* (x^2* L^2+L^2* Y0^2)));

    
    
a=asin((Y0+L*cos(theta))/L0);

a

F = 2*(k/L)*theta*(cos(a)/cos(a-theta));



end

