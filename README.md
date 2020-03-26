# Bitcoin Vanity Address Generator
Create Bitcoin vanity addresses with C.
Also doubles as an open source method to generate Bitcoin wallets.

## TODO
* Create make file
* Add search from dictionary words

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

## Loading Private Keys
Private keys are printed in base58 [Wallet Import Format](https://en.bitcoin.it/wiki/Wallet_import_format), which can be easily loaded into common desktop wallet software.

Proof that this all works can be found via these [ingoing and outgoing transactions](https://explorer.bitcoin.com/bch/address/bitcoincash:qpwqeggq855emxh77qfx60fkaaakz0zmcyf6xazy73) which involves transactions with a vanity address (legacy on BCH) on the Bitcoin Cash blockchain.
