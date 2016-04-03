# RSA-Encryption-System
A terminal-based RSA encryption system written in C++.

### Dependency
The program requires the [GNU bignum](https://gmplib.org/) library to run. Depending on where it is installed, individual compile commands might also vary.

For example, if GMP was configured with:
```
./configure --enable-cxx --prefix=$HOME/gmplib
```
Then compilating will require linking via:
```
-I$HOME/gmplib/include -L/$HOME/gmplib/lib -lgmpxx -lgmp
```
