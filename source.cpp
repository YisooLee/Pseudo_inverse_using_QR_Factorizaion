
#include <iostream>
#include <Eigen/Dense>
#include<Eigen/SVD>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

using namespace std;
using namespace Eigen;


double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

template<typename _Matrix_Type_>
_Matrix_Type_ pseudoInverse(const _Matrix_Type_ &a, double epsilon = std::numeric_limits<double>::epsilon())
{
	Eigen::JacobiSVD< _Matrix_Type_ > svd(a, Eigen::ComputeThinU | Eigen::ComputeThinV);
	double tolerance = epsilon * max(a.cols(), a.rows()) *svd.singularValues().array().abs()(0);
	return svd.matrixV() *  (svd.singularValues().array().abs() > tolerance).select(svd.singularValues().array().inverse(), 0).matrix().asDiagonal() * svd.matrixU().adjoint();
}

template<typename _Matrix_Type_>
_Matrix_Type_ pinv_qr(const _Matrix_Type_ &A)
{
	Eigen::FullPivLU< _Matrix_Type_ >lu_decomp(A);

	int rank = lu_decomp.rank();
	if (rank == 0) {
		cout << "WARN::Input Matrix seems to be zero matrix" << endl;
		return A;
	}
	else {
		if (A.rows() > A.cols()) {
			Eigen::ColPivHouseholderQR< _Matrix_Type_ > qr(A);
			qr.compute(A);
			Eigen::MatrixXd R = qr.matrixQR().topLeftCorner(rank, rank).template triangularView<Upper>();
			Eigen::MatrixXd Rpsinv2(A.cols(), A.rows());

			Rpsinv2.setZero();
			Rpsinv2.topLeftCorner(rank, rank) = R.inverse();

			return qr.colsPermutation() * Rpsinv2 * qr.householderQ().inverse();
		}
		else if (A.cols() > A.rows()) {
			Eigen::ColPivHouseholderQR< _Matrix_Type_ > qr(A.transpose());
			qr.compute(A.transpose());
			Eigen::MatrixXd R = qr.matrixQR().topLeftCorner(rank, rank).template triangularView<Upper>();
			Eigen::MatrixXd Rpsinv2(A.rows(), A.cols());

			Rpsinv2.setZero();
			Rpsinv2.topLeftCorner(rank, rank) = R.inverse();
			return (qr.colsPermutation() * Rpsinv2 * qr.householderQ().inverse()).transpose();
		}
		else if (A.cols() == A.rows()) {
			Eigen::CompleteOrthogonalDecomposition< _Matrix_Type_> cod(A);
			return cod.pseudoInverse();
		}
	}
}

int main()
{
	Eigen::MatrixXd A = MatrixXd::Random(24, 24);



	StartCounter();
	for (int i = 0; i < 100; i++) {
		A.setRandom();
		A.row(1).setZero();
		pseudoInverse(A);
	}
	cout << "svd" << "\t" << GetCounter() / 1000.0 << endl;

	StartCounter();
	for (int i = 0; i < 100; i++) {
		A.setRandom();
		A.row(1).setZero();
		pinv_qr(A);
	}
	cout << "qr" << "\t" << GetCounter() / 1000.0 << endl;





	getchar();
	return 0;
}
