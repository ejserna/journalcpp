#pragma once

//#include <iostream>
#include "Filters/Filters.h"
#include "Codable/Coders.h"
#include "Content/Content.h"
#include "Entry/Entry.h"

class Journal {
    public:
    Journal(){};
    virtual ~Journal(){};
    virtual void createEntry(Entry&& entry) = 0;
    virtual bool deleteEntry(const Entry& e) = 0;
    virtual bool editEntry(const Entry& oldEntry, const Entry& newEntry) = 0;
    virtual std::vector<Entry> filterEntries(FilterPredicate<Entry> predicate) = 0;        
    virtual std::vector<Entry> sortEntries(SortPredicate<Entry> predicate) = 0;
    virtual std::vector<Entry> listEntries() = 0;
};

class TextFileJournal : public virtual Journal {
    public:
    std::vector<std::shared_ptr<Entry> > entries = {};
    TextFileJournal(const std::string& directory);
    ~TextFileJournal(){};
    void createEntry(Entry&& entry) override;
    bool deleteEntry(const Entry& e) override;
    bool editEntry(const Entry& oldEntry, const Entry& newEntry) override;
    std::vector<Entry> filterEntries(FilterPredicate<Entry> predicate) override;
    std::vector<Entry> sortEntries(SortPredicate<Entry> predicate) override;
    std::vector<Entry> listEntries() override;
    private:
    std::string m_directory;
    std::vector<Entry> copyEntries();
    void createFileFromEntry(const Entry& e);
};
