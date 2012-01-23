M=1; 
m= 0.060; 
e= 0.9;  %efficiency term
F = 700;
k= 0.005;
l=0.58;
g= 9.82;
hojd=0;
%M is the mass of the bow and k is a factor which represents the sum of the
%KE’s of the moving parts of the bow. (k for medieval bows range from 0.03 and 0.07

theta = 45;
v = sqrt((e*F*l)/(m+k*M));
R= (v^2)*sind(2*theta)/g;
% R=(v^2)/g;

t = [0:0.01:7.63];
langd= t*v;

hojd=(langd-((g*langd.^2)/(2*(v*cosd(theta))^2)))-hojd;

hojd1=((v^2)*sind(2*theta))/(2*g);
tmax = 2*(hojd/v);


% plot(x,y);
% figure;

%3D
ax= axes('Xlim', [-50 50], 'YLim', [-10 600], 'ZLim', [0 200]);
view(3);
grid on;
axis equal;

[xc yc zc] = cylinder([0.2 0.0,]);
[x y z]= cylinder([0.2 0.2]);

h(1) = surface(xc, zc, yc, 'FaceColor', 'red');
h(2) = surface(10*x, -15*z, 10*y, 'FaceColor', 'red');

t= hgtransform('Parent', ax);

set(h, 'Parent', t);

set(gcf, 'Renderer', 'opengl');

drawnow

lat = hojd;
lon = langd;
bea = hojd./100;
% lon = [0 1 2 3 4 5 6 7 8 9 10];
% lat = [0 1 1 1 1 1 -0.5 -1 -1];
% bea = [0.5 0.4 0.3 0.2 0.1 0 -0.1 -0.2 -0.3 -0.4 -0.5 -0.5 ];

for i = 1:numel(lat)
    trans = makehgtform('translate', [0, lon(i), lat(i)]);
    rotx = makehgtform('xrotate',bea(i));
    set(t,'Matrix',trans*rotx);
%     set(t,'Matrix',trans);
    pause(0.01)
end
