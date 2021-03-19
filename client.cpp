#include <string>
#include <iostream>
#include "Journal.h"

enum class Action{
    CreateEntry,
    EditEntry,
    DeleteEntry,
    ListAllEntries,
    SortAscending,
    SortDescending,
    FilterByTitle,
    None
};

void printEntries(const std::vector<Entry>& entries, const std::shared_ptr<ContentViewer> viewer)
{
    for (auto const& entry : entries) {
        std::cout << entry.title << std::endl;
        if (viewer){
            for (auto content: entry.contents) {
                content->view(*viewer);
            }
        }
    }
}

int main(int argc, char** argv) 
{
    auto journal = TextFileJournal(std::string(getenv("HOME")) + "/" + "journal");
    Entry entry;
    Entry updatedEntry;
    Action action = Action::None;
    bool showContent = false;
    bool makeCaseInsensitive = false;
    std::shared_ptr<ConsoleContentViewer> viewer = nullptr;
    if (argc == 1) {
        std::cout << "./journal \t--create {content} --title {title}\n"
                                 "\t\t--delete {title}\n"
                                 "\t\t--list [--showContent] \n"
                                 "\t\t--edit {new content} --newTitle {title} --title {new title} \n"
                                 "\t\t--filter {title} [--showContent] [--caseInsensitive]\n"
                                 "\t\t--sortAscending [--showContent] \n"
                                 "\t\t--sortDescending [--showContent]" << std::endl;
    }
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--create") == 0 && i + 1 < argc) {
            action = Action::CreateEntry;

            auto data = std::string(argv[i + 1]);
            data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
            
            TextContent content = TextContent(std::move(data));
            entry.contents.push_back(std::make_shared<TextContent>(content));
        }
        else if (strcmp(argv[i], "--create") == 0 && i + 1 == argc) {
            std::cout << "please supply the journal contents to --create" << std::endl;
        }
        if (strcmp(argv[i], "--newTitle") == 0 && i + 1 < argc) {
            action = Action::EditEntry;
            auto title = std::string(argv[i + 1]);
            title.erase(std::remove(title.begin(), title.end(), '\n'), title.end());
            updatedEntry.title = title;
        }
        else if (strcmp(argv[i], "--newTitle") == 0 && i + 1 == argc) {
            std::cout << "please supply a new title to --newTitle to" << std::endl;
        }
        else if (strcmp(argv[i], "--edit") == 0 && i + 1 < argc) {
            action = Action::EditEntry;

            auto data = std::string(argv[i + 1]);
            data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
            
            TextContent content = TextContent(std::move(data));
            updatedEntry.contents.push_back(std::make_shared<TextContent>(content));
        }
        else if (strcmp(argv[i], "--edit") == 0 && i + 1 == argc) {
            std::cout << "please supply new content to --edit" << std::endl;
        }
        else if (strcmp(argv[i], "--delete") == 0 && i + 1 < argc) {
            auto title = std::string(argv[i + 1]);
            action = Action::DeleteEntry;

            title.erase(std::remove(title.begin(), title.end(), '\n'), title.end());
            entry.title = title;
        }
        else if (strcmp(argv[i], "--delete") == 0 && i + 1 == argc) {
            std::cout << "please supply the title name to  --delete" << std::endl;
        }
        else if (strcmp(argv[i], "--filter") == 0 && i + 1 < argc) {
            auto title = std::string(argv[i + 1]);
            action = Action::FilterByTitle;

            title.erase(std::remove(title.begin(), title.end(), '\n'), title.end());
            entry.title = title;
        }
        else if (strcmp(argv[i], "--filter") == 0 && i + 1 == argc) {
            std::cout << "please supply the title name to  --filter" << std::endl;
        }
        else if (strcmp(argv[i], "--sortAscending") == 0 ) {
            action = Action::SortAscending;
        }
        else if (strcmp(argv[i], "--sortDescending") == 0 ) {
            action = Action::SortDescending;
        }
        else if (strcmp(argv[i], "--title") == 0 && i + 1 < argc) {
            auto title =  std::string(argv[i + 1]);
            action = Action::CreateEntry;

            title.erase(std::remove(title.begin(), title.end(), '\n'), title.end());
            entry.title = title;
        }
        else if (strcmp(argv[i], "--title") == 0 && i + 1 == argc) {
            std::cout << "please supply the journal title to --title" << std::endl;
        }
        else if (strcmp(argv[i], "--list") == 0) {
            action = Action::ListAllEntries;
        }
        else if (strcmp(argv[i], "--showContent") == 0) {
            showContent = true;
        }
        else if (strcmp(argv[i], "--makeCaseInsensitive") == 0) {
            makeCaseInsensitive = true;
        }

    }
    
    viewer = showContent ? std::make_unique<ConsoleContentViewer>() : nullptr; 

    switch (action)
    {
    case Action::CreateEntry: {
        if (!entry.title.empty() && !entry.contents.empty())
        {
            journal.createEntry(std::move(entry));
        }
        break;
    }
    case Action::EditEntry: {
        if (!entry.title.empty() && !updatedEntry.title.empty() && !updatedEntry.contents.empty())
        {
            journal.editEntry(entry, updatedEntry);
        }
        break;
    }
    case Action::ListAllEntries: {
        auto entries = journal.listEntries();
        printEntries(entries, viewer);
        break;
    }
    case Action::DeleteEntry: {
        if (!entry.title.empty()) {
            journal.deleteEntry(entry);
        }
        break;
    }
    case Action::SortAscending: {
        auto entries = journal.sortEntries(EntryFilters::ascendingTitle());
        printEntries(entries, viewer);
        break;
    }
    case Action::SortDescending: {
        auto entries = journal.sortEntries(EntryFilters::descendingTitle());
        printEntries(entries, viewer);
        break;
    }
    case Action::FilterByTitle:{
        if (!entry.title.empty())
        {
            auto entries = journal.filterEntries(EntryFilters::isTitleEqualTo(entry.title, makeCaseInsensitive));
            printEntries(entries, viewer);
        }
        break;
    }
    case Action::None:
        break;
    }

    

    return 0;
}

