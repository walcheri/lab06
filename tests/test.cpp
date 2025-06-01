#include <gtest/gtest.h>
#include <stdexcept>
#include "Account.h"
#include "Transaction.h"
#include <gmock/gmock.h>

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
    
    EXPECT_CALL(account, GetBalance()).Times(testing::AtLeast(1));
    account.GetBalance();
    
    EXPECT_CALL(account, Lock()).Times(testing::AtLeast(1));
    account.Lock();
    
    EXPECT_CALL(account, ChangeBalance(1)).Times(testing::AtLeast(1));
    account.ChangeBalance(1);
    
    EXPECT_CALL(account, Unlock()).Times(testing::AtLeast(1));
    account.Unlock();
}

TEST(TransactionTest, MockSaveToDatabaseTest) {
    MockAccount from(1, 10000);
    MockAccount to(2, 10000);
    MockTransaction transaction;
    
    EXPECT_CALL(transaction, SaveToDataBase(testing::_, testing::_, 1999))
        .Times(testing::AtLeast(1));
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

TEST(TransactionTest, RealMethodsBehavior) {
    Account acc1(1, 10000);
    Account acc2(2, 10000);
    Transaction defaultFeeTransaction;
    Transaction customFeeTransaction;
    customFeeTransaction.set_fee(500);
    
    EXPECT_THROW(defaultFeeTransaction.Make(acc1, acc1, 100), std::logic_error);
    EXPECT_THROW(defaultFeeTransaction.Make(acc1, acc2, -100), std::invalid_argument);
    EXPECT_THROW(defaultFeeTransaction.Make(acc1, acc2, 0), std::logic_error);
    
    EXPECT_FALSE(customFeeTransaction.Make(acc1, acc2, 200));
    
    defaultFeeTransaction.Make(acc1, acc2, 1999);
    EXPECT_EQ(8000, acc1.GetBalance());
    EXPECT_EQ(11999, acc2.GetBalance());
}
