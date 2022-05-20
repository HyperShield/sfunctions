#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <random>
#include "sfunction.h"

int main(){
	//Initial conditions and parameters
	double dt = 0.00001;
	double t_start = 0;
	double t_end = 10;
	std::ofstream output;
	//Random number engine and normal distribution (mean,stddev)
	std::mt19937 generator(time(0));
	std::normal_distribution<double> distribution(0,0.5);
	
	//Input function (Step)
	sfunction<double,1,1> u({18},[&](std::vector<double> args) {return args[0]>=0 ? u[0] : 0;});
	
	//PID parameters{Kp,Ki,Kd,I_err,prev_err,dt}
	sfunction<double,5,2> PID({40,20,0,0,0});
	PID.set_evaluator(
		[&](std::vector<double> vargs)
		{
			auto err = vargs[0];
			auto dt = vargs[1];
			auto I = PID[3]+err*dt;
			auto u_PID = PID[0]*err+PID[1]*I+PID[2]*(err-PID[4])/dt;
			PID.set_parameters({PID[0],PID[1],PID[2],I,err});
			return u_PID;
		}
	);
	
	//Reference (sin(t))
	sfunction<double,1,1> ref({1},[&](std::vector<double> vargs) {return ref[0]*std::sin(vargs[0]+M_PI/2);});
	
	//Motor parameters
	double Jm = 0.0004; double Kb = 0.008; double Kt = 0.0045;
	double La = 0.01; double Ra = 0.3; double Dm = 0.03; double JL = 0.1;
	//Initial states
	double th=0; double w=0; double i=0;
	output.open("sim_result.txt");
	for(double t = t_start; t<t_end;t += dt){
		output << t << " " << th << " " << w << " " << i << '\n';
		//Control law
		//auto err = 0.5-th;
		auto err = ref(t)-th;
		auto V = PID(err,dt);
		//if(abs(V) > 24){
		//	V = V/abs(V)*24;
		//}
		//State derivatives
		auto dot_i = (V-Kb*w-Ra*i)/La;
		auto dot_w = (Kb*i-JL-Dm*w-distribution(generator))/Jm;
		//Forward Euler
		th = th + dt*w + 0.5*dt*dt*dot_w;
		w = w + dt*dot_w;
		i = i + dt*dot_i;
	}
	output.close();
	std::cout << th << "," << w << '\n';
	std::cout << i << '\n';
}
