# Example 02: Sodium Melting with Structure Factor CV

Material science application: solid-liquid coexistence in sodium.

## System

- 250 Na atoms in bcc configuration
- Temperature at melting point (~370 K)
- CV: Intensity of first peak in structure factor S(q)

## Files

- `Na250.cif` - Initial bcc structure
- `plumed_sodium_bm.dat` - VES with BM optimizer and decay
- `structure_factor.cpp` - Helper code for CV calculation

## Usage

```bash
# Run with LAMMPS + PLUMED
lmp -in in.lmp -plumed plumed_sodium_bm.dat
```

## Expected Results

Learning rate decay significantly stabilizes the optimization for this
challenging material science system. Without decay, BM optimizer shows
oscillatory behavior which is eliminated with the inverse decay schedule.

Decay time recommendation: tau_d = 300-500 iterations.
