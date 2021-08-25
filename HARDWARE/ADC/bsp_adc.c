#include "bsp_adc.h"
    

//内存中存储转换结果的数组
volatile u16 ADC_ConvertedValue[Sample_Num][Channel_Num];

//DMA配置：ADC1->CR --> ADC_ConvertedValue
void ADC_DMA_Config(void)
{
	 DMA_InitTypeDef DMA_InitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	
	 NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
   NVIC_Init(&NVIC_InitStructure);
				   
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	 DMA_DeInit(DMA1_Channel1);												//将DMA的通道1寄存器重设为缺省值
	 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;		//DMA外设ADC基地址
	 DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;		//DMA内存基地址
	 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//内存作为数据传输的目的地
	 DMA_InitStructure.DMA_BufferSize = Sample_Num*Channel_Num;				//DMA通道的DMA缓存的大小
	 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设地址不变
	 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//内存地址递增
	 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//DMA通道的DMA缓存的大小
	 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//DMA通道的DMA缓存的大小
	 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//工作在循环缓存模式
	 DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA通道 x拥有高优先级
	 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//DMA通道x没有设置为内存到内存传输
	 DMA_Init(DMA1_Channel1, &DMA_InitStructure);			//根据DMA_InitStruct中指定的参数初始化DMA的通道
	 DMA_Cmd(DMA1_Channel1,ENABLE);
}


// PA0 -- 通道1，PA1 --通道1，PA7 --通道7，PB0 -- 通道8，PB1 --通道9，PC0 -- 通道10、PC1 -- 通道11、PC2 -- 通道12、PC3 -- 通道13
void ADC1_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 ADC_InitTypeDef ADC_InitStructure;
	 NVIC_InitTypeDef nvic;//定义中断结构体	   
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	   
	//默认开启     PA0 -- 通道1，PA1 --通道1
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;		
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;			//模拟输入引脚
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	 ADC_DMA_Config();
	   
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	 RCC_ADCCLKConfig(RCC_PCLK2_Div6); 		//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	// ADC_DeInit(ADC1);
	 ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//ADC1和ADC2工作在独立模式
	 ADC_InitStructure.ADC_ScanConvMode = ENABLE;							//多通道
	 ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//连续转换
	 ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件启动转换
	 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//转换结果右对齐
	 ADC_InitStructure.ADC_NbrOfChannel = Channel_Num;						//通道数目
	 ADC_Init(ADC1, &ADC_InitStructure); 


    //12MHz:ADC_SampleTime_1Cycles5-1.166us ADC_SampleTime_7Cycles5-1.66us ADC_SampleTime_13Cycles5-2.166us
		//ADC_SampleTime_28Cycles5-3.4166us ADC_SampleTime_41Cycles5-4.5us  ADC_SampleTime_55Cycles5-5.666us
		//ADC_SampleTime_71Cycles5-7us
	 //ADC采样时间 = （采样时间+12.5个时钟周期+读取时间）  时间 = （12.5 + ADC_SampleTime_239Cycles5）/ ADC时钟 转换时间 = 21us+21us = 42us 频率 = 23.809523k,单次采样最高：857.14285k
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);	//通道，转换次序，转换时间 = 21us
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5); //转换时间 = 21us
	 

	 nvic.NVIC_IRQChannel = ADC1_2_IRQn;//制定专断通道
	 nvic.NVIC_IRQChannelCmd = ENABLE;//使能中断
	 nvic.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	 nvic.NVIC_IRQChannelSubPriority = 1;//子优先级
	 NVIC_Init(&nvic);//初始化
	 
//	 ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);//打开ADC中断       每次读取adc寄存器自动清除中断标志位，如要使用中断的话需要关闭dma   
	 ADC_DMACmd(ADC1, ENABLE);	         //使能dma
//	 DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断
	 
	 ADC_Cmd(ADC1, ENABLE);
	 
	 ADC_ResetCalibration(ADC1);					//使能复位校准
	 while(ADC_GetResetCalibrationStatus(ADC1));  	//等待复位校准结束
	 ADC_StartCalibration(ADC1);					//开始AD校准
	 while(ADC_GetCalibrationStatus(ADC1));			//等待校准结束
	 
	 ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能ADC的软件转换启动功能
}


//adc键盘检测函数，0-无任何值按下，返回按下键值
u8 ADC_Key(u16 adc,u8 mode){
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(adc < 2850))
	{
		delay_ms(5);//去抖动 
		key_up=0;	
		if(2850<=adc && adc<4080){return 9;}
		if(2550<=adc && adc<2850){return 8;}
		if(2150<=adc && adc<2550){return 7;}
		if(1750<=adc && adc<2150){return 6;}
		if(1350<=adc && adc<1750){return 5;}
		if(1150<=adc && adc<1350){return 4;}
		if(750<=adc && adc<1150){return 3;}
		if(300<=adc && adc<750){return 2;}
		if(adc<300){return 1;}

	}else if(adc >= 2850)key_up=1; 	    
 	return 0;// 无按键按下
	
}

//读取转换平均值
uint16_t ReadADCAverageValue(uint16_t Channel)
{
	uint8_t i;
	uint32_t sum = 0;
	for(i=0; i<Sample_Num; i++)
	{
		sum+=ADC_ConvertedValue[i][Channel];		//取平均值
	}
	return (sum/Sample_Num);
}

//adc中断函数
void ADC1_2_IRQHandler()
{
	if (ADC_GetITStatus(ADC1,ADC_IT_EOC)==SET) 
	{
		// 中断处理程序
		LED0 = ~LED0;
		
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}

//dma中断函数
void DMA1_Channel1_IRQHandler(void)
{
        if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
        {
//					LED0 = ~LED0;
          DMA_ClearITPendingBit(DMA1_IT_GL1); //清除全部中断标志
        }
}




