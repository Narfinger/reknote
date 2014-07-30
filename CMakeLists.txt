project(reknote)
cmake_minimum_required(VERSION 2.8.12)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


set(reknote_SRCS 
  reknote.cpp 
  main.cpp
)
SET(reknote_FORMS
  ui/reknote.ui
)
QT4_WRAP_UI(reknote_FORMS_HDRS ${reknote_FORMS})

add_executable(reknote ${reknote_SRCS} 
		       ${reknote_FORMS_HDRS})
target_link_libraries(reknote ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS reknote RUNTIME DESTINATION bin)
