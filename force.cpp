#include <math>
#include <iostream>
#include <vector>
 
using namespace std;


 
float force(_drawlength)
{
	//Deklarrera variabler
    private float _l = 0.5; //Lemmarnas l�ngd
	private float _y0 = 0.1; //L�ngd p� handtaget
	private float _mLimb = 0.093; // Massan p� l�mmarna
	private float _kappa = 620; //Fj�derkonstant f�r b�gen
	private float _mArrow = 0.025; //Massa p� pilen
	private float _theta0 = 0.3491; //Startvinkel 20grader i rad
	private float _x0  = 0; //Str�ngh�jd i startl�get
	private float _l0 = 0; //Halva str�ngens l�ngd
	private float _k = 0; //Fj�derkonstant relativt vinklarish
	private float _x = 0; //mittpunkten p� str�ngens position
	private float _i = 0; //Moment of inertia;
	private float _theta = 0; //Lemmens vinkel
	private float _alpha = 0; //Vinkel mellan str�ng och pil
	private float _f = 0; //kraften

	//KOD!

	_x0 = _l*math.sin(_theta0);
	_l0 = _y0+(_l*math.cos(_theta0));
	_k = (_l*_l0*math.sin(_theta0)*_kappa)/(2_theta0);

 
    return 0;
}