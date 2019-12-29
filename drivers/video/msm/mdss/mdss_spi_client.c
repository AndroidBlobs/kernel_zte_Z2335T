/* Copyright (c) 2017, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/qpnp/pin.h>
#include <linux/delay.h>

#define SPI_PANEL_CS_GPIO	18
static int g_spi_cs_gpio = -1;
#define MAX_READ_SPEED_HZ	4800000
#ifdef ZTE_FEATURE_SPI_PANEL_SPEED_HZ
static u32 max_write_speed_hz = ZTE_FEATURE_SPI_PANEL_SPEED_HZ;
#endif
#define SPI_PANEL_COMMAND_LEN	1
static struct spi_device *mdss_spi_client;

int mdss_spi_read_data(u8 reg_addr, u8 *data, u8 len)
{
	int rc = 0;
	u32 max_speed_hz;
	u8 memory_write_reg = 0x2c;

	struct spi_transfer t[3] = {
		[0] = {
			.tx_buf = &reg_addr,
			.len = 1,
		},
		[1] = {
			.rx_buf = data,
			.len = len,
		},
		[2] = {
			.tx_buf = &memory_write_reg,
			.len = 1,
		}
	};
	struct spi_message m;

	if (!mdss_spi_client) {
		pr_err("%s: spi client not available\n", __func__);
		return -EINVAL;
	}

	mdss_spi_client->bits_per_word = 8;
	max_speed_hz = mdss_spi_client->max_speed_hz;
	mdss_spi_client->max_speed_hz = MAX_READ_SPEED_HZ;
	gpio_set_value(g_spi_cs_gpio, 0);
	spi_message_init(&m);
	spi_message_add_tail(&t[0], &m);
	rc = spi_sync(mdss_spi_client, &m);
	spi_message_init(&m);
	spi_message_add_tail(&t[1], &m);
	rc = spi_sync(mdss_spi_client, &m);
	spi_message_init(&m);
	spi_message_add_tail(&t[2], &m);
	rc = spi_sync(mdss_spi_client, &m);
	gpio_set_value(g_spi_cs_gpio, 1);
	mdss_spi_client->max_speed_hz = max_speed_hz;

	return rc;
}

int mdss_spi_tx_command(const void *buf)
{
	int rc = 0;
	struct spi_transfer t = {
		.tx_buf = buf,
		.len    = SPI_PANEL_COMMAND_LEN,
	};
	struct spi_message m;

	if (!mdss_spi_client) {
		pr_err("%s: spi client not available\n", __func__);
		return -EINVAL;
	}
#ifdef ZTE_FEATURE_SPI_PANEL_SPEED_HZ
	max_write_speed_hz = mdss_spi_client->max_speed_hz;
	mdss_spi_client->max_speed_hz = ZTE_FEATURE_SPI_PANEL_SPEED_HZ;
#endif
	mdss_spi_client->bits_per_word = 8;
	gpio_set_value(g_spi_cs_gpio, 0);
	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	rc = spi_sync(mdss_spi_client, &m);
	gpio_set_value(g_spi_cs_gpio, 1);
#ifdef ZTE_FEATURE_SPI_PANEL_SPEED_HZ
	mdss_spi_client->max_speed_hz = max_write_speed_hz;
#endif

	return rc;
}

int mdss_spi_tx_parameter(const void *buf, size_t len)
{
	int rc = 0;
	struct spi_transfer t = {
		.tx_buf = buf,
		.len    = len,
	};
	struct spi_message m;

	if (!mdss_spi_client) {
		pr_err("%s: spi client not available\n", __func__);
		return -EINVAL;
	}
#ifdef ZTE_FEATURE_SPI_PANEL_SPEED_HZ
	max_write_speed_hz = mdss_spi_client->max_speed_hz;
	mdss_spi_client->max_speed_hz = ZTE_FEATURE_SPI_PANEL_SPEED_HZ;
#endif
	mdss_spi_client->bits_per_word = 8;
	gpio_set_value(g_spi_cs_gpio, 0);
	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	rc = spi_sync(mdss_spi_client, &m);
	gpio_set_value(g_spi_cs_gpio, 1);
#ifdef ZTE_FEATURE_SPI_PANEL_SPEED_HZ
	mdss_spi_client->max_speed_hz = max_write_speed_hz;
#endif

	return rc;
}

int mdss_spi_tx_pixel(const void *buf, size_t len)
{
	int rc = 0;
	struct spi_transfer t = {
		.tx_buf = buf,
		.len    = len,
		};
	struct spi_message m;

	if (!mdss_spi_client) {
		pr_err("%s: spi client not available\n", __func__);
		return -EINVAL;
	}
#ifdef ZTE_FEATURE_SPI_PANEL_SPEED_HZ
	max_write_speed_hz = mdss_spi_client->max_speed_hz;
	mdss_spi_client->max_speed_hz = ZTE_FEATURE_SPI_PANEL_SPEED_HZ;
#endif

	mdss_spi_client->bits_per_word = 16;
	gpio_set_value(g_spi_cs_gpio, 0);
	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	rc = spi_sync(mdss_spi_client, &m);
	gpio_set_value(g_spi_cs_gpio, 1);
#ifdef ZTE_FEATURE_SPI_PANEL_SPEED_HZ
	mdss_spi_client->max_speed_hz = max_write_speed_hz;
#endif

	return rc;
}

static int mdss_spi_client_probe(struct spi_device *spidev)
{
	int irq;
	int cs;
	int cpha, cpol, cs_high;
	u32 max_speed;
	struct device_node *np;

	irq = spidev->irq;
	cs = spidev->chip_select;
	cpha = (spidev->mode & SPI_CPHA) ? 1:0;
	cpol = (spidev->mode & SPI_CPOL) ? 1:0;
	cs_high = (spidev->mode & SPI_CS_HIGH) ? 1:0;
	max_speed = spidev->max_speed_hz;
	np = spidev->dev.of_node;
	pr_debug("cs[%x] CPHA[%x] CPOL[%x] CS_HIGH[%x] Max_speed[%d]\n",
		cs, cpha, cpol, cs_high, max_speed);
	g_spi_cs_gpio = of_get_named_gpio(spidev->dev.of_node,
		"qcom,spi-cs-gpio", 0);
	if (!gpio_is_valid(g_spi_cs_gpio)) {
		pr_err("%s:g_spi_cs_gpio not specified\n", __func__);
		g_spi_cs_gpio = SPI_PANEL_CS_GPIO;
	} else {
		pr_info("%s:g_spi_cs_gpio is gpio %d\n", __func__, g_spi_cs_gpio);
	}
	mdss_spi_client = spidev;

	return 0;
}


static const struct of_device_id mdss_spi_dt_match[] = {
	{ .compatible = "qcom,mdss-spi-client" },
	{},
};

static struct spi_driver mdss_spi_client_driver = {
	.probe = mdss_spi_client_probe,
	.driver = {
	.name = "mdss-spi-client",
	.owner  = THIS_MODULE,
	.of_match_table = mdss_spi_dt_match,
	},
};

static int __init mdss_spi_init(void)
{
	int ret;

	ret = spi_register_driver(&mdss_spi_client_driver);

	return 0;
}
module_init(mdss_spi_init);

static void __exit mdss_spi_exit(void)
{
	spi_register_driver(&mdss_spi_client_driver);
}
module_exit(mdss_spi_exit);

