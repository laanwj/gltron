// review: 64bit ok

/* Textbook standard linear congruent pseudo-random number generator  
   (say that ten times quick) */

/* The generator returns numbers in 0..(2**31 - 1) */
#define PERIOD      2147483647
#define T_RAND_MAX  PERIOD
/* A is a "magic" constant calculated by IBM in the dawn of computing */
#define A           48271
#define Q           44488
#define R           3399

/* The seed of the generator, hopefully set by the user before rand is 
   called */
static int seed = 42;

/* Seeds the random number generator with the seed s */
void nebu_srand(unsigned int s)
{
	seed = s % PERIOD;
}

/* Calculates a new random number */
int nebu_rand(void)
{
	seed = A * (seed % Q) - R * (long)(seed / Q);
	if(seed < 0)
		seed += PERIOD;
	
	return seed;
}

/* Bonus function, returns a random number in [0..1) */
float nebu_randf(void)
{
	return (float)nebu_rand()/PERIOD;
}

