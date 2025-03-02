#include "BMPImage.h"

BMPImage::BMPImage(const std::string& filename) { LoadFromFile(filename); }

void BMPImage::Display() const {
    int pixelSize = m_infoHeader.bitCount / 8;
    int rowSize = (m_width * pixelSize);
    for (int y = m_height - 1; y >= 0; --y) {
        for (int x = 0; x < m_width; ++x) {
            int index = (y * rowSize) + (x * pixelSize);
            uint8_t b = m_pixels[index];
            uint8_t g = m_pixels[index + 1];
            uint8_t r = m_pixels[index + 2];

            if (r == 0 && g == 0 && b == 0) {
                std::cout << '#';
            } else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
}

void BMPImage::DrawLine(int x1, int y1, int x2, int y2, int color) {
    int deltax = abs(x2 - x1);
    int deltay = abs(y2 - y1);
    int error = 0;
    int deltaerr = deltay + 1;
    int y = y1;
    int diry = y1 < y2 ? 1 : -1;
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    for (int x = x1; x <= x2; ++x) {
        Plot(x, y, color, color, color);
        error += deltaerr;
        if (error >= (deltax + 1)) {
            y += diry;
            error -= (deltax + 1);
        }
    }
}

void BMPImage::Save(const std::string& filename) {
    std::ofstream os(filename.c_str(), std::ios::out | std::ios::binary);
    if (!os) {
        throw std::runtime_error("Could not open file " + filename);
    }

    if (m_infoHeader.bitCount == 32) {
        WriteHeadersAndData(os);
    } else if (m_infoHeader.bitCount == 24) {
        if (m_infoHeader.width % 4 == 0) {
            WriteHeadersAndData(os);
        }
    }
}

void BMPImage::Plot(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= 0 && x < (int)m_width && y >= 0 && y < (int)m_height) {
        int pixelSize = m_infoHeader.bitCount / 8;
        int rowSize = (m_width * pixelSize + 3) & ~3;
        int index = (y * rowSize) + (x * pixelSize);

        m_pixels[index] = b;
        m_pixels[index + 1] = g;
        m_pixels[index + 2] = r;
    }
}

void BMPImage::LoadFromFile(const std::string& filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is) throw std::runtime_error("Could not open file " + filename);

    is.read(reinterpret_cast<char*>(&m_header), sizeof(m_header));
    if (!is) throw std::runtime_error("Could not read BMP header from file " + filename);
    if (m_header.fileType != 0x4D42) throw std::runtime_error("Not a valid BMP file " + filename);

    is.read(reinterpret_cast<char*>(&m_infoHeader), sizeof(m_infoHeader));
    if (!is) throw std::runtime_error("Could not read BMP info header from file " + filename);
    if (m_infoHeader.planes != 1) throw std::runtime_error("Unknown format");
    if (m_infoHeader.bitCount < 24) throw std::runtime_error("Unknown format");

    m_width = m_infoHeader.width;
    
    m_height = abs(m_infoHeader.height);
    std::cout << m_width << " " << m_height << std::endl;
    if (m_infoHeader.bitCount == 32) {
        m_infoHeader.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        m_header.dataOffset = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
    } else {
        m_infoHeader.size = sizeof(BMPInfoHeader);
        m_header.dataOffset = sizeof(BMPHeader) + sizeof(BMPInfoHeader);
    }
    m_header.fileSize = m_header.dataOffset;
    int pixelSize = m_infoHeader.bitCount / 8;
    m_pixels.resize(m_width * m_height * pixelSize);

    is.seekg(m_header.dataOffset, std::ios::beg);

    ReadPixels(is, pixelSize);

    is.close();
}

void BMPImage::ReadPixels(std::ifstream& is, int pixelSize) {
    if (m_width % 4 == 0) {
        is.read((char*)(m_pixels.data()), m_pixels.size());
        m_header.fileSize += static_cast<uint32_t>(m_pixels.size());
    } else {
        std::cout << "here111" << std::endl;
        int row_stride = m_infoHeader.width * pixelSize;
        uint32_t new_stride = row_stride;
        while (new_stride % 4 != 0) {
            new_stride++;
        }
        std::vector<uint8_t> padding_row(new_stride - row_stride);
        for (int y = 0; y < m_height; ++y) {
            is.read((char*)(m_pixels.data() + row_stride * y), row_stride);
            is.read((char*)padding_row.data(), padding_row.size());
        }
        m_header.fileSize += static_cast<uint32_t>(m_pixels.size()) + m_infoHeader.height * static_cast<uint32_t>(padding_row.size());
    }
}

void BMPImage::WriteHeaders(std::ofstream& of) {
    of.write((const char*)&m_header, sizeof(m_header));
    of.write((const char*)&m_infoHeader, sizeof(m_infoHeader));
    if (m_infoHeader.bitCount == 32) {
        of.write((const char*)&m_colorHeader, sizeof(m_colorHeader));
    }
}

void BMPImage::WriteHeadersAndData(std::ofstream& of) {
    WriteHeaders(of);
    of.write((const char*)m_pixels.data(), m_pixels.size());
}
