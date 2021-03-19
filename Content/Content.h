#pragma once

#include <iostream>
#include <string>
#include "../Codable/Codable.h"

class ImageContent;
class TextContent;

class ContentViewer {
    public:
    ContentViewer(){};
    virtual ~ContentViewer(){};
    virtual void loadImage(const ImageContent &img) const = 0;
    virtual void loadText(const TextContent &txt) const = 0;
};

class ConsoleContentViewer : public virtual ContentViewer {
    public:
    ConsoleContentViewer(){};
    ~ConsoleContentViewer(){};
    void loadImage(const ImageContent &img) const override;
    void loadText(const TextContent &txt) const override;
};

class Content: public virtual Codable  {
    public:
    Content(){};
    virtual ~Content(){};
    virtual void view(const ContentViewer& viewer) const = 0;
};

class ImageContent : public virtual Content {
    public:
    ImageContent(){};
    ImageContent(std::string&& _data) : 
                        data{std::move(std::make_shared<std::string>(_data))} 
                        {};
    ~ImageContent(){};
    void decode(const codable_properties_t&& props) override;
    const codable_properties_t encode() const override;
    void view(const ContentViewer& viewer) const override;
    std::shared_ptr<std::string> data = nullptr;
};

class TextContent : public virtual Content {
    public:
    TextContent(){};
    TextContent(std::string&& _data) : 
                        data{std::move(std::make_shared<std::string>(_data))} 
                        {};
    ~TextContent(){};
    void decode(const codable_properties_t&& props) override;
    const codable_properties_t encode() const override;
    void view(const ContentViewer& viewer) const override;
    std::shared_ptr<std::string> data = nullptr;
};