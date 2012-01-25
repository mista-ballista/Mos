clc;
clear all;
b = 0.03; %Bredd
h = 0.03; %Höjd

L = 2;

P = 1000; %Kraft
E = 9e9; %Youngs mod
% I = (b*h^3)/12; %Tröghetsmoment
% x1 = x(1:size(x,2)/2);
% x2 = x(size(x,2)/2: size(x,2));
%
% w1 = -(P.*x1 .*(4.*x1.^2 - 3.*L.^2))/(48*E*I);
% w2 = (P*(x2-L).*(L^2 - 8*L.*x2 + 4.*x2.^2))/(48*E*I);
%
% w = [w1 w2]
% w = w - max(w)
samples = 100;
for P = 500:100:2500
    [w,x] = bow(b, h, L, E, P, samples)
    min(w)
    plot(x,w)
    AXIS([0 L -1 1])
    hold on
end



