/**************************************************************************
 *Name        : CST816.c
 *Author      : xie biao
 *Version     : V1.0
 *Create      : 2020-08-21
 *Copyright   : August
**************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <drivers/i2c.h>
#include <drivers/gpio.h>
#include <dk_buttons_and_leds.h>
#include "cst816.h"

bool tp_trige_flag = false;

struct device *i2c_ctp;
struct device *gpio_ctp;

static struct gpio_callback gpio_cb;

static u8_t init_i2c(void)
{
	i2c_ctp = device_get_binding(CTP_DEV);
	if(!i2c_ctp)
	{
		printf("ERROR SETTING UP I2C\r\n");
		return -1;
	} 
	else
	{
		i2c_configure(i2c_ctp, I2C_SPEED_SET(I2C_SPEED_FAST));
		printf("I2C CONFIGURED\r\n");
		return 0;
	}
}

static s32_t platform_write(u8_t reg, u8_t *bufp, u16_t len)
{
	u8_t data[len+1];
	u32_t rslt = 0;

	data[0] = reg;
	memcpy(&data[1], bufp, len);
	rslt = i2c_write(i2c_ctp, data, len+1, CST816_I2C_ADDRESS);

	return rslt;
}

static s32_t platform_read(u8_t reg, u8_t *bufp, u16_t len)
{
	u32_t rslt = 0;

	rslt = i2c_write(i2c_ctp, &reg, 1, CST816_I2C_ADDRESS);
	if(rslt == 0)
	{
		rslt = i2c_read(i2c_ctp, bufp, len, CST816_I2C_ADDRESS);
	}
	
	return rslt;
}

void touch_panel_event_handle(tp_event tp_type, u16_t x_pos, u16_t y_pos)
{
	switch(tp_type)
	{
	case TP_EVENT_MOVING_UP:
		printk("tp moving up!\n");
		break;
	case TP_EVENT_MOVING_DOWN:
		printk("tp moving down!\n");
		break;
	case TP_EVENT_MOVING_LEFT:
		printk("tp moving left!\n");
		break;
	case TP_EVENT_MOVING_RIGHT:
		printk("tp moving right!\n");
		break;
	case TP_EVENT_SINGLE_CLICK:
		printk("tp single click! x:%d, y:%d\n", x_pos,y_pos);
		break;
	case TP_EVENT_DOUBLE_CLICK:
		printk("tp double click! x:%d, y:%d\n", x_pos,y_pos);
		break;
	case TP_EVENT_LONG_PRESS:
		printk("tp long press! x:%d, y:%d\n", x_pos,y_pos);
		break;
	case TP_EVENT_MAX:
		break;
	}
}

void CaptouchInterruptHandle(void)
{
	tp_trige_flag = true;
}

void tp_interrupt_proc(void)
{
	u8_t tmpbuf[128] = {0};
	u8_t TP_type = TP_EVENT_MAX;
	u8_t tp_temp[10]={0};

	platform_read(CST816_REG_GESTURE, &tp_temp[0], 1);//手势
	platform_read(CST816_REG_FINGER_NUM, &tp_temp[1], 1);//手指个数
	platform_read(CST816_REG_XPOS_L, &tp_temp[2], 1);//x坐标低位
	platform_read(CST816_REG_YPOS_L, &tp_temp[3], 1);//y坐标低位

	printk("tp_temp=%x,%x,%x,%x\n",tp_temp[0],tp_temp[1],tp_temp[2],tp_temp[3]);
	switch(tp_temp[0])
	{
	case GESTURE_NONE:
		break;
	case GESTURE_MOVING_UP:
		TP_type = TP_EVENT_MOVING_UP;
		break;
	case GESTURE_MOVING_DOWN:
		TP_type = TP_EVENT_MOVING_DOWN;
		break;
	case GESTURE_MOVING_LEFT:
		TP_type = TP_EVENT_MOVING_LEFT;
		break;
	case GESTURE_MOVING_RIGHT:
		TP_type = TP_EVENT_MOVING_RIGHT;
		break;
	case GESTURE_SINGLE_CLICK:
		TP_type = TP_EVENT_SINGLE_CLICK;
		break;
	case GESTURE_DOUBLE_CLICK:
		TP_type = TP_EVENT_DOUBLE_CLICK;
		break;
	case GESTURE_LONG_PRESS:
		TP_type = TP_EVENT_LONG_PRESS;
		break;
	}

	sprintf(tmpbuf, "TP_type:%d, x:%03d, y:%03d", TP_type, tp_temp[2], tp_temp[3]);
	LCD_ShowString(20,120,tmpbuf);
	if(TP_type != TP_EVENT_MAX)
	{
		touch_panel_event_handle(TP_type, tp_temp[2], tp_temp[3]);
	}
}

void CST816_init(void)
{
	u8_t tmpbuf[128] = {0};
	u8_t tp_temp_id=0;
	int flag = GPIO_DIR_IN|GPIO_INT|GPIO_INT_EDGE|GPIO_PUD_PULL_UP|GPIO_INT_ACTIVE_LOW|GPIO_INT_DEBOUNCE;
	
  	//端口初始化
  	gpio_ctp = device_get_binding(CTP_PORT);
	if(!gpio_ctp)
	{
		printk("Cannot bind gpio device\n");
		return;
	}

	init_i2c();

	gpio_pin_configure(gpio_ctp, CTP_EINT, flag);
	gpio_pin_disable_callback(gpio_ctp, CTP_EINT);
	gpio_init_callback(&gpio_cb, CaptouchInterruptHandle, BIT(CTP_EINT));
	gpio_add_callback(gpio_ctp, &gpio_cb);
	gpio_pin_enable_callback(gpio_ctp, CTP_EINT);

	gpio_pin_configure(gpio_ctp, CTP_RESET, GPIO_DIR_OUT);
	gpio_pin_write(gpio_ctp, CTP_RESET, 0);
	k_sleep(K_MSEC(10));
	gpio_pin_write(gpio_ctp, CTP_RESET, 1);
	k_sleep(K_MSEC(50));

	platform_read(CST816_REG_CHIPID, &tp_temp_id, 1);
	if(tp_temp_id == CST816_CHIP_ID)
	{
		printk("It's CST816!\n");
	}

	sprintf(tmpbuf, "CTP ID:%x", tp_temp_id);
	LCD_ShowString(20,100,tmpbuf);
}


void test_tp(void)
{
	u8_t tmpbuf[128] = {0};

	sprintf(tmpbuf, "test_tp");
	LCD_ShowString(20,80,tmpbuf);
	CST816_init();
}
