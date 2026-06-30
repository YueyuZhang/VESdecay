/* VESdecay: VesBiasOptimizerBM.cpp
 * BM optimizer with learning rate decay
 * Reference: Eqs. (13-14) in Zhang et al. (2026)
 */

#include "VesBiasOptimizerBM.h"
#include "VesBias.h"
#include <cmath>

namespace PLMD { namespace ves {

void VesBiasOptimizerBM::registerKeywords(Keywords& keys) {
  VesBiasOptimizerBase::registerKeywords(keys);
  keys.add("compulsory","LEARNING_RATE","0.05","the learning rate");
  keys.add("optional","DECAY_TYPE","INVERSE, EXPONENTIAL, or LINEAR");
  keys.add("optional","DECAY_TIME","damping time constant tau_d");
}

VesBiasOptimizerBM::VesBiasOptimizerBM(const ActionOptions& ao):
  VesBiasOptimizerBase(ao) {
  parse("LEARNING_RATE", initial_learning_rate);
  parse("DECAY_TYPE", decay_type);
  parse("DECAY_TIME", decay_time);
  use_decay = (decay_type.length() > 0 && decay_time > 0);
}

void VesBiasOptimizerBM::performIteration() {
  double alpha = getCurrentLearningRate();
  
  // Get gradient from VES bias
  VesBias* vesbias = plumed.getActionSet().select<VesBias*>().getLabel(label);
  std::vector<double> gradient;
  vesbias->getCoefficientGradient(gradient);
  
  if (grad_avg.size() != gradient.size()) {
    grad_avg.resize(gradient.size(), 0.0);
    hess_avg.resize(gradient.size(), 1.0);
  }
  
  std::vector<double> coeffs;
  vesbias->getBasisCoefficients(coeffs);
  
  // BM update with decaying learning rate (Eqs. 13-14 in paper)
  for (size_t i = 0; i < gradient.size(); i++) {
    grad_avg[i] = (1.0 - alpha) * grad_avg[i] + alpha * gradient[i];
    double diff = gradient[i] - grad_avg[i];
    hess_avg[i] = (1.0 - alpha) * hess_avg[i] + alpha * diff * diff;
    coeffs[i] -= alpha * grad_avg[i] / (hess_avg[i] + 1e-8);
  }
  
  vesbias->setBasisCoefficients(coeffs);
  iteration_counter++;
}

} }
