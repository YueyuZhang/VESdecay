# Keywords Reference

## Common Keywords

### DECAY_TYPE

Specifies the type of learning rate decay.

**Options:**
- `INVERSE` (Recommended): alpha_k = alpha_0 / (1 + k / tau_d)
- `EXPONENTIAL`: alpha_k = alpha_0 * exp(-k / tau_d)
- `LINEAR`: alpha_k = alpha_0 * max(0, 1 - k / tau_d)

**Default:** No decay

### DECAY_TIME

Damping time constant tau_d. Controls how fast the learning rate decays.

**Typical values:**
- Small systems: 100-200
- Medium systems: 200-500
- Large/complex systems: 500-1000

**Recommendation:** Set to approximately half the expected number
of iterations to converge (without decay).

## Usage per Optimizer

### BM Optimizer

```plumed
OPTIMIZER ...
  BM
  LEARNING_RATE=0.05
  DECAY_TYPE=INVERSE
  DECAY_TIME=200
...OPTIMIZER
```

### ADAM Optimizer

```plumed
OPTIMIZER ...
  ADAM
  LEARNING_RATE=0.001
  BETA1=0.9
  BETA2=0.999
  DECAY_TYPE=INVERSE
  DECAY_TIME=500
...OPTIMIZER
```

### SGD Optimizer

```plumed
OPTIMIZER ...
  SGD
  LEARNING_RATE=0.1
  MOMENTUM=0.9
  DECAY_TYPE=INVERSE
  DECAY_TIME=300
...OPTIMIZER
```

## Parameter Recommendations

| System | Optimizer | Initial LR | Decay Time |
|--------|-----------|------------|-----------|
| Double well | BM | 0.05 | 100-200 |
| Sodium melting | BM | 0.03 | 200-500 |
| Protein-ligand | BM | 0.01 | 300-800 |
| Neural network VES | ADAM | 0.001 | 300-500 |
