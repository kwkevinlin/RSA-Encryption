/*
 * Kevin Lin
 * RSA Encryption
 */

#include <iostream>
#include <time.h>
#include "ModularArithmetic.h"

using namespace std;

mpz_class ModularArithmetic::modadd(mpz_class& a, mpz_class& b, mpz_class& N) {
	//Or can write as:
	//mpz_class addVar = a + b;
	//mpz_mod(addVar.get_mpz_t(), addVar.get_mpz_t(), N.get_mpz_t()); // addVar = addVar % N
	return (a + b) % N;
}

mpz_class ModularArithmetic::modmult(mpz_class& a, mpz_class& b, mpz_class& N) {

	mpz_class ans = a * b;

	mpz_mod(ans.get_mpz_t(), ans.get_mpz_t(), N.get_mpz_t() );

	//Same as return (a * b) % N;
	return ans;
}

bool ModularArithmetic::isPrime(mpz_class& N, int k) {

	/*
	//Using Fermat's Test
	//mpz_class baseVal = N - 1; //-1 too slow
	gmp_randclass rand(gmp_randinit_default);
	mpz_class baseVal, expoN;
	rand.seed(time(NULL));
	expoN = N - 1;
	for (int i = 0; i < k; i++) {
		baseVal = rand.get_z_range(N-1) +2;
		if (modexp(baseVal, expoN, N) != 1)
			return false;
	}
	//cout << "isPrime: " << N << " is prime" << endl;
	return true;
	 */

	//Fermat (above) not working well enough (but does work), so using a GMP function instead

	for (int i = 0; i < k; i++) {
		int result = mpz_probab_prime_p (N.get_mpz_t(), k);
		if (result == 0) //If 0, then n is definitely composite
			return false;
		//Determine whether n is prime. Return 2 if n is definitely prime, return 1 if n is probably prime
		//(without being certain), or return 0 if n is definitely composite.
	}
	return true;

}

mpz_class ModularArithmetic::genPrime(int n) {


	mpz_class bitsExp = n - 1;
	mpz_class base = 2;
	mpz_class smallestBaseforBits;
	mpz_pow_ui (smallestBaseforBits.get_mpz_t(), base.get_mpz_t(), mpz_get_ui(bitsExp.get_mpz_t())); //mpz_get_ui converts mpz_class to unsigned long int
	//cout << base << "^" << bitsExp << ", " << "smallestBaseforBits: " << smallestBaseforBits << endl;

	gmp_randclass rand(gmp_randinit_default);
	mpz_class primeNum, parseN; int addT = 0;
	while(1) {
		rand.seed(time(NULL) + addT); //Generates WAY too fast that time doesn't change, so output still the same. Confirmed by cout, but reason is just a guess.
		parseN = n;					  //ALSO fixed by adding +1 bits to one of the genPrimes in RSA. Still approximately the same bits, but exponentially faster (only 5 - 10 s vs 5-10 mins)
		primeNum = rand.get_z_bits(parseN);
		if (primeNum > smallestBaseforBits) { //So all numbers have bits in the same base^n range
			if (isPrime(primeNum, 50)) {
				//cout << base << "^" << bitsExp << ", " << "smallestBaseforBits: " << smallestBaseforBits << endl;
				//cout << "Prime: " << primeNum << endl;
				return primeNum;
			}
		}
		addT++;
	}
}

mpz_class ModularArithmetic::modexp(mpz_class& x, mpz_class& y, mpz_class& N) {
	mpz_class modExp;
	//cout << "In modExp, m = " << x << ", \nN = " << N << ", \ne = " << y << endl << endl;
	mpz_powm (modExp.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t(), N.get_mpz_t());
	//cout << "modExp: " << modExp << endl;
	return modExp;

}

EuclidStruct ModularArithmetic::extendedEuclid(mpz_class& N, mpz_class& b) {

	//Me and my friends decided that using struct would be a very good alternative to the pointer alternative. It is not
	//associated with ModularArithmetic class, and also can store multiple variables, so it is an ideal choice. Another
	//dedicated class is too cumbersome.
	//Using a struct is the only thing we discussed, not the code below. I used the information from the book to write
	//the function below.
	EuclidStruct recur, result;
	mpz_class x;

	//If b == 0, return (1,0,a)
	if (b == 0) {
		result.x = 1;
		result.y = 0;
		result.d = N;
		return result;
	}

	//Then recursion starts
	mpz_class varMod = N % b;
	//mpz_mod(tempVar.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t() );

	recur = extendedEuclid(b, varMod);

	x = recur.x;
	recur.x = recur.y;
	recur.y = x - (N/b) * recur.y;

	//Becareful answer in recur.y, x and y must be reversed during recursion calculation
	return recur;

}

mpz_class ModularArithmetic::moddiv(mpz_class& a, mpz_class& b, mpz_class& N) {
	//cout << "Sending in xEuclid (" << N << ", " << b << ")" << endl;
	EuclidStruct eStruct = extendedEuclid(N, b);
	//cout << "Finished xEuclid. Results (X, Y, D) :" << eStruct.x << ", " << eStruct.y << ", " << eStruct.d << endl;
	//cout << "Result: " << a << " * " << eStruct.x << " (mod " << N << ")" << endl << endl;
	return modmult(a, eStruct.y, N); //x
}
