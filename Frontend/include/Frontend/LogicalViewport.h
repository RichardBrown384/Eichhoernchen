#pragma once

#include "Frontend/Toolkit.h"

namespace rbrown::frontend {
class LogicalViewport {
public:
    LogicalViewport(const int logicalWidth, const int logicalHeight) :
        m_logicalWidth(static_cast<float>(logicalWidth)),
        m_logicalHeight(static_cast<float>(logicalHeight)) {
    }

    void OnResize(const int windowWidth, const int windowHeight) {
        const auto w = static_cast<float>(windowWidth);
        const auto h = static_cast<float>(windowHeight);

        const auto scale = std::min(
            w / m_logicalWidth,
            h / m_logicalHeight
            );

        m_viewport.x = 0.5f * (w - scale * m_logicalWidth);
        m_viewport.y = 0.5f * (h - scale * m_logicalHeight);
        m_viewport.w = scale * m_logicalWidth;
        m_viewport.h = scale * m_logicalHeight;
    }

    [[nodiscard]] auto GetRect() const -> const toolkit::FloatRect& {
        return m_viewport;
    }

private:
    float m_logicalWidth{};
    float m_logicalHeight{};
    toolkit::FloatRect m_viewport{};
};
}
