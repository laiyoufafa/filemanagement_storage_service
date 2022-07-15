/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include <gtest/gtest.h>
#include "gtest/gtest-message.h"
#include "gtest/gtest-test-part.h"
#include "gtest/hwext/gtest-ext.h"
#include "gtest/hwext/gtest-tag.h"
#include "file_sharing/acl.h"

using namespace testing::ext;
using namespace OHOS::StorageDaemon;

namespace {
class AclTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void AclTest::SetUpTestCase(void)
{
    // input testsuit setup step，setup invoked before all testcases
}

void AclTest::TearDownTestCase(void)
{
    // input testsuit teardown step，teardown invoked after all testcases
}

void AclTest::SetUp(void)
{
    // input testcase setup step，setup invoked before each testcases
}

void AclTest::TearDown(void)
{
    // input testcase teardown step，teardown invoked after each testcases
}

/**
 * @tc.name: acl_entry_valid_test
 * @tc.desc: Verify that AclXattrEntry::IsValid() works as expected.
 * @tc.type: FUNC
 * @tc.require: AR000H09ML
 */
HWTEST_F(AclTest, acl_entry_valid_test, TestSize.Level1)
{
    AclXattrEntry entry {
        .tag = ACL_TAG::UNDEFINED,
        .perm = 6,
        .id = 0,
    };
    EXPECT_TRUE(!entry.IsValid());

    entry = {
        .tag = ACL_TAG::USER,
        .perm = 5,
        .id = ACL_UNDEFINED_ID,
    };
    EXPECT_TRUE(!entry.IsValid());
    entry.id = 0;
    EXPECT_TRUE(entry.IsValid());

    entry = {
        .tag = ACL_TAG::GROUP,
        .perm = 4,
        .id = ACL_UNDEFINED_ID,
    };
    EXPECT_TRUE(!entry.IsValid());
    entry.id = 0;
    EXPECT_TRUE(entry.IsValid());
}

/**
 * @tc.name: acl_empty_test
 * @tc.desc: Verify that Acl::IsEmpty() works as expected.
 * @tc.type: FUNC
 * @tc.require: AR000H09ML
 */
HWTEST_F(AclTest, acl_empty_test, TestSize.Level1)
{
    Acl acl;
    EXPECT_TRUE(acl.IsEmpty());

    ACL_PERM perm;
    perm.SetR();
    perm.SetE();

    /* failed insertion */
    int rc = acl.InsertEntry(
        { .tag = ACL_TAG::UNDEFINED,
          .perm = perm,
          .id = 0, }
    );
    EXPECT_TRUE(rc != 0 && acl.IsEmpty());

    /* successful insertion */
    rc = acl.InsertEntry(
        { .tag = ACL_TAG::USER,
          .perm = perm,
          .id = 1, }
    );
    EXPECT_TRUE(rc == 0 && !acl.IsEmpty());
}

/**
 * @tc.name: acl_valid_test
 * @tc.desc: Verify that Acl::IsValid() works as expected.
 * @tc.type: FUNC
 * @tc.require: AR000H09ML
 */
HWTEST_F(AclTest, acl_valid_test, TestSize.Level1)
{
    Acl acl;
    EXPECT_TRUE(!acl.IsValid());

    /* valid only when all necessary entries are added */
    int rc;
    rc = acl.InsertEntry(
        { .tag = ACL_TAG::USER_OBJ,
          .perm = 6,
          .id = ACL_UNDEFINED_ID, }
    );
    EXPECT_TRUE(rc == 0 && !acl.IsValid()); // invalid
    rc = acl.InsertEntry(
        { .tag = ACL_TAG::GROUP_OBJ,
          .perm = 6,
          .id = ACL_UNDEFINED_ID, }
    );
    EXPECT_TRUE(rc == 0 && !acl.IsValid()); // still invalid
    rc = acl.InsertEntry(
        { .tag = ACL_TAG::OTHER,
          .perm = 0,
          .id = ACL_UNDEFINED_ID, }
    );
    EXPECT_TRUE(rc == 0 && acl.IsValid()); // valid!

    /* still valid after inserting optional entries */
    rc = acl.InsertEntry(
        { .tag = ACL_TAG::USER,
          .perm = 4,
          .id = 0, }
    );
    EXPECT_TRUE(rc == 0 && acl.IsValid());
}

/**
 * @tc.name: acl_insert_test
 * @tc.desc: Verify that Acl::InsertEntry() works as expected.
 * @tc.type: FUNC
 * @tc.require: AR000H09ML
 */
HWTEST_F(AclTest, acl_insert_test, TestSize.Level1)
{
    Acl acl;
    ACL_PERM perm;
    perm.SetR();
    perm.SetE();

    /* failed insertion */
    int rc = acl.InsertEntry(
            { .tag = ACL_TAG::UNDEFINED,
              .perm = perm,
              .id = 0, }
            );
    EXPECT_TRUE(rc != 0);

    /*
     * successful insertions
     * first 99 operations should succeed
     */
    constexpr size_t ENTRIES_MAX_NUM = 100; // check Acl::ENTRIES_MAX_NUM
    for (int i = 1; i <= (ENTRIES_MAX_NUM - 1); ++i) {
        rc = acl.InsertEntry(
                { .tag = ACL_TAG::USER,
                  .perm = perm,
                  .id = i, }
                );
        EXPECT_TRUE(rc == 0);
    }
    /* the last one should fail */
    rc = acl.InsertEntry(
            { .tag = ACL_TAG::USER,
              .perm = perm,
              .id = ENTRIES_MAX_NUM, }
            );
    EXPECT_TRUE(rc != 0);
}
}
