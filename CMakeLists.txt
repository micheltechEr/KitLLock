# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico CACHE STRING "Board type")  # Ajuste se for necessário usar pico_w ou pico2

# Importa o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

project(KitLock C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona os arquivos do projeto
add_executable(KitLock
    KitLock.c
    inc/ssd1306.c   # Mantendo inc conforme solicitado
)

# Define nome e versão do programa
pico_set_program_name(KitLock "KitLock")
pico_set_program_version(KitLock "0.1")

# Habilita saída via UART/USB
pico_enable_stdio_uart(KitLock 1)
pico_enable_stdio_usb(KitLock 1)

# Linka as bibliotecas necessárias
target_link_libraries(KitLock
    pico_stdlib       # Biblioteca padrão do Pico
    hardware_gpio     # Controle de LEDs, botões e buzzer
    hardware_i2c      # Comunicação com o display OLED SSD1306
    hardware_pio
)

# Adiciona diretórios de includes
target_include_directories(KitLock PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc   # Mantendo inc conforme solicitado
)

# Gera formatos binários extras (UF2, bin, etc.)
pico_add_extra_outputs(KitLock)
