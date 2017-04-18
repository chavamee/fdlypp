#include "fdly.hpp"
#include <gtest/gtest.h>

using namespace std;

static string g_APIKey;
static string g_UserID;

void printUsage()
{
    cout << "Usage:" << endl << endl;
    cout << "--api-key <feedly api key>" << endl;
    cout << "--user-id <feedly user id>" << endl;
}

class ConnectionTests : public testing::Test {
    public:
        ConnectionTests() :
            m_user {g_APIKey, g_UserID},
            m_connection (m_user)
        {
        }

        virtual ~ConnectionTests()
        {
        }

        virtual void SetUp()
        {
        }

        virtual void TearDown()
        {
        }

        Fdly::User m_user;
        Fdly m_connection;
};

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
