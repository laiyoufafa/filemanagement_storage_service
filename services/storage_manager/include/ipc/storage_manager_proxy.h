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

#ifndef OHOS_STORAGE_MANAGER_STORAGE_MANAGER_PROXY_H
#define OHOS_STORAGE_MANAGER_STORAGE_MANAGER_PROXY_H

#include "iremote_proxy.h"
#include "istorage_manager.h"

namespace OHOS {
namespace StorageManager {

class StorageManagerProxy : public IRemoteProxy<IStorageManager> {
public: 
    explicit StorageManagerProxy(const sptr<IRemoteObject>& impl)
        : IRemoteProxy<IStorageManager>(impl) {}
    ~StorageManagerProxy() override{}

    int32_t PrepareAddUser(int32_t userId) override;
    int32_t RemoveUser(int32_t userId) override;
    int32_t PrepareStartUser(int32_t userId) override;
    int32_t StopUser(int32_t userId) override;
    int64_t GetFreeSizeOfVolume(std::string volumeUuid) override;
    int64_t GetTotalSizeOfVolume(std::string volumeUuid) override;
    std::vector<int64_t> GetBundleStats(std::string uuid, std::string pkgName) override;
private:
    static inline BrokerDelegator<StorageManagerProxy> delegator_;
};
} // StorageManager   
} // OHOS

#endif // OHOS_STORAGE_MANAGER_STORAGE_MANAGER_PROXY_H