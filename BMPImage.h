#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#define COLOR_BLACK 0
#define COLOR_WHITE 255
#pragma pack(push, 1)
struct BMPHeader {
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
};

struct BMPInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerM;
    int32_t yPixelsPerM;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
struct BMPColorHeader {
    uint32_t red_mask{0x00ff0000};
    uint32_t green_mask{0x0000ff00};
    uint32_t blue_mask{0x000000ff};
    uint32_t alpha_mask{0xff000000};
    uint32_t color_space_type{0x73524742};
    uint32_t unused[16]{0};
};
#pragma pack(pop)
class BMPImage {
   public:
    BMPImage(const std::string& filename);
    void Display() const;
    void Save(const std::string& filename);
    void DrawLine(int x1, int y1, int x2, int y2, int color);

   private:
    void Plot(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void LoadFromFile(const std::string& filename);
    void ReadPixels(std::ifstream& is, int pixelSize);
    void WriteHeaders(std::ofstream& of);
    void WriteHeadersAndData(std::ofstream& of);

   private:
    const static uint16_t m_colorHeaderSize = 84;
    BMPHeader m_header;
    BMPColorHeader m_colorHeader;
    BMPInfoHeader m_infoHeader;
    std::vector<uint8_t> m_pixels;
    int m_width;
    int m_height;
};