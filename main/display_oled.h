#include "ssd1306.h"

void display_init(SSD1306_t * dev, gpio_num_t CONFIG_SDA_GPIO, gpio_num_t CONFIG_SCL_GPIO, bool CONFIG_RESET_GPIO)
{
	i2c_master_init(dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
	ssd1306_init(dev, 128, 64);

	ssd1306_contrast(dev, 0xff);
	ssd1306_clear_screen(dev, false);
}