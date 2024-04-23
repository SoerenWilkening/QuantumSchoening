## Data for explicit run-times for hybrid SAT-solvers
Authors: V. Eshaghian, S. Wilkening, J.  °Aberg, D. Gross

Code for numerical calculations in "Paper Link".
The Code is only run and tested on Mac.

Build the code:

```
mkdir build && cd build
cmake ../
cmake -- build .
cd ..
```

Run the Code:

```
./qsch par
```

With ```par``` the user can determine, which calculation he wants to run.
```par = 0```: large scale calculation
```par = 1```: small scale calculation
