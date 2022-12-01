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

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ipc/storage_manager_proxy.h"
#include "ipc/storage_manager_stub.h"
#include "storage_manager_stub_mock.h"
#include "get_self_permissions.h"

#include "storage_service_errno.h"
#include "storage_service_log.h"

namespace OHOS {
namespace StorageManager {
using namespace testing::ext;

namespace {
    const int ERROR_CODE = 99999;
    int g_code[] = {
        IStorageManager::PREPARE_ADD_USER,
        IStorageManager::REMOVE_USER,
        IStorageManager::PREPARE_START_USER,
        IStorageManager::STOP_USER,
        IStorageManager::NOTIFY_VOLUME_CREATED,
        IStorageManager::NOTIFY_VOLUME_MOUNTED,
        IStorageManager::NOTIFY_VOLUME_DESTROYED,
        IStorageManager::MOUNT,
        IStorageManager::UNMOUNT,
        IStorageManager::NOTIFY_DISK_CREATED,
        IStorageManager::NOTIFY_DISK_DESTROYED,
        IStorageManager::PARTITION,
        IStorageManager::CREATE_USER_KEYS,
        IStorageManager::DELETE_USER_KEYS,
        IStorageManager::UPDATE_USER_AUTH,
        IStorageManager::ACTIVE_USER_KEY,
        IStorageManager::INACTIVE_USER_KEY,
        IStorageManager::UPDATE_KEY_CONTEXT,
        IStorageManager::GET_VOL_BY_UUID,
        IStorageManager::GET_VOL_BY_ID,
        IStorageManager::SET_VOL_DESC,
        IStorageManager::FORMAT,
        IStorageManager::GET_DISK_BY_ID,
    };
    int g_codes[] = {
        IStorageManager::GET_TOTAL,
        IStorageManager::GET_FREE,
        IStorageManager::GET_SYSTEM_SIZE,
        IStorageManager::GET_TOTAL_SIZE,
        IStorageManager::GET_FREE_SIZE,
        IStorageManager::GET_BUNDLE_STATUS,
        IStorageManager::GET_CURR_BUNDLE_STATS,
        IStorageManager::GET_CURR_USER_STATS,
        IStorageManager::GET_USER_STATS,
    };
}

class StorageManagerStubTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase(void) {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: Storage_Manager_StorageManagerStubTest_OnRemoteRequest_001
 * @tc.desc: Verify the OnRemoteRequest function with error descriptor.
 * @tc.type: FUNC
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageManagerStubTest, Storage_Manager_StorageManagerStubTest_OnRemoteRequest_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_001 start";

    std::vector<string> perms;
    perms.push_back("ohos.permission.STORAGE_MANAGER");
    perms.push_back("ohos.permission.MOUNT_UNMOUNT_MANAGER");
    perms.push_back("ohos.permission.MOUNT_FORMAT_MANAGER");
    uint64_t tokenId = 0;
    PermissionUtilsTest::SetAccessTokenPermission("StorageManagerPxyTest", perms, tokenId);
    ASSERT_TRUE(tokenId != 0);

    StorageManagerStubMock mock;

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);
    bool bRet = data.WriteInterfaceToken(u"error descriptor");
    EXPECT_TRUE(bRet) << "write token error";

