/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "huks_master.h"

#include <dlfcn.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

#include "hks_param.h"
#include "storage_service_log.h"

namespace OHOS {
namespace StorageDaemon {
HuksMaster::HuksMaster()
{
    LOGD("enter");
    HdiCreate();
    HdiModuleInit();
    LOGD("finish");
}

HuksMaster::~HuksMaster()
{
    LOGD("enter");
    HdiDestroy();
    LOGD("finish");
}

bool HuksMaster::HdiCreate()
{
    LOGD("enter");
    if (hdiHandle_ != nullptr || halDevice_ != nullptr) {
        return true;
    }

    hdiHandle_ = dlopen("libhuks_engine_core_standard.z.so", RTLD_LAZY);
    if (hdiHandle_ == nullptr) {
        LOGE("dlopen failed %{public}s", dlerror());
        return false;
    }

    auto createHdi = reinterpret_cast<HkmHalCreateHandle>(dlsym(hdiHandle_, "HuksCreateHdiDevicePtr"));
    if (createHdi == nullptr) {
        LOGE("dlsym failed %{public}s", dlerror());
        dlclose(hdiHandle_);
        hdiHandle_ = nullptr;
        return false;
    }

    halDevice_ = (*createHdi)();
    if (halDevice_ == nullptr) {
        LOGE("HuksHdiCreate failed");
        dlclose(hdiHandle_);
        hdiHandle_ = nullptr;
        return false;
    }
    LOGD("success");
    return true;
}

void HuksMaster::HdiDestroy()
{
    LOGD("enter");
    if (hdiHandle_ == nullptr) {
        LOGI("hdiHandle_ is nullptr, already destroyed");
        return;
    }

    auto destroyHdi = reinterpret_cast<HkmHalDestroyHandle>(dlsym(hdiHandle_, "HuksDestoryHdiDevicePtr"));
    if ((destroyHdi != nullptr) && (halDevice_ != nullptr)) {
        (*destroyHdi)(halDevice_);
    }

    // add dlclose(hdiHandle_) here will cause segmentfault when exit
    hdiHandle_ = nullptr;
    halDevice_ = nullptr;
    LOGD("finish");
}

int HuksMaster::HdiModuleInit()
{
    LOGD("enter");
    if (halDevice_ == nullptr) {
        LOGE("halDevice_ is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }
    if (halDevice_->HuksHdiModuleInit == nullptr) {
        LOGE("HuksHdiModuleInit is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }

    int ret = halDevice_->HuksHdiModuleInit();
    if (ret != HKS_SUCCESS) {
        LOGE("HuksHdiModuleInit failed, ret %{public}d", ret);
    }
    return ret;
}

int HuksMaster::HdiGenerateKey(const HksBlob &keyAlias, const HksParamSet *paramSetIn,
                               HksBlob &keyOut)
{
    LOGD("enter");
    if (halDevice_ == nullptr) {
        LOGE("halDevice_ is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }
    if (halDevice_->HuksHdiGenerateKey == nullptr) {
        LOGE("HuksHdiAccessGenerateKey is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }

    uint8_t d = 0;
    HksBlob keyIn = {1, &d};
    auto ret = halDevice_->HuksHdiGenerateKey(&keyAlias, paramSetIn, &keyIn, &keyOut);
    if (ret != HKS_SUCCESS) {
        LOGE("HuksHdiGenerateKey failed, ret %{public}d", ret);
    }
    return ret;
}

int HuksMaster::HdiAccessInit(const HksBlob &key, const HksParamSet *paramSet,
                              HksBlob &handle)
{
    LOGD("enter");
    if (halDevice_ == nullptr) {
        LOGE("halDevice_ is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }
    if (halDevice_->HuksHdiInit == nullptr) {
        LOGE("HuksHdiAccessInit is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }

    auto ret = halDevice_->HuksHdiInit(&key, paramSet, &handle);
    if (ret != HKS_SUCCESS) {
        LOGE("HuksHdiInit failed, ret %{public}d", ret);
    }
    return ret;
}

int HuksMaster::HdiAccessUpdate(const HksBlob &handle, const HksParamSet *paramSet,
                                const HksBlob &inData, HksBlob &outData)
{
    LOGD("enter");
    if (halDevice_ == nullptr) {
        LOGE("halDevice_ is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }
    if (halDevice_->HuksHdiUpdate == nullptr) {
        LOGE("HuksHdiAccessUpdate is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }

    auto ret = halDevice_->HuksHdiUpdate(&handle, paramSet, &inData, &outData);
    if (ret != HKS_SUCCESS) {
        LOGE("HuksHdiUpdate failed, ret %{public}d", ret);
    }
    return ret;
}

int HuksMaster::HdiAccessFinish(const HksBlob &handle, const HksParamSet *paramSet,
                                const HksBlob &inData, HksBlob &outData)
{
    LOGD("enter");
    if (halDevice_ == nullptr) {
        LOGE("halDevice_ is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }
    if (halDevice_->HuksHdiFinish == nullptr) {
        LOGE("HuksHdiAccessFinish is nullptr");
        return HKS_ERROR_NULL_POINTER;
    }

    auto ret = halDevice_->HuksHdiFinish(&handle, paramSet, &inData, &outData);
    if (ret != HKS_SUCCESS) {
        LOGE("HuksHdiFinish failed, ret %{public}d", ret);
    }
    return ret;
}

KeyBlob HuksMaster::GenerateRandomKey(uint32_t keyLen)
{
    LOGD("enter, size %{public}d", keyLen);
    KeyBlob out(keyLen);
    if (out.IsEmpty()) {
        return out;
    }

    auto ret = RAND_bytes(out.data.get(), out.size);
    if (ret <= 0) {
        LOGE("RAND_bytes failed return %{public}d, errno %{public}lu", ret, ERR_get_error());
        out.Clear();
    }
    return out;
}

static int AppendProcessName(HksParamSet *paramSet)
{
    static uint8_t processName[sizeof(uint32_t)] = {0};

    HksParam param[] = {
        { .tag = HKS_TAG_PROCESS_NAME,
          .blob =
            { sizeof(uint32_t), processName }
        },
    };
    return HksAddParams(paramSet, param, HKS_ARRAY_SIZE(param));
}

bool HuksMaster::GenerateKey(KeyBlob &keyOut)
{
    LOGD("enter");
    static const HksParam keyParam[] = {
        { .tag = HKS_TAG_KEY_GENERATE_TYPE, .uint32Param = HKS_KEY_GENERATE_TYPE_DEFAULT },
        { .tag = HKS_TAG_PURPOSE, .uint32Param = HKS_KEY_PURPOSE_ENCRYPT | HKS_KEY_PURPOSE_DECRYPT },
        { .tag = HKS_TAG_ALGORITHM, .uint32Param = HKS_ALG_AES },
        { .tag = HKS_TAG_KEY_SIZE, .uint32Param = HKS_AES_KEY_SIZE_256 },
        { .tag = HKS_TAG_DIGEST, .uint32Param = HKS_DIGEST_SHA256 },
        { .tag = HKS_TAG_BLOCK_MODE, .uint32Param = HKS_MODE_GCM },
        { .tag = HKS_TAG_PADDING, .uint32Param = HKS_PADDING_NONE }
    };
    HksParamSet *paramSet = nullptr;
    int ret = HKS_SUCCESS;

    do {
        ret = HksInitParamSet(&paramSet);
        if (ret != HKS_SUCCESS) {
            LOGE("HksInitParamSet failed ret %{public}d", ret);
            break;
        }
        ret = HksAddParams(paramSet, keyParam, HKS_ARRAY_SIZE(keyParam));
        if (ret != HKS_SUCCESS) {
            LOGE("HksAddParams failed ret %{public}d", ret);
            break;
        }
        ret = AppendProcessName(paramSet);
        if (ret != HKS_SUCCESS) {
            LOGE("AppendProcessName failed ret %{public}d", ret);
            break;
        }
        ret = HksBuildParamSet(&paramSet);
        if (ret != HKS_SUCCESS) {
            LOGE("HksBuildParamSet failed ret %{public}d", ret);
            break;
        }
        KeyBlob alias = GenerateRandomKey(CRYPTO_KEY_ALIAS_SIZE);
        HksBlob hksAlias = alias.ToHksBlob();
        keyOut.Alloc(CRYPTO_KEY_SHIELD_SIZE);
        HksBlob hksKeyOut = keyOut.ToHksBlob();
        ret = HdiGenerateKey(hksAlias, paramSet, hksKeyOut);
        if (ret != HKS_SUCCESS) {
            LOGE("HdiGenerateKey failed ret %{public}d", ret);
            break;
        }
        keyOut.size = hksKeyOut.size;
        LOGI("HdiGenerateKey success, out size %{public}d", keyOut.size);
    } while (0);

    HksFreeParamSet(&paramSet);
    return ret == HKS_SUCCESS;
}

static KeyBlob HashAndClip(const std::string &prefix, const KeyBlob &payload, const uint32_t length)
{
    KeyBlob res(SHA512_DIGEST_LENGTH);
    std::string header = prefix;
    if (header.empty()) {
        header = "dummy SHA512 header";
    }

    SHA512_CTX c;
    SHA512_Init(&c);
    SHA512_Update(&c, header.data(), header.size());
    if (!payload.IsEmpty()) {
        SHA512_Update(&c, payload.data.get(), payload.size);
    }
    SHA512_Final(res.data.get(), &c);

    res.size = length;
    return res;
}

static HksParamSet *GenHuksKeyBlobParam(KeyContext &ctx)
{
    return reinterpret_cast<HksParamSet *>(ctx.shield.data.get());
}

static int AppendAeTag(KeyBlob &cipherText, HksParamSet *paramSet)
{
    if (cipherText.size <= HKS_AE_TAG_LEN) {
        LOGE("cipherText size %{public}d is too small", cipherText.size);
        return HKS_ERROR_INVALID_KEY_INFO;
    }

    HksParam param[] = {
        { .tag = HKS_TAG_AE_TAG,
          .blob =
            { HKS_AE_TAG_LEN, cipherText.data.get() + cipherText.size - HKS_AE_TAG_LEN }
        },
    };
    cipherText.size -= HKS_AE_TAG_LEN;
    return HksAddParams(paramSet, param, HKS_ARRAY_SIZE(param));
}

static int AppendNonceAad(KeyContext &ctx, HksParamSet *paramSet)
{
    ctx.nonce = HashAndClip("NONCE SHA512 prefix", ctx.secDiscard, HKS_AE_NONCE_LEN);
    ctx.aad = HashAndClip("AAD SHA512 prefix", ctx.secDiscard, CRYPTO_AES_AAD_LEN);
    // pass the token here
    HksParam addParam[] = {
        { .tag = HKS_TAG_NONCE,
          .blob =
            { ctx.nonce.size, ctx.nonce.data.get() }
        },
        { .tag = HKS_TAG_ASSOCIATED_DATA,
          .blob =
            { ctx.aad.size, ctx.aad.data.get() }
        },
    };
    return HksAddParams(paramSet, addParam, HKS_ARRAY_SIZE(addParam));
}

static HksParamSet *GenHuksOptionParam(KeyContext &ctx, const UserAuth &auth, const bool isEncrypt)
{
    (void)auth;
    HksParamSet *paramSet = nullptr;
    HksParam encryptParam[] = {
        { .tag = HKS_TAG_ALGORITHM, .uint32Param = HKS_ALG_AES },
        { .tag = HKS_TAG_BLOCK_MODE, .uint32Param = HKS_MODE_GCM },
        { .tag = HKS_TAG_PADDING, .uint32Param = HKS_PADDING_NONE },
        { .tag = HKS_TAG_IS_KEY_ALIAS, .boolParam = false },
        { .tag = HKS_TAG_PURPOSE, .uint32Param = isEncrypt ? HKS_KEY_PURPOSE_ENCRYPT : HKS_KEY_PURPOSE_DECRYPT},
    };

    auto ret = HksInitParamSet(&paramSet);
    if (ret != HKS_SUCCESS) {
        LOGE("HksInitParamSet failed ret %{public}d", ret);
        return nullptr;
    }
    ret = HksAddParams(paramSet, encryptParam, HKS_ARRAY_SIZE(encryptParam));
    if (ret != HKS_SUCCESS) {
        LOGE("HksAddParams failed ret %{public}d", ret);
        HksFreeParamSet(&paramSet);
        return nullptr;
    }

    if (!isEncrypt) {
        ret = AppendAeTag(ctx.encrypted, paramSet);
        if (ret != HKS_SUCCESS) {
            LOGE("AppendNonceAad failed ret %{public}d", ret);
            HksFreeParamSet(&paramSet);
            return nullptr;
        }
    }

    ret = AppendNonceAad(ctx, paramSet);
    if (ret != HKS_SUCCESS) {
        LOGE("AppendNonceAad failed ret %{public}d", ret);
        HksFreeParamSet(&paramSet);
        return nullptr;
    }

    ret = AppendProcessName(paramSet);
    if (ret != HKS_SUCCESS) {
        LOGE("AppendProcessName failed");
        HksFreeParamSet(&paramSet);
        return nullptr;
    }

    ret = HksBuildParamSet(&paramSet);
    if (ret != HKS_SUCCESS) {
        LOGE("HksBuildParamSet failed ret %{public}d", ret);
        HksFreeParamSet(&paramSet);
        return nullptr;
    }

    return paramSet;
}

bool HuksMaster::HuksHalTripleStage(HksParamSet *paramSet1, const HksParamSet *paramSet2,
                                    const KeyBlob &keyIn, KeyBlob &keyOut)
{
    LOGD("enter");
    HksBlob hksKey = { paramSet1->paramSetSize, reinterpret_cast<uint8_t *>(paramSet1) };
    HksBlob hksIn = keyIn.ToHksBlob();
    HksBlob hksOut = keyOut.ToHksBlob();
    uint8_t h[sizeof(uint64_t)] = {0};
    HksBlob hksHandle = { sizeof(uint64_t), h};

    int ret = HdiAccessInit(hksKey, paramSet2, hksHandle);
    if (ret != HKS_SUCCESS) {
        LOGE("HdiAccessInit failed ret %{public}d", ret);
        return false;
    }

    ret = HdiAccessFinish(hksHandle, paramSet2, hksIn, hksOut);
    if (ret != HKS_SUCCESS) {
        LOGE("HdiAccessFinish failed ret %{public}d", ret);
        return false;
    }

    keyOut.size = hksOut.size;
    return true;
}

bool HuksMaster::EncryptKey(KeyContext &ctx, const UserAuth &auth, const KeyInfo &key)
{
    LOGD("enter");
    if (ctx.shield.IsEmpty()) {
        LOGE("bad shield input, size %{public}d", ctx.shield.size);
        return false;
    }
    if (key.key.IsEmpty()) {
        LOGE("bad rawKey input, size %{public}d", key.key.size);
        return false;
    }

    HksParamSet *paramSet1 = GenHuksKeyBlobParam(ctx);
    if (paramSet1 == nullptr) {
        LOGE("GenHuksKeyBlobParam failed");
        return false;
    }
    HksParamSet *paramSet2 = GenHuksOptionParam(ctx, auth, true);
    if (paramSet2 == nullptr) {
        LOGE("GenHuksOptionParam failed");
        return false;
    }

    ctx.encrypted.Alloc(CRYPTO_AES_256_KEY_ENCRYPTED_SIZE);
    auto ret = HuksHalTripleStage(paramSet1, paramSet2, key.key, ctx.encrypted);
    if (!ret) {
        LOGE("HuksHalTripleStage failed");
    }

    HksFreeParamSet(&paramSet2);
    LOGD("finish");
    return ret;
}

bool HuksMaster::DecryptKey(KeyContext &ctx, const UserAuth &auth, KeyInfo &key)
{
    LOGD("enter");
    if (ctx.shield.IsEmpty()) {
        LOGE("bad shield input, size %{public}d", ctx.shield.size);
        return false;
    }
    if (ctx.encrypted.IsEmpty()) {
        LOGE("bad encrypted input, size %{public}d", ctx.encrypted.size);
        return false;
    }

    HksParamSet *paramSet1 = GenHuksKeyBlobParam(ctx);
    if (paramSet1 == nullptr) {
        LOGE("GenHuksKeyBlobParam failed");
        return false;
    }
    HksParamSet *paramSet2 = GenHuksOptionParam(ctx, auth, false);
    if (paramSet2 == nullptr) {
        LOGE("GenHuksOptionParam failed");
        return false;
    }

    key.key.Alloc(CRYPTO_AES_256_XTS_KEY_SIZE);
    auto ret = HuksHalTripleStage(paramSet1, paramSet2, ctx.encrypted, key.key);
    if (!ret) {
        LOGE("HuksHalTripleStage failed");
    }

    HksFreeParamSet(&paramSet2);
    LOGD("finish");
    return ret;
}
} // namespace StorageDaemon
} // namespace OHOS
