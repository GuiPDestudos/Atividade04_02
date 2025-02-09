#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware/i2c.h"

#define SSD1306_I2C_ADDR  0x3C  // Endereço I2C do SSD1306
#define SSD1306_WIDTH     128
#define SSD1306_HEIGHT    64

void ssd1306_init(i2c_inst_t *i2c, uint sda, uint scl);
void ssd1306_clear(void);
void ssd1306_update(void);
void ssd1306_draw_pixel(uint8_t x, uint8_t y, bool color);
void ssd1306_write_text(const char *text, uint8_t x, uint8_t y);

#endif // SSD1306_H
