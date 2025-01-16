cmake_minimum_required(VERSION 3.28)
project(programacao_concorrente C)

set(CMAKE_C_STANDARD 11)

set(SOURCES
        src/main.c
        src/gestao-pedidos.c
        src/gestao-estoque.c
        src/reserva-mesa.c
        src/atendimento.c
        src/pedidos-delivery.c
        src/gestao-pedidos.c)

add_executable(programacao_concorrente ${SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(programacao_concorrente PRIVATE Threads::Threads)

target_compile_options(programacao_concorrente PRIVATE -Wall -pthread)

