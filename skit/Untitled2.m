clear all;
g=9.82;
vinkel= 45;
hojd=0;
y0=1; %Start höjd
v =hastighet(0.6);
% R= (v^2)*sind(2*vinkel)/g;
% R=(v^2)/g;

% Total distance
d = (v*cosd(vinkel)/g)*(v*sind(vinkel)+sqrt(((v*sind(vinkel))^2+2*g*y0)));
% hastighet i x led
timeOfFlight = (v*sind(vinkel)+sqrt(((v*sind(vinkel))^2+2*g*y0)))/g;
t = [0:0.01:timeOfFlight];
xv = v*cosd(vinkel);
x= t*xv;

% Hastighet vid X
% vabs = sqrt(v^2-2*g*x.*tand(vinkel)+((g*x)/(v*cosd(vinkel))^2));


% Hastighet i y led
yv = v*sind(vinkel)-(g*x)/(v*cosd(vinkel));


maxhojd=((v^2)*sind(2*vinkel))/(2*g);



% X kord för pil

% Y kord för pil
y= y0+x.*tand(vinkel)-(g*x.^2)/(2*((v*cosd(vinkel))^2));

plot(x,y);
axis equal;
%3D
% ax= axes('Xlim', [-50 50], 'YLim', [-10 600], 'ZLim', [0 200]);
% view(3);
% grid on;
% axis equal;
% 
% [xc yc zc] = cylinder([0.2 0.0,]);
% [x y z]= cylinder([0.2 0.2]);
% 
% h(1) = surface(xc, zc, yc, 'FaceColor', 'red');
% h(2) = surface(10*x, -15*z, 10*y, 'FaceColor', 'red');
% 
% t= hgtransform('Parent', ax);
% 
% set(h, 'Parent', t);
% 
% set(gcf, 'Renderer', 'opengl');
% 
% drawnow
% 
% lat = langd;
% lon = hojd;
% bea = hojd/100;
% % lon = [0 1 2 3 4 5 6 7 8 9 10];
% % lat = [0 1 1 1 1 1 -0.5 -1 -1];
% % bea = [0.5 0.4 0.3 0.2 0.1 0 -0.1 -0.2 -0.3 -0.4 -0.5 -0.5 ];
% 
% for i = 1:numel(lat)
%     trans = makehgtform('translate', [0, lon(i), lat(i)]);
%     rotx = makehgtform('xrotate',bea(i));
%     set(t,'Matrix',trans*rotx);
% %     set(t,'Matrix',trans);
%     pause(0.01)
% end
