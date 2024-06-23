	#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sfr Seg=0x80;//将0x80的地址映射到名为Seg的特殊功能寄存器，用于控制数码管的段选信号
sfr Motor=0xa0;//寄存器用于控制电机的运行
sfr Signal=0xb0;//用于接收外部发送的信号，仅使用了低四位
sbit Opendoor=P1^0;//请求开门 0有效
sbit Closedoor=P1^1;//请求关门，0有效
sbit Warning=P1^2;//报警,0表示报警(检测其他类型报警)
sbit Up=P1^3;//上行指示灯
sbit Down=P1^4;//下行指示灯
sbit Switch=P1^5;//表示电梯是否正常运行，在电路中我们用1表示电梯正常运行，0表示故障
sbit Overweight=P1^6;//超重信号由p1.6控制，0表示超重（检测超重报警）
sbit beep=P1^7;//蜂鸣器

char Floor_show[]={0xff,0xf9,0xa4,0xb0,0x99,0x92};//定义数码管的显示，表示楼层从1-5，Floor[0]表示
char Door1[4]={0x40,0x80,0x10,0x20};//门系统的四相四拍工作方式，反转（暂时还不知道怎么来的）
char Door2[4]={0x20,0x10,0x80,0x40};//正转，开门
char Hoist1[4]={0x04,0x08,0x01,0x02};//起重机四相四拍工作方式,反转下行
char Hoist2[4]={0x02,0x01,0x08,0x04};//正转，上行
char Location=1;//表示电梯所在的楼层
char Way=0;//表示电梯运行方向 0 4
char Layer=0;//表示电梯需要移动的楼层数，在实际应用中这种设置完全没有必要，在这里这样设置是为了后面计算
char Doorstatus=0;//表示电梯门的状态，0表示电梯门关着，1表示电梯门开着
char Zhusendarray[2]={0x0f};//	因初始状态Way=0 Location=1，所以为0x0f 
char Zhuacceptarray[]={0xFf};//临时存储接收到的信号值,初始化成1111 1111
char Lampcontrolarray[]={0x0f,0x1f,0x2f,0x3f,0xcf,0xbf,0xaf,0x9f};//灯显示状态信号（楼层和上下行显示）
char HoistControlArray[]={0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
void Zhu_requestsend();
void Zhu_status();

/*延时函数实现*/
void Delay(uint type)//写这么多的延时函数是为了下面起重机加速减速坐准备
{
    unsigned char a, b, c;
    switch (type)
    {
        case 1: // 1ms延时
            c = 10;
            b = 42;
            a = 2;
            break;
        case 2: // 100ms延时
            c = 19;
            b = 20;
            a = 130;
            break;
        case 3: // 62.5ms延时
            b = 224;
            a = 138;
            break;
        case 4: // 31.25ms延时
            c = 2;
            b = 220;
            a = 34;
            break;
        case 5: // 20.825ms延时
            b = 58;
            a = 178;
            break;
        case 6: // 300ms延时
            c = 5;
            b = 212;
            a = 140;
            break;
        case 7: // 2s延时
            c = 127;
            b = 235;
            a = 32;
            break;
        default:
            // 默认为1ms延时
            c = 10;
            b = 42;
            a = 2;
            break;
    }

    if (type != 3&&type != 5) 
    {
        for (; c > 0; c--)
        {
            for (; b > 0; b--)
            {
                for (; a > 0; a--)
                {
                    // 延时实现
                }
            }
        }
    }
    else // 单独处理62.5ms延时
    {
        for (; b > 0; b--)
        {
            for (; a > 0; a--)
            {
                // 延时实现
            }
        }
    }
}


/*Switch开关检测子程序*/
void Switch_judge()
{
while(1)
    {
	 if(Switch==0)//如果被按下那么即可跳出循环，否则就一直检测端口
	     break;
	  }
}





/*超重报警子函数*/
void Over_weight()
{
	if(Overweight==0)//在此处将低电平表示超重
	{
		beep=~beep;
		Delay(1);
	}
}




/*电梯(起重机)加速或者减速运行子程序*/
void Hoist_speed1(char Way)					 //电机0m/s--0.5m/s间 加速或减速行驶子程序（加速度 0.75）
{
char time,rotate;
	if(Warning==0)//根据电路图分析低电平时警报产生
	   {
	  	Switch=1;
		  while(1)
		    {
		     beep=~beep;
	         Delay(1);
		    }
	   }
switch(Way)
    {							 // Way:正反转判断	0 正传，否则 反转
    case 0:
	     {
         for(time=0;time<2;time++)
             for(rotate=0;rotate<4;rotate++)
               {
		            Motor=Hoist2[rotate];
	              Delay(3);			 
	             }
	     break;
	     }
	  default :
	     {
	     for(time=0;time<2;time++)
             for(rotate=0;rotate<4;rotate++)
                 {
		            Motor=Hoist1[rotate];
	              Delay(3);	
	             }
	     }
    }
}

void Hoist_speed2(char Way)// 电机 0.5m/s--1.0m/s间 加速或减速行驶子程序（加速度 0.75）
{
char time,rotate;
	if(Warning==0)
	   {
	  	Switch=1;
		while(1)
		    {
		     beep=~beep;
	        Delay(1);
		    }
	   } 
switch(Way)
    {
    case 0://Way0电机正转，电梯上升
	     {
         for(time=0;time<4;time++)
             for(rotate=0;rotate<4;rotate++)
             {
		         Motor=Hoist2[rotate];
	             Delay(4);				 
	         }
		 break;
	     }
    default:
	     {
         for(time=0;time<4;time++)
             for(rotate=0;rotate<4;rotate++)
             {
               Motor=Hoist1[rotate];
	             Delay(4);		 
	         }
	     }
    }
}
void Hoist_speed3(char Way)//电机 1.0m/s--1.5m/s间 加速或减速行驶子程序（加速度 0.75）
{
char time,rotate;
	if(Warning==0)
	   {
	  	Switch=1;
		  while(1)
		    {
		     beep=~beep;
	       Delay(1);
		    }
	   }
switch(Way)
    {
    case 0:
	     { 
              for(time=0;time<6;time++)
                  for(rotate=0;rotate<4;rotate++)
                  {
		              Motor=Hoist2[rotate];
	                  Delay(5);
	              }
		 break;
		 }
    default:
	     { 
              for(time=0;time<6;time++)
                  for(rotate=0;rotate<4;rotate++)
                  {
		              Motor=Hoist1[rotate];
	                  Delay(5);
	              }
		 }
    }
}
//0.75s
void Hoist_speed4(char Way,char one,char two )// 1.5m/s 匀速行驶子程序（加速度 0）（注：此为0.5s的匀速行驶）
{
char time,rotate;
	if(Warning==0)
	   {
	  	Switch=1;
		  while(1)
		    {
		     beep=~beep;
	       Delay(1);
		    }
	   }
switch(Way)
    {

    case 0:
	     { 
              for(time=0;time<6;time++)
                  for(rotate=0;rotate<4;rotate++)
                  {
		                Motor=Hoist2[rotate];
	                  Delay(5);
					  if(Layer!=1 && two==1 && one==3 && time*rotate==12 )		  //桥厢位置确定
					      {
					      Seg=Floor_show[++Location];//此处Flow_show表示数码管显示段码
						    one=0;
						    two=0;

						  }	
	              }
		 break;
		 }
    default:
	     { 
              for(time=0;time<6;time++)
                  for(rotate=0;rotate<4;rotate++)
                  {
		                Motor=Hoist1[rotate];
	                  Delay(5);
					  if(Layer!=1 && two==1 && one==3 && time*rotate==12 )
					      {
					      Seg=Floor_show[--Location];
						    one=0;
						    two=0;

						  }	
	              }
		 }
    }
}
/*电梯上下行指示灯亮灭子程序*/
void Lamp_of_updown()
{
	if(Way==0&Layer!=0)//Way=0时表示上行Way=4时表示下行（不用0和1也是为了下面的技术实现）
	{
		Up=1;//高电平有效
		Down=0;
	}
	else if(Way==4&Layer!=0)
	{
		Up=0;
		Down=1;
	}
	else if(Layer==0)//这种情况就是电梯门打开但是不想乘坐了所以既不上行也不下行
	{
		Up=0;
		Down=0;
	}
}

/*确定Layer和Way子程序*/
void Judge()
{
	switch(Zhuacceptarray[0]<<4)//根据接收到的请求信号,计算出电梯需要移动的楼层数(Layer)和移动方向(Way)
	{
		case 0x00://计算出Layer和Way的值位后续的电机控制和指示灯控制提供依据
		{
			if((Layer=Location-1)>0) Way=4;
			else Way=0;
			break;
		}
		case 0x10:
		{
			if((Layer=Location-2)>0) Way=4;
			else {Layer=2-Location;Way=0;}
			break;
		}
		case 0x20:
	  {
		if((Layer=Location-3)>0) Way=4;
		else {Layer=3-Location;Way=0;}
		break;
		}
	  case 0x30:
	  {
		if((Layer=Location-4)>0) Way=4;
		else {Layer=4-Location;Way=0;}
		break;
		}
	  case 0x40:
	  {
		if((Layer=5-Location)>0) Way=0;
		else Way=4;
		break;
		}
    case 0x50:
	  {
		if((Layer=4-Location)>0) Way=0;
		else {Layer=Location-4;Way=4;}
		break;
		}
	  case 0x60:
	  {
		if((Layer=3-Location)>0) Way=0;
		else {Layer=Location-3;Way=4;}
		break;
		}
	  case 0x70:
	  {
		if((Layer=2-Location)>0) Way=0;
		else {Layer=Location-2;Way=4;}
		break;
		}
	  case 0x80:
	    {
		   Layer=0;
			}
	}
}

/*电梯到达蜂鸣子程序*/
void Arrive_sound()//电梯到达子程序，当电梯到达乘客指定楼层后，蜂鸣器发出声音提示乘客到达
{
	unsigned char a,b,c;
        for(c=3;c>0;c--)
            for(b=2;b>0;b--)
                for(a=10;a>0;a--)
	
					{   
			 		 beep=~beep;
	 	    		 Delay(1);
					}			 
}
/*起重电机控制子程序（需要调用上面创建的加速减速匀速程序来实现）*/
void Hoist()
{
	char i;//匀速阶段子程序循环次数
	char One=0, Two=1;
	Lamp_of_updown();//电梯上下行指示灯
	Hoist_speed1(Way);//无论上行几层还是下行几层，首先经历的一定是加速过程
	Hoist_speed2(Way);//总的加速减速阶段
	Hoist_speed3(Way);
	//匀速过程
  for(i=0;i<3+6*(Layer-1);i++)
	{
		Hoist_speed4(Way,One,Two);
		One++;
		if(i%3==0)//每当电梯匀速运行1.5s重新扫描引脚状态
		{
			if((Signal<<4)^(Zhuacceptarray[0]<<4))//如果发生变化就进行更改
			{
				Zhuacceptarray[0]=Signal;
				Judge();
			}
			if(i!=3&&(i-3)%6==0)
			{
				Two=1;
				One=3;
			}
		}
	}
	Hoist_speed3(Way);//实现减速过程
	Hoist_speed2(Way);
	Hoist_speed1(Way);
	Switch_judge();//判断电梯能否正常运行
	Arrive_sound();//电梯门到达蜂鸣器
	Zhu_status();
	if(Way==0)
	
		Seg=Floor_show[++Location];//改变Location的值并传到寄存器Seg中
	else
		Seg=Floor_show[--Location];
}
/*电梯门打开函数实现*/
void Door_open()//电梯门的开启和关闭是通过一个电机驱动的,电机会以四相四拍的方式工作,每次转动 90 度。这样可以实现电梯门的缓慢开合。
{
	char a;
	for(a=0;a<8;a++)
 {
	Motor=Door2[a%4];//四次循环逐步将电梯门打开
	Delay(6);//延时300ms
	
 }
}
/*电梯门关闭函数实现*/
void Door_close()
{
	char a;
	if(Doorstatus==1)
	{
		for(a=0;a<8;a++)
	{
		Motor=Door1[a%4];
		Delay(6);//延时300ms
	}
	}
	
}


/*电梯内部请求实现*/
void Indoor_requst(char a)//a为请求的次数,主要是来设置时间，这个时间用来检测各种引脚的状态，
{                       	//在实际中就是电梯门开启时间间隔
	char b;
	for(b=0;b<a;b++)
	{
		//检测超重报警
		Over_weight();
		//检测其他类型报警
		if(Warning==0)
		{
			Switch=1;
			while(1)
			{
				beep=~beep;//并报警
				Delay(1);
			}
		}
		if(Opendoor==0&&Doorstatus==1)//有开门请求并且门处于开状态
		{
			Signal=0x4f;//修改Signal
		}
		if(Closedoor==0&&Doorstatus==0)//有关门请求且门已经处于关闭状态
		{Signal=0x5f;}
		if(Opendoor==0&&Doorstatus==0)//有开门请求但是门门处于关闭状态
		{
			Door_open();//先将门打开
			Signal=0x4f;//然会再修改信号
			Doorstatus=1;//然后将门的状态置为高电平,门处于打开状态
		}
		if(Closedoor==0&&Doorstatus==1)
		{
			Door_close();
			Signal=0x5f;
			Doorstatus=0;
		}
		Delay(2);//延迟100ms
	}
}
/*电梯门控制子程序*/
void Door_control()
{
	
	Door_open();//电梯门开启
	Doorstatus=1;
	Indoor_requst(50);//50*100ms=5s,设定5s开门时间电梯门自动关闭
	Door_close();//电梯门自动关闭
	Doorstatus=0;
	Indoor_requst(5);//给出5*100ms的时间乘客在这段时间内可以进行开门等其他操作
}

/*主机信号传递子程序*/
void Zhu_requestsend()//主要作用是根据电梯的位置和运行方向生成相应的信号发送出去
{
	if(Location==1)//电梯在1楼
	{
		Zhusendarray[0]=Lampcontrolarray[0];
		
		Signal=Zhusendarray[0];
	}
	else if(Location==2&&Way==0)//二楼且上行
	{
		Zhusendarray[0]=Lampcontrolarray[1];//Lampcontrolarray最高位表示电梯运行方向（0上行）次低四位表示楼层
	  Signal=Zhusendarray[0];
	}
	else if(Location==3&&Way==0)//三楼上行
	{
		Zhusendarray[0]=Lampcontrolarray[2];
	  Signal=Zhusendarray[0];
	}
	else if(Location==4&&Way==0)
	{
		Zhusendarray[0]=Lampcontrolarray[3];
	  Signal=Zhusendarray[0];
	}
	else if(Location==5)
	{
		Zhusendarray[0]=Lampcontrolarray[4];
	  Signal=Zhusendarray[0];
	}
	else if(Location==4&&Way==4)
	{
		Zhusendarray[0]=Lampcontrolarray[5];
	  Signal=Zhusendarray[0];
	}
	else if(Location==3&&Way==4)//三楼上行
	{
		Zhusendarray[0]=Lampcontrolarray[6];
	  Signal=Zhusendarray[0];
	}
	else if(Location==2&&Way==4)//二楼且上行
	{
		Zhusendarray[0]=Lampcontrolarray[7];//Lampcontrolarray最高位表示电梯运行方向（0上行）次低四位表示楼层
	  Signal=Zhusendarray[0];
	}
}
/*用户请求信号接受子程序*/
void Zhu_requestaccept1()//用户进入电梯后外呼内应都没有时
{
	while(1)
	{
		Switch_judge();
		Zhu_requestsend();//发送当前位置的请求信号
		if((Signal<<4)^(Zhuacceptarray[0]<<4))
		{
			Zhuacceptarray[0]=Signal;
			Judge();
			if(Layer!=0)
				break;
		}
	}
}
	

void Zhu_requestaccept()
{
	if((Signal<<4) ^ (Zhuacceptarray[0]<<4))//异或，不同为真，不同说明有新的信号需要接收
	{
		if(Doorstatus==1) Door_close();
		if((Signal<<4)^0x80) Zhuacceptarray[0]=Signal;
		Judge();//判断下Way和Layer的值
		if(Layer==0)//当电梯处于外呼楼层时直接开门
		{
			Door_control();
			Zhu_requestaccept1();
		}
		else//当电梯不在外呼楼层时
		{
			Hoist();//电梯到达外呼楼层后再打开
		  Door_control();//控制电梯门开闭
			Judge();//根据当下情况更新Layer和Way的值
			Lamp_of_updown();//更新电梯上下行指示灯
		}
	}
}
void Zhu_status()
{
	Layer=0;
}

void main()
{
	Seg=Floor_show[Location];//将当前楼层的显示信息赋值给寄存器Seg
	Up=0;
	Down=0;//先将上下行指示灯关闭
	Switch=1;
	Signal=0x0f;
	Motor=0xdd;
	beep=0;
	while(1)
	{
		Switch_judge();//当电梯正常运转时跳出这个函数循环执行下面的程序
		Zhu_requestaccept();//接受请求信号,然后根据信号来确定开门更新等操作
		Indoor_requst(5);//内部请求，用于处理进入电梯后的各种情况
		if(Layer!=0)//表示按下了某一按键
		{
			Hoist();//电梯运转
			Door_control();//电梯门控制
			Judge();////根据当下情况更新Layer和Way的值
			Lamp_of_updown();//更新电梯上下行指示灯
		}
		Zhu_requestsend();//发送当前位置的请求信号
	}
}
