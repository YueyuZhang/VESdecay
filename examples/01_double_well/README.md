# Example 01: Wolfe-Quapp Double Well Model

Benchmark system for testing learning rate decay in VES.

## System

Wolfe-Quapp potential: V(x,y) = x^4 + y^4 - 2x^2 - y^2 + xy

Two metastable basins separated by a free energy barrier.

## Files

- `plumed_bm_constant.dat` - BM optimizer, constant LR
- `plumed_bm_decay.dat` - BM optimizer, inverse decay (tau_d=200)
- `plumed_adam_constant.dat` - ADAM optimizer, constant LR
- `plumed_adam_decay.dat` - ADAM optimizer, inverse decay

## Usage

```bash
# Run with PLUMED and your favorite MD engine
plumed driver --plumed plumed_bm_decay.dat
```

## Expected Results

| Method | Iterations to converge |
|--------|----------------------|
| BM constant LR | ~400 |
| BM + inverse decay | ~200 |
| ADAM constant LR | ~550 |
| ADAM + inverse decay | ~350 |

**Speedup: ~2x for BM with decay!**
