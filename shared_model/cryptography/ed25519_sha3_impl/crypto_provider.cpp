/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cryptography/ed25519_sha3_impl/crypto_provider.hpp"
#include "cryptography/ed25519_sha3_impl/internal/ed25519_impl.hpp"

namespace shared_model {
  namespace crypto {

    Signed CryptoProvider::sign(const Blob &blob,
                                const Keypair &keypair) const {
      return signer_.sign(blob, keypair);
    }

    bool CryptoProvider::verify(const Signed &signedData,
                                const Blob &orig,
                                const PublicKey &publicKey) const {
      return verifier_.verify(signedData, orig, publicKey);
    }

    Seed CryptoProvider::generateSeed() const {
      return Seed(iroha::create_seed().to_string());
    }

    Seed CryptoProvider::generateSeed(const std::string &passphrase) const {
      return Seed(iroha::create_seed(passphrase).to_string());
    }

    Keypair CryptoProvider::generateKeypair() const {
      auto keypair = iroha::create_keypair();
      return Keypair(PublicKey(keypair.pubkey.to_string()),
                     PrivateKey(keypair.privkey.to_string()));
    }

    Keypair CryptoProvider::generateKeypair(const Seed &seed) const {
      auto keypair =
          iroha::create_keypair(seed.makeOldModel<Seed::OldSeedType>());
      return Keypair(PublicKey(keypair.pubkey.to_string()),
                     PrivateKey(keypair.privkey.to_string()));
    }
  }  // namespace crypto
}  // namespace shared_model
