
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
            case ErrorType::Fatal: {
                std::string error_message = error.GetErrorMessage() + "\n\nInfinity Package Builder will now quit";
                if (const boxer::Selection selection = boxer::show(error_message.c_str(), "Fatal Error, Application must exit", boxer::Style::Error, boxer::Buttons::OK);
                    selection == boxer::Selection::OK) {
                    if (const auto app = Infinity::Application::Get(); app.has_value()) {
                        (*app)->Close();
                    }
                }
                break;
            }
            case ErrorType::NonFatal: {
                std::string error_message = error.GetErrorMessage() + "\n\nWould you like to quit the Infinity Package Builder? \nNote: certain function might break if you do not restart";
                if (const boxer::Selection selection = boxer::show(error_message.c_str(), "NonFatal Error", boxer::Style::Warning, boxer::Buttons::YesNo); selection == boxer::Selection::Yes) {
                    if (const auto app = Infinity::Application::Get(); app.has_value()) {
                        (*app)->Close();
                    }
                }
                break;
            }
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
