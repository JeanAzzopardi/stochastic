/*
 * Uniform.cpp
 *
 *  Created on: 08-Jun-2009
 *      Author: Dimitrios Milios
 */

#include "Uniform.h"

#include "exceptions.h"
#include <typeinfo>

namespace stochastic {

Uniform::Uniform()
{
	this->alpha = 0;
	this->beta = 1;
}

Uniform::Uniform(double alpha, double beta)
{
	if (alpha >= beta)
		throw InvalidParametersException();

	this->alpha = alpha;
	this->beta = beta;
}

Uniform::~Uniform()
{
}

double Uniform::pdf(double x)
{
	if (x < alpha || x > beta)
		return 0;
	return 1 / (beta - alpha);
}

double Uniform::nextSample()
{
	return generator.nextDouble(alpha, beta);
}

/*
 *
 * --- Implement Binary Operators: '+', '-', '*', '/'
 * --- for Uniform Approximation Component
 */

ApproximationComponent * Uniform::add(ApproximationComponent * rightarg)
{
	ApproximationComponent * result;
	if (typeid(* this) != typeid(* rightarg))
		throw stochastic::IncomparableInnerRepresentationException();
	result = new Uniform;

	return result;
}

ApproximationComponent * Uniform::subtract(ApproximationComponent * rightarg)
{
	ApproximationComponent * result;
	if (typeid(* this) != typeid(* rightarg))
		throw stochastic::IncomparableInnerRepresentationException();
	result = new Uniform;

	return result;
}

ApproximationComponent * Uniform::multiply(ApproximationComponent * rightarg)
{
	ApproximationComponent * result;
	if (typeid(* this) != typeid(* rightarg))
		throw stochastic::IncomparableInnerRepresentationException();
	result = new Uniform;

	return result;
}

ApproximationComponent * Uniform::divide(ApproximationComponent * rightarg)
{
	ApproximationComponent * result;
	if (typeid(* this) != typeid(* rightarg))
		throw stochastic::IncomparableInnerRepresentationException();
	result = new Uniform;

	return result;
}

/*
 *
 * --- Implement Binary Operators: min, max
 * --- for Uniform Approximation Component
 */

ApproximationComponent * Uniform::min(ApproximationComponent * secondarg)
{
	ApproximationComponent * result;
	if (typeid(* this) != typeid(* secondarg))
		throw stochastic::IncomparableInnerRepresentationException();
	result = new Uniform;

	return result;
}

ApproximationComponent * Uniform::max(ApproximationComponent * secondarg)
{
	ApproximationComponent * result;
	if (typeid(* this) != typeid(* secondarg))
		throw stochastic::IncomparableInnerRepresentationException();
	result = new Uniform;

	return result;
}

} // namespace stochastic
