clear all;
clc;

%Konstanter
g=9.82; %gravitation
vinkel= 45; %utg�ngsvinkel
y0=1; %Start h�jd
draTillbaka = 0.6; %Hur l�ngt vi drar pilen
v = hastighet(draTillbaka); %Pilens utg�ngshastighet n�r tilen dras tillbaka

% Total distance
d = (v*cosd(vinkel)/g)*(v*sind(vinkel)+sqrt(((v*sind(vinkel))^2+2*g*y0)));

% hastighet i x led
timeOfFlight = (v*sind(vinkel)+sqrt(((v*sind(vinkel))^2+2*g*y0)))/g; %hur l�ng tid som pilen �r i luften
t = [0:0.0001:timeOfFlight];

xv = v*cosd(vinkel); %v0 i x-led
x= t*xv; % X kord f�r pil

% Hastighet vid X
% vabs = sqrt(v^2-2*g*x.*tand(vinkel)+((g*x)/(v*cosd(vinkel))^2));

% Hastighet i y led
yv = v*sind(vinkel)-(g*x)/(v*cosd(vinkel));

% Y kord f�r pil
y= y0+x.*tand(vinkel)-(g*x.^2)/(2*((v*cosd(vinkel))^2));

plot(x,y);
axis equal;

