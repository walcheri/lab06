#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>
#include "Account.h"
#include "Transaction.h"

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

    EXPECT_CALL(account, GetBalance()).Times(AtLeast(1));
    account.GetBalance();
    
    EXPECT_CALL(account, Lock()).Times(AtLeast(1));
    account.Lock();
    
    EXPECT_CALL(account, ChangeBalance(1)).Times(AtLeast(1));
    account.ChangeBalance(1);
    
    EXPECT_CALL(account, Unlock()).Times(AtLeast(1));
    account.Unlock();
}

TEST(TransactionTest, MockSaveToDatabaseTest) {
    Account from(1, 10000);
    Account to(2, 10000);
    MockTransaction transaction;
    
    EXPECT_CALL(transaction, SaveToDataBase(from, to, 1999)).Times(AtLeast(1));
    transaction.Make(from, to, 1999);
}

TEST(AccountTest, RealMethodsBehavior) {
    Account account(1, 1000);

    ASSERT_EQ(1000, account.GetBalance());

    account.Lock();
    account.ChangeBalance(2000);
    account.Unlock();
    
    EXPECT_EQ(3000, account.GetBalance());
    
    ASSERT_THROW(account.ChangeBalance(1), std::runtime_error);
    EXPECT_EQ(3000, account.GetBalance()); 
}


TEST(TransactionTest, RealMethodsBehavior) {
    Account acc1(1, 10000);
    Account acc2(2, 10000);
    Transaction defaultFeeTransaction;
    Transaction customFeeTransaction;
    customFeeTransaction.set_fee(500);
    

    ASSERT_THROW(defaultFeeTransaction.Make(acc1, acc1, 100), std::logic_error);
    ASSERT_THROW(defaultFeeTransaction.Make(acc1, acc2, -100), std::invalid_argument);
    ASSERT_THROW(defaultFeeTransaction.Make(acc1, acc2, 0), std::logic_error);

    EXPECT_FALSE(customFeeTransaction.Make(acc1, acc2, 200));
    

    defaultFeeTransaction.Make(acc1, acc2, 1999);
    EXPECT_EQ(8000, acc1.GetBalance());
    EXPECT_EQ(11999, acc2.GetBalance());
}
