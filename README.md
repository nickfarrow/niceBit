# Bitcoin Vanity Address Generator
Create Bitcoin vanity addresses with C.
Also doubles as an open source method to generate Bitcoin wallets.

## TODO
* Create make file
* Convert to Wallet Import Format locally
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
Private keys are currently printed in hex. To load in a wallet such as Electron you must first convert to the more conventional [Wallet Import Format](https://en.bitcoin.it/wiki/Wallet_import_format).

To do this I have been using [this website](https://gobittest.appspot.com/PrivateKey), which possibly could steal your private keys...

Proof that this all works can be found via these [ingoing and outgoing transactions](https://explorer.bitcoin.com/bch/address/bitcoincash:qpwqeggq855emxh77qfx60fkaaakz0zmcyf6xazy73) which involves transactions with a vanity address (legacy on BCH) on the Bitcoin Cash blockchain.
