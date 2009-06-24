// =============================================================

/* 'stochastic' is a library
 * for performing operations between random variables
 *
 * main() is used for test purposes
 *
 *
 *      Author: Dimitrios Milios
 */

// =============================================================


// load "stochastic" library
#include "../src/stochastic.h"

#include "Gnuplot.h"
#include <cstdio>
#include <iostream>
#include <cmath>

int printArguments(int argc, char *argv[])
{
	int i;
	std::cout << "\nThe arguments were:\n";
	for (i = 0; i < argc; i++)
		std::cout << argv[i]<< "\n" ;
	return 0;
}

int main(int argc, char *argv[])
{
	using namespace stochastic;
	ApproximatedDistribution::setFixedNumberOfComponents(100);
	Gnuplot::setAccuracy(1000);

	Gnuplot plot;

	Gaussian g, g1(1);
	Uniform u(-4, 4);
	Exponential e;
	EmpiricalDistribution emp("pLin.txt");
	(new RandomVariable(&emp))->produceFileOfSamples(100);

	std::vector <MixtureComponent *> c;
	std::vector <double> w;
	c.push_back(new Gaussian);
	w.push_back(1);
	c.push_back(new Gaussian(4, 2));
	w.push_back(1);
	MixtureModel m(c, w);

	PiecewiseUniform pu = &m;

	RandomVariable r1 = new Gaussian(2, 8);
	RandomVariable r2 = new Gaussian(3, 1);
	RandomVariable r3;

	r3 = MonteCarloOperations::min(r1, r2);

	//plot.addRV(r1);
	//plot.addRV(r2);
	plot.addRV(r3);

	plot.plotCurves();

	return printArguments(argc, argv);
}
