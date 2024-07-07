cmake_minimum_required(VERSION 3.5)

# Nome del progetto
project(DinamometerQt)

# Imposta il percorso di Qt5
set(CMAKE_PREFIX_PATH "/usr/lib/arm-linux-gnueabihf/cmake/Qt5")
set(Qt5_DIR "/usr/lib/arm-linux-gnueabihf/cmake/Qt5")
set(QT_RCC_EXECUTABLE "/usr/lib/qt5/bin/rcc")

# Imposta il C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Alibilita AUTOMOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON) # Abilita l'automazione per i file qrc
set(CMAKE_BUILD_TYPE Debug)

#directory contenente i file header
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)

#Aggiungi file sorgente
file(GLOB SOURCES "src/*.cpp")

#Aggiungi file headers
file(GLOB HEADERS "include/*.h")

#Aggiunta resources
set(RESOURCES "resources/images.qrc")

#Crea l'eseguibile
add_executable(DinamometerQt ${SOURCES} ${HEADERS} ${RESOURCES})

#Linka le librerie Qt necessarie
find_package(Qt5 COMPONENTS Core Widgets Svg REQUIRED)
target_link_libraries(DinamometerQt Qt5::Core Qt5::Widgets Qt5::Svg)
