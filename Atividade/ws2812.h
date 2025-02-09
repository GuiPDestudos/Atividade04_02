#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware/i2c.h"

#define SSD1306_I2C_ADDR  0x3C  // Endereço padrão do display
#define SSD1306_WIDTH     128
#define SSD1306_HEIGHT    64

typedef struct {
    i2c_inst_t *i2c;
    uint8_t address;
    uint8_t width;
    uint8_t height;
    uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
} ssd1306_t;

void ssd1306_init(ssd1306_t *oled, i2c_inst_t *i2c, uint sda, uint scl);
void ssd1306_clear(ssd1306_t *oled);
void ssd1306_update(ssd1306_t *oled);
void ssd1306_draw_pixel(ssd1306_t *oled, uint8_t x, uint8_t y, bool color);
void ssd1306_write_text(ssd1306_t *oled, const char *text, uint8_t x, uint8_t y);

#endif // SSD1306_H
