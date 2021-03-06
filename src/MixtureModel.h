/*
 * MixtureModel.h
 *
 *  Created on: 08-Jun-2009
 *      Author: Dimitrios Milios
 */

#ifndef MIXTUREMODEL_H_
#define MIXTUREMODEL_H_

#include "Distribution.h"
#include "MixtureComponent.h"
#include "exceptions.h"
#include <vector>

namespace stochastic {

class MixtureModel : public stochastic::Distribution
{
protected:
	std::vector<stochastic::MixtureComponent *> components;
	std::vector<double> weights;
	std::vector<double> cumulativeWeights;
	std::vector<double> constructCumulativeWeights(std::vector<double> &);
	MixtureModel()
	{
	}

public:
	MixtureModel(std::vector<MixtureComponent*>, std::vector<double>);
	virtual ~MixtureModel();
	const virtual char *getName();
	double pdf(double);
	double cdf(double);
	double nextSample();
	double getLeftMargin();
	double getRightMargin();
};

} // namespace stochastic

#endif /* MIXTUREMODEL_H_ */
