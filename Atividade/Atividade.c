#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include "ws2812.h"
#include "ssd1306.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define SSD1306_ADDRESS 0x3C

#define BTN_A 5
#define BTN_B 6

#define LED_R 11
#define LED_G 12
#define LED_B 13

#define WS2812_PIN 7
#define WS2812_NUM_LEDS 25

volatile bool btn_a_pressed = false;
volatile bool btn_b_pressed = false;
bool led_green_state = false;
bool led_blue_state = false;

ssd1306_t oled; 

void debounce_timer_callback(uint gpio, uint32_t events) {
    sleep_ms(50); // Debounce delay
    if (gpio_get(gpio) == 0) { 
        if (gpio == BTN_A) btn_a_pressed = true;
        if (gpio == BTN_B) btn_b_pressed = true;
    }
}

void button_irq_handler(uint gpio, uint32_t events) {
    add_alarm_in_ms(50, (alarm_callback_t)debounce_timer_callback, (void *)(uintptr_t)gpio, false);
}

void toggle_led(uint gpio, bool *state, const char *msg) {
    *state = !(*state);
    gpio_put(gpio, *state);
    printf("%s %s\n", msg, *state ? "Ligado" : "Desligado");

    ssd1306_clear(&oled);
    ssd1306_draw_string(&oled, 0, 0, msg, 1);
    ssd1306_draw_string(&oled, 0, 10, *state ? "Ligado" : "Desligado", 1);
    ssd1306_show(&oled);
}

void setup() {
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Configuração dos LEDs
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);

    // Configuração dos botões
    gpio_init(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_pull_up(BTN_B);

    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);

    // ** Inicializa I2C antes de configurar os pinos **
    i2c_init(I2C_PORT, 400 * 1000);  

    // ** Define funções GPIO corretamente **
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Inicializa o display SSD1306
    ssd1306_init(&oled, I2C_PORT, 128, 64);

    // Inicializa os LEDs WS2812
    ws2812_init(WS2812_PIN);

}

void display_ws2812_digit(uint8_t digit) {
    uint32_t colors[10] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0xFFFFFF, 0x888888, 0x444444, 0x222222};
    
    ws2812_clear();
    for (int i = 0; i < WS2812_NUM_LEDS; i++) {
        ws2812_set_pixel(i, colors[digit]);
    }
    ws2812_show();
}

void serial_input_handler() {
    if (uart_is_readable(UART_ID)) {
        char ch = uart_getc(UART_ID);
        printf("Recebido: %c\n", ch);

        ssd1306_clear(&oled);
        ssd1306_draw_char(&oled, 50, 30, ch, 2);
        ssd1306_show(&oled);

        if (ch >= '0' && ch <= '9') {
            display_ws2812_digit(ch - '0');
        }
    }
}

int main() {
    setup();

    while (1) {
        serial_input_handler();

        if (btn_a_pressed) {
            toggle_led(LED_G, &led_green_state, "LED Verde:");
            btn_a_pressed = false;
        }
        if (btn_b_pressed) {
            toggle_led(LED_B, &led_blue_state, "LED Azul:");
            btn_b_pressed = false;
        }

        sleep_ms(10);
    }
}