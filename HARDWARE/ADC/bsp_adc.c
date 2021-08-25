#include "bsp_adc.h"
    

//�ڴ��д洢ת�����������
volatile u16 ADC_ConvertedValue[Sample_Num][Channel_Num];

//DMA���ã�ADC1->CR --> ADC_ConvertedValue
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
	 DMA_DeInit(DMA1_Channel1);												//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;		//DMA����ADC����ַ
	 DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;		//DMA�ڴ����ַ
	 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	 DMA_InitStructure.DMA_BufferSize = Sample_Num*Channel_Num;				//DMAͨ����DMA����Ĵ�С
	 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//�����ַ����
	 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//�ڴ��ַ����
	 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//DMAͨ����DMA����Ĵ�С
	 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//DMAͨ����DMA����Ĵ�С
	 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//������ѭ������ģʽ
	 DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMAͨ�� xӵ�и����ȼ�
	 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	 DMA_Init(DMA1_Channel1, &DMA_InitStructure);			//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
	 DMA_Cmd(DMA1_Channel1,ENABLE);
}


// PA0 -- ͨ��1��PA1 --ͨ��1��PA7 --ͨ��7��PB0 -- ͨ��8��PB1 --ͨ��9��PC0 -- ͨ��10��PC1 -- ͨ��11��PC2 -- ͨ��12��PC3 -- ͨ��13
void ADC1_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 ADC_InitTypeDef ADC_InitStructure;
	 NVIC_InitTypeDef nvic;//�����жϽṹ��	   
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	   
	//Ĭ�Ͽ���     PA0 -- ͨ��1��PA1 --ͨ��1
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;		
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;			//ģ����������
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	 ADC_DMA_Config();
	   
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	 RCC_ADCCLKConfig(RCC_PCLK2_Div6); 		//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	// ADC_DeInit(ADC1);
	 ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//ADC1��ADC2�����ڶ���ģʽ
	 ADC_InitStructure.ADC_ScanConvMode = ENABLE;							//��ͨ��
	 ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//����ת��
	 ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�������ת��
	 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//ת������Ҷ���
	 ADC_InitStructure.ADC_NbrOfChannel = Channel_Num;						//ͨ����Ŀ
	 ADC_Init(ADC1, &ADC_InitStructure); 


    //12MHz:ADC_SampleTime_1Cycles5-1.166us ADC_SampleTime_7Cycles5-1.66us ADC_SampleTime_13Cycles5-2.166us
		//ADC_SampleTime_28Cycles5-3.4166us ADC_SampleTime_41Cycles5-4.5us  ADC_SampleTime_55Cycles5-5.666us
		//ADC_SampleTime_71Cycles5-7us
	 //ADC����ʱ�� = ������ʱ��+12.5��ʱ������+��ȡʱ�䣩  ʱ�� = ��12.5 + ADC_SampleTime_239Cycles5��/ ADCʱ�� ת��ʱ�� = 21us+21us = 42us Ƶ�� = 23.809523k,���β�����ߣ�857.14285k
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);	//ͨ����ת������ת��ʱ�� = 21us
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5); //ת��ʱ�� = 21us
	 

	 nvic.NVIC_IRQChannel = ADC1_2_IRQn;//�ƶ�ר��ͨ��
	 nvic.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�
	 nvic.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
	 nvic.NVIC_IRQChannelSubPriority = 1;//�����ȼ�
	 NVIC_Init(&nvic);//��ʼ��
	 
//	 ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);//��ADC�ж�       ÿ�ζ�ȡadc�Ĵ����Զ�����жϱ�־λ����Ҫʹ���жϵĻ���Ҫ�ر�dma   
	 ADC_DMACmd(ADC1, ENABLE);	         //ʹ��dma
//	 DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�
	 
	 ADC_Cmd(ADC1, ENABLE);
	 
	 ADC_ResetCalibration(ADC1);					//ʹ�ܸ�λУ׼
	 while(ADC_GetResetCalibrationStatus(ADC1));  	//�ȴ���λУ׼����
	 ADC_StartCalibration(ADC1);					//��ʼADУ׼
	 while(ADC_GetCalibrationStatus(ADC1));			//�ȴ�У׼����
	 
	 ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ADC�����ת����������
}


//adc���̼�⺯����0-���κ�ֵ���£����ذ��¼�ֵ
u8 ADC_Key(u16 adc,u8 mode){
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(adc < 2850))
	{
		delay_ms(5);//ȥ���� 
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
 	return 0;// �ް�������
	
}

//��ȡת��ƽ��ֵ
uint16_t ReadADCAverageValue(uint16_t Channel)
{
	uint8_t i;
	uint32_t sum = 0;
	for(i=0; i<Sample_Num; i++)
	{
		sum+=ADC_ConvertedValue[i][Channel];		//ȡƽ��ֵ
	}
	return (sum/Sample_Num);
}

//adc�жϺ���
void ADC1_2_IRQHandler()
{
	if (ADC_GetITStatus(ADC1,ADC_IT_EOC)==SET) 
	{
		// �жϴ������
		LED0 = ~LED0;
		
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}

//dma�жϺ���
void DMA1_Channel1_IRQHandler(void)
{
        if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
        {
//					LED0 = ~LED0;
          DMA_ClearITPendingBit(DMA1_IT_GL1); //���ȫ���жϱ�־
        }
}




