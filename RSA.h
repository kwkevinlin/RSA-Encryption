/*
 * Kevin Lin
 * RSA Encryption
 */

#ifndef RSA_H_
#define RSA_H_

#include <gmpxx.h>

using namespace std;

class RSA
{

private:

	mpz_class privateD;

public:

	//Constructors
	mpz_class primeP, primeQ, publicE, bigN;

	RSA(int n);
	RSA(int n, string filename);
	RSA(int n, string pubFilename, string privFilename);
	RSA(string filename);

	mpz_class encrypt(mpz_class& m, mpz_class& N, mpz_class& e);
	mpz_class decrypt(mpz_class& c);

	//Accessor
	mpz_class getPrivateD() {
		return privateD;
	}

};


#endif /* RSA_H_ */
