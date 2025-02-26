#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

#define DISPLAY_I2C_SDA    14
#define DISPLAY_I2C_SCL    15
#define OLED_WIDTH         128
#define OLED_HEIGHT        64
#define OLED_ADDR          0x3C
#define I2C_PORT           i2c1

#define LED_PIN_GREEN      11
#define LED_PIN_RED        13
#define BUZZER_PIN         10  

#define MAX_ATTEMPTS       3
#define PASSWORD           "KITLAB"
#define MAX_COMMAND_LEN    20

ssd1306_t display;

void setup_leds() {
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_put(LED_PIN_GREEN, 0);
    gpio_put(LED_PIN_RED, 0);
}

void setup_buzzer() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);
}

void beep(int times, int duration) {
    for (int i = 0; i < times; i++) {
        gpio_put(BUZZER_PIN, 1);
        sleep_ms(duration);
        gpio_put(BUZZER_PIN, 0);
        sleep_ms(duration);
    }
}

void led_green_on() {
    gpio_put(LED_PIN_GREEN, 1);
    gpio_put(LED_PIN_RED, 0);
}

void led_red_on() {
    gpio_put(LED_PIN_RED, 1);
    gpio_put(LED_PIN_GREEN, 0);
}

void display_message(const char *message) {
    ssd1306_fill(&display, false);           // Limpa o display
    ssd1306_draw_string(&display, message, 0, 0); // Desenha a string na posição (0,0)
    ssd1306_send_data(&display);             // Envia os dados para o display
}

void clear_serial_buffer() {
    while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT) {
        // Loop vazio para consumir caracteres
    }
}

int read_input(char *buffer, int max_length, uint32_t timeout) {
    int pos = 0;
    int32_t ch;  // Usando int32_t para lidar com PICO_ERROR_TIMEOUT

    while (pos < max_length - 1) {
        ch = getchar_timeout_us(timeout);
        
        if (ch == PICO_ERROR_TIMEOUT) {
            if (pos > 0) {
                buffer[pos] = '\0';
                return pos;
            }
            continue;
        }

        if (ch == '\r' || ch == '\n') {
            buffer[pos] = '\0';
            return pos;
        }

        if (ch >= 32 && ch <= 126) {  // Apenas caracteres imprimíveis
            buffer[pos++] = (char)ch;
            putchar('*');  // Eco visual com asterisco
        }
    }

    buffer[pos] = '\0';
    return pos;
}

int read_command() {
    char command[MAX_COMMAND_LEN] = {0};
    const char *valid_commands[] = {"status", "mensagem", "sair"};
    const int num_commands = 3;

    printf("\nDigite um comando (status, mensagem, sair): ");
    int len = read_input(command, MAX_COMMAND_LEN, 1000000);  // 1 segundo de timeout
    
    if (len == 0) return -1;
    
    printf("\n[DEBUG] Comando lido: '%s'\n", command);

    for (int i = 0; i < num_commands; i++) {
        if (strcmp(command, valid_commands[i]) == 0) {
            return i;
        }
    }
    
    return -1;
}

int read_password() {
    char entered_password[sizeof(PASSWORD)] = {0};  // Tamanho baseado na senha
    
    printf("\nDigite a senha: ");
    int len = read_input(entered_password, sizeof(entered_password), 1000000);  // 1 segundo de timeout
    
    if (len == 0) return 0;
    
    return strcmp(entered_password, PASSWORD) == 0;  // Retorna 1 se igual, 0 se diferente
}

int main() {
    stdio_init_all();
    sleep_ms(2000);  // Aguarda inicialização do USB/serial
    
    setvbuf(stdin, NULL, _IONBF, 0);   // Desabilita buffer de entrada
    setvbuf(stdout, NULL, _IONBF, 0);  // Desabilita buffer de saída
    
    printf("Sistema iniciado.\n");
    setup_leds();
    setup_buzzer();

    // Configuração do I2C
    i2c_init(I2C_PORT, 100000);  // 100kHz
    gpio_set_function(DISPLAY_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_I2C_SDA);
    gpio_pull_up(DISPLAY_I2C_SCL);
    
    // Inicialização do display
    ssd1306_init(&display, OLED_WIDTH, OLED_HEIGHT, false, OLED_ADDR, I2C_PORT);
    ssd1306_config(&display);

    int attempts = 0;
    while (1) {
        attempts = 0;  // Reseta tentativas a cada ciclo completo

        while (attempts < MAX_ATTEMPTS) {
            display_message("Digite a senha:");

            if (read_password()) {
                display_message("Acesso Permitido");
                led_green_on();
                beep(1, 200);
                printf("\n✅ Acesso Permitido!\n");
                clear_serial_buffer();

                while (1) {
                    int command_result = read_command();

                    switch (command_result) {
                        case 0:  // status
                            display_message("Sistema OK!");
                            printf("✅ Sistema OK!\n");
                            break;
                        case 1:  // mensagem
                            display_message("Embarcatech 2025");
                            printf("Embarcatech 2025");
                            break;
                        case 2:  // sair
                            display_message("Bloqueando...");
                            printf("🔒 Bloqueando novamente...\n");
                            sleep_ms(1000);
                            led_red_on();
                            goto exit_command_loop;
                        default:
                            display_message("Comando invalido!");
                            printf("❌ Comando invalido!\n");
                            break;
                    }
                    sleep_ms(1000);  // Delay para evitar comandos muito rápidos
                }
                exit_command_loop:
                    attempts = 0;  // Reseta após logout
            } else {
                printf("\n❌ Senha incorreta\n");
                display_message("Senha Incorreta");
                led_red_on();
                beep(2, 100);
                sleep_ms(1000);
                attempts++;
            }
        }

        display_message("Acesso Bloqueado");
        led_red_on();
        beep(3, 100);
        printf("\n⛔ Acesso Bloqueado! Aguarde 30 segundos...\n");
        sleep_ms(30000);  // Bloqueio por 30 segundos
    }

    return 0;  // Nunca alcançado, mas mantido por boa prática
}