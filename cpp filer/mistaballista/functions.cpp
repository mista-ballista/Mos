
//%RSUM1: Computes a Riemann Sum for the function f on
//%the interval [a,b] with a regular partition of n points.
//%The points on the intervals are chosen as the right endpoints.



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

arrowpos(float t)
