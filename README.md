# Pseudo_inverse_using_QR_Factorizaion

This is a example function for calculating Pseudo inverse using QR Factorization.

In general, a pseudo inverse of a big sized matrix (i.e. 24 x 24) using Singular Vector Decomposition(SVD) has heavy calculation cost.

However, the computation time with QR factorization for pseudo inverse is much shorter than that with SVD.

I tested a few of cases including 
1) squared matrix with row rank-deficient or column rank-deficient.
2) thin matrix with column rank-deficient.
3) fat matrix with row rank-deficient.

'''Dependancy: Eigen Library (more than 3.0.0)'''

'''Question: ggory15@snu.ac.kr'''
