#include <iostream>
#include <cmath>
#include <fstream>
#include <random>
#include "sfunction.h"

int main(){
	//Initial conditions and parameters
	double dt = 0.1;
	double t_start = 0;
	double t_end = 10;
	double m = 1;
	double x=0,dot_x=0;
	double A = 2,w=0.1,phi=3;

	//Input step function
	sfunction<double,1,1> u({2},[&](std::vector<double> vargs) {auto t = vargs[0];return t>=0 ? u[0] : 0;});

	//Spring
	sfunction<double,1,1> fs({0.5},[&](std::vector<double> vargs) {auto x = vargs[0]; return x*fs[0];});
	fs.set_parameters({2});
	
	//Damper
	sfunction<double,1,1> fd({0.1},[&](std::vector<double> vargs) {auto v = vargs[0]; return v*fd[0];});

	//Mass, spring, damper simulator
	double x1=0,x2=0;
	for(double t = t_start; t<t_end;t += dt){
		//Evaluate acceleration from Newton's second law
		auto ddot_x = 1/m*(u(t)-fs(x)-fd(dot_x));
		//Simple forward Euler integration
		x += dt*dot_x+0.5*dt*dt*ddot_x;
		dot_x += dt*ddot_x;
	}
	std::cout << x << "," << dot_x << '\n';
}
