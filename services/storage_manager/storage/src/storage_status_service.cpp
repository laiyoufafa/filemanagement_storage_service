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

#include "storage/storage_status_service.h"
#include "accesstoken_kit.h"
#include "ipc_skeleton.h"
#include "hap_token_info.h"
#include "storage_service_constant.h"
#include "storage_service_errno.h"
#include "storage_service_log.h"
#include "installd_client.h"

using namespace std;

namespace OHOS {
namespace StorageManager {
StorageStatusService::StorageStatusService() {}
StorageStatusService::~StorageStatusService() {}

int StorageStatusService::GetCurrentUserId()
{
    int uid = -1;
    uid = IPCSkeleton::GetCallingUid();
    int userId = uid / 200000;
    return userId;
}

std::string StorageStatusService::GetCallingPkgName()
{
    uint32_t pid = IPCSkeleton::GetCallingTokenID();
    Security::AccessToken::HapTokenInfo tokenInfo = Security::AccessToken::HapTokenInfo();
    Security::AccessToken::AccessTokenKit::GetHapTokenInfo(pid, tokenInfo);
    return tokenInfo.bundleName;
}

BundleStats StorageStatusService::GetBundleStats(std::string pkgName)
{
    BundleStats result;
    int userId = GetCurrentUserId();
    LOGD("StorageStatusService::userId is:%d", userId);
    if (userId < 0 || userId > StorageService::MAX_USER_ID) {
        LOGE("StorageStatusService::Invaild userId.");
        return result;
    }
    vector<int64_t> bundleStats;
    int errorcode = AppExecFwk::InstalldClient::GetInstance()->GetBundleStats(pkgName, userId, bundleStats);
    if (bundleStats.size() != dataDir.size() || errorcode != E_OK) {
        LOGE("StorageStatusService::An error occurred in querying bundle stats.");
        return result;
    }
    for (uint i = 0; i < bundleStats.size(); i++) {
        if (bundleStats[i] == E_ERR) {
            LOGE("StorageStatusService::Failed to query %s data.", dataDir[i].c_str());
            bundleStats[i] = 0;
        }
    }
    result.appSize_ = bundleStats[APP];
    result.cacheSize_ = bundleStats[CACHE];
    result.dataSize_ = bundleStats[LOCAL] + bundleStats[DISTRIBUTED] + bundleStats[DATABASE];
    return result;
}

StorageStats StorageStatusService::GetUserStorageStats()
{
    StorageStats result;
    return result;
}

StorageStats StorageStatusService::GetUserStorageStats(int32_t userId)
{
    StorageStats result;
    return result;
}

BundleStats StorageStatusService::GetCurrentBundleStats()
{
    BundleStats result;
    int userId = GetCurrentUserId();
    LOGD("StorageStatusService::userId is:%d", userId);
    if (userId < 0 || userId > StorageService::MAX_USER_ID) {
        LOGE("StorageStatusService::Invaild userId.");
        return result;
    }
    std::string pkgName = GetCallingPkgName();
    LOGD("StorageStatusService::pkgName is %{public}s", pkgName.c_str());
    vector<int64_t> bundleStats;
    int errorcode = AppExecFwk::InstalldClient::GetInstance()->GetBundleStats(pkgName, userId, bundleStats);
    if (bundleStats.size() != dataDir.size() || errorcode != E_OK) {
        LOGE("StorageStatusService::An error occurred in querying bundle stats.");
        return result;
    }
    for (uint i = 0; i < bundleStats.size(); i++) {
        if (bundleStats[i] == E_ERR) {
            LOGE("StorageStatusService::Failed to query %s data.", dataDir[i].c_str());
            bundleStats[i] = 0;
        }
    }
    result.appSize_ = bundleStats[APP];
    result.cacheSize_ = bundleStats[CACHE];
    result.dataSize_ = bundleStats[LOCAL] + bundleStats[DISTRIBUTED] + bundleStats[DATABASE];
    return result;
}
} // StorageManager
} // OHOS
