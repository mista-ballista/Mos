function [ vArrow ] = hastighet( xdrag )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

% L*sin(X)+L0cos(arcsin((Y0+L*cos(X))/L0))=x find X
% 0.75*sin(X)+0.95cos(arcsin((0.25+0.75*cos(X))/0.95))=1

clc
close all
% Moment of inertia
% b=0.3;
% h=0.5;
% I = b*h^3/12;




L = 0.75; %Lemmarnas l�ngd
L0 = 0.95; %Halva str�ngens l�ngd
Y0 = 0.25; %L�ngd p� handtag
% x = 1;
M = 0.93; %Massan p� lemmar

kappa =620; %Fj�der konstant f�r b�gen
m = 0.2; %Massan p� pilen

theta0=acos((L0-Y0)/L); %Startvinkel
x0=L*sin(theta0); %Str�ngh�jd i startl�ge

x = [x0+xdrag:-0.001:x0]; %Dra upp till xdrag

%Moment of inertia fr�n pdf
I = 0.3*M*L^2;

theta = zeros(size(x,2),1);
k=(L*L0*sin(theta0)*kappa)/2*theta0;
a = zeros(size(x,2)-1,1);
thetaprim= zeros(size(x,2),1);
xprim = zeros(size(x,2),1);
xbiss = zeros(size(x,2),1);
E =  zeros(size(x,2)-1,1);
v = zeros(size(x,2)-1,1);
for i=1:size(x,2)

theta(i) = acos((-x(i)^2* L * Y0+sqrt(x(i)^6 *(-L^2)+2* x(i)^4* L^4+2* x(i)^4* L^2* L0^2-2* x(i)^4* L^2* Y0^2-x(i)^2* L^6+2* x(i)^2*...
 L^4* L0^2+2* x(i)^2* L^4* Y0^2-x(i)^2* L^2* L0^4+2* x(i)^2* L^2* L0^2* Y0^2-x(i)^2* L^2* Y0^4)+L^3 *(-Y0)+L* L0^2*...
Y0-L* Y0^3)/(2* (x(i)^2* L^2+L^2* Y0^2)));
end

theta1 = theta(2:size(theta));
theta2 = theta(1:size(theta)-1);
thetaprim = theta1 - theta2;

for i=1:size(x,2)-1
% thetaprim(i) = 1/(L* sqrt(1-(x(i)-L0*cos(a(i)))^2/L^2));
% Fel h�r, a ska g� fr�n 90 grader och minska ner till max 45?
a(i)=2.55 - asin((Y0+L*cos(theta(i))/L0));

xprim(i) = L*(cos(a(i)-theta(i))/cos(a(i)));
xbiss(i) = L*(sin(a(i)-theta(i)/cos(a(i))))-(L^2/L0)*(sin(theta(i))^2/cos(a(i))^3);
E(i) =(0.5*m*xprim(i)^2+I)*thetaprim(i)+k*(theta(i)^2-theta0^2); %Kinetic energy f�r pilen
% E = 1/2 *mv^2
v(i) = sqrt((2*-E(i))/m);

end

test = sqrt((2*abs(E))/m);
vArrow = sqrt((2*sum(test)/m));


plot(a*180/pi);
title('Alpha i grader');
figure;
plot(theta*180/pi);
title('Theta i grader');
figure;

plot(E);
title('E');
figure;

plot(v);
title('v');

figure;
plot(x);
title('str�nges mittpunkt');

figure;
plot(xprim);
title('str�ngens hastighet');

figure;
plot(xbiss);
title('str�ngens acceleration');
vArrow
end