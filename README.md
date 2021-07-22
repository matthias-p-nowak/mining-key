# mining-key
This program mines a key which can be used to authorize software usage.

# Activation keys

Software usage can be restricted using hard to guess keys which the software can check for correctness. Microsoft Windows had activation keys, which are checked online and permit usage of the software. Similar schemes were based on obfuscation and simply checked the entered key to satisfy certain constraints. Simple schemes allow one to generate accepted keys.

## Hash based restrictions

If one hashes the entered key prepended by a secret, then one can check the resulting hash on leading zeros. If the number of leading zeros is high enough, then the provided key is accepted. Using a cryptographic hash function like SHA256 makes a simple key generation impossible. Instead, the key has to be mined.

## Example

Lets say, we are using *secret* as our secret. The user got the supplied key **matthias-2022-05-07-vnytz**. Using this pattern, one can provide identifying information together with an expiry date. Checking the provided key on validity involves the following steps:

1. combine *secret* with *matthias-2022-05-07-vnytz* -> *secret-matthias-2022-05-07-vnytz*
2. calculate the sha256 hash sum of this string -> *00000002349f907b....*
3. check the hash on the number of leading zeros -> 30 leading zeros
