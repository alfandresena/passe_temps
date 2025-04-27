cmake_minimum_required(VERSION 3.14)
project(DbManagerProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Définir la politique pour éviter l'avertissement CMP0135
cmake_policy(SET CMP0135 NEW)

# Forcer l'utilisation de /MD pour Release et /MDd pour Debug
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Définir les répertoires de sortie
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})

# Trouver les packages Qt nécessaires
find_package(Qt6 REQUIRED COMPONENTS Core Sql)

# Sources de la bibliothèque DBManager
set(DBMANAGER_SOURCES
    dbmanager.cpp
    dbmanager.h
)

# Créer la bibliothèque
add_library(dbmanager STATIC ${DBMANAGER_SOURCES})
target_link_libraries(dbmanager PRIVATE Qt6::Core Qt6::Sql)

# Configuration pour les tests
enable_testing()

# Utiliser FetchContent pour télécharger et configurer GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

# Vérifier que GoogleTest est correctement récupéré
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  message(FATAL_ERROR "Échec de la récupération de GoogleTest")
endif()

# Créer l'exécutable de test
add_executable(dbmanager_test dbmanager_test.cpp)
target_link_libraries(dbmanager_test PRIVATE
    dbmanager
    Qt6::Core
    Qt6::Sql
    gtest
    gtest_main
)

# Ajouter le test au CTest
add_test(NAME DBManagerTests COMMAND dbmanager_test)
