/**
 * @file
 * Contains the Feedly class which interaces with the Feedly Web API and defines
 * various types to facilitate usage.
 */
#ifndef FDLY_HEADER_SRC_H
#define FDLY_HEADER_SRC_H

#include "json.hpp"
#include <cpr/cpr.h>

#include <stdexcept>
#include <string>
#include <map>
#include <vector>

using json = nlohmann::json;

/**
 * @class Interface with the Feedly API
 */
class Fdly {
    public:
        struct User {
            std::string ID;
            std::string AuthToken;
        };

        struct Entry {
            /**
             * Actions that can be applied to entries.
             */
            enum class Action {
                READ,
                UNREAD
            };

            std::string content;
            std::string title;
            std::string id;
            std::string originURL;
            std::string originTitle;

            Entry(
                    std::string p_content,
                    std::string p_title,
                    std::string p_id,
                    std::string p_originURL,
                    std::string p_originTitle) :
                content(p_content),
                title(p_title),
                id(p_id),
                originURL(p_originURL),
                originTitle(p_originTitle)
            {
            }

            Entry(const Entry& other) :
                content(other.content),
                title(other.title),
                id(other.id),
                originURL(other.originURL),
                originTitle(other.originTitle)
            {
            }

            Entry(Entry&& other) :
                content(other.content),
                title(other.title),
                id(other.id),
                originURL(other.originURL),
                originTitle(other.originTitle)
            {
            }

            bool operator==(const Entry& rhs)
            {
                return id == rhs.id;
            }
        };

        struct Category {
            enum class Action {
                READ,
                UNREAD
            };

            const std::string ID;


            unsigned int Unread()
            {
                // TODO
            }

            unsigned int Read()
            {
                // TODO
            }
        };

        class Feed {
            public:
            enum class Action {
                READ,
                UNREAD
            };

            unsigned int Unread()
            {
                // TODO
            }

            unsigned int Read()
            {
                // TODO
            }
        };

        /*
         * Default constructor is not allowed
         */
        Fdly() = delete;

        /**
         * Construct a Feedly wrapper with the given user.
         *
         * @param user  User to accesss Feedly API with
         */
        Fdly(User& user, std::string apiVersion = APIVersion3) :
            m_user(user),
            m_effectiveAPIVersion(apiVersion),
            m_rootUrl(Fdly::FeedlyUrl + "/" + apiVersion)
        {
        }


        /**
         * Ensure that we can Authenticate with the Feedly API.
         *
         * @return - true if Authentication was sucessful.
         *         - false if Authentication failed.
         */
        bool CanAuthenticate()
        {
            auto r = cpr::Get(cpr::Url{m_rootUrl + "/profile"},
                              cpr::Header{{"Authorization", "OAuth " + m_user.AuthToken}});

            if (r.status_code == 200) {
                return true;
            }

            return false;
        }


        /**
         * Return the available categories.
         *
         * @return a map with the category label as a key and the category id as a value
         */
        std::map<std::string, std::string> Categories() const
        {
            auto r = cpr::Get(cpr::Url{m_rootUrl + "/categories"},
                              cpr::Header{{"Authorization", "OAuth " + m_user.AuthToken}});


            if (r.status_code not_eq 200) {
                std::string error = "Could not get categories: " + std::to_string(r.status_code);
                throw std::runtime_error(error.c_str());
            }

            auto jsonResp = json::parse(r.text);

            std::map<std::string, std::string> categories;
            for (auto& ctg : jsonResp) {
                const std::string label = ctg["label"];
                const std::string id = ctg["id"];
                categories[label] = id;
            }

            return categories;
        }


        /**
         * Mark category with an action.
         *
         * @param action      the action to apply
         */
        void MarkCategoryAs(std::string categoryID, Category::Action action, const std::string& lastReadEntryId = "") const
        {
            if (categoryID.empty()) {
                throw std::runtime_error("Category ID cannot be empty");
            }

            json j;
            j["type"] = "categories";
            j["categoryIds"] = {categoryID};

            if (not lastReadEntryId.empty()) {
                j["lastReadEntryId"] = lastReadEntryId;
            }
            j["action"] = ActionToString(action);

            auto r = cpr::Post(cpr::Url{m_rootUrl + "/markers"},
                    cpr::Header{{"Authorization", "OAuth " + m_user.AuthToken},
                    {"Content-Type", "application/json"}},
                    cpr::Body{j.dump()});

            if (r.status_code not_eq 200) {
                std::string error = "Could not mark category with " + ActionToString(action) + ": " + std::to_string(r.status_code);
                throw std::runtime_error(error.c_str());
            }
        }


        /**
         * Mark entry with an action.
         *
         * @param action  the action to apply
         */
        void MarkEntryAs(Entry::Action action)
        {
            // TODO
        }

        /**
         * Mark a multiple entries with an action.
         *
         * @param entryId  list of IDs for the entries to apply the action to
         * @param action  the action to apply
         */
        void MarkEntriesWithAction(const std::vector<std::string>& entryIds, Entry::Action action)
        {
            if (entryIds.size() > 0) {
                json j;
                j["type"] = "entries";

                for (auto& id : entryIds) {
                    j["entryIds"].push_back(id);
                }

                j["action"] = ActionToString(action);

                auto r = cpr::Post(cpr::Url{m_rootUrl + "/markers"},
                        cpr::Body{j.dump()},
                        cpr::Header{{"Authorization", "OAuth " + m_user.AuthToken},
                        {"Content-Type", "application/json"}});
                if (r.status_code not_eq 200) {
                    std::string error = "Could not mark entires with " + ActionToString(action) + ": " + std::to_string(r.status_code);
                    throw std::runtime_error(error.c_str());
                }
            }
        }

