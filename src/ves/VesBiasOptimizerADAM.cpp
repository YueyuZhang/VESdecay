/* VESdecay: VesBiasOptimizerADAM.cpp
 * ADAM optimizer with learning rate decay
 */

#include "VesBiasOptimizerADAM.h"
#include "VesBias.h"
#include <cmath>

namespace PLMD { namespace ves {

void VesBiasOptimizerADAM::registerKeywords(Keywords& keys) {
  VesBiasOptimizerBase::registerKeywords(keys);
  keys.add("compulsory","LEARNING_RATE","0.001","the learning rate");
  keys.add("optional","BETA1","0.9","first moment decay");
  keys.add("optional","BETA2","0.999","second moment decay");
  keys.add("optional","DECAY_TYPE","INVERSE, EXPONENTIAL, or LINEAR");
  keys.add("optional","DECAY_TIME","damping time constant");
}

VesBiasOptimizerADAM::VesBiasOptimizerADAM(const ActionOptions& ao):
  VesBiasOptimizerBase(ao), beta1(0.9), beta2(0.999), epsilon(1e-8) {
  parse("LEARNING_RATE", initial_learning_rate);
  parse("BETA1", beta1);
  parse("BETA2", beta2);
  parse("DECAY_TYPE", decay_type);
  parse("DECAY_TIME", decay_time);
  use_decay = (decay_type.length() > 0 && decay_time > 0);
}

void VesBiasOptimizerADAM::performIteration() {
  double alpha = getCurrentLearningRate();
  iteration_counter++;
  
  VesBias* vesbias = plumed.getActionSet().select<VesBias*>().getLabel(label);
  std::vector<double> gradient;
  vesbias->getCoefficientGradient(gradient);
  
  if (m.size() != gradient.size()) {
    m.resize(gradient.size(), 0.0);
    v.resize(gradient.size(), 0.0);
  }
  
  std::vector<double> coeffs;
  vesbias->getBasisCoefficients(coeffs);
  
  for (size_t i = 0; i < gradient.size(); i++) {
    m[i] = beta1 * m[i] + (1.0 - beta1) * gradient[i];
    v[i] = beta2 * v[i] + (1.0 - beta2) * gradient[i] * gradient[i];
    double m_hat = m[i] / (1.0 - std::pow(beta1, iteration_counter));
    double v_hat = v[i] / (1.0 - std::pow(beta2, iteration_counter));
    coeffs[i] -= alpha * m_hat / (std::sqrt(v_hat) + epsilon);
  }
  
  vesbias->setBasisCoefficients(coeffs);
}

} }
