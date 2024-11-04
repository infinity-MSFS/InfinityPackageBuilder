
#pragma once

#include <expected>
#include <functional>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>


namespace InfinityPackageBuilder::Utils {
    class Router {
    public:
        static void configure(std::unordered_map<int, std::function<void()>> pages);

        static std::optional<Router *> getInstance();

        std::expected<bool, std::string> setPage(int pageId);

        [[nodiscard]] std::optional<int> getPage() const;

        void RenderCurrentPage();

    private:
        explicit Router(std::unordered_map<int, std::function<void()>> pages);

        static std::unique_ptr<Router> m_Instance;
        int m_CurrentPageID;
        std::unordered_map<int, std::function<void()>> m_Pages;
    };
} // namespace InfinityPackageBuilder::Utils
