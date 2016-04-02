#ifndef SELF_INHIBITING_PROCESS
#define SELF_INHIBITING_PROCESS
#include <vector>
#include <cstdlib>
#include <map>
#include "Process.h"
#include "Optimizer.h"
/*
	
	This class defines the Self-Inhibiting process which implements the general process internface IPorcess.
	 
*/

class SelfInhibitingProcess : public IProcess
{

protected:

	std::vector<std::vector<Eigen::MatrixXd > > arrayK_;

	std::vector<std::vector<std::vector<std::vector<std::pair<double, unsigned> > > > > arrayG_;

//  This variable is process-specific. It records totoal number of sequences used to fit the process.
	unsigned num_sequences_;

	Eigen::VectorXd observation_window_T_;

//  This function requires process-specific implementation. It initializes the temporal features used to calculate the negative loglikelihood and the gradient. 
	void Initialize(const std::vector<Sequence>& data);

	void RestoreOptionToDefault();

	void GetNegLoglikelihood(double& objvalue, Eigen::VectorXd& gradient);

	void PostProcessing();

public:
	//  Records the options

	enum Regularizer {L1, L22, NONE};
	enum RegCoef {LAMBDA0, LAMBDA};

	struct OPTION
	{
		Regularizer base_intensity_regularizer;
		Regularizer excitation_regularizer;
		std::map<RegCoef, double> coefficients;	
	};

protected:

	OPTION options_;

public:

	//  Constructor : n is the number of parameters in total; num_dims is the number of dimensions in the process;
	SelfInhibitingProcess(const unsigned& n, const unsigned& num_dims) : IProcess(n, num_dims), num_sequences_(0)
	{
		options_.base_intensity_regularizer = NONE;
		options_.excitation_regularizer = NONE;
		options_.coefficients[LAMBDA0] = 0;
		options_.coefficients[LAMBDA] = 0;
	}

	//  MLE esitmation of the parameters
	void fit(const std::vector<Sequence>& data, const OPTION& options);

//  This virtual function requires process-specific implementation. It calculates the negative loglikelihood of the given data. This function must be called after the Initialize method to return the negative loglikelihood of the data with respect to the current parameters. 
//	The returned negative loglikelihood is stored in the variable objvalue;
//	The returned gradient vector wrt the current parameters is stored in the variable Gradient; 
	virtual void NegLoglikelihood(double& objvalue, Eigen::VectorXd& gradient);

	//  Return the stochastic gradient on the random sample k.
	virtual void Gradient(const unsigned &k, Eigen::VectorXd& gradient);

//  This virtual function requires process-specific implementation. It returns the intensity value on each dimension in the variable intensity_dim for the given sequence stored in data and the given time t;
//  This function returns the summation of the individual intensity function on all dimensions. 
	virtual double Intensity(const double& t, const Sequence& data, Eigen::VectorXd& intensity_dim);

//  This virtual function requires process-specific implementation. It returns the upper bound of the intensity function on each dimension at time t given the history data in the variable intensity_upper_dim;
//	This function returns the summation of the individual intensity upper bound on all dimensions. 
	virtual double IntensityUpperBound(const double& t, const double& L, const Sequence& data, Eigen::VectorXd& intensity_upper_dim);

//  This virtual function requires process-specific implementation. It returns the upper bound of the intensity function on each dimension at time t given the history data in the variable intensity_upper_dim;
//	This function returns the integral of the intensity from a to b
	virtual double IntensityIntegral(const double& lower, const double& upper, const Sequence& data);

//  This function predicts the next event by simulation;
	virtual double PredictNextEventTime(const Sequence& data, const unsigned& num_simulations);

	// void PlotIntensityFunction(const Sequence& data, const unsigned& dim_id);

	// void PlotIntensityFunction(const Sequence& data);
	
	
};

#endif