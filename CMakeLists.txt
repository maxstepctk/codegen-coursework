# CMakeList.txt: проект CMake для codegen-coursework; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("codegen-coursework")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (codegen-coursework "src/codegen-coursework.cpp" "src/codegen-coursework.h" "src/dynarray.h" "src/string.h" "src/binarytree.h" "src/stack.h" "src/syntaxtree.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET codegen-coursework PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
