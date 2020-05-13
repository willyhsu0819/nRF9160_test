#include <stdlib.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include "lcd.h"
#include "font.h" 

//#include "nrf_log.h"
//#include "nrf_delay.h"

#ifdef LCD_LH096TIG11G_ST7735SV
#include "LCD_LH096TIG11G_ST7735SV.h"

#define SPI_DEV "SPI_3"

extern struct device * spi_devl;

static const struct spi_config spi_cfg = {
  .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB |
		     SPI_MODE_CPOL | SPI_MODE_CPHA,
  .frequency = 4000000,
  .slave = 0,
};

static void spi_init(void)
{
  spi_devl = device_get_binding(SPI_DEV);

  if (!SPI_DEV) 
  {
    printk("Could not get %s device\n", SPI_DEV);
    return;
  }
}

//LCD延时函数
void Delay(unsigned int dly)
{
    nrf_delay_ms(dly);
}

//数据接口函数
//i:8位数据
void Write_Data(uint8_t i) 
{	
  int err;
  static u8_t tx_buffer[1] = {0};
  static u8_t rx_buffer[1] = {0};


  tx_buffer[0] = i;

  const struct spi_buf tx_buf = {
          .buf = tx_buffer,
          .len = sizeof(tx_buffer)
  };
  const struct spi_buf_set tx = {
          .buffers = &tx_buf,
          .count = 1
  };

  struct spi_buf rx_buf = {
          .buf = rx_buffer,
          .len = sizeof(rx_buffer),
  };
  const struct spi_buf_set rx = {
          .buffers = &rx_buf,
          .count = 1
  };

  err = spi_transceive(spi_devl, &spi_cfg, &tx, &rx);
  if(err)
  {
    printk("SPI error: %d\n", err);
  }
  else
  {
    printk("TX sent: %x\n", tx_buffer[0]);
    printk("RX recv: %x\n", rx_buffer[0]);
    tx_buffer[0]++;
  }	
}

//----------------------------------------------------------------------
//写寄存器函数
//i:寄存器值
void WriteComm(unsigned int i)
{
  //nrf_gpio_pin_clear(CS);				//CS置0
  nrf_gpio_pin_clear(RS);				//RS清0

  Write_Data(i);  

  //nrf_gpio_pin_set(CS);
}

//写LCD数据
//i:要写入的值
void WriteData(unsigned int i)
{
  //nrf_gpio_pin_clear(CS);
  nrf_gpio_pin_set(RS);
		
  Write_Data(i);  

  //nrf_gpio_pin_set(CS);
}

void WriteDispData(unsigned char DataH,unsigned char DataL)
{
  Write_Data(DataH);  
  Write_Data(DataL);  
}

//LCD画点函数
//color:要填充的颜色
void WriteOneDot(unsigned int color)
{ 
  //nrf_gpio_pin_clear(CS);
  nrf_gpio_pin_set(RS);

  Write_Data(color>>8);  
  Write_Data(color);  

  //nrf_gpio_pin_set(CS);
}

////////////////////////////////////////////////测试函数//////////////////////////////////////////
void BlockWrite(unsigned int x,unsigned int y,unsigned int w,unsigned int h) //reentrant
{
  x += 26;
  y += 1;
	
  WriteComm(0x2A);             
  WriteData(x>>8);             
  WriteData(x);             
  WriteData((x+w)>>8);             
  WriteData((x+w));             
	
  WriteComm(0x2B);             
  WriteData(y>>8);             
  WriteData(y);             
  WriteData((y+h)>>8);//	WriteData((Yend+1)>>8);             
  WriteData((y+h));//	WriteData(Yend+1);   	

  WriteComm(0x2c);
}

void DispColor(unsigned int color)
{
  unsigned int i,j;

  BlockWrite(0,0,COL-1,ROW-1);

  nrf_gpio_pin_set(RS);

  for(i=0;i<ROW;i++)
  {
    for(j=0;j<COL;j++)
    {    
      Write_Data(color>>8);  
      Write_Data(color);  
    }
  }
}

//测试函数（显示RGB条纹）
void DispBand(void)	 
{
  unsigned int i,j,k;
  //unsigned int color[8]={0x001f,0x07e0,0xf800,0x07ff,0xf81f,0xffe0,0x0000,0xffff};
  unsigned int color[8]={0xf800,0xf800,0x07e0,0x07e0,0x001f,0x001f,0xffff,0xffff};//0x94B2
  //unsigned int gray16[]={0x0000,0x1082,0x2104,0x3186,0x42,0x08,0x528a,0x630c,0x738e,0x7bcf,0x9492,0xa514,0xb596,0xc618,0xd69a,0xe71c,0xffff};

  BlockWrite(0,0,COL-1,ROW-1);

  nrf_gpio_pin_set(RS);

  for(i=0;i<8;i++)
  {
    for(j=0;j<ROW/8;j++)
    {
      for(k=0;k<COL;k++)
      {
        Write_Data(color[i]>>8);  
        Write_Data(color[i]);  
      } 
    }
  }
  for(j=0;j<(ROW%8);j++)
  {
    for(k=0;k<COL;k++)
    {
      Write_Data(color[7]>>8);  
      Write_Data(color[7]);  
    } 
  }
}

