#pragma once

#include <string>
#include <variant>

enum FilteringType {
	LINEAR = 0,
    NEAREST
};

class Texture {
    public:
        std::string src;
        int width, height, channels;
        std::variant<unsigned char*, float*> data;

        Texture() = default;
        Texture(const std::string& src, const FilteringType type);
        Texture(unsigned char* _data, int _width, int _height, int _channels);

        void bind();
        [[nodiscard]] unsigned int getID() { return m_texture; }

    private:
        unsigned int m_texture;
};