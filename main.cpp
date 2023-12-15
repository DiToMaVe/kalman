#include <iostream>
#include <vector>

#include <Eigen/Dense>

#include "kalman.hpp"

using Eigen::MatrixXd;
using Eigen::VectorXd;


int main()
{
	int n = 3;
	int m = 1;
	int p = 1;
	int N = 20;

	MatrixXd A(n, n);
	MatrixXd B(n, p);
	MatrixXd C(m, n);
	MatrixXd D(m, p);
	MatrixXd Q(n, n);
	MatrixXd R(m, m);
	MatrixXd S(n, m);
	MatrixXd P0(n, n);
	VectorXd x0(n);
	MatrixXd U;
	MatrixXd Y;

	// Initial state
	x0 = MatrixXd::Random(n, 1);
	//std::cout << "xf: \n" << xf << std::endl;
	P0 << .1, .1, .1, .1, 10000, 10, .1, 10, 100;

	// Some random measurements
	U = MatrixXd::Random(N, p);
	Y = MatrixXd::Random(N, m);

	// Discrete LTI system
	double dt = 1.0 / 30; // Time step
	A << 1, dt, 0, 0, 1, dt, 0, 0, 1;
	B << 0, 0.1, 0.1;
	C << 1, 0, 0;
	D << 0.5;

	// Covariance matrices
	Q << .05, .05, .0, .05, .05, .0, .0, .0, .0;
	R << 5;
	S = MatrixXd::Identity(n, m);

	Measurements Z(U, Y);
	LssModel M(A, B, C, D, P0, Q, R, S, x0, P0);

	std::vector<VectorXd> gainSequence;
	std::vector<VectorXd> stateSequence;
	std::vector<MatrixXd> covSequence;

	std::tie(gainSequence, stateSequence, covSequence) = Kalman::kf(Z, M);
}
