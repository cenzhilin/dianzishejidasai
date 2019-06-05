
#include <reg51.h>
//#include <intrins.h>

#define COM P1// �ӵ�λ���߸�λ

#define smg_DATA  P2 //��������ݹܽ�


unsigned code smgduan[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

unsigned  int data0=0;  //Ԥ�ȶ���,��ʡ��ʼ��ʱ��
unsigned  int ten[10];
unsigned  char intTIME =0; //�����ⲿ�жϴ���.
unsigned char x =0 ;

bit flag =  0;    //


unsigned  int j=0;   //

 void delay1ms(void)   //800us
{
        unsigned char a,b;
    for(b=159;b>0;b--)
        for(a=1;a>0;a--);
}

void delay10ms(void){
	  unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=4;b>0;b--)
            for(a=248;a>0;a--);
}

void init_E0(){
	IT0=1;
  PX0=1;
	EX0=1;
	TR1 = 1 ;    //��T1 
	EA=1; //��ȫ���ж�
}

void initT0 () //T0 ��ʱ����ʱ 250   
{
TMOD =0X02;  //��ʱ��0 ��ʽ��,8λ��װ��,
ET0 = 1;
TR0 = 0;
TH0 =236 ;// 20us  250us
TL0 =236;  // 20us

}

void interrupt_0 () interrupt 0
{
if(TR0 == 0){flag =0;j=0;}   // ��ΪTR0 ����һ�ν�������0,���г�ʼ����ֵ.,Ȼ����һ�� ��ʼ TR0 ����0;
	TR0=~TR0;	    //һ������ ,����ʱ��, һ������,�ض�ʱ��
if(TR0==0){flag =1; }  //TR0 ��1��Ϊ0 ��,FLAG = 1 ˵���������,�ض�ʱ��,�ó���J
}

void int_t0() interrupt 1{
j++;
}




unsigned int handle1 (void){   // ����һ������
if(flag == 1){ //�ó���J,���Կ��Խ���������;
	data0  =  j*2; // ���һ���� ms Ϊ ��λ�� ���� 0.04 *100;
	data0  =  100000/data0; //�ó�Ƶ�� (1000/����)*10 *100;
data0 = 10*data0; 	 //  ��ʽΪ10
	//ƥ�䵽ת��
	//data0 = data0 - 0.067*data0;
	ten[intTIME] = data0; ;
   intTIME +=1;
	return data0;
}
}


unsigned int handle3(){
unsigned char x;
unsigned int temp;
unsigned int z = 0;

	for(x=0;x<9;x++){ //ten[9]����������;  
if(ten[x]>ten[x+1]){
temp = ten[x];
ten[x] = ten[x+1] ;
ten[x+1] = temp;
}
}
for(x=8;x>1;x--){ //ten[9]����������;  ��8��ʼ ,��ten[0] ��С
if(ten[x]<ten[x-1]){
temp = ten[x];
ten[x  ] = ten[x - 1 ] ;
ten[x -1 ] = temp;
}
}

for(x = 1 ; x< 9; x++){ // ���� x =1 - 8 ;
z += ten[x] ;
}

z= z *0.125;

return z ;
	
}

void display(unsigned int datas){ //��ʾģ��,��ʾת��,����һ��4λת����ֵ ;    
	
	//if(datas<1000){while(1){
		COM = 0x01; //ǧλ,������ .
		smg_DATA = smgduan[datas/1000];
		delay1ms();
		smg_DATA = 0xff;

		COM = 0x02; // ��λ
		smg_DATA = smgduan[datas/100%10];
		delay1ms();
		smg_DATA = 0xff;
	delay1ms();
		COM =0x04; //ʮλ
		smg_DATA =smgduan[datas/10%10] ;
		delay1ms();
		smg_DATA = 0xff;
 delay1ms();

		COM = 0x08; //��λ
		smg_DATA = smgduan[datas%10];
		delay1ms();
		smg_DATA = 0xff;
	//}}
	
	
}
		
	



void main(){
unsigned  int  z=0 ;
init_E0();
initT0 ();
	
	while(1){     //����ѭ��	
		handle1();
display(1);
		if(intTIME == 10){ //�ж�110�κ�,�ر��ж�,������ʾѭ��
	
			EA = 0 ;
	z = handle3();

	while(1)
		{	
			if(z<1000){z=z*10;
while(1){
		display(z); //���FLAG = 1,˵���ó���J�ı���,����µ�ֵ,�������ԭ����ֵ,��Ϊ��ȫ�ֱ���,����ֵ��һֱ����
    COM =0x04;
			smg_DATA = 0x7f;
	delay1ms();
	smg_DATA = 0xff;
	delay1ms();
}


}
else {
while(1){
display(z);

}
}

}

}
}
}