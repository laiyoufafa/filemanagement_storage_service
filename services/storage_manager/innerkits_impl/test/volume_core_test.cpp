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

#include "volume_core.h"

namespace {
using namespace std;
using namespace OHOS;
using namespace StorageManager;
class VolumeCoreTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: SUB_STORAGE_Volume_core_Get_0000
 * @tc.name: Volume_core_Get_0000
 * @tc.desc: Test function of Get interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000GGUPF
 */
HWTEST_F(VolumeCoreTest, Volume_core_Get_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "VolumeCoreTest-begin Volume_core_Get_0000";
    std::string id = "456";
    int type = 2;
    std::string diskId = "123";
    int32_t state = UNMOUNTED;
    VolumeCore volumecore(id,type,diskId,state);

    auto result1 = volumecore.GetId();
    EXPECT_EQ(result1, id);
    auto result2 = volumecore.GetType();
    EXPECT_EQ(result2, type);
    auto result3 = volumecore.GetDiskId();
    EXPECT_EQ(result3, diskId);
    auto result4 = volumecore.GetState();
    EXPECT_EQ(result4, state);
    volumecore.SetState(state);
    GTEST_LOG_(INFO) << "VolumeCoreTest-end Volume_core_Get_0000";
}

/**
 * @tc.number: SUB_STORAGE_Volume_core_Marshalling_0000
 * @tc.name: Volume_core_Marshalling_0000
 * @tc.desc: Test function of Marshalling interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000GGUPF
 */
HWTEST_F(VolumeCoreTest, Volume_core_Marshalling_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "VolumeCoreTest-begin Volume_core_Marshalling_0000";
    std::string id = "456";
    int type = 2;
    std::string diskId = "123";
    int32_t state = UNMOUNTED;
    VolumeCore volumecore(id,type,diskId,state);
    Parcel parcel;
    auto result = volumecore.Marshalling(parcel);
    GTEST_LOG_(INFO) << parcel.ReadString();
    GTEST_LOG_(INFO) << parcel.ReadInt32();
    GTEST_LOG_(INFO) << parcel.ReadString();
    GTEST_LOG_(INFO) << parcel.ReadInt32();
    GTEST_LOG_(INFO) << parcel.ReadBool();
    EXPECT_EQ(result, true);
    GTEST_LOG_(INFO) << "VolumeCoreTest-end Volume_core_Marshalling_0000";
}

/**
 * @tc.number: SUB_STORAGE_Volume_core_Unmarshalling_0000
 * @tc.name: Volume_core_Unmarshalling_0000
 * @tc.desc: Test function of Unmarshalling interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000GGUPF
 */
HWTEST_F(VolumeCoreTest, Volume_core_Unmarshalling_0000, testing::ext::TestSize.Level1)
{
    GTEST_LOG_(INFO) << "VolumeCoreTest-begin Volume_core_Unmarshalling_0000";
    std::string id = "456";
    int type = 2;
    std::string diskId = "123";
    int32_t state = UNMOUNTED;
    bool errorFlag = false;
    Parcel parcel;
    parcel.WriteString(id);
    parcel.WriteInt32(type);
    parcel.WriteString(diskId);
    parcel.WriteInt32(state);
    parcel.WriteBool(errorFlag);
    auto result = std::make_unique<VolumeCore>();
    auto volumecore = std::make_unique<VolumeCore>();
    result = volumecore->Unmarshalling(parcel);
    EXPECT_NE(result, nullptr);
    GTEST_LOG_(INFO) << "VolumeCoreTest-end Volume_core_Unmarshalling_0000";
}
}