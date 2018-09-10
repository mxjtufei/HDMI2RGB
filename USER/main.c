#include "led.h"
#include "delay.h"
#include "sys.h"
#include "dlp.h"
#include "key.h"
#include "adv7611.h"
int main(void)
{
	uint8_t power_state = 0;
	uint8_t dlp_mode = 0;
	SystemInit(); 			 //ϵͳʱ�ӳ�ʼ��Ϊ72M	  SYSCLK_FREQ_72MHz
	delay_init(72);	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LEDInit();
	DLPInit();
	KeyInit();
	IOInit();
	I2CInit();
	
	while(1)
	{
		switch(KeyScan())
		{
			case key_num_poweron:
				if(power_state==0)
				{
					LEDSet(led_num_state, 1);
					DLPStart();
					power_state = 1;
                    if(ADV7611Init()==-1)
                    {
                        LEDSet(led_num_error, 1);
                    }
                    ADV7611Start();
				}
				else
				{
					LEDSet(led_num_state, 0);
					DLPStop();
					power_state = 0;
				}
				break;
			case key_num_dlp_mode:
				dlp_mode++;
				if (dlp_mode == 3)
					dlp_mode = 0;
				switch (dlp_mode)
				{
				case 0:
					DLPModeSet(dlp_mode_powersave);
					break;
				case 1:
					DLPModeSet(dlp_mode_normal);
					break;
				case 2:
					DLPModeSet(dlp_mode_brighten);
					break;
				}

				break;
		}
		if(ADV7611Check()==-1)
			 LEDSet(led_num_error, 1);
		else
			LEDSet(led_num_error, 0);
	}
}

