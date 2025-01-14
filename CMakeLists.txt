cmake_minimum_required(VERSION 3.28)
project(programacao_concorrente C)

set(CMAKE_C_STANDARD 11)

set(SOURCES
        main.c
        gestao_pedidos.c
        gestao_estoque.c
        reserva_mesa.c
        atendimento.c
        pedidos-delivery.c)

add_executable(programacao_concorrente ${SOURCES})

target_compile_options(programacao_concorrente PRIVATE -Wall -pthread)

