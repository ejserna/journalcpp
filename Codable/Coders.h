#pragma once

#include <iostream>
#include <unordered_map>
#include <utility> 
#include <optional>
#include <memory>
#include "../Codable/Codable.h"
#include "../Content/Content.h"

class ContentCoder {
    public:
    std::shared_ptr<Content> decode(const codable_properties_t&& properties) const{
        // We make an Image if there's image_data
        if (properties.find("image_data") != properties.end()) {
            std::shared_ptr<ImageContent> img = std::make_shared<ImageContent>();
            img->decode(std::move(properties));
            return std::move(img);
        }
        // We make Text if there's text_data
        else if (properties.find("text_data") != properties.end()) {
            std::shared_ptr<TextContent> txt = std::make_shared<TextContent>();
            txt->decode(std::move(properties));
            return std::move(txt);
        }
        else {
            return nullptr;
        }
    }
    const codable_properties_t encode(const Content& obj) const{
        return std::move(obj.encode());
    }
};

