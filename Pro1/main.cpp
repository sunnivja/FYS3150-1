#include <iostream>
#include <cmath>
#include <fstream>
#include "armadillo"

using namespace std;
using namespace arma;

// Exact solution we want to compare with
double source_term(double x){
    return 100*exp(-10*x);
   }

double exact_solution(double x){
    return 1 - (1 - exp(-10))*x - exp(-10*x);

}


int main()
{
    // Declaring variables and vectors
    int n = 12;
    double h = 1./(n-1);
    double *a = new double[n];
    double *b = new double[n];
    double *c = new double[n];
    double *u = new double[n];
    double *f = new double[n];
    double *x = new double[n];
    double *u_exact = new double[n];
    cout << "So far so good" << endl;
    // Fill vectors with values
    for(int i = 0; i < n; i++){
        a[i] = -1;
        b[i] = 2;
        c[i] = -1;
        f[i] = h*h*source_term(i*h);
    }
    b[0] = 1;
    c[0] = 0;
    f[0] = 0;
    a[n-1] = 0;
    b[n-1] = 1;
    f[n-1] = 0;

    // Forward substitution
    for(int i = 1; i < n; i++){
        b[i] = b[i] - (c[i-1]*a[i])/b[i-1];
        f[i] = f[i] - (f[i-1]*a[i])/b[i-1];
    }
    u[n-1] = f[n-1]/b[n-1];
    // Backward substitution
    for(int i = n-2; i >= 0; i--){
        u[i] = (f[i] - c[i]*u[i+1])/b[i];
    }
    cout << "So far so good" << endl;
    // Then u is equal to f_tilde2
    for(int i = 0; i < n; i++){
    }
    // Fillling x with values
    for(int i = 0; i < n; i++){
        x[i] = i*h;
    }
    // Discretize exact solution to compare with approximation
    for(int i = n-1; i > 0; i--){
        u_exact[i] = exact_solution(x[i]);
    }

    // LU decomposition
    // First we must make the matrices (Previosly defined as vectors)
    //double A[n][n];
    mat A = zeros<mat>(n,n);

    // // Define elements along diagonal
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if (i==j){
                A(i,j) = 2;
            }
            else if (i == j-1){
                A(i,j) = -1;
            }
            else if (i == j+1){
                A(i,j) = -1;
            }
            else {
                A(i,j) = 0;
            }
        }
    }
    A.print("A=");
    vec f_vec = zeros<vec>(n);
    for (int i = 0; i< n; i++){
        f_vec(i) = h*h*exact_solution(x[i]);
    }
    // Solve Av = f:
    //vec v_vec = zeros<vec>(n);
    //v_vec = solve(A,f_vec);
    //v_vec.print("v=");

    // LU decomposition
    mat L,U;
    lu(L,U,A);
    L.print("L = ");
    U.print("U = ");

    mat y_vec = solve(L,f_vec);
    vec v_vec = solve(U,y_vec);

    //v_vec(0) = 0;
    //v_vec(n-1) = 0;

    // Write result to file 1 (General alorithm) and LU v in last column
    ofstream myfile;
    cout << "We have openened a file!" << endl;
    myfile.open("res2.txt");
    for(int i = 0; i < n; i++){
        myfile << i << " " << u[i] << " " << u_exact[i] << " " << v_vec[i] << endl;
    }
    myfile.close();
    cout << "File closed, deleting values..." << endl;
    delete [] a;
    delete [] b;
    delete [] c;
    delete [] f;
    delete [] u;
    delete [] x;

    cout << "Done!" << endl;

    return 0;
}

