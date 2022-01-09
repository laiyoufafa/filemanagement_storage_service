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

#ifndef OHOS_STORAGE_MANAGER_ISTORAGE_MANAGER_H
#define OHOS_STORAGE_MANAGER_ISTORAGE_MANAGER_H

#include "iremote_broker.h"

namespace OHOS {
namespace StorageManager {

class IStorageManager : public IRemoteBroker {
public:
    virtual int32_t PrepareAddUser(int32_t userId) = 0;
    virtual int32_t RemoveUser(int32_t userId) = 0;
    virtual int32_t PrepareStartUser(int32_t userId) = 0;
    virtual int32_t StopUser(int32_t userId) = 0;
    virtual int64_t GetFreeSizeOfVolume(std::string volumeUuid) = 0;
    virtual int64_t GetTotalSizeOfVolume(std::string volumeUuid) = 0;
    virtual std::vector<int64_t> GetBundleStats(std::string uuid, std::string pkgName) = 0;

    enum {
        PREPARE_ADD_USER = 1,
        REMOVE_USER,
        PREPARE_START_USER,
        STOP_USER,
        GET_TOTAL,
        GET_FREE,
        GET_BUNDLE_STATUS
    };

    DECLARE_INTERFACE_DESCRIPTOR(u"OHOS.StorageManager.IStorageManager");
};
} // StorageManager
} // OHOS

#endif // OHOS_STORAGE_MANAGER_ISTORAGER_MANAGER_H