#include "lcd.h"

#ifdef LCD_ORCT012210N_ST7789V2

//#include "boards.h"

//�ӿڶ���
//extern xdata unsigned char buffer[512];
//------------------------------------------------------
#define ROW 240			//��ʾ���С�
#define COL 240			//����

//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

//LCM
#define LCD_PORT	"GPIO_0"
#define CS		23
#define	RST		24
#define	RS		21
#define	SCL		22
#define	SDA		20
#define VDD		18

//TP 
#define TP_0			12
#define TP_1			12

//CTP
#define CTP_EINT	25
#define	CTP_SDA		0
#define	CTP_SCL		1
#define	CTP_RSET	16

//LEDK(LED����)
#define LEDK	31
#define LEDA	14

#define X_min 0x0043		 //TP���Է�Χ��������
#define X_max 0x07AE
#define Y_min 0x00A1
#define Y_max 0x0759
//------------------------------------------------------

extern void BlockWrite(unsigned int x,unsigned int y,unsigned int w,unsigned int h);
extern void WriteOneDot(unsigned int color);
extern void Write_Data(uint8_t i);
extern void LCD_Clear(uint16_t color);
extern void LCD_Init(void);
extern void LCD_SleepIn(void);
extern void LCD_SleepOut(void);

#endif/*LCD_LH096TIG11G_ST7735SV*/