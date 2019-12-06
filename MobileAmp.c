#include <stdio.h>
#include <bcm2835.h>
#include "tremolo_values.h"

int main(int argc, char **argv)
{
	bcm2835_init()
	bcm2835_spi_begin()
	//SPI configuration
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);  //cdiv = 64 -> 3.9 MHz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0,HIGH);

	//Pulldown for each GPIO toggle 
	bcm2835_gpio_fsel(TOGGLE1, BCM2835_GPIO_FSEL_INPT);            
	bcm2835_gpio_set_pud(TOGGLE1, BCM2835_GPIO_PUD_DOWN);              
	
	bcm2835_gpio_fsel(TOGGLE2, BCM2835_GPIO_FSEL_INPT);            
	bcm2835_gpio_set_pud(TOGGLE2, BCM2835_GPIO_PUD_DOWN);              
	
	bcm2835_gpio_fsel(TOGGLE3, BCM2835_GPIO_FSEL_INPT);             
	bcm2835_gpio_set_pud(TOGGLE3, BCM2835_GPIO_PUD_DOWN);             

	bcm2835_gpio_fsel(TOGGLE4, BCM2835_GPIO_FSEL_INPT);             
	bcm2835_gpio_set_pud(TOGGLE4, BCM2835_GPIO_PUD_DOWN);            
	
	bcm2835_gpio_fsel(TOGGLE5, BCM2835_GPIO_FSEL_INPT);				
	bcm2835_gpio_set_pud(TOGGLE5, BCM2835_GPIO_PUD_DOWN);            
	
	bcm2835_gpio_fsel(TOGGLE6, BCM2835_GPIO_FSEL_INPT);				
	bcm2835_gpio_set_pud(TOGGLE6, BCM2835_GPIO_PUD_DOWN);           
	
	bcm2835_gpio_fsel(TOGGLE7, BCM2835_GPIO_FSEL_INPT);				
	bcm2835_gpio_set_pud(TOGGLE7, BCM2835_GPIO_PUD_DOWN);            

	bcm2835_gpio_fsel(TOGGLE8, BCM2835_GPIO_FSEL_INPT);				
	bcm2835_gpio_set_pud(TOGGLE8, BCM2835_GPIO_PUD_DOWN);            
	
	bcm2835_gpio_fsel(TOGGLE9, BCM2835_GPIO_FSEL_INPT);				
	bcm2835_gpio_set_pud(TOGGLE9, BCM2835_GPIO_PUD_DOWN);            
	
	bcm2835_gpio_fsel(TOGGLE10, BCM2835_GPIO_FSEL_INPT);			
	bcm2835_gpio_set_pud(TOGGLE10, BCM2835_GPIO_PUD_DOWN);            
	
	//PWM configuration
	bcm2835_gpio_fsel(18, BCM2835_GPIO_FSEL_ALT7);
	bcm2835_gpio_fsel(13, BCM2835_GPIO_FSEL_ALT2);
	bcm2835_pwm_set_clock(2);
	bcm2835_pwm_set_mode(0,0,0);
	bcm2835_pwm_set_range(0,128);
	bcm2835_pwm_set_mode(0,1,0);
	bcm2835_pwm_set_range(1,128);
	
	while(1)	{
		
		//read ADC
		bcm2835_spi_transfernb(miso,mosi,3);
		input_signal = mosi[2] + ((mosi[1] & 0x0F) << 8);   
		
		rd_timer++;
		
		if (rd_timer == 50000)			//every ~half second 
		{
			read_timer = 0;
			uint8_t TOGGLE1_val = bcm2835_gpio_lev(TOGGLE1);
			uint8_t TOGGLE2_val = bcm2835_gpio_lev(TOGGLE2);
			uint8_t TOGGLE3_val = bcm2835_gpio_lev(TOGGLE3);
			uint8_t TOGGLE4_val = bcm2835_gpio_lev(TOGGLE4);
			uint8_t TOGGLE5_val = bcm2835_gpio_lev(TOGGLE5);
			uint8_t TOGGLE6_val = bcm2835_gpio_lev(TOGGLE6);
			uint8_t TOGGLE7_val = bcm2835_gpio_lev(TOGGLE7);
			uint8_t TOGGLE8_val = bcm2835_gpio_lev(TOGGLE8);
			uint8_t TOGGLE9_val = bcm2835_gpio_lev(TOGGLE9);
			uint8_t TOGGLE10_val = bcm2835_gpio_lev(TOGGLE10);
			
			
			if (TOGGLE1_val == 1)
			{
				dist_on_or_off = 1;
			}
			else if (TOGGLE1_val == 0)
			{
				dist_on_or_off = 0;
			}				
			
			if (TOGGLE2_val == 1)
			{
				delay_on_or_off = 1;
			}
			else if (TOGGLE2_val == 0)
			{
				delay_on_or_off = 0;
			}		
			if (TOGGLE3_val == 1)
			{
				
				if (distortion_value<2047) distortion_value=distortion_value+10;
			}
									
			if (TOGGLE4_val == 1)
			{
				
				if (distortion_value>0) distortion_value=distortion_value-10;
			}		
			if (TOGGLE5_val == 1)
			{
				fuzz_on_or_off = 1;
			}
			else if (TOGGLE5_val == 0)
			{
				fuzz_on_or_off = 0;
			}
			if (TOGGLE6_val == 1)
			{
				trem_on_or_off = 1;
			}
			else if (TOGGLE6_val == 0)
			{
				trem_on_or_off = 0;
			}
			if (TOGGLE7_val == 1)
			{
				if (Delay_Depth<DELAY_MAX)Delay_Depth=Delay_Depth+10000;
			}
			
			if (TOGGLE8_val == 1)
			{
				if (delay_long<DELAY_MAX)delay_long=delay_long-10000;
			}
			
			if (TOGGLE9_val == 1)
			{
				if (max_count<50) max_count=max_count++;
			}
			
			if (TOGGLE10_val == 1)
			{
				if (max_count>1) max_ct=max_ct--;
			}
			
				
			
		}
	
		//Effects
		if(dist_on_or_off == 1){
				
			if (input_signal > 2047 + distortion_value) signal_in= 2047 + distortion_value;
			if (signal_in < 2047 - distortion_value) signal_in= 2047 - distortion_value;
		}
		if(delay_on_or_off == 1){
			
			delay_circbuff[DelayCounter] = signal_in;
			DelayCounter++;
			
			if(DelayCounter >= delay_long) DelayCounter = 0; 
			
			signal_out = (delay_circbuff[delay_count]+signal_in)>>1;
		}
		if(fuzz_on_or_off == 1){
			
			if (signal_in > 1047 + fuzz_value) signal_in= 4095;
			if (signal_in < 1047 - fuzz_value) signal_in= 0;
		
		}
		if(trem_on_or_off == 1){
			
			divider++;
			if (divider==max_ct){ divider=0; sample++;}
 
			if(sample==999)sample=1;
			
			signal_out= (int)(((float)signal_in) * ((float)sinewav[sample]/4095.0));
			
		}

		//PWM 
		bcm2835_pwm_set_data(1,signal_out & 0x3F);
		bcm2835_pwm_set_data(0,signal_out >> 6);
	}

	
	bcm2835_spi_end();
	bcm2835_close();
	return 0;

}

