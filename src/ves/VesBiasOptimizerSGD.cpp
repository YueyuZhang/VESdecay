/* VESdecay: VesBiasOptimizerSGD.cpp
 * SGD optimizer with learning rate decay
 */

#include "VesBiasOptimizerSGD.h"
#include "VesBias.h"

namespace PLMD { namespace ves {

void VesBiasOptimizerSGD::registerKeywords(Keywords& keys) {
  VesBiasOptimizerBase::registerKeywords(keys);
  keys.add("compulsory","LEARNING_RATE","0.1","the learning rate");
  keys.add("optional","MOMENTUM","0.0","momentum parameter");
  keys.add("optional","DECAY_TYPE","INVERSE, EXPONENTIAL, or LINEAR");
  keys.add("optional","DECAY_TIME","damping time constant");
}

VesBiasOptimizerSGD::VesBiasOptimizerSGD(const ActionOptions& ao):
  VesBiasOptimizerBase(ao), momentum(0.0) {
  parse("LEARNING_RATE", initial_learning_rate);
  parse("MOMENTUM", momentum);
  parse("DECAY_TYPE", decay_type);
  parse("DECAY_TIME", decay_time);
  use_decay = (decay_type.length() > 0 && decay_time > 0);
}

void VesBiasOptimizerSGD::performIteration() {
  double alpha = getCurrentLearningRate();
  
  VesBias* vesbias = plumed.getActionSet().select<VesBias*>().getLabel(label);
  std::vector<double> gradient;
  vesbias->getCoefficientGradient(gradient);
  
  if (momentum > 0 && velocity.size() != gradient.size()) {
    velocity.resize(gradient.size(), 0.0);
  }
  
  std::vector<double> coeffs;
  vesbias->getBasisCoefficients(coeffs);
  
  for (size_t i = 0; i < gradient.size(); i++) {
    if (momentum > 0) {
      velocity[i] = momentum * velocity[i] - alpha * gradient[i];
      coeffs[i] += velocity[i];
    } else {
      coeffs[i] -= alpha * gradient[i];
    }
  }
  
  vesbias->setBasisCoefficients(coeffs);
  iteration_counter++;
}

} }
