#include "Journal.h"
#include <fstream>
#include <filesystem>
#include <string>
TextFileJournal::TextFileJournal(const std::string& directory) 
{
    if (!directory.empty()) {
        m_directory = directory;

        // Load entries if there are entries
        if (std::filesystem::exists(m_directory))
        {
            for (const auto& entryFile: std::filesystem::directory_iterator(m_directory)){
                // We will only parse .txt files
                if (entryFile.path().extension() == ".txt") {
                    std::ifstream file(entryFile.path(), std::ios_base::in | std::ios_base::binary);
                    std::string propertyIdentifier;
                    std::string propertyValue;
                    
                    Entry entry;
                    entry.title = entryFile.path().stem();
                    // Parse contents
                    while(std::getline(file, propertyIdentifier)) {
                        codable_properties_t properties; 
                        std::getline(file, propertyValue);
                        properties[propertyIdentifier] = propertyValue;
                        auto content = ContentCoder().decode(std::move(properties));
                        entry.contents.push_back(std::move(content));
                    }
                    entries.push_back(std::make_shared<Entry>(std::move(entry)));
                }
            }
        }
        else 
        {
            std::filesystem::create_directory(m_directory);
        }
    }
}

void TextFileJournal::createFileFromEntry(const Entry& entry){
    if (!m_directory.empty()) {
        std::ofstream entryFile;
        this->deleteEntry(entry);
        entryFile.open(m_directory + "/" + entry.title + ".txt", std::ios_base::out | std::ios_base::binary);
        for (auto const& content: entry.contents) {
            auto properties = ContentCoder().encode(*content);
            for (auto property = properties.begin(); property != properties.end(); property++) {
                entryFile << property->first << std::endl;
                entryFile << property->second << std::endl; 
            }
        }
        entries.push_back(std::make_shared<Entry>(entry));
        entryFile.close();
    }  
}

void TextFileJournal::createEntry(Entry&& entry) {
    createFileFromEntry(entry);
}

bool TextFileJournal::deleteEntry(const Entry& entry) {
    for (auto entryIt = entries.begin(); entryIt != entries.end(); entryIt++) {
        if ((*entryIt)->title == entry.title){
            entries.erase(entryIt);
            return std::filesystem::remove(m_directory + "/" + entry.title + ".txt");
        }
    }
    return false;
}

bool TextFileJournal::editEntry(const Entry& oldEntry, const Entry& newEntry) {
    if (!m_directory.empty()){
        deleteEntry(oldEntry);
        createFileFromEntry(newEntry);
        return true;
    }
    return false;
}

std::vector<Entry> TextFileJournal::filterEntries(FilterPredicate<Entry> predicate) {
    std::vector<Entry> copy = copyEntries();
    filter<Entry>(copy, predicate);
    return std::move(copy);
}

std::vector<Entry> TextFileJournal::sortEntries(SortPredicate<Entry> predicate) {
    std::vector<Entry> copy = copyEntries();
    sort<Entry>(copy, predicate);
    return std::move(copy);
}

std::vector<Entry> TextFileJournal::listEntries() {
    std::vector<Entry> copy = copyEntries();
    auto allEntriesFilter = [&](const Entry& entry) {
        return true;
    };
    filter<Entry>(copy, allEntriesFilter);
    return std::move(copy);
}

std::vector<Entry> TextFileJournal::copyEntries() {
    std::vector<Entry> copy;
    for (auto const& entry: entries) {
        copy.push_back(*entry);
    }
    return std::move(copy);
}