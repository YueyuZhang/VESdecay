# VESdecay: Variational Enhanced Sampling with Learning Rate Decay

Implementation of the learning rate decay method for Variational Enhanced Sampling (VES) as described in:

> **Zhang et al. (2026)** "Faster Convergence in Variational Enhanced Sampling Using Decaying Learning Rates"  
> 

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






## Examples

### 1. Wolfe-Quapp Double Well Model

Benchmark system for testing learning rate decay in VES. Two metastable basins separated by a free energy barrier.

```plumed
# PLUMED input: BM optimizer with inverse decay
ves: ...
  BASIS ...
    CHEBYSHEV MINIMUM=-2.0 MAXIMUM=2.0 ORDER=15
  ... BASIS

  OPTIMIZER ...
    BM
    LEARNING_RATE=0.05
    DECAY_TYPE=INVERSE
    DECAY_TIME=200
  ... OPTIMIZER
... VES
```

**Results:**
| Method | Iterations to converge | Speedup |
|--------|----------------------|---------|
| BM (constant LR) | ~400 | 1.0x |
| BM + inverse decay | ~200 | **2.0x** |

See: [`examples/01_double_well/`](examples/01_double_well/)

---

### 2. Sodium Melting with Structure Factor CV

Material science application: solid-liquid coexistence in sodium (250 atoms, bcc structure). Collective variable based on first peak of structure factor S(q).

```plumed
# PLUMED input for sodium melting
sf: STRUCTURE_FACTOR ...
  ATOMS=1-250 SPECIES=Na
  QLOW=2.9 QHIGH=3.1 NBINS=10
... STRUCTURE_FACTOR

ves: VES ...
  ARG=sf.q_peak
  OPTIMIZER ...
    BM
    LEARNING_RATE=0.03
    DECAY_TYPE=INVERSE
    DECAY_TIME=400
  ... OPTIMIZER
... VES
```

**Key finding:** Learning rate decay stabilizes optimization for realistic material systems, eliminating oscillations observed with constant learning rates.

See: [`examples/02_sodium_melting/`](examples/02_sodium_melting/)
