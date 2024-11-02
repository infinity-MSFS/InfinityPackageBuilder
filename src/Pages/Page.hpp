
#pragma once

#include <utility>

class Page {
public:
    Page(const float padding_x, const float padding_y) : m_Padding({padding_x, padding_y}) {}

    virtual ~Page() = default;

    virtual void RenderPage() = 0;

private:
    std::pair<float, float> m_Padding; // (X,Y)
};
