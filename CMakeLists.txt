cmake_minimum_required(VERSION 3.0)
project(PassGuard)

# Trouver et inclure la bibliothèque GTKMM
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# Inclure la bibliothèque nlohmann/json
find_package(nlohmann_json REQUIRED)

# Inclure la bibliothèque OpenSSL
find_package(OpenSSL REQUIRED)

# Ajouter les répertoires d'inclusion
include_directories(${GTKMM_INCLUDE_DIRS} ${nlohmann_json_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

# Ajouter les répertoires de bibliothèques
link_directories(${GTKMM_LIBRARY_DIRS})

# Créer l'exécutable
add_executable(PassGuard main.cpp json_functions.cpp sslfun.cpp passwords_functions.cpp helper.cpp)
set_target_properties(PassGuard PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Lier les bibliothèques
target_link_libraries(PassGuard ${GTKMM_LIBRARIES} nlohmann_json::nlohmann_json ${OPENSSL_LIBRARIES})