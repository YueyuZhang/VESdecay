# VESdecay: Variational Enhanced Sampling with Learning Rate Decay

Implementation of the learning rate decay method for Variational Enhanced Sampling (VES) as described in:

> **Zhang et al. (2026)** "Faster Convergence in Variational Enhanced Sampling Using Decaying Learning Rates"  
> *Materials Genome Engineering Advances*

## Overview

Learning rate decay reduces VES convergence iterations by **30-50%** without additional computational cost.
Support for three optimizers:
- **BM** (Bach-Moulines) - ~50% faster convergence
- **ADAM** - ~30-40% faster convergence
- **SGD** - Stochastic gradient descent

## Key Features

### Decay Schedules

| Type | Formula |
|------|---------|
| **INVERSE** | alpha_k = alpha_0 / (1 + k / tau_d) |
| **EXPONENTIAL** | alpha_k = alpha_0 * exp(-k / tau_d) |
| **LINEAR** | alpha_k = alpha_0 * max(0, 1 - k / tau_d) |

## Installation

```bash
# Apply patch to PLUMED
cd /path/to/plumed2
patch -p1 < /path/to/VESdecay/patches/ves_learning_rate_decay.patch

# Recompile PLUMED
./configure --enable-modules=ves
make -j4
```

## Usage

```plumed
VES ...
  OPTIMIZER ...
    BM
    LEARNING_RATE=0.05
    DECAY_TYPE=INVERSE
    DECAY_TIME=200
  ...OPTIMIZER
... VES
```

## Citation

```bibtex
@article{zhang2026vesdecay,
  title={Faster Convergence in Variational Enhanced Sampling},
  author={Zhang, Y. and others},
  journal={Materials Genome Engineering Advances},
  year={2026}
}
```
