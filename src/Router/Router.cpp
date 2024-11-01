
#include "Router.hpp"


namespace InfinityPackageBuilder::Utils {
    std::unique_ptr<Router> Router::m_Instance = nullptr;

    void Router::configure(std::unordered_map<int, std::function<void()>> pages) {
        if (!m_Instance) {
            m_Instance = std::unique_ptr<Router>(new Router(std::move(pages)));
        }
    }

    Router &Router::getInstance() {
        if (!m_Instance) {
            throw std::runtime_error("Router is not configured. Call configure() before trying to get the instance.");
        }
        return *m_Instance;
    }

    bool Router::setPage(const int pageId) {
        if (m_Pages.contains(pageId)) {
            m_CurrentPageID = pageId;
            return true;
        } else {
            std::ostringstream oss;
            oss << "Error: Attempted to access page ID " << pageId;
            throw std::runtime_error(oss.str());
        }
    }

    int Router::getPage() const { return m_CurrentPageID; }

    void Router::RenderCurrentPage() {
        if (m_Pages.contains(m_CurrentPageID)) {
            m_Pages[m_CurrentPageID]();
        } else {
            throw std::runtime_error("Failed to render page: Page does not exist.");
        }
    }

    Router::Router(std::unordered_map<int, std::function<void()>> pages) :
        m_CurrentPageID(0), m_Pages(std::move(pages)) {}
} // namespace InfinityPackageBuilder::Utils
