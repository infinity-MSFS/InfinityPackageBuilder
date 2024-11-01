
#pragma once

#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <memory>
#include <sstream>


namespace InfinityPackageBuilder::Utils {
    class Router {
    public:
        static void configure(std::unordered_map<int, std::function<void()> > pages);

        static Router &getInstance();

        bool setPage(int pageId);

        [[nodiscard]] int getPage() const;

        void RenderCurrentPage();

    private:
        explicit Router(std::unordered_map<int, std::function<void()> > pages);

        static std::unique_ptr<Router> m_Instance;
        int m_CurrentPageID;
        std::unordered_map<int, std::function<void()> > m_Pages;
    };
}
