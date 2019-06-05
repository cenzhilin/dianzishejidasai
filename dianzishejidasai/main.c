
#include <reg51.h>
//#include <intrins.h>

#define COM P1// 接低位或者高位

#define smg_DATA  P2 //数码管数据管脚


unsigned code smgduan[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

unsigned  int data0=0;  //预先定义,节省初始化时间
unsigned  int ten[10];
unsigned  char intTIME =0; //计算外部中断次数.
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
	TR1 = 1 ;    //开T1 
	EA=1; //开全局中断
}

void initT0 () //T0 定时器定时 250   
{
TMOD =0X02;  //定时器0 方式二,8位重装载,
ET0 = 1;
TR0 = 0;
TH0 =236 ;// 20us  250us
TL0 =236;  // 20us

}

void interrupt_0 () interrupt 0
{
if(TR0 == 0){flag =0;j=0;}   // 因为TR0 在上一次结束后是0,进行初始化数值.,然后下一次 开始 TR0 还是0;
	TR0=~TR0;	    //一次跳变 ,开定时器, 一次跳变,关定时器
if(TR0==0){flag =1; }  //TR0 从1变为0 后,FLAG = 1 说明计数完成,关定时器,得出了J
}

void int_t0() interrupt 1{
j++;
}




unsigned int handle1 (void){   // 储存一次周期
if(flag == 1){ //得出了J,所以可以进行运算了;
	data0  =  j*2; // 算出一个以 ms 为 单位的 周期 0.04 *100;
	data0  =  100000/data0; //得出频率 (1000/周期)*10 *100;
data0 = 10*data0; 	 //  公式为10
	//匹配到转速
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

	for(x=0;x<9;x++){ //ten[9]的数是最大的;  
if(ten[x]>ten[x+1]){
temp = ten[x];
ten[x] = ten[x+1] ;
ten[x+1] = temp;
}
}
for(x=8;x>1;x--){ //ten[9]的数是最大的;  从8开始 ,让ten[0] 最小
if(ten[x]<ten[x-1]){
temp = ten[x];
ten[x  ] = ten[x - 1 ] ;
ten[x -1 ] = temp;
}
}

for(x = 1 ; x< 9; x++){ // 计算 x =1 - 8 ;
z += ten[x] ;
}

z= z *0.125;

return z ;
	
}

void display(unsigned int datas){ //显示模块,显示转速,输入一个4位转速数值 ;    
	
	//if(datas<1000){while(1){
		COM = 0x01; //千位,共阴极 .
		smg_DATA = smgduan[datas/1000];
		delay1ms();
		smg_DATA = 0xff;

		COM = 0x02; // 百位
		smg_DATA = smgduan[datas/100%10];
		delay1ms();
		smg_DATA = 0xff;
	delay1ms();
		COM =0x04; //十位
		smg_DATA =smgduan[datas/10%10] ;
		delay1ms();
		smg_DATA = 0xff;
 delay1ms();

		COM = 0x08; //个位
		smg_DATA = smgduan[datas%10];
		delay1ms();
		smg_DATA = 0xff;
	//}}
	
	
}
		
	



void main(){
unsigned  int  z=0 ;
init_E0();
initT0 ();
	
	while(1){     //程序循环	
		handle1();
display(1);
		if(intTIME == 10){ //中断110次后,关闭中断,进入显示循环
	
			EA = 0 ;
	z = handle3();

	while(1)
		{	
			if(z<1000){z=z*10;
while(1){
		display(z); //如果FLAG = 1,说明得出的J改变了,输出新的值,否则输出原本的值,因为是全局变量,所以值会一直保留
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