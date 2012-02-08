#include <math>


float value=0;
float dx;
float c;

float  rsum(float a,float b,int n)
{
	dx = (b-a)/n;

	for(int k=1; k<n; k++)
	{
		c = a+k*dx;
		value += kraft(c);
	}
	value=dx*value;

	return value;
}


float _limbLength;
float _arrowMass;
float g;
float theta0;
float x0;
float _vinkel;
float _totalDrag;
float _pullBack;
float _work;
float _BallistaHeight;
float _totalDistance;
float _timeOfFlight;
float v;
float x;
float xv;
float yv;
float y;

Arrowpos(float t)
{
_limbLength = 0.5; /*Lemmarnas l�ngd*/
_arrowMass = 0.025; /*Massan p� pilen*/

theta0= 0.3491;

x0=_limbLength*sin(theta0); /*Str�ngh�jd i startl�ge*/




g=9.82; /*gravitation*/
_vinkel= 45*pi/180; /*utg�ngsvinkel*/
_BallistaHeight=1; /*Start h�jd*/
_totalDrag = 0.7;
_pullBack = 0.7-x0; /*Hur l�ngt vi drar pilen*/
m = 0.025; /*%Massan p� pilen*/
_work = rsum(_pullBack,x0,600); /*Pilens utg�ngshastighet n�r tilen dras tillbaka*/

v = sqrt(2*-_work/m);

// Total distance
_totalDistance = (v*cosd(vinkel)/g)*(v*sind(vinkel)+sqrt(((v*sind(vinkel))^2+2*g*_BallistaHeight)));

//hastighet i x led
_timeOfFlight = (v*sind(vinkel)+sqrt(((v*sind(vinkel))^2+2*g*_BallistaHeight)))/g; /*hur l�ng tid som pilen �r i luften*/
t = [0:0.0001:_timeOfFlight];

xv = v*cosd(vinkel); /*v0 i x-led*/
x= t*xv; /*% X kord f�r pil*/

// Hastighet vid X
// vabs = sqrt(v^2-2*g*x.*tand(vinkel)+((g*x)/(v*cosd(vinkel))^2));

// Hastighet i y led
yv = v*sind(vinkel)-(g*x)/(v*cosd(vinkel));

// Y kord f�r pil
y= _BallistaHeight+x.*tand(vinkel)-(g*x.^2)/(2*((v*cosd(vinkel))^2));


}
