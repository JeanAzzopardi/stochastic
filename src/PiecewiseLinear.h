/*
 * PiecewiseLinear.h
 *
 *  Created on: 15-Jun-2009
 *      Author: Dimitrios Milios
 */

#ifndef PIECEWISELINEAR_H_
#define PIECEWISELINEAR_H_

#include "ApproximatedDistribution.h"

#include "Linear.h"

namespace stochastic {

class PiecewiseLinear : public stochastic::ApproximatedDistribution
{
private:
	void fit(std::vector <double>);
	void fit(Distribution *);

public:
	PiecewiseLinear(const char *);
	PiecewiseLinear(Distribution *);
	virtual ~PiecewiseLinear();

	const char * getName();
};

} // namespace stochastic

#endif /* PIECEWISELINEAR_H_ */
