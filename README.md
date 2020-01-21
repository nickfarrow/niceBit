# Bitcoin Vanity Address Generator (WORK IN PROGRESS)
## TODO
* Fix memory leak, likely to do with creating a new libsecp256k1 context every time...
* Create make file
* Add search from dictionary words
* Are there really that many repeated numbers at the END of an address? Seems like these are being found way too often...?

## Dependencies
* libsep256k1
* openssl
## Compilation
```
gcc niceBit.c -o niceBit -lcrypto -lsecp256k1
```
## Usage
```
./niceBit
```
