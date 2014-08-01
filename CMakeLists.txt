project(reknote)
cmake_minimum_required(VERSION 2.8.12)
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Xml)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


set(reknote_SRCS 
  spike.cpp 
  spikestreemodel.cpp 
  reknote.cpp 
  main.cpp
)
SET(reknote_FORMS
  ui/reknote.ui
)
QT5_WRAP_UI(reknote_FORMS_HDRS ${reknote_FORMS})

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

add_executable(reknote ${reknote_SRCS} 
		       ${reknote_FORMS_HDRS})
target_link_libraries(reknote ${QT_QTCORE_LIBRARY} Qt5::Widgets Qt5::Xml)
install(TARGETS reknote RUNTIME DESTINATION bin)
