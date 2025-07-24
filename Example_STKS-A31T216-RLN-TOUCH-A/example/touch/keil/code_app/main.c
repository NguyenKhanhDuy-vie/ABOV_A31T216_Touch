#include "main.h"
#include <stdbool.h>
#include "led_driver.h"
str_flag t_sysbit;
#define standby 0x0
//#define lock    0x1
#define mode1   0x5
#define mode2   0x9
#define mode3   0x11
uint8_t const coDigit_0[9][8] = {
    {0x00,0x7E,0xFF,0x81,0x81,0xFF,0x7E,0x00},  /* 0 */
    {0x00,0x02,0x02,0xFF,0xFF,0x00,0x00,0x00},  /* 1 */
    {0x00,0xC6,0xE7,0xB1,0x99,0x8F,0x86,0x00},  /* 2 */
    {0x00,0x42,0xCB,0x89,0x89,0xFF,0x76,0x00},  /* 3 */
    {0x00,0x30,0x3C,0x2F,0xFF,0xFF,0x20,0x00},  /* 4 */
    {0x00,0x4F,0xCF,0x89,0x89,0xF9,0x71,0x00},  /* 5 */
    {0x00,0x7E,0xFF,0x89,0x89,0xFB,0x72,0x00},  /* 6 */
    {0x00,0x01,0x01,0xE1,0xFD,0x1F,0x03,0x00},  /* 7 */
//  {0x00,0x76,0xFF,0x89,0x89,0xFF,0x76,0x00},  /* 8 */
//  {0x00,0x4E,0xDF,0x91,0x91,0xFF,0x7E,0x00},  /* 9 */
    {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}   /* nothing, */
};
void SysTick_Configure(void)
{
    /* 1_msec interrupt */
    SysTick_Config(SystemCoreClock/1000);
}
void Change_SysClock(enum system_clock sysclk)
{
    struct scu_clock_cfg cfg;

    switch (sysclk)
    {
        case SYS_CLK_32MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_1;
            cfg.mclk_hz = 32000000;
            break;
        case SYS_CLK_16MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_2;
            cfg.mclk_hz = 16000000;
            break;
        case SYS_CLK_8MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_4;
            cfg.mclk_hz = 8000000;
            break;
        case SYS_CLK_4MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_8;
            cfg.mclk_hz = 4000000;
            break;
        case SYS_CLK_2MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_16;
            cfg.mclk_hz = 2000000;
            break;
        case SYS_CLK_1MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_32;
            cfg.mclk_hz = 1000000;
            break;
    }

    /* Update Main Clock */
    HAL_SCU_UpdateMainClock(&cfg);
}

