function(check_and_init_submodule submodule_path submodule_name)
    find_program(GIT_EXECUTABLE git)
    if (NOT GIT_EXECUTABLE)
        message(FATAL_ERROR "Git is required to initialize submodules but was not found.")
    endif ()
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${submodule_path}/.git")
        message("${Yellow}${submodule_name} submodule not initialized. Cloning it now...${ColorReset}")
        execute_process(COMMAND git submodule update --init --recursive ${submodule_path}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMODULE_RESULT)
        if (NOT GIT_SUBMODULE_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to initialize submodule: ${submodule_name}. Please ensure Git is available and try again.")
        else ()
            message("${Blue}Submodule: ${Green}${submodule_name} ${Blue}initialized successfully.${ColorReset}")
        endif ()
    else ()
        message("${Blue}Submodule: ${Green}${submodule_name} ${Blue}is initialized.${ColorReset}")
    endif ()
endfunction()


function(load_encryption_key_from_env_file)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.env")
        file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.env" ENV_FILE)

        string(REGEX MATCH "ENCRYPTION_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(ENCRYPTION_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "MASTER_ADMIN_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(ADMIN_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "GROUP_DECRYPT_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(GROUP_DECRYPT_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "AERO_DYNAMICS_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(AERO_DYNAMICS_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "DELTA_SIM_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(DELTA_SIM_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "LUNAR_SIM_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(LUNAR_SIM_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "OUROBOROS_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(OUROBOROS_KEY ${CMAKE_MATCH_1})
        string(REGEX MATCH "QBIT_SIM_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
        set(QBIT_SIM_KEY ${CMAKE_MATCH_1})

        if (ENCRYPTION_KEY)
            message("${Blue}ENCRYPTION_KEY found, defining: ${Green}ENCRYPTION_KEY${ColorReset}")
            add_compile_definitions(ENCRYPTION_KEY="${ENCRYPTION_KEY}")
        else ()
            message(FATAL_ERROR "ENCRYPTION_KEY is not set. Please set it in the .env file.")
        endif ()

        if (ADMIN_KEY)
            message("${Blue}ADMIN_KEY found, defining: ${Green}ADMIN_KEY${ColorReset}")
            add_compile_definitions(ADMIN_KEY="${ADMIN_KEY}")
        else ()
            message(FATAL_ERROR "ADMIN_KEY is not set. Please set it in the .env file.")
        endif ()

        if (GROUP_DECRYPT_KEY)
            message("${Blue}GROUP_DECRYPT_KEY found, defining: ${Green}GROUP_DECRYPT_KEY${ColorReset}")
            add_compile_definitions(GROUP_DECRYPT_KEY="${GROUP_DECRYPT_KEY}")
        else ()
            message(FATAL_ERROR "GROUP_DECRYPT_KEY is not set. Please set it in the .env file.")
        endif ()

        if (AERO_DYNAMICS_KEY)
            message("${Blue}AERO_DYNAMICS_KEY found, defining: ${Green}AERO_DYNAMICS_KEY${ColorReset}")
            add_compile_definitions(AERO_DYNAMICS_KEY="${AERO_DYNAMICS_KEY}")
        else ()
            message(FATAL_ERROR "AERO_DYNAMICS_KEY is not set. Please set it in the .env file.")
        endif ()

        if (DELTA_SIM_KEY)
            message("${Blue}DELTA_SIM_KEY found, defining: ${Green}DELTA_SIM_KEY${ColorReset}")
            add_compile_definitions(DELTA_SIM_KEY="${DELTA_SIM_KEY}")
        else ()
            message(FATAL_ERROR "DELTA_SIM_KEY is not set. Please set it in the .env file.")
        endif ()

        if (LUNAR_SIM_KEY)
            message("${Blue}LUNAR_SIM_KEY found, defining: ${Green}LUNAR_SIM_KEY${ColorReset}")
            add_compile_definitions(LUNAR_SIM_KEY="${LUNAR_SIM_KEY}")
        else ()
            message(FATAL_ERROR "LUNAR_SIM_KEY is not set. Please set it in the .env file.")
        endif ()

        if (OUROBOROS_KEY)
            message("${Blue}OUROBOROS_KEY found, defining: ${Green}OUROBOROS_KEY${ColorReset}")
            add_compile_definitions(OUROBOROS_KEY="${OUROBOROS_KEY}")
        else ()
            message(FATAL_ERROR "OUROBOROS_KEY is not set. Please set it in the .env file.")
        endif ()

        if (QBIT_SIM_KEY)
            message("${Blue}QBIT_SIM_KEY found, defining: ${Green}QBIT_SIM_KEY${ColorReset}")
            add_compile_definitions(QBIT_SIM_KEY="${QBIT_SIM_KEY}")
        else ()
            message(FATAL_ERROR "QBIT_SIM_KEY is not set. Please set it in the .env file.")
        endif ()
    else ()
        message(FATAL_ERROR ".env file is missing. Please create the file with necessary variables.")
    endif ()
endfunction()

function(print_system_dependencies_info)
    if (Vulkan_FOUND)
        message("${Blue}Vulkan Version: ${Green}${Vulkan_VERSION}${ColorReset}")
    else ()
        message(FATAL_ERROR "CURL Not Found")
    endif ()

    if (CURL_FOUND)
        message("${Blue}Curl Version: ${Green}${CURL_VERSION_STRING}${ColorReset}")
    else ()
        message(FATAL_ERROR "CURL Not Found")
    endif ()

    if (OPENSSL_FOUND)
        message("${Blue}Open SSL Version: ${Green}${OPENSSL_VERSION}${ColorReset}")
    else ()
        message(FATAL_ERROR "Open SSL Not Found")
    endif ()

    if (unofficial-minizip_VERSION)
        get_target_property(MINIZIP_INCLUDE_DIR unofficial::minizip::minizip INTERFACE_INCLUDE_DIRECTORIES)

        message("${Blue}MiniZip Version: ${Green}${unofficial-minizip_VERSION}")
    else ()
        message(FATAL_ERROR "MiniZip Not Found")
    endif ()

    #    if (zoe_VERSION)
    #        message("${Blue}Zoe Version: ${Green}${zoe_VERSION} ${ColorReset}")
    #    else ()
    #        message(FATAL_ERROR "Zoe Not Found")
    #    endif ()

    if (WIN32)
        if (WinToast_LIBRARY_DEBUG)
            message("${Blue}WinToast Found: ${Green}${WinToast_LIBRARY_DEBUG} ${ColorReset}")
        elseif (WinToast_LIBRARY_RELEASE)
            message("${Blue}WinToast Found: ${Green}${WinToast_LIBRARY_RELEASE} ${ColorReset}")
        else ()
            message(FATAL_ERROR "WinToast Not Found")
        endif ()
    endif ()
endfunction()