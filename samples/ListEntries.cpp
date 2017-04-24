#include "fdly.hpp"
#include <string>

using namespace std;

void print_usage()
{
    cout << "Usage:" << endl;
    cout << "   --api-key <API Key>" << endl;
    cout << "   --user-id <User ID>" << endl;
}

int main (int argc, char** argv)
{
    string apiKey;
    string userID;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--api-key" ) == 0) {
            i++;
            if (i < argc) {
                apiKey = argv[i];
            }
        } else if (strcmp(argv[i], "--user-id") == 0) {
            i++;
            if (i < argc) {
                userID = argv[i];
            }
        }
    }

    if (userID.empty() || apiKey.empty()) {
        print_usage();
        return 1;
    }

    Fdly::User user {userID, apiKey};
    Fdly fdly {user};

    auto categories = fdly.GetCategories();
    for (const auto& ctg : categories) {
        cout << "==== " << ctg.Label << "(" << ctg.ID << ")" << " ====" << endl;

        Fdly::Entries entries = fdly.GetEntries(ctg);
        for (const auto& entry : entries) {
            cout << "   " << entry.Title << endl << "   " << entry.ID << endl << endl;
        }

        cout << endl;
    }

    return 0;
}