        /**
         * Subscribe to a feed
         *
         * @param feed       the feed to subscribe to
         * @param title      the title for the new feed
         * @param categories an optional list of category ids that the feed should be added to
         */
        void AddSubscription(const std::string& feed, const std::string& title, const std::vector<std::string> *const categories = nullptr)
        {
            json j;
            j["id"] = "feed/" + feed;
            j["title"] = title;

            if (categories && categories->size() > 0) {
                for (const auto& ctg : *categories) {
                    j["categories"].push_back({{"id", ctg}});
                }
            } else {
                j["categories"] = {};
            }

            auto r = cpr::Post(cpr::Url{m_rootUrl + "/subscription"},
                    cpr::Header{{"Authorization", "OAuth " + m_user.AuthToken}},
                    cpr::Body{j.dump()});

            if (r.status_code not_eq 200) {
                std::string error = "Could not add subscription: " + std::to_string(r.status_code);
                throw std::runtime_error(error.c_str());
            }
        }


        /**
         * Return entries for a specific category.
         *
         * @param categoryId     the category to fetch entries from
         * @param sortByOldest   return the list of entries ordered by oldest
         * @param count          number of entries to fetch
         * @param unreadOnly     fetch only unread entries
         * @param continuationId fetch entries after a specific id
         * @param newerThan      fetch entries newer than timestamp in ms
         *
         * @return a list of entries
         */
        std::vector<Entry> Entries(
                const std::string& categoryId,
                bool sortByOldest = false,
                unsigned int count = 20,
                bool unreadOnly = true,
                std::string continuationId = "",
                unsigned long newerThan = 0
                ) const
        {
            auto params = cpr::Parameters({
                    {"ranked",       sortByOldest ? "oldest" : "newest"},
                    {"unreadOnly",   unreadOnly ? "true" : "false"},
                    {"count",        std::to_string(count)}
                    });

            if (not continuationId.empty()) {
                params.AddParameter(cpr::Parameter{"continuation", continuationId});
            }

            if (newerThan > 0) {
                params.AddParameter(cpr::Parameter{"newerThan", std::to_string(newerThan)});
            }

            if (categoryId == "All") {
                params.AddParameter(cpr::Parameter{"streamId", "user/" + m_user.ID + "/category/global.all"});
            } else if (categoryId == "Uncategorized") {
                params.AddParameter(cpr::Parameter{"streamId", "user/" + m_user.ID + "/category/global.uncategorized"});
            } else if (categoryId == "Saved") {
                params.AddParameter(cpr::Parameter{"streamId", "user/" + m_user.ID + "/tag/global.saved"});
            } else {
                params.AddParameter(cpr::Parameter{"streamId", categoryId});
            }

            auto r = cpr::Get(cpr::Url{m_rootUrl + "/streams/contents"},
                    cpr::Header{{"Authorization", "OAuth " + m_user.AuthToken}},
                    params);

            if (r.status_code not_eq 200) {
                std::string error = "Could not get entries: " + std::to_string(r.status_code);
                throw std::runtime_error(error.c_str());
            }

            auto j = json::parse(r.text);

            std::vector<Fdly::Entry> entries;
            for (auto& item : j["items"]) {
                std::string title = item["title"];
                std::string id = item["id"];
                std::string originID = item["originId"];

                std::string content;
                if (item["summary"]["content"].is_string()) {
                    content = item["summary"]["content"];
                }

                std::string originTitle;
                if (item["origin"]["title"].is_string()) {
                    originTitle = item["origin"]["title"];
                }

                entries.emplace_back(
                        content,
                        title,
                        id,
                        originID,
                        originTitle
                        );
            }

            return entries;
        }

        /**
         * Get a list of unread counts
         */
        void UnreadCounts()
        {
            //TODO
        }

        /**
         * Check if Feedly is available
         *
         * @return true if we can reach feedly.com, false otherwise
         */
        static bool IsAvailable();

        static const std::string APIVersion3;

        static const std::string FeedlyUrl;

    private:
        inline std::string ActionToString(Entry::Action action) const
        {
            switch (action) {
                case Entry::Action::READ:
                    return "markAsRead";
                case Entry::Action::UNREAD:
                    return "undoMarkAsRead";
            }

            return "";
        }

        inline std::string ActionToString(Category::Action action) const
        {
            switch (action) {
                case Category::Action::READ:
                    return "markAsRead";
                case Category::Action::UNREAD:
                    return "undoMarkAsRead";
            }

            return "";
        }

        inline std::string ActionToString(Feed::Action action) const
        {
            switch (action) {
                case Feed::Action::READ:
                    return "markAsRead";
                case Feed::Action::UNREAD:
                    return "undoMarkAsRead";
            }

            return "";
        }

        Fdly::User m_user;
        std::string m_effectiveAPIVersion;
        const std::string m_rootUrl;
};

const std::string Fdly::FeedlyUrl = "https://cloud.feedly.com";

const std::string Fdly::APIVersion3 = "v3";

bool Fdly::IsAvailable()
{
    auto r = cpr::Get(cpr::Url{FeedlyUrl});
    if (r.status_code == 200) {
        return true;
    }

    return false;
}
#endif /* ifndef FEEDLY_H_ */
