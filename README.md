# Parallel Integer Factorization using Pollard’s Rho and P-1 Methods

## Overview

This project repository implements **parallel integer factorization** for large composite numbers (moduli), leveraging both **Pollard’s Rho** and **Pollard’s P-1** algorithms. The solution is designed for high performance and scalability, using multi-threading (via POSIX threads) to trigger both methods simultaneously and efficiently discover non-trivial factors of large numbers.

- **Languages/Technologies:** C, GMP (GNU Multiple Precision Arithmetic Library), POSIX Threads, Python (automation)
- **Key Features:** Parallel execution, large-number support, automation for batch processing

## Motivation & Impact

Factoring large integers is a foundational problem in computational number theory and cryptography (e.g., RSA). This project demonstrates:
- **Advanced algorithmic knowledge** (Pollard’s Rho, P-1)
- **Systems-level parallel programming**
- **Practical automation for large-scale experiments**

**Major Achievement:**  
Successfully factored large moduli (up to 200+ bits and beyond) by parallelizing two advanced factorization algorithms, significantly reducing computation time and increasing the probability of finding factors for difficult numbers.

## How It Works

- **pollards_rho.c**:  
  - Implements both Pollard’s Rho and Pollard’s P-1 methods.
  - Launches two threads per modulus: one for each method.
  - As soon as either method finds a factor, the result is reported and both threads are joined.
  - Thread-safe reporting using mutexes.

- **automate.py**:  
  - Automates batch processing of many moduli from a CSV file.
  - Runs the C program for each modulus, enabling large-scale experiments.

- **Input Data:**  
  - `all-moduli.csv` contains a variety of test moduli, including numbers up to 200+ digits and real cryptographic challenge numbers.

- **Output:**  
  - Factors are printed to the console and can be redirected or logged for further analysis.

## Example Usage

### 1. Build the C Program

```sh
gcc -o pollards_rho pollards_rho.c -lgmp -lpthread
```

### 2. Run on a Single Modulus

```sh
./pollards_rho 98834976202698839303077
```

### 3. Batch Automation

```sh
python3 automate.py
```

This will process all moduli listed in `all-moduli.csv`.

## Results

- Successfully factored a wide range of moduli, including numbers with 80, 100, 120, 140, 160, 180, and 200+ bits.
- Demonstrated the effectiveness of parallelization:  
  - **Reduced time-to-factor** for hard numbers.
  - **Increased reliability** by running two complementary algorithms in parallel.

## Files

- `pollards_rho.c` – Main C implementation (parallel factorization)
- `automate.py` – Python script for batch automation
- `all-moduli.csv` – Sample moduli (input data)
- `output.xlsx` – Obtained output/results
- `device_config.txt` - Device configuration details

## Cite/Reference

If you use or build upon this project, please reference:

> Parallel Integer Factorization using Pollard’s Rho and P-1 Methods, by Ketul Chhaya, 2024.

---