void ledM(uint8_t numIndx){
	uint16_t seg_data[13];      /* COM0 ~ COM12 */
    uint32_t* ptrDisp;

    uint8_t comIndx;
	for (comIndx = 0; comIndx < 13; comIndx++)
    {
        seg_data[comIndx] = 0;
    }
		for (comIndx = 0; comIndx < 8; comIndx++)   /* COM0/1/2/3/4/5/7/8 */    
    {
        if (coDigit_0[numIndx][comIndx] & BIT(0))
        {
            seg_data[comIndx] |= BIT(6);        /* SEG6 */
        }
        seg_data[comIndx] |= (uint16_t)(coDigit_0[numIndx][comIndx]<<8)&0xfe00;   /* SEG9/10/11/12/13/14/15 */
    }
		ptrDisp = (uint32_t*)(&LED->DISPRAM0);  
    for (comIndx = 0; comIndx < 8; comIndx++)
    {
        if (comIndx < 6)    /* COM0 ~ COM5 */
        {
            *(ptrDisp+comIndx) = seg_data[comIndx];
        }
        else                /* COM7 ~ COM8 */
        {
            *(ptrDisp+(comIndx+1)) = seg_data[comIndx];
        }
    }
}
void ledT(uint8_t numIndex) {
    uint16_t seg_data[13] ;   // COM0 ~ COM12
    uint32_t* ptrDisp;
    uint8_t comIndx;
		for (comIndx = 0; comIndx < 13; comIndx++)
    {
        seg_data[comIndx] = 0;
    }
   for (comIndx = 0; comIndx < 8; comIndx++)
    {
        for (int i = 0; i < numIndex+1; i++)
        {
            if (i < 6)      /* 0,1,2,3,4,5 */
            {
                seg_data[comIndx] |= BIT(i);
            }
            else            /* 7,8 */
            {
                seg_data[comIndx] |= BIT(i+1);
            }
        }
    }
		ptrDisp = (uint32_t*)(&LED->DISPRAM0);  
    for (comIndx = 0; comIndx < 8; comIndx++)
    {
        if (comIndx < 6)    /* COM0 ~ COM5 */
        {
            *(ptrDisp+comIndx) = seg_data[comIndx];
        }
        else                /* COM7 ~ COM8 */
        {
            *(ptrDisp+(comIndx+1)) = seg_data[comIndx];
        }
    }
}
void init ()
{
	uint8_t ch;
	func_p_Touch_Freq_Sel = Touch_Freq_Sel;
	user.hold_key_release_time = TIME_HOLD_KEY_RELEASE;
	for(ch = 0; ch < ts.actv_ch_cnt; ch++)
    {
        user.big_diff_thd[ch] = big_diff_thd[ch];
        if(user.big_diff_thd[ch] > 0x7FFF)
        {
            user.big_diff_thd[ch] = 0x7FFF;
        }
    } 
	SCU->PER2 &= ~(1<<29);
    SCU->PCER2 &= ~(1<<29);
    SCU->PER2 |= (1<<29);
    SCU->PCER2 |= (1<<29);

    fpRenewLED = led_RenewData;
    led.flag.seg_current = 1;
    led.flag.op_mode = 2;
    led.com_sel = COMSEL_00_12;
    led.seg_sel = SEGSEL_00_15;

    LED_Set_Current(LED_CURRENT_13mA);    
    LED_Set_Actv_Time(LED_USE_COM_NUM, TIME_LED_OPTERATING, TIME_LED_INTERVAL);
		LED_Init_IO();
		LED_Init_Reg();
}
int main(void) {
	int i = 5;
	uint8_t temp[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	Port_Init();                                
	System_Setting_Config(); 
	Change_SysClock(SYS_CLK_32MHZ);
	SysTick_Config(SystemCoreClock / 1000);
	Init_Touch(); 
	init();
	bool lock = 0 ;
	uint8_t mode = standby;
	while (1) {
		Touch_Do_Task();
		if (ts.flag.touch_sensing_end == 1) {
				Touch_Key_Scenario();
				ts.flag.touch_sensing_end = 0;
		}
		if (ts.detect_key == 0x03 && mode != standby ) {if (lock ==1) {lock =0;LED->DISPRAM0 &= ~(1 << 6);} else lock = 1;}
		if (lock == 1) {
			switch (mode){
		case mode1   : ledM(1) ; break ;
		case mode2   : ledM(2) ; break ;
		case mode3   : ledM(3) ; break ;
	};ledT(temp[i]);LED->DISPRAM0 |= (1 << 6);ts.detect_key = 0;
		} else {
		switch (ts.detect_key){
			case 0x01 : 
				if(mode == standby){
					mode = mode1;
				}else mode = standby; 
			break ;
			case 0x04 : 
				if(mode!=standby || (lock == 0)){
					mode = mode1; 
				}
			break ;
				
			case 0x08 : 
				if(mode!=standby || (lock == 0)){
					mode = mode2; 
				}
			break;
				
			case 0x10 : 
				if(mode!=standby || (lock == 0)){
					mode = mode3; 
				}
			break;
				
			default :
				break ;
		}
		
		switch (mode){
			case standby : ledM(0) ; break ;
			case mode1   : ledM(1) ; break ;
			case mode2   : ledM(2) ; break ;
			case mode3   : ledM(3) ; break ;
		}
		
		if (mode == standby){
			ledT(0);
		}else {
			if( ts.detect_key & BIT(5) ){
				i++;
				if(i > 7) i = 7;
			}else if( ts.detect_key & BIT(6) ){
				i--;
				if(i < 0) i = 0;
			}
			ledT(temp[i]);
		}
		ts.detect_key = 0;
	} }
	
}
