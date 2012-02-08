clc
clear all
close all

M= 1;
m= 0.025;
c= 1/6;
K= 200;
r=1;


thetaMax=0.5;
theta= 0.5;
theta0= 20*pi/180;
% thetaE=20*pi/180;
thetaE=0;

% x0 = r*sin(theta0);
x0=0.15;
x1 = r*sin(theta);
y= cos(theta);
l= r*cos(theta);

% x=r*(sin(theta)+sqrt(cos(theta0)^2-cos(theta)^2));
x=r;

% v=r*(cos(theta)+(cos(2*theta)/sqrt(cos(theta0)^2-cos(theta)^2)))*omega;



A = (thetaMax^2-thetaE*thetaMax-theta0^2+thetaE*theta);
B = m+(cos(theta0)^2-cos(theta)^2)/(cos(2*theta)+cos(theta)*sqrt(cos(theta0)^2-cos(theta)^2))^2;

v = sqrt(K*(A/(B*c*M)));
