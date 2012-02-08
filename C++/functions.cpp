#include <math.h>
#include <iostream>
#include <vector>
#include "functions.h"

using namespace std;

const float Pi = 3.1415926535897932384626433832795f;
const float _theta0 = 0.3491f;

float value=0;
float dx;
float c;
float g;
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
float _l; //Lemmarnas längd
float _halfHandle; //Längd på handtaget
float _mLimb; // Massan på lämmarna
float _kappa; //Fjäderkonstant för bågen
float _mArrow; //Massa på pilen
float _x0; //Stränghöjd i startläget
float _l0; //Halva strängens längd
float _k; //Fjäderkonstant relativt vinklarish
float _x; //mittpunkten på strängens position
float _i; //Moment of inertia;
float _theta; //Lemmens vinkel
float _alpha; //Vinkel mellan sträng och pil
float _f; //kraften






float  rsum(float a,float b,int n)
{
	dx = (b-a)/n;

	for(int k=1; k<=n; k++)
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
    _l = 0.5f;
	_halfHandle = 0.1f;
	_mLimb = 0.093f; 
	_kappa = 620.0f;
	_mArrow = 0.025f;

	//KOD!
	_x0 = _l * sin(_theta0);
	_l0 = _halfHandle + (_l * cos(_theta0));
	_k = (_l * _l0 * sin(_theta0) * _kappa) / (2 * _theta0);
	_x = _x0 + _drawLength;
	_i = 0.3f * _mLimb * pow(_l,2);
	float _x2 = pow(_x, 2);
	float _x4 = pow(_x, 4);
	float _x6 = pow(_x, 6);

	float _halfHandle2 = pow(_halfHandle,2);
	float _halfHandle3 = pow(_halfHandle,3);
	float _halfHandle4 = pow(_halfHandle,4);
	
	float _l2 = pow(_l, 2);
	float _l3 = pow(_l, 3);
	float _l4 = pow(_l, 4);
	float _l6 = pow(_l, 6);
	
	float _l02 = pow(_l0, 2);
	float _l04 = pow(_l0, 4);
	

	_theta = acos((-_x2 * _l * _halfHandle + sqrt(_x6 *(-_l2) + 2 * _x4 * _l4 + 2 * _x4 * _l2 * _l02-2 * _x4 * _l2 * _halfHandle2-_x2 * _l6+2 * _x2 *_l4 * _l02 + 2 * _x2 * _l4 * _halfHandle2 - _x2 * _l2 * _l04 + 2 * _x2 * _l2 * _l02 * _halfHandle2-_x2 * _l2 * _halfHandle4) + _l3 * (-_halfHandle) + _l * _l02 * _halfHandle - _l * _halfHandle3) / (2 * (_x2 * _l2 + _l2 * _halfHandle2)));
	
	_alpha = asin((_halfHandle + _l * cos(_theta)) / _l0);
	
	_f = 2 * (_k / _l) *  _theta * (cos(_alpha) / cos(_alpha - _theta));

    return _f;
}

void Arrowpos()
{
	vector<float> t;
	vector<float> x;
	vector<float> yv;
	vector<float> y;
	

	_l = 0.5f; /*Lemmarnas längd*/
	_mArrow = 0.025f; /*Massan på pilen*/
	_x0 = _l*sin(_theta0); /*Stränghöjd i startläge*/
	g = 9.82f; /*gravitation*/
	_vinkel= (45.0f*Pi/180.0f); /*utgångsvinkel*/
	_BallistaHeight = 1.0f; /*Start höjd*/
	_totalDrag = 0.7f;
	_pullBack = _totalDrag-_x0; /*Hur långt vi drar pilen*/
	_work = rsum(_pullBack,_x0,600); /*Pilens utgångshastighet när tilen dras tillbaka*/
	xv = v*cos(_vinkel); /*v0 i x-led*/
	

	v = sqrt(2*-_work/_mArrow);

	// Total distance
	float temp = pow((v*sin(_vinkel)),2);
	float temp2 = pow((v*cos(_vinkel)),2);
	_totalDistance = (v*cos(_vinkel)/g)*(v*sin(_vinkel)+sqrt(temp+2*g*_BallistaHeight));

	//hastighet i x led
	 
	_timeOfFlight = (v*sin(_vinkel)+sqrt((temp + 2*g*_BallistaHeight)))/g; /*hur lång tid som pilen är i luften*/
	//t = [0:0.0001:_timeOfFlight];
	int index = 0;
	for (float i=0.0f; i<_timeOfFlight; i+=0.0001f)
	{
		t.push_back(i);	
		x.push_back(t[index]*xv); /*% X kord för pil*/
		yv.push_back(v*sin(_vinkel)-(g*x[index])/(v*cos(_vinkel)));// Hastighet i y led
		y.push_back(_BallistaHeight+x[index]*tan(_vinkel)-(g*pow(x[index],2))/(2*temp2));// Y kord för pil
		index++;
	}	

	// Hastighet vid X
	// vabs = sqrt(v^2-2*g*x.*tand(vinkel)+((g*x)/(v*cosd(vinkel))^2));
	cout << v;
}
