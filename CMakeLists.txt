cmake_minimum_required(VERSION 3.28)
project(programacao_concorrente C)

set(CMAKE_C_STANDARD 11)

add_executable(programacao_concorrente src/atendimento.h
        src/gestao-pedidos.h
        src/atendimento.h
        src/gestao-estoque.h
        src/pedidos-delivery.h
        src/reserva-mesa.h)

