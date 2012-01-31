function [ v ] = Untitled(  )
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
l=0.75;
kappa=620;
L=0.95;
y=0.25;
theta0=acosd((L-y)/l);
x0=l*sind(theta0);
theta=(0.731196*180)/pi;

(2.90035*180)/pi;

k=(l*L*sind(theta0)*kappa)/2*theta0

alpha=asind((y+l*cosd(theta))/L);

v0=(l*cosd(alpha-theta))/cos(alpha)

% syms X
% Test = solve(0.75*sind(X)+0.95*cosd(arcsind((0.25+0.75*cos(X))/0.95))-1)
% Test = solve(0.75*sind(X)+0.95*cosd(arcsind((0.25+0.75*cos(X))/0.95))-x)
% 0.75*sin(X)+0.95cos(arcsin((0.25+0.75*cos(X))/0.95))=1
X = solve('0.75*sind(X)+0.95*cosd(arcsind((0.25+0.75*cos(X))/0.95))-1=0')

% x är draglängd
x= l*sind(theta)+L*cosd(alpha);
F= 2*(k/l)*theta*(cosd(alpha)/cosd(alpha-theta));
P=(k/l)*(theta*(pi/180))/sind(theta);
G=k*theta;


end