    int32_t ret = mock.OnRemoteRequest(IStorageManager::PREPARE_ADD_USER, data, reply, option);
    EXPECT_TRUE(ret == E_PERMISSION_DENIED) << "descriptor error";

    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_001 end";
}

/**
 * @tc.name: Storage_Manager_StorageManagerStubTest_OnRemoteRequest_002
 * @tc.desc: Verify the OnRemoteRequest function with error code.
 * @tc.type: FUNC
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageManagerStubTest, Storage_Manager_StorageManagerStubTest_OnRemoteRequest_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_002 start";

    StorageManagerStubMock mock;

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    bool bRet = data.WriteInterfaceToken(StorageManagerProxy::GetDescriptor());
    EXPECT_TRUE(bRet) << "write token error";

    int32_t ret = mock.OnRemoteRequest(ERROR_CODE, data, reply, option);
    EXPECT_TRUE(ret != E_OK) << "request code error";

    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_002 end";
}

/**
 * @tc.name: Storage_Manager_StorageManagerStubTest_OnRemoteRequest_003
 * @tc.desc: Verify the OnRemoteRequest function.
 * @tc.type: FUNC
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageManagerStubTest, Storage_Manager_StorageManagerStubTest_OnRemoteRequest_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_003 start";

    StorageManagerStubMock mock;

    EXPECT_CALL(mock, PrepareAddUser(testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, RemoveUser(testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, PrepareStartUser(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, StopUser(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, NotifyVolumeCreated(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, NotifyVolumeMounted(testing::_, testing::_, testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, NotifyVolumeDestroyed(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, Mount(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, Unmount(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, NotifyDiskCreated(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, NotifyDiskDestroyed(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, Partition(testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, GetVolumeByUuid(testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, GetVolumeById(testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, SetVolumeDescription(testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, Format(testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, GetDiskById(testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, GenerateUserKeys(testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, DeleteUserKeys(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, UpdateUserAuth(testing::_, testing::_, testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, ActiveUserKey(testing::_, testing::_, testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, InactiveUserKey(testing::_)).WillOnce(testing::Return(E_OK));
    EXPECT_CALL(mock, UpdateKeyContext(testing::_)).WillOnce(testing::Return(E_OK));

    for (auto c : g_code) {
        MessageParcel data;
        MessageParcel reply;
        MessageOption option(MessageOption::TF_SYNC);
        bool bRet = data.WriteInterfaceToken(StorageManagerProxy::GetDescriptor());
        EXPECT_TRUE(bRet) << "write token error";
        int32_t ret = mock.OnRemoteRequest(c, data, reply, option);
        EXPECT_TRUE(ret == E_OK);
        EXPECT_TRUE(reply.ReadInt32() == E_OK);
    }

    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_003 end";
}

/**
 * @tc.name: Storage_Manager_StorageManagerStubTest_OnRemoteRequest_004
 * @tc.desc: Verify the OnRemoteRequest function.
 * @tc.type: FUNC
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageManagerStubTest, Storage_Manager_StorageManagerStubTest_OnRemoteRequest_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_004 start";

    StorageManagerStubMock mock;

    int64_t bytes = 1024;
    BundleStats bs(100, 101, 102);
    StorageStats ss(100, 101, 102, 103, 104, 105);
    EXPECT_CALL(mock, GetFreeSizeOfVolume(testing::_)).WillOnce(testing::Return(bytes));
    EXPECT_CALL(mock, GetTotalSizeOfVolume(testing::_)).WillOnce(testing::Return(bytes));
    EXPECT_CALL(mock, GetSystemSize()).WillOnce(testing::Return(bytes));
    EXPECT_CALL(mock, GetTotalSize()).WillOnce(testing::Return(bytes));
    EXPECT_CALL(mock, GetFreeSize()).WillOnce(testing::Return(bytes));
    EXPECT_CALL(mock, GetBundleStats(testing::_)).WillOnce(testing::Return(bs));
    EXPECT_CALL(mock, GetCurrentBundleStats()).WillOnce(testing::Return(bs));
    EXPECT_CALL(mock, GetUserStorageStats()).WillOnce(testing::Return(ss));
    EXPECT_CALL(mock, GetUserStorageStats(testing::_)).WillOnce(testing::Return(ss));

    for (auto c : g_codes) {
        MessageParcel data;
        MessageParcel reply;
        MessageOption option(MessageOption::TF_SYNC);
        bool bRet = data.WriteInterfaceToken(StorageManagerProxy::GetDescriptor());
        EXPECT_TRUE(bRet) << "write token error";
        int32_t ret = mock.OnRemoteRequest(c, data, reply, option);
        EXPECT_TRUE(ret == E_OK);
        if (c == IStorageManager::GET_BUNDLE_STATUS || c == IStorageManager::GET_CURR_BUNDLE_STATS) {
            auto res = *BundleStats::Unmarshalling(reply);
            EXPECT_EQ(res.appSize_, bs.appSize_);
            EXPECT_EQ(res.cacheSize_, bs.cacheSize_);
            EXPECT_EQ(res.dataSize_, bs.dataSize_);
        } else if (c == IStorageManager::GET_CURR_USER_STATS || c == IStorageManager::GET_USER_STATS) {
            auto res = *StorageStats::Unmarshalling(reply);
            EXPECT_EQ(res.total_, ss.total_);
            EXPECT_EQ(res.audio_, ss.audio_);
            EXPECT_EQ(res.video_, ss.video_);
            EXPECT_EQ(res.image_, ss.image_);
            EXPECT_EQ(res.file_, ss.file_);
            EXPECT_EQ(res.app_, ss.app_);
        } else {
            EXPECT_TRUE(reply.ReadInt32() == bytes);
        }
    }

    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_004 end";
}

/**
 * @tc.name: Storage_Manager_StorageManagerStubTest_OnRemoteRequest_005
 * @tc.desc: Verify the OnRemoteRequest function.
 * @tc.type: FUNC
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageManagerStubTest, Storage_Manager_StorageManagerStubTest_OnRemoteRequest_005, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_005 start";

    StorageManagerStubMock mock;

    VolumeExternal ve;
    ve.SetFsType(EXFAT);
    std::vector<VolumeExternal> vec1 = {ve};
    std::string diskId = "disk-1-1";
    int64_t size = 0;
    std::string sysPath = "/dev/block/sda";
    std::string vendor = "unknown";
    int32_t flag = 0;
    Disk disk(diskId, size, sysPath, vendor, flag);
    std::vector<Disk> vec2 = {disk};
    EXPECT_CALL(mock, GetAllVolumes()).WillOnce(testing::Return(vec1));
    EXPECT_CALL(mock, GetAllDisks()).WillOnce(testing::Return(vec2));
    
    MessageParcel data1;
    MessageParcel reply1;
    MessageOption option1(MessageOption::TF_SYNC);
    bool bRet1 = data1.WriteInterfaceToken(StorageManagerProxy::GetDescriptor());
    EXPECT_TRUE(bRet1) << "write token error";
    int32_t ret1 = mock.OnRemoteRequest(IStorageManager::GET_ALL_VOLUMES, data1, reply1, option1);
    EXPECT_TRUE(ret1 == E_OK);
    uint32_t size1 = reply1.ReadUint32();
    EXPECT_EQ(size1, 1);
    auto res1 = *VolumeExternal::Unmarshalling(reply1);
    EXPECT_EQ(ve.GetFsType(), res1.GetFsType());

    MessageParcel data2;
    MessageParcel reply2;
    MessageOption option2(MessageOption::TF_SYNC);
    bool bRet2 = data2.WriteInterfaceToken(StorageManagerProxy::GetDescriptor());
    EXPECT_TRUE(bRet2) << "write token error";
    int32_t ret2 = mock.OnRemoteRequest(IStorageManager::GET_ALL_DISKS, data2, reply2, option2);
    EXPECT_TRUE(ret2 == E_OK);
    uint32_t size2 = reply2.ReadUint32();
    EXPECT_EQ(size2, 1);
    auto res2 = *Disk::Unmarshalling(reply2);
    EXPECT_EQ(disk.GetDiskId(), res2.GetDiskId());

    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_005 end";
}

/**
 * @tc.name: Storage_Manager_StorageManagerStubTest_OnRemoteRequest_006
 * @tc.desc: Verify the OnRemoteRequest function.
 * @tc.type: FUNC
 * @tc.require: AR000GK4HB
 */
HWTEST_F(StorageManagerStubTest, Storage_Manager_StorageManagerStubTest_OnRemoteRequest_006, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_006 start";

    StorageManagerStubMock mock;

    VolumeExternal ve;
    ve.SetFsType(EXFAT);
    std::vector<VolumeExternal> vec1;
    std::vector<Disk> vec2;
    EXPECT_CALL(mock, GetAllVolumes()).WillOnce(testing::Return(vec1));
    EXPECT_CALL(mock, GetAllDisks()).WillOnce(testing::Return(vec2));
    
    MessageParcel data1;
    MessageParcel reply1;
    MessageOption option1(MessageOption::TF_SYNC);
    bool bRet1 = data1.WriteInterfaceToken(StorageManagerProxy::GetDescriptor());
    EXPECT_TRUE(bRet1) << "write token error";
    int32_t ret1 = mock.OnRemoteRequest(IStorageManager::GET_ALL_VOLUMES, data1, reply1, option1);
    EXPECT_TRUE(ret1 == E_OK);
    uint32_t size1 = reply1.ReadUint32();
    EXPECT_EQ(size1, 0);

    MessageParcel data2;
    MessageParcel reply2;
    MessageOption option2(MessageOption::TF_SYNC);
    bool bRet2 = data2.WriteInterfaceToken(StorageManagerProxy::GetDescriptor());
    EXPECT_TRUE(bRet2) << "write token error";
    int32_t ret2 = mock.OnRemoteRequest(IStorageManager::GET_ALL_DISKS, data2, reply2, option2);
    EXPECT_TRUE(ret2 == E_OK);
    uint32_t size2 = reply2.ReadUint32();
    EXPECT_EQ(size2, 0);

    GTEST_LOG_(INFO) << "Storage_Manager_StorageManagerStubTest_OnRemoteRequest_006 end";
}
} // STORAGE_MANAGER
} // OHOS
