cmake_minimum_required(VERSION 3.28)
project(programacao_concorrente C)

set(CMAKE_C_STANDARD 11)

add_executable(programacao_concorrente src/gestao-estoque.c
        src/atendimento.c
        src/pedidos-delivery.c)

