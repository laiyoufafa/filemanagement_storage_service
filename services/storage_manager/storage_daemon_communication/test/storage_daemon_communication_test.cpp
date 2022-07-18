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

#include <cstdio>
#include <gtest/gtest.h>

#include "storage_daemon_communication/storage_daemon_communication.h"
#include "user/multi_user_manager_service.h"
#include "storage_service_errno.h"

namespace {
using namespace std;
using namespace OHOS;
using namespace StorageManager;
class StorageDaemonCommunicationTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_Connect_0000
 * @tc.name: Daemon_communication_Connect_0000
 * @tc.desc: Test function of Connect interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_Connect_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_Connect_0000";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    if (sdCommunication != nullptr) {
        result = sdCommunication->Connect();
    }
    EXPECT_EQ(result, E_OK);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_Connect_0000";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_PrepareAddUser_0000
 * @tc.name: Daemon_communication_PrepareAddUser_0000
 * @tc.desc: Test function of PrepareAddUser interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_PrepareAddUser_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_PrepareAddUser_0000";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t userId = 101;
    uint32_t flag = 3;
    int32_t result;
    if (sdCommunication != nullptr) {
        result = sdCommunication->PrepareAddUser(userId, flag);
    }
    EXPECT_EQ(result, E_OK);
    sdCommunication->RemoveUser(userId, flag);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_PrepareAddUser_0000";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_RemoveUser_0000
 * @tc.name: Daemon_communication_RemoveUser_0000
 * @tc.desc: Test function of RemoveUser interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_RemoveUser_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_RemoveUser_0000";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t userId = 102;
    uint32_t flag = 3;
    int32_t result;
    if (sdCommunication != nullptr) {
        sdCommunication->PrepareAddUser(userId, flag);
        result = sdCommunication->RemoveUser(userId, flag);
    }
    EXPECT_EQ(result, E_OK);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_RemoveUser_0000";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_PrepareStartUser_0000
 * @tc.name: Daemon_communication_PrepareStartUser_0000
 * @tc.desc: Test function of PrepareStartUser interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_PrepareStartUser_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_PrepareStartUser_0000";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t userId = 104;
    uint32_t flag = 3;
    int32_t result;
    if (sdCommunication != nullptr) {
        sdCommunication->PrepareAddUser(userId, flag);
        result = sdCommunication->PrepareStartUser(userId);
    }
    EXPECT_EQ(result, E_OK);
    sdCommunication->StopUser(userId);
    sdCommunication->RemoveUser(userId, flag);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_PrepareStartUser_0000";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_StopUser_0000
 * @tc.name: Daemon_communication_StopUser_0000
 * @tc.desc: Test function of StopUser interface.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_StopUser_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_StopUser_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t userId = 106;
    uint32_t flag = 3;
    int32_t result;
    if (sdCommunication != nullptr) {
        sdCommunication->PrepareAddUser(userId, flag);
        sdCommunication->PrepareStartUser(userId);
        result = sdCommunication->StopUser(userId);
    }
    EXPECT_EQ(result, E_OK);
    sdCommunication->RemoveUser(userId, flag);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_StopUser_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_Mount_0000
 * @tc.name: Daemon_communication_Mount_0000
 * @tc.desc: Test function of Mount interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000GGUOT
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_Mount_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_Mount_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    std::string volumeId = "vol-2-1";
    int32_t flag = 1;
    if (sdCommunication != nullptr) {
        result = sdCommunication->Mount(volumeId, flag);
    }
    EXPECT_EQ(result, E_NON_EXIST);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_Mount_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_Unmount_0000
 * @tc.name: Daemon_communication_Unmount_0000
 * @tc.desc: Test function of Unmount interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000GGUOT
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_Unmount_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_Unmount_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    std::string volumeId = "vol-2-2";
    if (sdCommunication != nullptr) {
        result = sdCommunication->Unmount(volumeId);
    }
    EXPECT_EQ(result, E_NON_EXIST);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_Unmount_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_Check_0000
 * @tc.name: Daemon_communication_Check_0000
 * @tc.desc: Test function of Check interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000GGUOT
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_Check_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_Check_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    std::string volumeId = "vol-2-3";
    if (sdCommunication != nullptr) {
        result = sdCommunication->Check(volumeId);
    }
    EXPECT_EQ(result, E_NON_EXIST);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_Check_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_Partition_0000
 * @tc.name: Daemon_communication_Partition_0000
 * @tc.desc: Test function of Partition interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000GGOUT
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_Partition_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_Partition_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    std::string diskId = "disk-2-4";
    int32_t type = 1;
    if (sdCommunication != nullptr) {
        result = sdCommunication->Partition(diskId, type);
    }
    EXPECT_EQ(result, E_NON_EXIST);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_Partition_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_Format_0000
 * @tc.name: Daemon_communication_Format_0000
 * @tc.desc: Test function of Format interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000H09L6
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_Format_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_Format_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    string volumeId = "vol-2-5";
    string fsType = "fsType-1";
    if (sdCommunication != nullptr) {
        result = sdCommunication->Format(volumeId, fsType);
    }
    EXPECT_EQ(result, E_NON_EXIST);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_Format_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_SetVolumeDescription_0000
 * @tc.name: Daemon_communication_SetVolumeDescription_0000
 * @tc.desc: Test function of SetVolumeDescription interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000H09L6
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_SetVolumeDescription_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_SetVolumeDescription_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    string fsUuid = "uuid-1";
    string description = "description-1";
    if (sdCommunication != nullptr) {
        result = sdCommunication->SetVolumeDescription(fsUuid, description);
    }
    EXPECT_EQ(result, E_NON_EXIST);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_SetVolumeDescription_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_DeleteUserKeys_0000
 * @tc.name: Daemon_communication_DeleteUserKeys_0000
 * @tc.desc: Test function of DeleteUserKeys interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000H0F7I
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_DeleteUserKeys_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_DeleteUserKeys_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    int32_t userId = 100;
    if (sdCommunication != nullptr) {
        result = sdCommunication->DeleteUserKeys(userId);
    }
    EXPECT_EQ(result, E_OK);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_DeleteUserKeys_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_UpdateUserAuth_0000
 * @tc.name: Daemon_communication_UpdateUserAuth_0000
 * @tc.desc: Test function of UpdateUserAuth interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000H0FG3
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_UpdateUserAuth_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_UpdateUserAuth_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    uint32_t userId = 200;
    string auth = "auth-1";
    string compSecret = "compSecret-1";
    if (sdCommunication != nullptr) {
        result = sdCommunication->UpdateUserAuth(userId, auth, compSecret);
    }
    EXPECT_EQ(result, E_OK);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_UpdateUserAuth_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_ActiveUserKey_0000
 * @tc.name: Daemon_communication_ActiveUserKey_0000
 * @tc.desc: Test function of ActiveUserKey interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000H0FG3
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_ActiveUserKey_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_ActiveUserKey_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    uint32_t userId = 300;
    string auth = "auth-1";
    string compSecret = "compSecret-1";
    if (sdCommunication != nullptr) {
        result = sdCommunication->ActiveUserKey(userId, auth, compSecret);
    }
    EXPECT_EQ(result, E_OK);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_ActiveUserKey_0000 SUCCESS";
}

/**
 * @tc.number: SUB_STORAGE_Daemon_communication_UpdateKeyContext_0000
 * @tc.name: Daemon_communication_UpdateKeyContext_0000
 * @tc.desc: Test function of UpdateKeyContext interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: AR000H0F7I
 */
HWTEST_F(StorageDaemonCommunicationTest, Daemon_communication_UpdateKeyContext_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-begin Daemon_communication_UpdateKeyContext_0000 SUCCESS";
    std::shared_ptr<StorageDaemonCommunication> sdCommunication =
        DelayedSingleton<StorageDaemonCommunication>::GetInstance();
    int32_t result;
    uint32_t userId = 300;
    if (sdCommunication != nullptr) {
        result = sdCommunication->UpdateKeyContext(userId);
    }
    EXPECT_EQ(result, E_OK);
    GTEST_LOG_(INFO) << "StorageDaemonCommunicationTest-end Daemon_communication_UpdateKeyContext_0000 SUCCESS";
}
} // namespace