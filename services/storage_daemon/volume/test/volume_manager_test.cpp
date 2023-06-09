/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include <gtest/gtest.h>

#include "external_volume_info.h"
#include "storage_service_errno.h"
#include "storage_service_log.h"
#include "volume/volume_manager.h"

namespace OHOS {
namespace StorageDaemon {
using namespace testing::ext;

class VolumeManagerTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase(void) {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: Storage_Service_VolumeManagerTest_Instance_001
 * @tc.desc: Verify the Instance function.
 * @tc.type: FUNC
 * @tc.require: SR000GGUOT
 */
HWTEST_F(VolumeManagerTest, Storage_Service_VolumeManagerTest_Instance_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Storage_Service_VolumeManagerTest_Instance_001 start";

    VolumeManager *volumeManager = VolumeManager::Instance();
    ASSERT_TRUE(volumeManager != nullptr);

    GTEST_LOG_(INFO) << "Storage_Service_VolumeManagerTest_Instance_001 end";
}

/**
 * @tc.name: Storage_Service_VolumeManagerTest_Instance_002
 * @tc.desc: Verify the Instance function.
 * @tc.type: FUNC
 * @tc.require: SR000GGUOT
 */
HWTEST_F(VolumeManagerTest, Storage_Service_VolumeManagerTest_Instance_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Storage_Service_VolumeManagerTest_Instance_002 start";

    VolumeManager *volumeManagerFirst = VolumeManager::Instance();
    ASSERT_TRUE(volumeManagerFirst != nullptr);
    VolumeManager *volumeManagerSecond = VolumeManager::Instance();
    ASSERT_TRUE(volumeManagerSecond != nullptr);

    ASSERT_TRUE(volumeManagerFirst == volumeManagerSecond);
    GTEST_LOG_(INFO) << "Storage_Service_VolumeManagerTest_Instance_002 end";
}
} // STORAGE_DAEMON
} // OHOS