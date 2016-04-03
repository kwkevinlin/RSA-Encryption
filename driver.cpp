/*
 * Kevin Lin
 * RSA Encryption
 */

#include "ModularArithmetic.h"
#include "RSA.h"
#include <gmpxx.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>

/*
 * Note:
 *     Compile with:
 *          g++ -o driver driver.cpp ModularArithmetic.cpp RSA.cpp -std=c++0x -I$HOME/gmplib/include -L/$HOME/gmplib/lib -lgmpxx -lgmp
 *
 *     Error:
 *         Link to 64 bit version?
 *         ./driver: error while loading shared libraries: libgmpxx.so.4: cannot open shared object file: No such file or directory
 */

using namespace std;

int main() {

	/*
	 1) RSADriver reads a text message from a file (say “I like green eggs and ham. How about you?”).
	 2) It reads each character at the time, including spaces, and converts the string to a string of 3-digit ints
	    (ASCII representation of each character). This long string of 3-digit ints is the numeric message m to be encrypted
	 3) Encrypt message m with the public key read to obtain the number c to be send
	 4) To decrypt c, use the private key to convert back to a string of 3-digit ints
	 5) Read off the number string and convert back to characters
	 */


	//OPENING FILE AND READING MESSAGE------------------------------------------------------------------------------------------------
	//cout << "Enter filename with unencrypted message." << endl;
	string msgFile;
	//getline(cin, msgFile);
	msgFile = "msgfile.txt";
	ifstream inFile;
	inFile.open(msgFile.c_str());
	if (inFile.fail()) {
		cout << "File could not be opened. Program terminated." << endl;
		exit(1); }
	else
		cout << "File " << msgFile << " successfully opened." << endl << endl;

	string msgPlain, msgASCII, numStr;
	getline(inFile, msgPlain); //Unencrypted message read to messagePlain
	inFile.close();
	cout << "Message (Plain): " << msgPlain << endl;

	//Converting String (MSG) to String (ASCII)
	for (int i = 0; i < msgPlain.length(); i++) {
		char x = msgPlain.at(i);
		stringstream ss;
		ss << int(x);
		numStr = ss.str();
		if (int(x) < 100) //Means ASCII less than 3 digits
			numStr = "0" + numStr; //Concatenate 0 in front of int(x)
		//cout << "ASCII: " << numStr << endl;
		msgASCII = msgASCII + numStr; //Concatenate to form msgASCII string
	}

	cout << "Message (ASCII): " << msgASCII << endl << endl;


	//CALLING RSA CONSTRUCTOR ---------------------------------------------------------------------------------------------------
	//Constructor 1
	//RSA rsaClass(1024);
	//Constructor 2
	RSA rsaClass(1024, "pubKey.txt", "privKey.txt");
	//Constructor 3
	//RSA rsaClass("privateKeyTest.txt");


	cout << "Message: " << msgASCII << endl;






	//Convert string to mpz_class
	mpz_class msgPlainInMPZ;
	int setStr = mpz_set_str(msgPlainInMPZ.get_mpz_t(), msgASCII.c_str(), 10);
	if (setStr == -1)
		cout << "Error in string->mpz_class conversion." << endl;
	//WARNING: Conversion will leave off the '0' if there is one at the BEGINNING. ex: 'sa' = 083097 will become 83097

	//ENCRPYTING-------------------------------------------------------------------------------------------------------------------------------
	//Encrpyt message: encryptMsg = m ^e (mod N)
	//cout << "M: " << msgPlainInMPZ << ", e: " << rsaClass.publicE << ", N: " << rsaClass.bigN << endl;
	mpz_class encryptMsg = rsaClass.encrypt(msgPlainInMPZ, rsaClass.publicE, rsaClass.bigN);

	//cout << "Encrpyting: " << msgPlainInMPZ << "^" << rsaClass.publicE << " (mod " << rsaClass.bigN << ")" << endl;
	cout << "Encrypted: " << encryptMsg << endl;


	//DECRYPTING--------------------------------------------------------------------------------------------------------------------------------
	//Decrypt message: message = encryMsg^d (mod N)
	mpz_class decryptMsgASCII = rsaClass.decrypt(encryptMsg);
	//cout << "Decrypted: " << decryptMsgASCII << endl;

	//Converting mpz_class to String
	//mpz_class var.get_str();
	string decryptedStr = decryptMsgASCII.get_str();

	//Checking to see if we need to append 0 or if error in msg size
	if (decryptedStr.length() % 3 == 2)
		decryptedStr = "0" + decryptedStr; //Concatenate the missing 0 in front that disappears when modexp
	else if (decryptedStr.length() % 3 == 1)
		cout << "Error in decrypting. % 3 == 1" << endl << endl;

	//Converting from string (ASCII) to string (alphabets/char)
	cout << "Decrypted: " << decryptedStr << endl;
	string finalStr, conHolder, strHolder;
	char charHolder;
	for (int i = 0; i < decryptedStr.length(); i = i + 3) { //decryptedStr
		//cout << "Processing: " << decryptedStr[i] << ", " << decryptedStr[i+1] << ", " << decryptedStr[i+2] << endl;
		conHolder = decryptedStr.substr(i, 3);
		//cout << "Adding: " << conHolder << endl;
		charHolder = char(stoi(conHolder));
		//cout << "Char is: " << char(charHolder) << endl;
		strHolder = string(1, charHolder);
		finalStr = finalStr + strHolder;
	}

	cout << "Message (Decrypted): " << finalStr << endl;

	cout << endl << "Program terminated." << endl;

	return 0;

}



