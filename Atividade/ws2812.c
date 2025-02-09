// ssd1306.c
#include "ssd1306.h"
#include <string.h>
#include "hardware/i2c.h"
#include "hardware/gpio.h"

static i2c_inst_t *ssd1306_i2c;

void ssd1306_init(i2c_inst_t *i2c, uint sda, uint scl) {
    ssd1306_i2c = i2c;
    i2c_init(ssd1306_i2c, 400 * 1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);
    
    uint8_t init_cmds[] = {
        0xAE, // Display off
        0x20, 0x00, // Memory addressing mode: horizontal
        0xB0, // Page start address
        0xC8, // COM output scan direction
        0x00, 0x10, // Low and high column address
        0x40, // Start line address
        0x81, 0xFF, // Contrast control
        0xA1, // Segment re-map
        0xA6, // Normal display
        0xA8, 0x3F, // Multiplex ratio
        0xA4, // Output follows RAM content
        0xD3, 0x00, // Display offset
        0xD5, 0x80, // Display clock divide ratio
        0xD9, 0xF1, // Pre-charge period
        0xDA, 0x12, // COM pins hardware configuration
        0xDB, 0x40, // VCOMH deselect level
        0x8D, 0x14, // Charge pump setting
        0xAF // Display on
    };
    i2c_write_blocking(ssd1306_i2c, SSD1306_I2C_ADDR, init_cmds, sizeof(init_cmds), false);
}

void ssd1306_clear(void) {
    uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8] = {0};
    i2c_write_blocking(ssd1306_i2c, SSD1306_I2C_ADDR, buffer, sizeof(buffer), false);
}

void ssd1306_draw_pixel(int x, int y, bool color) {
    // Implementação do desenho de pixel no buffer
}

void ssd1306_show(void) {
    // Atualiza o display com os dados do buffer
}

void ssd1306_write_text(int x, int y, const char *text) {
    // Implementação para escrever texto na tela
}
