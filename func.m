function [ v ] = Untitled(  )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
m=5;
L=10;
b=0.3;
h=0.5;
E= 11e9;
omega0= 2.5;
omega=omega0+4;
I = b*h^3/12;

P = omega*48*E*I/L^3;

V = P*t/m;


end

