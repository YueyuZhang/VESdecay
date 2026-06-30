# Installation Guide

## Prerequisites

- PLUMED 2.7 or higher (2.8+ recommended)
- C++11 compatible compiler (GCC 4.8+, Clang 3.3+, Intel 15+)
- Git

## Method 1: Apply Patch to Existing PLUMED Installation

1. Download the patch:

```bash
cd /path/to/VESdecay
```

2. Go to your PLUMED source directory:

```bash
cd /path/to/plumed2
```

3. Apply the patch:

```bash
patch -p1 < /path/to/VESdecay/patches/ves_learning_rate_decay.patch
```

4. Reconfigure and recompile:

```bash
./configure --enable-modules=ves
make -j4
make install
```

## Method 2: Manual File Replacement

If patch doesn't work, manually replace the following files:

```bash
# Copy modified files
cp VESdecay/src/ves/VesBiasOptimizerBase.h  /path/to/plumed2/src/ves/
cp VESdecay/src/ves/VesBiasOptimizerBM.cpp /path/to/plumed2/src/ves/
cp VESdecay/src/ves/VesBiasOptimizerADAM.cpp /path/to/plumed2/src/ves/
cp VESdecay/src/ves/VesBiasOptimizerSGD.cpp /path/to/plumed2/src/ves/

# Recompile
cd /path/to/plumed2
make -j4
```

## Verify Installation

Run the double well example to verify:

```bash
cd VESdecay/examples/01_double_well
plumed driver --plumed plumed_bm_decay.dat
```

Check that the COLVAR file is created and the bias is evolving correctly.
