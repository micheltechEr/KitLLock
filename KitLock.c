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

#define MAX_ATTEMPTS       3
#define PASSWORD           "KITLAB"

ssd1306_t display;

void setup_leds() {
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_put(LED_PIN_GREEN, 0);
    gpio_put(LED_PIN_RED, 0);
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
    ssd1306_fill(&display, false);
    ssd1306_draw_string(&display, message, 0, 0);
    ssd1306_send_data(&display);
}

// Função para descartar caracteres residuais na entrada
void flush_input_buffer() {
    while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT);
}

int main() {
    int attempts = 0;
    int pos = 0;
    const int password_len = strlen(PASSWORD);
    
    stdio_init_all();
    sleep_ms(2000);
    
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    
    printf("Sistema iniciado.\n");
    setup_leds();
    
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(DISPLAY_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_I2C_SDA);
    gpio_pull_up(DISPLAY_I2C_SCL);
    
    ssd1306_init(&display, OLED_WIDTH, OLED_HEIGHT, false, OLED_ADDR, I2C_PORT);
    ssd1306_config(&display);
    
    while (attempts < MAX_ATTEMPTS) {
        display_message("Digite a senha:");
        printf("\nTentativa %d/%d - Digite a senha: ", attempts + 1, MAX_ATTEMPTS);
        fflush(stdout);
        
        pos = 0;  // Reinicia a posição para a nova tentativa
        
        // Loop para ler a senha caractere a caractere
        while (pos < password_len) {
            char ch = getchar_timeout_us(0xFFFFFFFF); // Aguarda entrada
            if (ch == PASSWORD[pos]) {
                pos++;
                printf("*");  // Feedback: exibe * para cada caractere correto
                fflush(stdout);
            } else {
                printf("\n❌ Senha incorreta\n");
                display_message("Senha Incorreta");
                led_red_on();
                sleep_ms(1000);
                flush_input_buffer();  // Limpa o buffer de entrada
                break;  // Sai do loop se houver erro
            }
        }
        
        if (pos == password_len) {
            display_message("Acesso Permitido");
            led_green_on();
            printf("\n✅ Acesso Permitido!\n");
            return 0;
        } else {
            attempts++; // Incrementa a tentativa apenas se a senha estiver incorreta
        }
    }
    
    display_message("Acesso Bloqueado");
    led_red_on();
    printf("\n⛔ Acesso Bloqueado\n");
    
    return 0;
}
