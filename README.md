# Infinity Package Manger

Welcome to the **Infinity Package Manager**!

---

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Requirements](#requirements)
- [Contribution Guidelines](#contribution-guidelines)
    - [Project Structure and Layers](#project-structure-and-layers)
    - [Coding Standards](#coding-standards)
    - [Asset Management](#asset-management)
    - [Asynchronous Operations](#non-blocking-practices)
    - [Code Formatting](#code-formatting)
- [License](#license)

---

## Project Overview

The **Infinity Package Manager** uses Vulkan and ImGui to deliver a high-performance, responsive graphical user interface. This tool is ideal for Infinity Launcher devs that would like a near seamless
approach to building, diffing, and distributing their aircraft from a single program.

---

## Features:

### Package Builder

- Build MSFS aircraft packages without directly using the MSFS SDK.
- Overview of many project settings in a much more user-friendly UI.
- Ease the versioning process.

### Package Differ

- Create diffs for projects that match the **Infinity MSFS** Diff standard.
- Cache Previous versions or automatically download prior versions to base diffs

### Release Publisher

- Creates and publishes diffs in proper format for the **Infinity Launcher** using either GitHub (default) or CDNs like CloudFare using a custom **Lua** script. The release manager exposes the
  necessary methods for handling API keys, account management and uploading.

### Launcher JSON Manager

- Port of the existing launcher JSON manager, allows project managers to edit their page through a GUI.

---

## Requirements

### Building

Ensure you have the following prerequisites:

- **C++23 or newer** compiler (for async/await, optional and expected)
- **Vulkan SDK** **You must set the environment variable `VULKAN_SDK` to your vulkan SDK path**
- **CMake** (minimum version 3.28)
- **Vcpkg** Will save you from the common windows + c++ nightmare
- **OpenSSL** install via `yay -S openssl` on linux or `vcpkg install openssl` on windows
- **LibCurl** install via `yay -S curl` on linux or `vcpkg install curl:x64-windows` on windows

#### Setup

- **`.env`** should contain a single entry of `ENCRYPTION_KEY=01234567890123456789012345678901` containing a 256-bit key. A fatal error will be thrown if the .env file is not setup properly.

#### Linux

- Pass either `-DINFINITY_USE_X11=ON -DINFINITY_USE_WAYLAND=OFF` or `-DINFINITY_USE_X11=OFF -DINFINITY_USE_WAYLAND=ON` for Linux build.`

### Running

- Prebuilt binary if not building from source
- **Latest [C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe)**

---

## Contribution Guidelines

### Project Structure and Layers

1. **Layered Page Development**
    - Every new feature should be developed within a dedicated page/layer.
    - Ensure that each layer has its own defined resize logic.
    - Layers that need intercommunication can achieve this with a shared memory structure that is constructed in `main.cpp`

2. **Separation of Concerns**
    - For modularity and clarity, split each feature into distinct parts: **GUI layer** and **Backend Function**.
    - Every page and major GUI component should have its own layer and respective source file.
    - Each layer should follow the example layers found in `main.cpp` which extend a class `InfinityRenderer::Layer`.

### Coding Standards

To maintain clean and readable code across contributions, we enforce the following coding standards:

- **Consistent Naming Conventions**
    - All naming of Classes, Functions, and Structs should follow PascalCase.
    - Variable naming should follow Hungarian notation i.e. `m_Variable` for member variable and `g_Variable` for global variable.
    - local variables should follow snake_case.
- **Null Prevention / Value Presence**
    - Use `std::optional` to represent values that may or may not be present, rather than relying on null pointers or ambiguous default values. This makes it clear when a variable or return value is
      optional.
- **Error handling**
    - **Use `std::expected` for Rich Error Reporting:** When an operation may fail with a known error state, use `std::expected<T, E>` to provide detailed error information `(E)` instead of only
      using
      a
      success boolean or null value.
    - **Return Meaningful Error Types:** Avoid raw string or int values for the error case of `std::expected`, Prefer to use the `InfinityPackageBuilder::Errors::Error` struct which can be
      directly piped into a warning or error popup
        ```cpp
       enum class ErrorType {
         Fatal,
         NonFatal,
         Warning,
      };
          
      struct Error {
         ErrorType error_type;
         std::string error_message;
  
         [[nodiscard]] ErrorType GetErrorType() const { return error_type; }
         [[nodiscard]] std::string &GetErrorMessage() { return error_message; }
      };
      ```

### Asset Management

We prioritize **memory-based asset management** over file-based systems for faster access and improved application portability.

- **In-Memory Assets**
    - Avoid loading assets directly from the filesystem where possible. Prefer to encode the file in a header and load into memory.

- **Asset Pooling**
    - Consider pooling or caching assets in memory to avoid redundancy and improve performance.

### Non-Blocking Practices

To enhance the responsiveness of the application, all backend tasks should be **non-blocking**:

- **Async/Await method**
    - Tasks that require action upon completion should be handled in the c++20 async/await format.
    - **Note:** Callbacks are still an acceptable way to handle an incoming async task;

- **Multithreaded method**
    - For tasks that can "break off" the main thread (void functions) running your function in a detached thread is an easy way to execute time-consuming functions without blocking the main thread.

### Code Formatting

We use **Clang-Format** for code formatting. Please ensure that all contributions adhere to the project's style:

1. Install Clang-Format (version X.X or newer).
2. Use the provided `.clang-format` configuration file in the project root to automatically apply the required styling.
3. Run Clang-Format on any code you contribute:
   ```bash
   clang-format -i <file-name.cpp>
    ```
4. (Optional) Many IDEs such as CLion implement Clang-format on save

---

## License

This project is licensed under the MIT License. For more details, see the [LICENSE](LICENSE) file.