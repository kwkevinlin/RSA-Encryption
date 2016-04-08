/*
 * Kevin Lin
 * RSA Encryption
 */

#include <gmpxx.h>
#include <fstream>
#include <iostream>
#include "RSA.h"
#include "ModularArithmetic.h"

/* Questions
 * How to store keys in file? by line? "Private: (N, d)"? Convention? How are we going to read them? By code, or human read?
 * Difference: mpz_mod (GMP function) and (a*b) & N? Both passes test
 *
 */

using namespace std;


//TRY THIS FOR ALL ERRORS:
//.get_mpz_t()

//Constructors

RSA::RSA(int n) {

	// n/2 bits
	cout << "Generating numbers. Could take a few seconds.." << endl;
	primeP = ModularArithmetic::genPrime((n/2) +1); //Reason for this explained in genPrime, but basically it generates too fast, so if called together primes will be the same
	while(1) {
		primeQ = ModularArithmetic::genPrime(n/2);
		if (primeP != primeQ)
			break; //So primeP != primeQ, which is causing problems
	}
	//primeP = 11;
	///primeQ = 13;
	bigN = primeP * primeQ;
	cout << "P = " << primeP << ", Q = " << primeQ << ", \nN = " << bigN << endl;

	//Phi
	mpz_class phi = (primeP - 1)*(primeQ - 1);
	cout << "Phi = " << phi << endl;

	//Generating e
	mpz_class gcdMax; //gcdMax pass by reference? (ignore for now, changed)
	cout << "Finding public 'e'." << endl;
	for (int i = 2; i < 100; i++) {
		publicE = i;

		mpz_gcd(gcdMax.get_mpz_t(), publicE.get_mpz_t(), phi.get_mpz_t());

		if (gcdMax == 1) //Then they are relatively prime
			break;
	} //Will only leave while(1) loop if GCD == 1
	cout << "Public 'e' = " << publicE << ". Confirmed by GCD: " << gcdMax << endl;

	//Finding d //12345678
	//EuclidStruct temp = ModularArithmetic::extendedEuclid(publicE, phi); //function returns struct with vars x, y, d
	mpz_class one = 1;
	privateD = ModularArithmetic::moddiv(one, publicE, phi);

	cout << "Finding private 'd'.\n" << "Private 'd' = " << privateD << endl << endl;

}

RSA::RSA(int n, string pubFilename, string privFilename) {

	// n/2 bits
	cout << "Generating numbers. Could take a few seconds.." << endl;
	primeP = ModularArithmetic::genPrime((n/2) +1); //Reason for this explained in genPrime, but basically it generates too fast, so if called together primes will be the same
	while(1) {
		primeQ = ModularArithmetic::genPrime(n/2);
		if (primeP != primeQ)
			break; //So primeP != primeQ, which is causing problems
	}
	//primeP = 11;
	///primeQ = 13;
	bigN = primeP * primeQ;
	cout << "P = " << primeP << ", Q = " << primeQ << ", \nN = " << bigN << endl;

	//Phi
	mpz_class phi = (primeP - 1)*(primeQ - 1);
	cout << "Phi = " << phi << endl;

	//Generating e
	mpz_class gcdMax; //gcdMax pass by reference? (ignore for now, changed)
	cout << "Finding public 'e'." << endl;
	for (int i = 2; i < 100; i++) {
		publicE = i;

		mpz_gcd(gcdMax.get_mpz_t(), publicE.get_mpz_t(), phi.get_mpz_t());

		if (gcdMax == 1) //Then they are relatively prime
			break;
	} //Will only leave while(1) loop if GCD == 1
	cout << "Public 'e' = " << publicE << ". Confirmed by GCD: " << gcdMax << endl;

	//Finding d //12345678
	//EuclidStruct temp = ModularArithmetic::extendedEuclid(publicE, phi); //function returns struct with vars x, y, d
	mpz_class one = 1;
	privateD = ModularArithmetic::moddiv(one, publicE, phi);

	cout << "Finding private 'd'.\n" << "Private 'd' = " << privateD << endl << endl;

	//Writing to file
	ofstream outFilePublic;
	while(1) { //PUBLIC file
		outFilePublic.open(pubFilename.c_str());
		if (outFilePublic.fail()) {
			cout << "File could not be created. Please re-enter file name:" << endl;
			getline(cin, pubFilename);
		}
		else
			break; //Proceed to write to file
	}
	outFilePublic << bigN << endl << publicE;
	outFilePublic.close();

	ofstream outFilePrivate;
	while(1) { //PUBLIC file
		outFilePrivate.open(privFilename.c_str());
		if (outFilePrivate.fail()) {
			cout << "File could not be created. Please re-enter file name:" << endl;
			getline(cin, privFilename);
		}
		else
			break; //Proceed to write to file
	}
	outFilePrivate << bigN << endl << privateD;
	outFilePrivate.close();

	cout << "Files " << pubFilename << " and " << privFilename << " written." << endl << endl;

}

RSA::RSA(string filename) {

	//Ask for filename before calling this

	ifstream inFile;
	while(1) {
		inFile.open(filename.c_str());
		if (inFile.fail()) {
			cout << "File could not be opened. Please re-enter file name (including .txt)." << endl;
			getline(cin, filename);
		}
		else
			break; //Proceed to open file
	}

	cout << endl << "Reading in as format: line1-> N, line2-> d" << endl;
	inFile >> bigN >> privateD;
	cout << "N = " << bigN << endl << "D = " << privateD << endl;

}

/*
mpz_class findGCD(mpz_class& a, mpz_class& b) {
	if (b == 0)
		return a;
	mpz_class tempMod = a % b;
	return findGCD(b, tempMod);
} */

//Class Functions
mpz_class RSA::encrypt(mpz_class& m, mpz_class& N, mpz_class& e) {
	//Actual =                 m= m          N= e          e= N
	mpz_class tM, tE, tN;   //WARNING, VALUES DOES NOT MATCH, FIX:
	tM = m; tE = N, tN = e; //These are the actual corresponding values
	if (tM > tN)
		cout << "Error: m > N\n";
	//modexp(mpz_class& a, mpz_class& b, mpz_class& N);
	//cout << "In Encrpyt, m = " << m << ", \nN = " << e << ", e = " << N << endl;
	mpz_class temp = ModularArithmetic::modexp(tM, tE, tN);
	//cout << "In Encrpyt, m = " << m << ", \ne = " << N << ", N = " << e << endl;
	return temp;

}

mpz_class RSA::decrypt(mpz_class& c) {
	if (c > bigN)
		cout << "Error: c > N\n";
	//modexp(mpz_class& a, mpz_class& b, mpz_class& N);
	//cout << "In Decrpyt, c = " << c << endl;
	return ModularArithmetic::modexp(c, privateD, bigN);
	//return c^d (mod N)
}


