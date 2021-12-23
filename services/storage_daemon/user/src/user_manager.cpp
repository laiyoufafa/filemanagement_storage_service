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

#include "user/user_manager.h"
#include "ipc/istorage_daemon.h"
#include "utils/errno.h"
#include "utils/file_utils.h"
#include "utils/string_utils.h"
#include "utils/log.h"
#include "utils/user_path.h"
#include <stdlib.h>

using namespace std;

namespace OHOS {
namespace StorageDaemon {
constexpr int32_t UMOUNT_RETRY_TIMES = 3;
UserManager* UserManager::instance_ = nullptr;

UserManager* UserManager::Instance()
{
    if (instance_ == nullptr) {
        instance_ = new UserManager();
    }

    return instance_;
}

int32_t UserManager::CheckUserState(int32_t userId, UserState state)
{
    auto iterator = users_.find(userId);
    if (iterator == users_.end()) {
        LOGE("the user %{public}d doesn't exist", userId);
        return E_NON_EXIST;
    }

    UserInfo& user = iterator->second;
    if (user.GetState() != state) {
        LOGE("the user's state %{public}d is invalid", user.GetState());
        return E_USER_STATE;
    }

    return E_OK;
}

inline void UserManager::SetUserState(int32_t userId, UserState state)
{
    // because of mutex lock, user must exist without checking
    users_.at(userId).SetState(state);
}

int32_t UserManager::AddUser(int32_t userId)
{
    LOGI("add user %{public}d", userId);

    if (users_.count(userId) != 0) {
        return E_EXIST;
    }
    users_.insert({ userId, UserInfo(userId, USER_CREAT) });

    return E_OK;
}

int32_t UserManager::RemoveUser(int32_t userId)
{
    LOGI("remove user %{public}d", userId);

    users_.erase(userId);

    return E_OK;
}

int32_t UserManager::StartUser(int32_t userId)
{
    LOGI("start user %{public}d", userId);

    if (CheckUserState(userId, USER_PREPARE)) {
        return E_USER_STATE;
    }

    int32_t err = Mount(StringPrintf(HMDFS_SOURCE.c_str(), userId),
                        StringPrintf(HMDFS_TARGET.c_str(), userId).c_str(),
                        nullptr, MS_BIND, nullptr);
    if (err) {
        LOGE("failed to mount, err %{public}d", errno);
        return err;
    }

    SetUserState(userId, USER_START);

    return E_OK;
}

int32_t UserManager::StopUser(int32_t userId)
{
    LOGI("stop user %{public}d", userId);

    if (CheckUserState(userId, USER_START)) {
        return E_USER_STATE;
    }

    int32_t count = 0;
    int32_t err;
    while (count < UMOUNT_RETRY_TIMES) {
        err = UMount(StringPrintf(HMDFS_TARGET.c_str(), userId));
        if (err == E_OK) {
            break;
        } else if (errno == EBUSY) {
            count++;
            continue;
        } else {
            LOGE("failed to umount, errno %{public}d", errno);
            return err;
        }
    }

    SetUserState(userId, USER_PREPARE);

    return E_OK;
}

int32_t UserManager::PrepareUserDirs(int32_t userId, uint32_t flags)
{
    LOGI("prepare user dirs for %{public}d, flags %{public}u", userId, flags);

    if (CheckUserState(userId, USER_CREAT)) {
        return E_USER_STATE;
    }

    int err = E_OK;
    if (flags & IStorageDaemon::CRYPTO_FLAG_EL1) {
        if ((err = PrepareUserEl1Dirs(userId))) {
            LOGE("failed to prepare user el1 dirs for userid %{public}d", userId);
            return err;
        }
    }

    if (flags & IStorageDaemon::CRYPTO_FLAG_EL2) {
        if ((err = PrepareUserEl2Dirs(userId))) {
            LOGE("failed to prepare user el2 dirs for userid %{public}d", userId);
            return err;
        }
    }

    SetUserState(userId, USER_PREPARE);

    return err;
}

int32_t UserManager::DestroyUserDirs(int32_t userId, uint32_t flags)
{
    LOGI("destroy user dirs for %{public}d, flags %{public}u", userId, flags);

    if (CheckUserState(userId, USER_PREPARE)) {
        return E_USER_STATE;
    }

    int err = E_OK;
    if (flags & IStorageDaemon::CRYPTO_FLAG_EL1) {
        err = DestroyUserEl1Dirs(userId);
        if (err != E_OK) {
            LOGE("failed to destroy user el1 dirs for userid %{public}d", userId);
        }
    }

    if (flags & IStorageDaemon::CRYPTO_FLAG_EL2) {
        err = DestroyUserEl2Dirs(userId);
        if (err != E_OK) {
            LOGE("failed to destroy user el2 dirs for userid %{public}d", userId);
        }
    }

    SetUserState(userId, USER_CREAT);

    return err;
}

inline int32_t PrepareUserDirsFromVec(int32_t userId, std::vector<DirInfo> dirVec)
{
    int32_t err = E_OK;

    for (DirInfo &dir : dirVec) {
        if ((err = PrepareDir(StringPrintf(dir.path.c_str(), userId), dir.mode, dir.uid, dir.gid))) {
                return err;
        }
    }

    return err;
}

// Destory dirs as much as possible, and return one of err;
inline int32_t DestroyUserDirsFromVec(int32_t userId, std::vector<DirInfo> dirVec)
{
    int32_t err = E_OK;

    for (DirInfo &dir : dirVec) {
        if (IsEndWith(dir.path.c_str(), "%d")) {
            int32_t ret = RmDirRecurse(StringPrintf(dir.path.c_str(), userId));
            err = ret ? ret : err;
        }
    }

    return err;
}

int32_t UserManager::PrepareUserEl1Dirs(int32_t userId)
{
    return PrepareUserDirsFromVec(userId, g_el1DirVec);
}

int32_t UserManager::PrepareUserEl2Dirs(int32_t userId)
{
    int32_t err = E_OK;

    err = PrepareUserDirsFromVec(userId, g_el2DirVec);
    if (err) {
        return err;
    }

    return PrepareUserHmdfsDirs(userId);
}

int32_t UserManager::PrepareUserHmdfsDirs(int32_t userId)
{
    return PrepareUserDirsFromVec(userId, g_hmdfsDirVec);
}

int32_t UserManager::DestroyUserEl1Dirs(int32_t userId)
{
    return DestroyUserDirsFromVec(userId, g_el1DirVec);
}

int32_t UserManager::DestroyUserEl2Dirs(int32_t userId)
{
    int err = E_OK;

    err = DestroyUserDirsFromVec(userId, g_hmdfsDirVec);
    if (err) {
        return err;
    }

    return DestroyUserDirsFromVec(userId, g_el2DirVec);
}

int32_t UserManager::DestroyUserHmdfsDirs(int32_t userId)
{
    return DestroyUserDirsFromVec(userId, g_hmdfsDirVec);
}

} // StorageDaemon
} // OHOS