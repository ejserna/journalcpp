#include "Content.h"

void ImageContent::decode(const codable_properties_t&& props) {
    if (props.find("image_data") != props.end()) {
        data = std::make_shared<std::string>(props.at("image_data"));
    }
}

const codable_properties_t ImageContent::encode() const {
    codable_properties_t props;
    if (data) {
        props["image_data"] = *data;
    }
    return props;
}

void ImageContent::view(const ContentViewer& viewer) const {
    viewer.loadImage(*this);
}

void TextContent::decode(const codable_properties_t&& props) {
    if (props.find("text_data") != props.end()) {
        data = std::make_shared<std::string>(props.at("text_data"));
    }
}

const codable_properties_t TextContent::encode() const {
    codable_properties_t props;
    if (data) {
        props["text_data"] = *data;
    }
    return props;
}

void TextContent::view(const ContentViewer& viewer) const {
    viewer.loadText(*this);
}

void ConsoleContentViewer::loadImage(const ImageContent &img) const{
    std::cout << "Not implemented. Can't view images in console" << std::endl;
}

void ConsoleContentViewer::loadText(const TextContent &txt) const{
    if (txt.data) {
        std::cout << "\t" << *(txt.data) << std::endl;
    }
}


