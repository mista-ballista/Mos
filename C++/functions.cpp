#include <math.h>


float value=0;
float dx;
float c;
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
float _l; //Lemmarnas l�ngd
float _y0; //L�ngd p� handtaget
float _mLimb; // Massan p� l�mmarna
float _kappa; //Fj�derkonstant f�r b�gen
float _mArrow; //Massa p� pilen
float _theta0; //Startvinkel 20grader i rad
float _x0; //Str�ngh�jd i startl�get
float _l0; //Halva str�ngens l�ngd
float _k; //Fj�derkonstant relativt vinklarish
float _x; //mittpunkten p� str�ngens position
float _i; //Moment of inertia;
float _theta; //Lemmens vinkel
float _alpha; //Vinkel mellan str�ng och pil
float _f; //kraften

float  rsum(float a,float b,int n)
{
	dx = (b-a)/n;

	for(int k=1; k<n; k++)
	{
		c = a+k*dx;
		value += force(c);
	}
	value=dx*value;

	return value;
}

float force(float _drawLength)
{
	//Deklarrera variabler
    _l = 0.5;
	_y0 = 0.1;
	_mLimb = 0.093; 
	_kappa = 620;
	_mArrow = 0.025;
	_theta0 = 0.3491;

	//KOD!
	_x0 = _l * sin(_theta0);
	_l0 = _y0 + (_l * cos(_theta0));
	_k = (_l * _l0 * sin(_theta0) * _kappa) / (2 * _theta0);
	_x = _x0 + _drawLength;
	_i = 0.3 * _mLimb * pow(_l,2);
	float _x2 = pow(_x, 2);
	float _x4 = pow(_x, 4);
	float _x6 = pow(_x, 6);

	float _y02 = pow(_y0,2);
	float _y03 = pow(_y0,3);
	float _y04 = pow(_y0,4);
	
	float _l2 = pow(_l, 2);
	float _l3 = pow(_l, 3);
	float _l4 = pow(_l, 4);
	float _l6 = pow(_l, 6);
	
	float _l02 = pow(_l0, 2);
	float _l04 = pow(_l0, 4);
	

	_theta = acos((-_x2 * _l * _y0 + sqrt(_x6 *(-_l2) + 2 * _x4 * _l4 + 2 * _x4 * _l2 * _l02-2 * _x4 * _l2 * _y02-_x2 * _l6+2 * _x2 *_l4 * _l02 + 2 * _x2 * _l4 * _y02 - _x2 * _l2 * _l04 + 2 * _x2 * _l2 * _l02 * _y02-_x2 * _l2 * _y04) + _l3 * (-_y0) + _l * _l02 * _y0 - _l * _y03) / (2 * (_x2 * _l2 + _l2 * _y02)));
	_alpha = asin((_y0 + _l * cos(_theta)) / _l0);
	_f = 2 * (_k / _l) *  _theta * (cos(_alpha) / cos(_alpha - _theta));


    return _f;
}

Arrowpos(float t)
{
_limbLength = 0.5; /*Lemmarnas l�ngd*/
_arrowMass = 0.025; /*Massan p� pilen*/

theta0= 0.3491;

x0=_limbLength*sin(theta0); /*Str�ngh�jd i startl�ge*/




g=9.82; /*gravitation*/
_vinkel= 45*Pi/180; /*utg�ngsvinkel*/
_BallistaHeight=1; /*Start h�jd*/
_totalDrag = 0.7;
_pullBack = 0.7-x0; /*Hur l�ngt vi drar pilen*/
m = 0.025; /*%Massan p� pilen*/
_work = rsum(_pullBack,x0,600); /*Pilens utg�ngshastighet n�r tilen dras tillbaka*/

v = sqrt(2*-_work/m);

// Total distance
_totalDistance = (v*cos(_vinkel)/g)*(v*sin(vinkel)+sqrt(((v*sind(vinkel))^2+2*g*_BallistaHeight)));

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
