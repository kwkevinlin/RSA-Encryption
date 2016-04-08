/*
 * Kevin Lin
 * RSA Encryption
 */

#ifndef MODULARARITHMETIC_H_
#define MODULARARITHMETIC_H_

//#include <gmp.h>
#include <gmpxx.h>

//Used exclusively for extendedEuclid's algorithm
struct EuclidStruct //Explained in .cpp
{
	mpz_class x;
	mpz_class y;
	mpz_class d;

};

class ModularArithmetic
{

public:
	static mpz_class modadd(mpz_class& a, mpz_class& b, mpz_class& N);
	static mpz_class modmult(mpz_class& a, mpz_class& b, mpz_class& N);
	static mpz_class moddiv(mpz_class& a, mpz_class& b, mpz_class& N);
	static mpz_class modexp(mpz_class& a, mpz_class& b, mpz_class& N);
	static EuclidStruct extendedEuclid(mpz_class& N, mpz_class& b);
	static bool isPrime(mpz_class& N, int k);
	static mpz_class genPrime(int n);



};


#endif /* MODULARARITHMETIC_H_ */
