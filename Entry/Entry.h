#pragma once

#include <iostream>
#include <vector>
#include "../Content/Content.h"
class Entry {
    public:
    Entry(){};
    Entry(const std::string& _title, 
          std::vector<std::shared_ptr<Content> >&& _contents) :
                            title{_title},
                            contents{std::move(_contents)} {};
    ~Entry(){};
    std::vector<std::shared_ptr<Content> > contents;
    std::string title {};
};