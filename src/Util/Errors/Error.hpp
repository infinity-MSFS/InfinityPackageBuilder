
#pragma once

#include <string>
#include <utility>

#include "Backend/Application/Application.hpp"
#include "boxer/boxer.h"


namespace Infinity {

    enum class ErrorType {
        Fatal,
        NonFatal,
        Warning,
    };


    struct Error {
        ErrorType error_type;
        std::string error_message;


        Error(ErrorType error_type, std::string message) : error_type(error_type), error_message(std::move(message)) {}
        [[nodiscard]] ErrorType GetErrorType() const { return error_type; }
        [[nodiscard]] std::string &GetErrorMessage() { return error_message; }

        void Dispatch();
    };


    inline void ShowErrorPopup(Error &error) {
        switch (error.GetErrorType()) {
            case ErrorType::Fatal:
                if (const boxer::Selection selection = boxer::show(error.GetErrorMessage().c_str(), "Fatal Error", boxer::Style::Error, boxer::Buttons::Quit); selection == boxer::Selection::Quit) {
                    if (const auto app = Infinity::Application::Get(); app.has_value()) {
                        (*app)->Close();
                    }
                }
                break;
            case ErrorType::NonFatal:
                if (const boxer::Selection selection = boxer::show(error.GetErrorMessage().c_str(), "NonFatal Error", boxer::Style::Error, boxer::Buttons::OK); selection == boxer::Selection::Quit) {
                    // if (const auto app = Infinity::Application::Get(); app.has_value()) {
                    //     static_cast<InfinityRenderer::Application *>(*app)->Close();
                    // }
                }
                break;
            case ErrorType::Warning:
                boxer::show(error.GetErrorMessage().c_str(), "Warning", boxer::Style::Warning, boxer::Buttons::OK);
                break;
            default:
                break;
        }
    }


    inline void ShowErrorPopup(const ErrorType error_type, const std::string &error_message) {
        auto error = Error{error_type, error_message};
        ShowErrorPopup(error);
    }

    inline void Error::Dispatch() { ShowErrorPopup(*this); }


} // namespace Infinity
