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
#include <set>
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

            std::string Content;
            std::string Title;
            std::string ID;
            std::string OriginURL;
            std::string OriginTitle;

            Entry(
                    std::string p_content,
                    std::string p_title,
                    std::string p_id,
                    std::string p_originURL,
                    std::string p_originTitle) :
                Content(p_content),
                Title(p_title),
                ID(p_id),
                OriginURL(p_originURL),
                OriginTitle(p_originTitle)
            {
            }

            Entry(const Entry& other) :
                Content(other.Content),
                Title(other.Title),
                ID(other.ID),
                OriginURL(other.OriginURL),
                OriginTitle(other.OriginTitle)
            {
            }

            Entry(Entry&& other) :
                Content(other.Content),
                Title(other.Title),
                ID(other.ID),
                OriginURL(other.OriginURL),
                OriginTitle(other.OriginTitle)
            {
            }

            inline bool operator==(const Entry& rhs)
            {
                return ID == rhs.ID;
            }

            friend inline bool operator>(const Entry& lhs, const Entry& rhs)
            {
                return lhs.Title.compare(rhs.Title) > 0;
            }

            friend inline bool operator<(const Entry& lhs, const Entry& rhs)
            {
                return lhs.Title.compare(rhs.Title) < 0;
            }
        };

        class Entries {
            public:
                class iterator {
                    public:
                        using value_type = Entry;
                        using difference_type = std::ptrdiff_t;
                        using pointer = Entry*;
                        using reference = Entry&;
                        using iterator_category = std::forward_iterator_tag;

                        iterator(std::vector<Entry>::iterator entriesIter) :
                            m_entriesIter(entriesIter)
                        {
                        }

                        Entry& operator*()
                        {
                            return *m_entriesIter;
                        }

                        const Entry& operator*() const
                        {
                            return *m_entriesIter;
                        }

                        iterator& operator++()
                        {
                            m_entriesIter++;
                            return *this;
                        }

                        bool operator==(const iterator& it) { return m_entriesIter == it.m_entriesIter; }
                        bool operator!=(const iterator& it) { return m_entriesIter != it.m_entriesIter; }

                    private:
                        std::vector<Entry>::iterator m_entriesIter;
                };

                class const_iterator {
                    public:
                        using value_type = const Entry;
                        using difference_type = std::ptrdiff_t;
                        using pointer = const Entry*;
                        using reference = const Entry&;
                        using iterator_category = std::forward_iterator_tag;

                        const_iterator(std::vector<Entry>::const_iterator entriesIter) :
                            m_entriesIter(entriesIter)
                        {
                        }

                        const Entry& operator*() const
                        {
                            return *m_entriesIter;
                        }

                        const_iterator& operator++()
                        {
                            m_entriesIter++;
                            return *this;
                        }

                        bool operator==(const const_iterator& it) { return m_entriesIter == it.m_entriesIter; }
                        bool operator!=(const const_iterator& it) { return m_entriesIter != it.m_entriesIter; }

                    private:
                        std::vector<Entry>::const_iterator m_entriesIter;
                };

                Entries() = default;

                Entries(const std::vector<Entry>& entries) :
                    m_entries(entries)
                {
                }

                void push_back(const Entry& entry)
                {
                    m_entries.push_back(entry);
                }

                void push_back(Entry&& entry)
                {
                    m_entries.push_back(entry);
                }

                template<class... Args>
                void emplace_back(Args&&... args)
                {
                    m_entries.emplace_back(std::forward<Args>(args)...);
                }

                inline std::size_t size()
                {
                    return m_entries.size();
                }

                bool empty()
                {
                    return m_entries.empty();
                }

                iterator begin()
                {
                    return iterator { m_entries.begin() };
                }

                iterator end()
                {
                    return iterator { m_entries.end() };
                }

                const_iterator begin() const
                {
                    return const_iterator { m_entries.begin() };
                }

                const_iterator end() const
                {
                    return const_iterator { m_entries.end() };
                }

            private:
                std::vector<Entry> m_entries;
        };

        struct Category {
            enum class Action {
                READ,
                UNREAD
            };

            std::string Label;
            std::string ID;

            friend inline bool operator>(const Category& lhs, const Category& rhs)
            {
                return lhs.Label.compare(rhs.Label) > 0;
            }

            friend inline bool operator<(const Category& lhs, const Category& rhs)
            {
                return lhs.Label.compare(rhs.Label) < 0;
            }

            inline bool operator==(const Category& rhs)
            {
                return this->Label == rhs.Label && this->ID == rhs.ID;
            }

            unsigned int Unread()
            {
                // TODO
            }

            unsigned int Read()
            {
                // TODO
            }
        };

        class Categories {
            public:
                class iterator {
                    public:
                        using value_type = Category;
                        using difference_type = std::ptrdiff_t;
                        using pointer = Category*;
                        using reference = Category&;
                        using iterator_category = std::forward_iterator_tag;

                        iterator(std::set<Category, std::less<Category>>::iterator ctgsIter) :
                            m_ctgsIter(ctgsIter)
                        {
                        }

                        const Category& operator*()
                        {
                            return *m_ctgsIter;
                        }

                        iterator& operator++()
                        {
                            m_ctgsIter++;
                            return *this;
                        }

                        bool operator==(const iterator& it) { return m_ctgsIter == it.m_ctgsIter; }
                        bool operator!=(const iterator& it) { return m_ctgsIter != it.m_ctgsIter; }

                    private:
                        std::set<Category, std::less<Category>>::iterator m_ctgsIter;
                };

                class const_iterator {
                    public:
                        using value_type = const Category;
                        using difference_type = std::ptrdiff_t;
                        using pointer = const Category*;
                        using reference = const Category&;
                        using iterator_category = std::forward_iterator_tag;

                        const_iterator(std::set<Category, std::less<Category>>::const_iterator ctgsIter) :
                            m_ctgsIter(ctgsIter)
                        {
                        }

                        const Category& operator*() const
                        {
                            return *m_ctgsIter;
                        }


                        const_iterator& operator++()
                        {
                            m_ctgsIter++;
                            return *this;
                        }

                        bool operator==(const const_iterator& it) const { return m_ctgsIter == it.m_ctgsIter; }
                        bool operator!=(const const_iterator& it) const { return m_ctgsIter != it.m_ctgsIter; }

                    private:
                        std::set<Category, std::less<Category>>::const_iterator m_ctgsIter;
                };


                Categories() = default;
                Categories(const Categories& categories) = default;
                Categories(Categories&& category) = default;
                Categories& operator=(Categories& categories) = default;
                Categories& operator=(Categories&& categories) = default;

                Categories(const std::vector<Category>& categories)
                {
                    for (const auto& ctg : categories) {
                        m_categories.insert(ctg);
                    }
                }

                Categories(Categories& categories) :
                    m_categories(categories.m_categories)
                {
                }

                const Category& operator[](const std::string& id) const
                {
                    auto predicate = [&] (const Category& ctg) { return ctg.ID == id; };
                    auto value = std::find_if(m_categories.begin(), m_categories.end(), predicate);
                    if (value == std::end(m_categories)) {
                        throw std::runtime_error("Category with ID not found");
                    }
                    return *value;
                }

                const Category& getByLabel(const std::string& label) const
                {
                    auto predicate = [&] (const Category& ctg) { return ctg.Label == label; };
                    auto value = std::find_if(m_categories.begin(), m_categories.end(), predicate);
                    if (value == std::end(m_categories)) {
                        throw std::runtime_error("Category with ID not found");
                    }
                    return *value;
                }

                bool append(const Category& category)
                {
                    auto p = m_categories.insert(category);
                    return p.second;
                }

                inline std::size_t size()
                {
                    return m_categories.size();
                }

                inline bool empty() const
                {
                    return m_categories.empty();
                }

                iterator begin()
                {
                    return iterator { m_categories.begin() };
                }

                iterator end()
                {
                    return iterator { m_categories.end() };
                }

                const_iterator begin() const
                {
                    return const_iterator(m_categories.cbegin());
                }

                const_iterator end() const
                {
                    return const_iterator(m_categories.cend());
                }

            private:
                std::set<Category, std::less<Category>> m_categories;
        };

        struct Feed {
            enum class Action {
                READ,
                UNREAD
            };

            std::string Title;
            std::string Url;
            std::string VisualUrl;
            std::string ID;
            std::string SortID;
            int         Updated;
            int         Added;
            class Categories  Categories;
        };

        class Feeds {
            public:
                class iterator {
                    public:
                        using value_type = Feed;
                        using difference_type = std::ptrdiff_t;
                        using pointer = Feed*;
                        using reference = Feed&;
                        using iterator_category = std::forward_iterator_tag;

                        iterator(std::vector<Feed>::iterator feedsIter) :
                            m_feedsIter(feedsIter)
                        {
                        }

                        Feed& operator*()
                        {
                            return *m_feedsIter;
                        }

                        const Feed& operator*() const
                        {
                            return *m_feedsIter;
                        }

                        iterator& operator++()
                        {
                            m_feedsIter++;
                            return *this;
                        }

                        bool operator==(const iterator& it) { return m_feedsIter == it.m_feedsIter; }
                        bool operator!=(const iterator& it) { return m_feedsIter != it.m_feedsIter; }

                    private:
                        std::vector<Feed>::iterator m_feedsIter;
                };

                class const_iterator {
                    public:
                        using value_type = const Feed;
                        using difference_type = std::ptrdiff_t;
                        using pointer = const Feed*;
                        using reference = const Feed&;
                        using iterator_category = std::forward_iterator_tag;

                        const_iterator(std::vector<Feed>::const_iterator feedsIter) :
                            m_feedsIter(feedsIter)
                        {
                        }

                        const Feed& operator*() const
                        {
                            return *m_feedsIter;
                        }

                        const_iterator& operator++()
                        {
                            m_feedsIter++;
                            return *this;
                        }

                        bool operator==(const const_iterator& it) { return m_feedsIter == it.m_feedsIter; }
                        bool operator!=(const const_iterator& it) { return m_feedsIter != it.m_feedsIter; }

                    private:
                        std::vector<Feed>::const_iterator m_feedsIter;
                };

                Feeds() = default;

                Feeds(const std::vector<Feed>& feeds) :
                    m_feeds(feeds)
                {
                }

                void push_back(const Feed& feed)
                {
                    m_feeds.push_back(feed);
                }

                void push_back(Feed&& feed)
                {
                    m_feeds.push_back(feed);
                }

                inline std::size_t size()
                {
                    return m_feeds.size();
                }

                bool empty()
                {
                    return m_feeds.empty();
                }

                iterator begin()
                {
                    return iterator { m_feeds.begin() };
                }

                iterator end()
                {
                    return iterator { m_feeds.end() };
                }

                const_iterator begin() const
                {
                    return const_iterator { m_feeds.begin() };
                }

                const_iterator end() const
                {
                    return const_iterator { m_feeds.end() };
                }

            private:
                std::vector<Feed> m_feeds;

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
        Categories GetCategories() const
        {
            auto r = cpr::Get(cpr::Url{m_rootUrl + "/categories"},
                              cpr::Header{{"Authorization", "OAuth " + m_user.AuthToken}});


            if (r.status_code not_eq 200) {
                std::string error = "Could not get categories: " + std::to_string(r.status_code);
                throw std::runtime_error(error.c_str());
            }

            auto jsonResp = json::parse(r.text);

            Categories categories;
            for (auto& ctg : jsonResp) {
                categories.append(Category {.Label = ctg["label"], .ID = ctg["id"]});
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
                    std::string error = "Could not mark entries with " + ActionToString(action) + ": " + std::to_string(r.status_code);
                    throw std::runtime_error(error.c_str());
                }
            }
        }

        /**
         * Get list of subscribed feeds
         */
        Feeds GetSubscriptions()
        {
            auto r = cpr::Get(cpr::Url{m_rootUrl + "/subscriptions"},
                              cpr::Header{{"Authorization", "OAuth " + m_user.AuthToken}});

            if (r.status_code not_eq 200) {
                std::string error = "Could not get subscriptions: " + std::to_string(r.status_code);
                throw std::runtime_error(error.c_str());
            }

            auto j = json::parse(r.text);

            Feeds feeds;
            for (const auto& feed : j) {
                Feed tmp;
                tmp.Title = feed["title"];
                tmp.ID = feed["id"];
                tmp.Url = feed["website"];
                tmp.VisualUrl = feed["visualUrl"];
                tmp.Updated = feed["updated"];

                Categories ctgs {};
                for (const auto& ctg : feed["categories"]) {
                    Category tmp;
                    tmp.Label = ctg["label"];
                    tmp.ID = ctg["id"];
                    ctgs.append(tmp);
                }

                tmp.Categories = ctgs;

                feeds.push_back(tmp);
            }

            return feeds;
        }

        /**
         * Subscribe to a feed
         *
         * @param feed  the feed to subscribe to
         */
        void AddSubscription(const Feed& feed)
        {
            json j;
            j["id"] = "feed/" + feed.Url;
            j["title"] = feed.Title;

            if (not feed.Categories.empty()) {
                for (const auto& ctg : feed.Categories) {
                    j["categories"].push_back({{"label", ctg.Label},
                                               {"id",    ctg.ID}});
                }
            } else {
                j["categories"] = json::array();
            }

            auto r = cpr::Post(cpr::Url{m_rootUrl + "/subscriptions"},
                               cpr::Header{{"Authorization", "OAuth " + m_user.AuthToken},
                                           {"Content-Type", "application/json"}},
                               cpr::Body{j.dump()});

            if (r.status_code not_eq 200) {
                std::string error = "Could not add subscription: " + std::to_string(r.status_code);
                throw std::runtime_error(error.c_str());
            }
        }

        Entries GetEntries(
                const Category& category,
                bool sortByOldest = false,
                unsigned int count = 20,
                bool unreadOnly = true,
                std::string continuationId = "",
                unsigned long newerThan = 0
                ) const
        {
            return GetEntries(category.ID, sortByOldest, count, unreadOnly, continuationId, newerThan);
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
        Entries GetEntries(
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
                    {"unreadOnly",   unreadOnly   ? "true"   : "false" },
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

            Entries entries;
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
         * @return true if we can reach cloud.feedly.com, false otherwise
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
