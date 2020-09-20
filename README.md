<div align="center">
  <img src="https://i.imgur.com/8KgjpAK.png">
</div>

# A Simple Bitcoin Vanity Address Generator
Create Bitcoin vanity addresses with C.
An open source method to generate nice Bitcoin addresses, also serving as an example of how to use the [bitcoin-core/secp256k1](https://github.com/bitcoin-core/secp256k1) C library.

# Tutorial
For a tutorial on how this code works, see http://nicholasfarrow.com/Cryptography-in-Bitcoin-with-C/.

## Dependencies
* [libsep256k1](https://github.com/bitcoin-core/secp256k1) (easy install)
* openssl (you likely have this)
## Compilation
```
gcc niceBit.c -o niceBit -lcrypto -lsecp256k1
```
## Usage
```
./niceBit
```

Specify minimum number of consecutive digits:
```
./niceBit -n 5
```

Search for words from a file:
```
./niceBit -f FILE
```

Search for alphanumeric substitutions of words from a file:
```
./niceBit -aC -f FILE
```

The file should contain one word per line.

## Loading Private Keys
Private keys are printed in base58 [Wallet Import Format](https://en.bitcoin.it/wiki/Wallet_import_format), which can be easily loaded into common desktop wallet software.

Proof that this all works can be found via these [ingoing and outgoing transactions](https://explorer.bitcoin.com/bch/address/1Hh555555Yhogq3c13DfSkC4VFrG4ypcib) which involves transactions with a vanity address (legacy on BCH: `1Hh555555Y...`) on the Bitcoin Cash blockchain.


## TODO
* Include secp256k1 in lib
