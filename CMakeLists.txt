project(reknote)
cmake_minimum_required(VERSION 2.8.12)
find_package(Qt5Core 5.2.1)
find_package(Qt5Widgets 5.2.1)
find_package(Qt5Xml 5.2.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


#libgit2
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(Libgit2 REQUIRED)
include_directories(${LIBGIT2_INCLUDE_DIR})

set(reknote_SRCS 
  gitrepository.cpp
  historydialog.cpp
  noteitemdelegate.cpp
  notecommands.cpp
  noteview.cpp 
  spike.cpp 
  spikestreemodel.cpp 
  reknote.cpp 
  main.cpp
)
SET(reknote_FORMS
  ui/history.ui
  ui/reknote.ui
)
QT5_WRAP_UI(reknote_FORMS_HDRS ${reknote_FORMS})
qt5_add_resources(reknote_RES "images/res.qrc")

list(APPEND CMAKE_CXX_FLAGS " -std=c++11 -march=native -Wall -pedantic -Wextra -Wundef -Wcast-align -Wwrite-strings -Wunreachable-code -Winit-self ")

add_executable(reknote ${reknote_SRCS} 
		       ${reknote_FORMS_HDRS}
		       ${reknote_RES})
target_link_libraries(reknote ${QT_QTCORE_LIBRARY} ${LIBGIT2_LIBRARIES} Qt5::Widgets Qt5::Xml)
install(TARGETS reknote RUNTIME DESTINATION bin)
