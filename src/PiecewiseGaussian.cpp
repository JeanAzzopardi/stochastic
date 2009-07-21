/*
 * PiecewiseGaussian.cpp
 *
 *  Created on: 15-Jun-2009
 *      Author: Dimitrios Milios
 */

#include "PiecewiseGaussian.h"

#include <algorithm>
#include <limits>
#include <cmath>
#include "exceptions.h"

namespace stochastic {

PiecewiseGaussian::PiecewiseGaussian()
{
}

PiecewiseGaussian::PiecewiseGaussian(Distribution * distribution)
{
	if (!distribution)
		throw stochastic::UndefinedDistributionException();

	PiecewiseGaussian temp = * (PiecewiseGaussian *)fit(distribution);
	this->weights = temp.weights;
	this->components = temp.components;
	this->cumulativeWeights = temp.cumulativeWeights;
}

PiecewiseGaussian::~PiecewiseGaussian()
{
}

const char * PiecewiseGaussian::getName()
{
	return "pNorm";
}

// FIXME: possibly discard this
// seems more reasonable to use 68% rule, when (initially) gaussians are not overlapping
PiecewiseBase * PiecewiseGaussian::experiment(Distribution * distribution)
{
	PiecewiseGaussian * result = new PiecewiseGaussian;

	MixtureComponent * component;
	double weight;
	double start = distribution->getLeftMargin();
	double end = distribution->getRightMargin();
	double support = end - start;
	double step = support / (double) fixedNumberOfComponents;
	int i;

	double stadardDeviation = support / (8 * fixedNumberOfComponents);
	double range = 4 * stadardDeviation;
	double x = start + range;
	for (i = 0; i < fixedNumberOfComponents; i++)
	{
		weight = distribution->cdf(x + range) - distribution->cdf(x - range);
		double pdf_c = distribution->pdf(x) / weight;
		double var = 1 / (2 * 3.14 * pow(pdf_c, 2));
		if (weight)
		{
			component = new Gaussian(x, var * 1.4);
			result->components.push_back(component);
			result->weights.push_back(weight);
		}
		x += step;
	}
	result->cumulativeWeights = result->constructCumulativeWeights(result->weights);

	return result;
}


PiecewiseBase * PiecewiseGaussian::fit(Distribution * distribution)
{
	return experiment(distribution);



	PiecewiseGaussian * result = new PiecewiseGaussian;

	MixtureComponent * component;
	double weight;
	double start = distribution->getLeftMargin();
	double end = distribution->getRightMargin();
	double support = end - start;
	double step = support / (double) fixedNumberOfComponents;
	int i;
	double stadardDeviation = support / (8 * fixedNumberOfComponents);
	double range = 4 * stadardDeviation;
	double x = start + range;


	// check for support so as to revise the step
	for (i = 0; i < fixedNumberOfComponents; i++)
	{
		weight = distribution->cdf(x + range) - distribution->cdf(x - range);
		if (weight == 0)
			support -= step;
		x += step;
	}
	// revise the step
	step = support / (double) fixedNumberOfComponents;
	stadardDeviation = support / (8 * fixedNumberOfComponents);
	range = 4 * stadardDeviation;
	x = start + range;


	for (i = 0; i < fixedNumberOfComponents; i++)
	{
		weight = distribution->cdf(x + range) - distribution->cdf(x - range);
		if (weight)
		{
			double var = pow(range, 2);
			component = new Gaussian(x, var);
			result->components.push_back(component);
			result->weights.push_back(weight);
		}
		x += step;
	}
	result->cumulativeWeights = constructCumulativeWeights(result->weights);
	return result;
}

/*
 *
 * Methods for computing the results of operations between components
 * Implement pure virtual methods from 'PiecewiseBase'
 *
 * */

MixtureComponent * PiecewiseGaussian::sumOfComponents(
		MixtureComponent * arg1, MixtureComponent * arg2)
{
	double m1 = ((Gaussian *) arg1)->getMean();
	double m2 = ((Gaussian *) arg2)->getMean();
	double var1 = ((Gaussian *) arg1)->getVariance();
	double var2 = ((Gaussian *) arg2)->getVariance();
	return new Gaussian(m1 + m2, var1 + var2);
}

MixtureComponent * PiecewiseGaussian::differenceOfComponents(
		MixtureComponent * arg1, MixtureComponent * arg2)
{
	double m1 = ((Gaussian *) arg1)->getMean();
	double m2 = ((Gaussian *) arg2)->getMean();
	double var1 = ((Gaussian *) arg1)->getVariance();
	double var2 = ((Gaussian *) arg2)->getVariance();
	return new Gaussian(m1 - m2, var1 + var2);
}

MixtureComponent * PiecewiseGaussian::productOfComponents(
		MixtureComponent * arg1, MixtureComponent * arg2)
{
	double a1 = arg1->getLeftMargin();
	double b1 = arg1->getRightMargin();
	double a2 = arg2->getLeftMargin();
	double b2 = arg2->getRightMargin();

	std::vector<double> margins;
	margins.push_back(a1 * a2);
	margins.push_back(a1 * b2);
	margins.push_back(b1 * a2);
	margins.push_back(b1 * b2);

	std::vector<double>::iterator a = std::min_element(
			margins.begin(), margins.end());
	std::vector<double>::iterator b = std::max_element(
			margins.begin(), margins.end());

	double var = pow((* b - * a) / 8, 2);
	double m = (* a + * b) / 2;
	return new Gaussian(m, var);
}

MixtureComponent * PiecewiseGaussian::ratioOfComponents(
		MixtureComponent * arg1, MixtureComponent * arg2)
{
	double a1 = arg1->getLeftMargin();
	double b1 = arg1->getRightMargin();
	double a2 = arg2->getLeftMargin();
	double b2 = arg2->getRightMargin();

	std::vector<double> margins;
	margins.push_back(a1 / a2);
	margins.push_back(a1 / b2);
	margins.push_back(b1 / a2);
	margins.push_back(b1 / b2);

	if (std::abs(a2) < 0.001)
		return 0;
	if (std::abs(b2) < 0.001)
		return 0;

	std::vector<double>::iterator a = std::min_element(
			margins.begin(), margins.end());
	std::vector<double>::iterator b = std::max_element(
			margins.begin(), margins.end());

	double var = pow((* b - * a) / 8, 2);
	double m = (* a + * b) / 2;
	return new Gaussian(m, var);
}

MixtureComponent * PiecewiseGaussian::minOfComponents(
		MixtureComponent * arg1, MixtureComponent * arg2)
{
	double a = std::min<double>(arg1->getLeftMargin(), arg2->getLeftMargin());
	double b = std::min<double>(arg1->getRightMargin(), arg2->getRightMargin());
	double var = pow((b - a) / 8, 2);
	double m = (a + b) / 2;
	return new Gaussian(m, var);
}

MixtureComponent * PiecewiseGaussian::maxOfComponents(
		MixtureComponent * arg1, MixtureComponent * arg2)
{
	double a = std::max<double>(arg1->getLeftMargin(), arg2->getLeftMargin());
	double b = std::max<double>(arg1->getRightMargin(), arg2->getRightMargin());
	double var = pow((b - a) / 8, 2);
	double m = (a + b) / 2;
	return new Gaussian(m, var);
}


/*
 *
 *
 * for functions of ONE random variable
 *
 * */

MixtureComponent * PiecewiseGaussian::sumOfComponents(
		MixtureComponent * distr_arg, double c_arg)
{
	double m = ((Gaussian *) distr_arg)->getMean();
	double var = ((Gaussian *) distr_arg)->getVariance();
	return new Gaussian(m + c_arg, var);
}

MixtureComponent * PiecewiseGaussian::differenceOfComponents(double c_arg,
		MixtureComponent * distr_arg)
{
	double m = ((Gaussian *) distr_arg)->getMean();
	double var = ((Gaussian *) distr_arg)->getVariance();
	return new Gaussian(c_arg - m, var);
}

MixtureComponent * PiecewiseGaussian::productOfComponents(
		MixtureComponent * distr_arg, double c_arg)
{
	double m = ((Gaussian *) distr_arg)->getMean();
	double var = ((Gaussian *) distr_arg)->getVariance();
	return new Gaussian(m * c_arg, var * pow(c_arg, 2));
}

MixtureComponent * PiecewiseGaussian::ratioOfComponents(double c_arg,
		MixtureComponent * distr_arg)
{
	double a = distr_arg->getLeftMargin();
	double b = distr_arg->getRightMargin();

	if (std::abs(a) < 0.001)
		return 0;
	if (std::abs(b) < 0.001)
		return 0;

	a = c_arg / a;
	b = c_arg / b;
	double lmargin = std::min<double>(a,b);
	double rmargin = std::max<double>(a,b);
	double var = pow((rmargin - lmargin) / 8, 2);
	double m = (lmargin + rmargin) / 2;
	return new Gaussian(m, var);
}

MixtureComponent * PiecewiseGaussian::minOfComponents(
		MixtureComponent * dist_arg, double c_arg)
{
}

MixtureComponent * PiecewiseGaussian::maxOfComponents(
		MixtureComponent * dist_arg, double c_arg)
{
}

} // namespace stochastic
