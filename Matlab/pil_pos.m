clear all;
clc;

%Konstanter

L = 0.5; %Lemmarnas längd
m = 0.025; %Massan på pilen

theta0= 0.3491;

x0=L*sin(theta0); %Stränghöjd i startläge




g=9.82; %gravitation
vinkel= 45; %utgångsvinkel
y0=1; %Start höjd
totaltDrag = 0.7;
draTillbaka = 0.7-x0;%Hur långt vi drar pilen
m = 0.025; %Massan på pilen
E = rsum1(draTillbaka,x0,600); %Pilens utgångshastighet när tilen dras tillbaka

v = sqrt(2*-E/m);

% Total distance
d = (v*cosd(vinkel)/g)*(v*sind(vinkel)+sqrt(((v*sind(vinkel))^2+2*g*y0)));

% hastighet i x led
timeOfFlight = (v*sind(vinkel)+sqrt(((v*sind(vinkel))^2+2*g*y0)))/g; %hur lång tid som pilen är i luften
t = [0:0.0001:timeOfFlight];

xv = v*cosd(vinkel); %v0 i x-led
x= t*xv; % X kord för pil

% Hastighet vid X
% vabs = sqrt(v^2-2*g*x.*tand(vinkel)+((g*x)/(v*cosd(vinkel))^2));

% Hastighet i y led
yv = v*sind(vinkel)-(g*x)/(v*cosd(vinkel));

% Y kord för pil
y= y0+x.*tand(vinkel)-(g*x.^2)/(2*((v*cosd(vinkel))^2));
figure;
plot(x,y);
axis equal;

