// #define NDEBUG
#include <cassert>
#include <assert.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>
#include "../Journal.h"

std::shared_ptr<Journal> setup(std::string directory)
{
    auto journal = TextFileJournal(directory);
    assert (std::filesystem::exists(directory) == true);
    return std::make_shared<TextFileJournal>(journal);
}

bool testEntryCreation(Journal &journal, std::string directory ,std::string title, std::shared_ptr<Content> content)
{
    std::string fullFilePath = directory + "/" + title + ".txt";
    std::vector<std::shared_ptr<Content> > contents;
    contents.push_back(content);
    Entry e(title, std::move(contents));
    journal.createEntry(std::move(e));

    assert (std::filesystem::exists(fullFilePath) == true); 

    auto entries = journal.listEntries();
    for (auto entry : entries){
        if (entry.title == title) {
            return true;
        }
    }
    return false;
}

bool testEntryDeletion(Journal &journal, std::string directory, std::string title)
{
    std::string fullFilePath = directory + "/" + title + ".txt";
    Entry e;
    e.title = title;
    journal.deleteEntry(e);

    assert (std::filesystem::exists(fullFilePath) == false); 

    auto entries = journal.listEntries();
    for (auto entry : entries){
        if (entry.title == title) {
            return false;
        }
    }
    return true;
}

bool testEntryEditing(Journal &journal, std::string directory ,std::string oldTitle, std::string newTitle ,std::shared_ptr<Content> content)
{
    std::string fullOldFilePath = directory + "/" + oldTitle + ".txt";
    std::string fullNewFilePath = directory + "/" + newTitle + ".txt";
    std::vector<std::shared_ptr<Content> > oldContents;
    std::vector<std::shared_ptr<Content> > newContents;
    
    oldContents.push_back(content);
    newContents.push_back(content);
    Entry oldEntry(oldTitle, std::move(oldContents));
    Entry newEntry(newTitle, std::move(newContents));

    // First create an entry
    journal.createEntry(std::move(oldEntry));
    assert (std::filesystem::exists(fullOldFilePath) == true); 

    std::vector<std::shared_ptr<Content> > contents;
    contents.push_back(content);

    // Now we will trigger an update on the oldEntry
    oldEntry = Entry(oldTitle, std::move(contents));
    journal.editEntry(oldEntry, newEntry);

    assert (std::filesystem::exists(fullOldFilePath) == false); 
    assert (std::filesystem::exists(fullNewFilePath) == true); 

    // Finally, make sure the old entry isn't cached
    auto entries = journal.listEntries();
    for (auto entry : entries){
        assert (entry.title != oldTitle);
    }

    // And make sure that the new one is there
    for (auto entry : entries){
        if (entry.title == newTitle){
            return true;
        }
    }
    
    return false;
}

bool testSort(Journal &journal,
             std::vector<std::string>& ordered,
             SortPredicate<Entry> predicate)
{
    auto entries = journal.sortEntries(predicate);
    assert (entries.size() == ordered.size());
    for (int i = 0; i < entries.size(); i++) {
        assert (entries[i].title == ordered[i]);
    }
    return true;
}

bool testFilter(Journal &journal,
             std::vector<std::string>& filtered,
             FilterPredicate<Entry> predicate)
{
    auto entries = journal.filterEntries(predicate);
    assert (entries.size() == filtered.size());
    for (int i = 0; i < entries.size(); i++) {
        assert (entries[i].title == filtered[i]);
    }
    return true;
}

std::vector<std::string> generateRandomWords(unsigned int wordSize,unsigned int count)
{
    std::vector<std::string> words {};

    srand (time(NULL)); 
    for (int i = 0; i < count; i++)
    {     
          std::string word = "";
          for (int j = 0; j < wordSize; j++) {
              word.push_back(char('a' + rand() % 26));
          }

          words.push_back(word);
    }
    return std::move(words);
}

int main()
{
    auto directory = "/tmp/tests";
    auto journal = setup(directory);
    std::cout << "1. Test journal creation" << std::endl;
    assert (journal != nullptr);
    
    TextContent textContent;
    textContent.data = std::make_shared<std::string>("test1");

    std::cout << "2. Test entry creation" << std::endl;
    assert (testEntryCreation(*journal, directory ,"title", std::make_shared<TextContent>(textContent)) == true);

    std::cout << "3. Test entry deletion" << std::endl;
    assert (testEntryDeletion(*journal, directory ,"title") == true);

    std::cout << "4. Test entry editing" << std::endl;
    assert (testEntryEditing(*journal, directory, "title", "newTitle", std::make_shared<TextContent>(textContent)) == true);
    assert (testEntryDeletion(*journal, directory ,"newTitle") == true);

    std::cout << "5. Test entry sorting" << std::endl;
    std::vector<std::string> words = generateRandomWords(10, 1000);
    for (auto word: words){
        assert (testEntryCreation(*journal, directory, word, std::make_shared<TextContent>(textContent)) == true);
    }

    std::sort(words.begin(), words.end(),std::less<std::string>());
    assert (testSort(*journal, words, EntryFilters::ascendingTitle()) == true);

    std::sort(words.begin(), words.end(),std::greater<std::string>());
    assert (testSort(*journal, words, EntryFilters::descendingTitle()) == true);

    std::cout << "6. Test entry filtering" << std::endl;
    for (auto word: words){
        assert (testEntryCreation(*journal, directory, word, std::make_shared<TextContent>(textContent)) == true);
    }
    std::vector<std::string> originalWords = words;

    std::string entryStrContains = "";
    entryStrContains.push_back(words[0][0]);
    auto it = std::copy_if (words.begin(), words.end(), words.begin(), [entryStrContains](std::string& s){return s.find(entryStrContains) != std::string::npos;} );
    words.resize(std::distance(words.begin(), it));

    assert (testFilter(*journal, words, EntryFilters::titleContains(entryStrContains)) == true);

    for (auto word: originalWords){
        assert (testEntryDeletion(*journal, directory, word) == true);
    }

    std::cout << "All tests passed" << std::endl;
}