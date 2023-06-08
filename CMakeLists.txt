cmake_minimum_required(VERSION 3.22)

#add_compile_options(-Ofast)
#add_link_options(-Ofast)

add_compile_options(-Ofast -flto)
add_link_options(-Ofast -flto)

#add_compile_options(-Wall -O0 -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -D_GLIBCXX_DEBUG -Wconversion -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -g -rdynamic)
#add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize-recover)
#add_link_options(-fsanitize=address -fsanitize=undefined -fno-sanitize-recover)
#add_link_options(-rdynamic)

project(backup)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