//测试函数（画边框）
void DispFrame(void)
{
  unsigned int i,j;
	
  BlockWrite(0,0,COL-1,ROW-1);

  nrf_gpio_pin_set(RS);
  
  Write_Data(0xf8);  
  Write_Data(0x00);  

  for(i=0;i<COL-2;i++)
  {
    Write_Data(0xFF);  
    Write_Data(0xFF);  
  }
  Write_Data(0x00);  
  Write_Data(0x1F);  

  for(j=0;j<ROW-2;j++)
  {
    Write_Data(0xf8);  
    Write_Data(0x00);  
    for(i=0;i<COL-2;i++)
    {
      Write_Data(0x00);  
      Write_Data(0x00);  
    }
    Write_Data(0x00);  
    Write_Data(0x1f);  
  }

  Write_Data(0xf8);  
  Write_Data(0x00);  
  for(i=0;i<COL-2;i++)
  {
    Write_Data(0xff);  
    Write_Data(0xff);  
  }
  Write_Data(0x00);  
  Write_Data(0x1f);  
}

//////////////////////////////////////////////////////////////////////////////////////////////
//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color)
{
  uint32_t index=0;      
  uint32_t totalpoint=ROW;
  
  totalpoint*=COL; 			//得到总点数
  
  BlockWrite(0,0,COL-1,ROW-1);//定位

  nrf_gpio_pin_set(RS);

  for(index=0;index<totalpoint;index++)
  {
    Write_Data(color>>8);
    Write_Data(color);  
  }
} 

//屏幕睡眠
void LCD_SleepIn(void)
{
  WriteComm(0x28);	
  WriteComm(0x10);  		//Sleep in	
  Delay(120);                  //延时120ms
}

//屏幕唤醒
void LCD_SleepOut(void)
{
  WriteComm(0x11);  		//Sleep out	
  Delay(120);                  //延时120ms
  WriteComm(0x29);
}

//LCD初始化函数
void LCD_Init(void)
{
  //端口初始化
  nrf_gpio_range_cfg_output(NRF_GPIO_PIN_MAP(0,03),NRF_GPIO_PIN_MAP(0,04));
  nrf_gpio_range_cfg_output(NRF_GPIO_PIN_MAP(0,28),NRF_GPIO_PIN_MAP(0,29));

  spi_init();
  
  nrf_gpio_pin_set(RST);
  Delay(10);
  nrf_gpio_pin_clear(RST);
  Delay(10);
  nrf_gpio_pin_set(RST);
  Delay(120);

  WriteComm(0x11);     //Sleep out
  Delay(120);          //Delay 120ms

  WriteComm(0xB1);     //Normal mode
  WriteData(0x05);   
  WriteData(0x3C);   
  WriteData(0x3C);  
   
  WriteComm(0xB2);     //Idle mode
  WriteData(0x05);   
  WriteData(0x3C);   
  WriteData(0x3C);  
   
  WriteComm(0xB3);     //Partial mode
  WriteData(0x05);   
  WriteData(0x3C);   
  WriteData(0x3C);   
  WriteData(0x05);   
  WriteData(0x3C);   
  WriteData(0x3C);  
  
  WriteComm(0xB4);     //Dot inversion
  WriteData(0x00); 

  WriteComm(0xB6);    //column inversion
  WriteData(0xB4); 
  WriteData(0xF0);	

  WriteComm(0xC0);     //AVDD GVDD
  WriteData(0xAB);   
  WriteData(0x0B);   
  WriteData(0x04);  
  
  WriteComm(0xC1);     //VGH VGL
  WriteData(0xC5);   	//C0

  WriteComm(0xC2);     //Normal Mode
  WriteData(0x0D);   
  WriteData(0x00);  
  
  WriteComm(0xC3);     //Idle
  WriteData(0x8D);   
  WriteData(0x6A);  
  
  WriteComm(0xC4);     //Partial+Full
  WriteData(0x8D);   
  WriteData(0xEE); 
  
  WriteComm(0xC5);     //VCOM
  WriteData(0x0F);  

  WriteComm(0x36); 	//MX,MY,RGB mode
  WriteData(0xC8); 	//my mx ml MV,rgb,000; =1,=MH=MX=MY=ML=0 and RGB filter panel 
        
  WriteComm(0xE0);     //positive gamma
  WriteData(0x07);   
  WriteData(0x0E);   
  WriteData(0x08);   
  WriteData(0x07);   
  WriteData(0x10);   
  WriteData(0x07);   
  WriteData(0x02);   
  WriteData(0x07);   
  WriteData(0x09);   
  WriteData(0x0F);   
  WriteData(0x25);   
  WriteData(0x36);   
  WriteData(0x00);   
  WriteData(0x08);   
  WriteData(0x04);   
  WriteData(0x10); 
  
  WriteComm(0xE1);     //negative gamma
  WriteData(0x0A);   
  WriteData(0x0D);   
  WriteData(0x08);   
  WriteData(0x07);   
  WriteData(0x0F);   
  WriteData(0x07);   
  WriteData(0x02);   
  WriteData(0x07);   
  WriteData(0x09);   
  WriteData(0x0F);   
  WriteData(0x25);   
  WriteData(0x35);   
  WriteData(0x00);   
  WriteData(0x09);   
  WriteData(0x04);   
  WriteData(0x10);
  
  WriteComm(0xFC);    
  WriteData(0x80);  

  WriteComm(0xF0);    
  WriteData(0x11);  

  WriteComm(0xD6);    
  WriteData(0xCB);  
  
  WriteComm(0x3A);     
  WriteData(0x05);  
  
  WriteComm(0x21);     //Display inversion
  WriteComm(0x29);     //Display on

  //点亮背光
  nrf_gpio_pin_clear(LEDK);
  
  LCD_Clear(BLACK);		//清屏为黑色
}

#endif
