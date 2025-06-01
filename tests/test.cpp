#include <gtest/gtest.h>
#include <stdexcept>
#include "Account.h"
#include "Transaction.h"
#include <gmock/gmock.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Ref;

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};

class MockTransaction : public Transaction {
public:
    MockTransaction() : Transaction() {}
    MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

TEST(AccountTest, MockMethodsTest) {
    MockAccount account(1, 1000);
    
    EXPECT_CALL(account, GetBalance()).WillOnce(Return(1000));
    EXPECT_EQ(1000, account.GetBalance());
    
    EXPECT_CALL(account, Lock()).Times(1);
    account.Lock();
    
    EXPECT_CALL(account, ChangeBalance(100)).Times(1);
    account.ChangeBalance(100);
    
    EXPECT_CALL(account, Unlock()).Times(1);
    account.Unlock();
}

TEST(TransactionTest, MockSaveToDatabaseTest) {
    MockAccount from(1, 10000);
    MockAccount to(2, 10000);
    MockTransaction transaction;
    
    EXPECT_CALL(transaction, SaveToDataBase(Ref(from), Ref(to), 1999))
        .Times(1);
    transaction.Make(from, to, 1999);
}

TEST(AccountTest, RealMethodsBehavior) {
    Account account(1, 1000);
    
    EXPECT_EQ(1000, account.GetBalance());
    
    account.Lock();
    account.ChangeBalance(2000);
    account.Unlock();
    
    EXPECT_EQ(3000, account.GetBalance());
    
    EXPECT_THROW(account.ChangeBalance(1), std::runtime_error);
    EXPECT_EQ(3000, account.GetBalance());
}

