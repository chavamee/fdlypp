#include "fdly.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

static string g_APIKey;
static string g_UserID;

void printUsage()
{
    cout << "Usage:" << endl << endl;
    cout << "--api-key <feedly api key>" << endl;
    cout << "--user-id <feedly user id>" << endl;
}

class APIAccessTests : public testing::Test {
    public:
        APIAccessTests() :
            m_user {g_UserID, g_APIKey},
            m_connection (m_user)
        {
        }

        virtual ~APIAccessTests()
        {
        }

        virtual void SetUp()
        {
            ASSERT_TRUE(m_connection.IsAvailable());
            ASSERT_TRUE(m_connection.CanAuthenticate());
        }

        virtual void TearDown()
        {
        }

        Fdly::User m_user;
        Fdly m_connection;
};

TEST_F(APIAccessTests, GetSubscriptions)
{
}

TEST_F(APIAccessTests, GetCategories)
{
    auto categories = m_connection.Ctgs();

    EXPECT_GT(categories.size(), 0);

    for (const auto& ctg : categories) {
        ASSERT_FALSE(ctg.Label.empty());
        ASSERT_FALSE(ctg.ID.empty());
        cout << ctg.Label << " " << ctg.ID << endl;
    }
}

TEST_F(APIAccessTests, GetEntries)
{
    auto categories = m_connection.Ctgs();

    EXPECT_GT(categories.size(), 0);

    for (const auto& ctg : categories) {
        ASSERT_FALSE(ctg.Label.empty());
        ASSERT_FALSE(ctg.ID.empty());

        cout << ctg.Label << endl << endl;

        auto entries = m_connection.Entries(ctg);
        for (const auto& entry : entries) {
            ASSERT_FALSE(entry.Title.empty());
            ASSERT_FALSE(entry.ID.empty());
            cout << "   " << entry.Title << endl << "   " <<  entry.ID << endl << endl;
        }

        cout << endl;
    }

}

TEST_F(APIAccessTests, AddedSubscriptionWithoutCategories)
{
    Fdly::Feed feed{ "Design Milk", "http://feeds.feedburner.com/design-milk" };

    ASSERT_NO_THROW(m_connection.AddSubscription(feed));
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "--api-key" ) == 0) {
          i++;
          if (i < argc) {
              g_APIKey = argv[i];
          }
      } else if (strcmp(argv[i], "--user-id") == 0) {
          i++;
          if (i < argc) {
              g_UserID = argv[i];
          }
      }
  }

  if (g_APIKey.empty() || g_UserID.empty()) {
      printUsage();
      return 1;
  }

  return RUN_ALL_TESTS();
}
