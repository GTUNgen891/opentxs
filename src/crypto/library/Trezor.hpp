// Copyright (c) 2018 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

namespace opentxs::crypto::implementation
{
class Trezor final : virtual public crypto::Trezor,
                     virtual public EncodingProvider
#if OT_CRYPTO_WITH_BIP39
    ,
                     virtual public crypto::Bip39
#endif
#if OT_CRYPTO_WITH_BIP32
    ,
                     public Bip32
#endif
#if OT_CRYPTO_SUPPORTED_KEY_SECP256K1
    ,
                     public AsymmetricProvider,
                     public EcdsaProvider
#endif
{
public:
#if OT_CRYPTO_WITH_BIP32
    std::shared_ptr<proto::AsymmetricKey> GetChild(
        const proto::AsymmetricKey& parent,
        const std::uint32_t index) const override;
    std::shared_ptr<proto::AsymmetricKey> GetHDKey(
        const EcdsaCurve& curve,
        const OTPassword& seed,
        proto::HDPath& path) const override;
    bool RandomKeypair(OTPassword& privateKey, Data& publicKey) const override;
    std::string SeedToFingerprint(
        const EcdsaCurve& curve,
        const OTPassword& seed) const override;
    std::shared_ptr<proto::AsymmetricKey> SeedToPrivateKey(
        const EcdsaCurve& curve,
        const OTPassword& seed) const override;
#endif

#if OT_CRYPTO_WITH_BIP39
    bool SeedToWords(const OTPassword& seed, OTPassword& words) const override;
    void WordsToSeed(
        const OTPassword& words,
        OTPassword& seed,
        const OTPassword& passphrase) const override;
#endif

    std::string Base58CheckEncode(
        const std::uint8_t* inputStart,
        const std::size_t& inputSize) const override;
    bool Base58CheckDecode(const std::string&& input, RawData& output)
        const override;
    bool RIPEMD160(
        const std::uint8_t* input,
        const size_t inputSize,
        std::uint8_t* output) const override;

    bool Sign(
        const Data& plaintext,
        const key::Asymmetric& theKey,
        const proto::HashType hashType,
        Data& signature,  // output
        const OTPasswordData* pPWData = nullptr,
        const OTPassword* exportPassword = nullptr) const override;
    bool Verify(
        const Data& plaintext,
        const key::Asymmetric& theKey,
        const Data& signature,
        const proto::HashType hashType,
        const OTPasswordData* pPWData = nullptr) const override;

    ~Trezor() = default;

private:
    friend opentxs::Factory;

    typedef bool DerivationMode;
    const DerivationMode DERIVE_PRIVATE = true;
    const DerivationMode DERIVE_PUBLIC = false;

    const std::uint8_t KeyMax[32]{
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFC, 0x2F};

#if OT_CRYPTO_SUPPORTED_KEY_SECP256K1
    bool ECDH(
        const Data& publicKey,
        const OTPassword& privateKey,
        OTPassword& secret) const override;
    bool ScalarBaseMultiply(const OTPassword& privateKey, Data& publicKey)
        const override;
#endif

#if OT_CRYPTO_WITH_BIP32
    const curve_info* secp256k1_{nullptr};

    static std::string CurveName(const EcdsaCurve& curve);

    static std::unique_ptr<HDNode> InstantiateHDNode(
        const EcdsaCurve& curve,
        const OTPassword& seed);
    static std::unique_ptr<HDNode> GetChild(
        const HDNode& parent,
        const std::uint32_t index,
        const DerivationMode privateVersion);

    std::unique_ptr<HDNode> DeriveChild(
        const EcdsaCurve& curve,
        const OTPassword& seed,
        proto::HDPath& path) const;
    std::unique_ptr<HDNode> SerializedToHDNode(
        const proto::AsymmetricKey& serialized) const;
    std::shared_ptr<proto::AsymmetricKey> HDNodeToSerialized(
        const proto::AsymmetricKeyType& type,
        const HDNode& node,
        const DerivationMode privateVersion) const;
    std::unique_ptr<HDNode> InstantiateHDNode(const EcdsaCurve& curve) const;
    bool ValidPrivateKey(const OTPassword& key) const;
#endif

    Trezor(const api::Crypto& crypto);
    Trezor() = delete;
    Trezor(const Trezor&) = delete;
    Trezor(Trezor&&) = delete;
    Trezor& operator=(const Trezor&) = delete;
    Trezor& operator=(Trezor&&) = delete;
};
}  // namespace opentxs::crypto::implementation
