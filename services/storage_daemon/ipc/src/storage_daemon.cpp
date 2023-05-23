/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ipc/storage_daemon.h"

#include <dlfcn.h>

#ifdef USER_CRYPTO_MANAGER
#include "crypto/key_manager.h"
#endif
#ifdef EXTERNAL_STORAGE_MANAGER
#include "disk/disk_manager.h"
#include "volume/volume_manager.h"
#endif
#include "file_sharing/file_sharing.h"
#include "quota/quota_manager.h"
#include "storage_service_constant.h"
#include "storage_service_errno.h"
#include "storage_service_log.h"
#include "user/user_manager.h"
#include "user/mount_manager.h"
#include "system_ability_definition.h"
#include "cloud_daemon_manager.h"


namespace OHOS {
namespace StorageDaemon {
using namespace OHOS::FileManagement::CloudFile;

typedef int32_t (*CreateShareFileFunc)(std::string, uint32_t, uint32_t);
typedef int32_t (*DeleteShareFileFunc)(uint32_t, std::vector<std::string>);
int32_t StorageDaemon::Shutdown()
{
    return E_OK;
}

int32_t StorageDaemon::Mount(std::string volId, uint32_t flags)
{
#ifdef EXTERNAL_STORAGE_MANAGER
    LOGI("Handle Mount");
    return VolumeManager::Instance()->Mount(volId, flags);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::UMount(std::string volId)
{
#ifdef EXTERNAL_STORAGE_MANAGER
    LOGI("Handle UMount");
    return VolumeManager::Instance()->UMount(volId);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::Check(std::string volId)
{
#ifdef EXTERNAL_STORAGE_MANAGER
    LOGI("Handle Check");
    return VolumeManager::Instance()->Check(volId);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::Format(std::string volId, std::string fsType)
{
#ifdef EXTERNAL_STORAGE_MANAGER
    LOGI("Handle Format");
    return VolumeManager::Instance()->Format(volId, fsType);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::Partition(std::string diskId, int32_t type)
{
#ifdef EXTERNAL_STORAGE_MANAGER
    LOGI("Handle Partition");
    return DiskManager::Instance()->HandlePartition(diskId);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::SetVolumeDescription(std::string volId, std::string description)
{
#ifdef EXTERNAL_STORAGE_MANAGER
    LOGI("Handle SetVolumeDescription");
    return VolumeManager::Instance()->SetVolumeDescription(volId, description);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::PrepareUserDirs(int32_t userId, uint32_t flags)
{
#ifdef USER_CRYPTO_MANAGER
    int32_t ret = KeyManager::GetInstance()->GenerateUserKeys(userId, flags);
    if (ret != E_OK) {
        LOGE("Generate user %{public}d key error", userId);
        return ret;
    }
#endif

    return UserManager::GetInstance()->PrepareUserDirs(userId, flags);
}

int32_t StorageDaemon::DestroyUserDirs(int32_t userId, uint32_t flags)
{
    int32_t ret = UserManager::GetInstance()->DestroyUserDirs(userId, flags);
    if (ret != E_OK) {
        LOGW("Destroy user %{public}d dirs failed, please check", userId);
    }

#ifdef USER_CRYPTO_MANAGER
    return KeyManager::GetInstance()->DeleteUserKeys(userId);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::StartUser(int32_t userId)
{
    return UserManager::GetInstance()->StartUser(userId);
}

int32_t StorageDaemon::StopUser(int32_t userId)
{
    return UserManager::GetInstance()->StopUser(userId);
}

int32_t StorageDaemon::InitGlobalKey(void)
{
#ifdef USER_CRYPTO_MANAGER
    return KeyManager::GetInstance()->InitGlobalDeviceKey();
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::InitGlobalUserKeys(void)
{
#ifdef USER_FILE_SHARING
    // File sharing depends on the /data/service/el1/public be decrypted.
    // A hack way to prepare the sharing dir, move it to callbacks after the parameter ready.
    if (SetupFileSharingDir() == -1) {
        LOGE("Failed to set up the directory for file sharing");
    }
#endif

#ifdef USER_CRYPTO_MANAGER
    int ret = KeyManager::GetInstance()->InitGlobalUserKeys();
    if (ret) {
        LOGE("Init global users els failed");
        return ret;
    }
#endif

    return UserManager::GetInstance()->PrepareUserDirs(GLOBAL_USER_ID, CRYPTO_FLAG_EL1);
}

int32_t StorageDaemon::GenerateUserKeys(uint32_t userId, uint32_t flags)
{
#ifdef USER_CRYPTO_MANAGER
    return KeyManager::GetInstance()->GenerateUserKeys(userId, flags);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::DeleteUserKeys(uint32_t userId)
{
#ifdef USER_CRYPTO_MANAGER
    return KeyManager::GetInstance()->DeleteUserKeys(userId);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::UpdateUserAuth(uint32_t userId, uint64_t secureUid,
                                      const std::vector<uint8_t> &token,
                                      const std::vector<uint8_t> &oldSecret,
                                      const std::vector<uint8_t> &newSecret)
{
#ifdef USER_CRYPTO_MANAGER
    return KeyManager::GetInstance()->UpdateUserAuth(userId, secureUid, token, oldSecret, newSecret);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::ActiveUserKey(uint32_t userId,
                                     const std::vector<uint8_t> &token,
                                     const std::vector<uint8_t> &secret)
{
#ifdef USER_CRYPTO_MANAGER
    return KeyManager::GetInstance()->ActiveUserKey(userId, token, secret);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::InactiveUserKey(uint32_t userId)
{
#ifdef USER_CRYPTO_MANAGER
    return KeyManager::GetInstance()->InActiveUserKey(userId);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::UpdateKeyContext(uint32_t userId)
{
#ifdef USER_CRYPTO_MANAGER
    return KeyManager::GetInstance()->UpdateKeyContext(userId);
#else
    return E_OK;
#endif
}

int32_t StorageDaemon::CreateShareFile(std::string uri, uint32_t tokenId, uint32_t flag)
{
    void *dlhandler = dlopen("libfileshare_native.z.so", RTLD_LAZY);
    if (dlhandler == NULL) {
        LOGE("CreateShareFile cannot open so, errno = %{public}s", dlerror());
        return E_ERR;
    }
    CreateShareFileFunc createShareFile = nullptr;
    createShareFile = reinterpret_cast<CreateShareFileFunc>(dlsym(dlhandler, "CreateShareFile"));
    if (createShareFile == nullptr) {
        LOGE("CreateShareFile dlsym failed, errno = %{public}s", dlerror());
        dlclose(dlhandler);
        return E_ERR;
    }
    int ret = createShareFile(uri, tokenId, flag);
    dlclose(dlhandler);
    return ret;
}

int32_t StorageDaemon::DeleteShareFile(uint32_t tokenId, std::vector<std::string>sharePathList)
{
    void *dlhandler = dlopen("libfileshare_native.z.so", RTLD_LAZY);
    if (dlhandler == NULL) {
        LOGE("DeleteShareFile cannot open so, errno = %{public}s", dlerror());
        return E_ERR;
    }
    DeleteShareFileFunc deleteShareFile = nullptr;
    deleteShareFile = reinterpret_cast<DeleteShareFileFunc>(dlsym(dlhandler, "DeleteShareFile"));
    if (deleteShareFile == nullptr) {
        LOGE("DeleteShareFile dlsym failed, errno = %{public}s", dlerror());
        dlclose(dlhandler);
        return E_ERR;
    }
    int32_t ret = deleteShareFile(tokenId, sharePathList);
    dlclose(dlhandler);
    return ret;
}

int32_t StorageDaemon::SetBundleQuota(const std::string &bundleName, int32_t uid,
    const std::string &bundleDataDirPath, int32_t limitSizeMb)
{
    return QuotaManager::GetInstance()->SetBundleQuota(bundleName, uid, bundleDataDirPath, limitSizeMb);
}

void StorageDaemon::SystemAbilityStatusChangeListener::OnAddSystemAbility(int32_t systemAbilityId,
                                                                          const std::string &deviceId)
{
    LOGI("SystemAbilityId:%{public}d", systemAbilityId);
    if (systemAbilityId == FILEMANAGEMENT_CLOUD_DAEMON_SERVICE_SA_ID) {
        MountManager::GetInstance()->SetCloudState(true);
    }
}

void StorageDaemon::SystemAbilityStatusChangeListener::OnRemoveSystemAbility(int32_t systemAbilityId,
                                                                             const std::string &deviceId)
{
    LOGI("SystemAbilityId:%{public}d", systemAbilityId);
    if (systemAbilityId == FILEMANAGEMENT_CLOUD_DAEMON_SERVICE_SA_ID) {
        MountManager::GetInstance()->SetCloudState(false);
    }
}

} // StorageDaemon
} // OHOS
