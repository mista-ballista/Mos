clear all;
clc;

%Konstanter

L = 0.5; %Lemmarnas l�ngd
m = 0.025; %Massan p� pilen

theta0= 0.3491;

x0=L*sin(theta0); %Str�ngh�jd i startl�ge




g=9.82; %gravitation
vinkel= 45; %utg�ngsvinkel
y0=1; %Start h�jd
totaltDrag = 0.7;
draTillbaka = 0.7-x0;%Hur l�ngt vi drar pilen
m = 0.025; %Massan p� pilen
E = rsum1(draTillbaka,x0,600); %Pilens utg�ngshastighet n�r tilen dras tillbaka

v = sqrt(2*-E/m);

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
figure;
plot(x,y);
axis equal;

