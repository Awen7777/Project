#include<reg51.h>
/*功能寄存器*/
sfr Lamp=0x80;//指示灯
sfr Lamp0=0xa0;//指示灯
sfr Keyboard=0xb0;//键盘
sfr Signal=0x90; 

sbit Lamp1up=P0^0;//P0口控制外呼上下行指示灯
sbit Lamp2up=P0^1;//表示二楼上行指示灯
sbit Lamp3up=P0^2;
sbit Lamp4up=P0^3;
sbit Lamp5down=P0^4;//表示五楼下行指示灯
sbit Lamp2down=P0^5;
sbit Lamp3down=P0^6;
sbit Lamp4down=P0^7;

sbit Lamp1=P2^0;//P2引脚为内选请求指示灯
sbit Lamp2=P2^1;//二楼指示灯
sbit Lamp3=P2^2;
sbit Lamp4=P2^3;
sbit Lamp5=P2^4;
sbit Lampopen=P2^5;
sbit Lampclose=P2^6;
sbit Lampdanger=P2^7;
char Location=1;
char Way=0;
char Keyboardarray[]={0xfe,0xfd,0xfb,0xf7};//键盘行选
char Buttonarray[] = {0xee,0xde,0xbe,0x7e,
				  0xed,0xdd,0xbd,0x7d,
				  0xeb,0xdb,0xbb,0x7b,
				  0xe7,0xd7,0xb7,0x77};
char Hoistcontrolarray[]={0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};//电梯控制
char Cong_sentsavearray[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
char Lamp_controlarray[]={0x0f,0x1f,0x2f,0x3f,0xcf,0xbf,0xaf,0x9f};//1up 2up 3up 4up 5down ......
char Cong_acceptsavearray[2]={0xff,0xff};
void Cong_RequestSent(char Location,char Way);
void Cong_RequestAccept();
/*指示灯点亮子程序*/
void Lamp_light()
{
	 char H,L,i=10;
     for(H=0;H<4;H++)
	     {		                                                                   	// H:行选
	     Keyboard=Keyboardarray[H];			                                      // 各行键盘激活语句
	     for(L=0;L<4;L++)	            	                                      // L:列选
	          {
	          if(Keyboard==Buttonarray[H*4+L])
			      {
				   switch(H*4+L)				                                 	//点亮按键相应请求灯
				       {
					    case 0:
						    {
						     if(Location==1)
							     Signal=0xf8;
							 else Lamp1up=1;
							 break;
							 }
						case 1:
						     {
							  if(Location==2)
							      Signal=0xf8;
							  else Lamp2up=1;
							  break;
							 }
						case 2:
						      {
							   if(Location==3)
							      Signal=0xf8;
							   else Lamp3up=1; 
					           break;
							  }
						case 3:
						      {
							   if(Location==4)
							      Signal=0xf8;
							   else Lamp4up=1;
							   break;
							  }
						case 4:
						      {
							   if(Location==5)
							      Signal=0xf8;
							   else Lamp5down=1; 
							   break;
							  }
						case 5:
						      {
							  if(Location==2)
							      Signal=0xf8;
							  
							  else Lamp2down=1;
							  break;
							  }
						case 6:
						      {
							  if(Location==3)
							      Signal=0xf8;
							  else Lamp3down=1; 
							  break;
							  }
						case 7:
						      {
							  if(Location==4)
							      Signal=0xf8;
							  else Lamp4down=1; 
							  break;
							  }
						case 8:
						      {
							  if(Location==1)
							      Signal=0xf8;
							  else Lamp1=1;
							  break;
							  }
						case 9:
						      {
							  if(Location==2)
							      Signal=0xf8;
							  else Lamp2=1; 
							  break;
							  }
						case 10:
						      {
							  if(Location==3)
							      Signal=0xf8;
							  else Lamp3=1; 
							  break;
							  }
						case 11:
						      {
							  if(Location==4)
							      Signal=0xf8;
							  else Lamp4=1;
							  break;
							  }
						case 12:
						      {
							  if(Location==5)
							      Signal=0xf8;
							  else Lamp5=1;
							  break;
							  }
						case 13:
						      {
							  Lampopen=1;
							  break;
							  }
						case 14:
						      {
							  Lampclose=1;
							  break;
							  }
						case 15:
						      {
							  Lampdanger=1;
							  break;
							  }
}}
        }
    }
}
/*处理电梯请求接受操作*/
void Cong_RequestAccept()//根据当前的状态来更新指示灯的状态,并保存相关的信息供其他函数使用
{
	Cong_acceptsavearray[0]=Signal;//将寄存器Signal请求保存到Cong_acceptsavearray
	if(Cong_acceptsavearray[0]==0x08)//0x08表示同层请求
		Signal=0xff;
	switch((((Cong_acceptsavearray[0]>>4)<<4)+0x0f))//确定电梯楼层和运行方向							 
    {																		
   case 0x0f:{Location=1;Way=0;Lamp1up=0;Lamp1=0;Cong_sentsavearray[0]=0xff;break;}
	 case 0x1f:{Location=2;Way=0;Lamp2up=0;Lamp2=0;Cong_sentsavearray[1]=0xff;break;} 
	 case 0x2f:{Location=3;Way=0;Lamp3up=0;Lamp3=0;Cong_sentsavearray[2]=0xff;break;}
	 case 0x3f:{Location=4;Way=0;Lamp4up=0;Lamp4=0;Cong_sentsavearray[3]=0xff;break;}
	 case 0xcf:{Location=5;Way=4;Lamp5down=0;Lamp5=0;Cong_sentsavearray[4]=0xff;break;}
	 case 0xbf:{Location=4;Way=4;Lamp4down=0;Lamp4=0;Cong_sentsavearray[5]=0xff;break;}
	 case 0xaf:{Location=3;Way=4;Lamp3down=0;Lamp3=0;Cong_sentsavearray[6]=0xff;break;}
	 case 0x9f:{Location=2;Way=4;Lamp2down=0;Lamp2=0;Cong_sentsavearray[7]=0xff;break;}
	 case 0x4f:{Lampopen=0;}
	 case 0x5f:{Lampclose=0;}
	 case 0x6f:{Lampdanger=0;}

	}
}
void Cong_RequestActivate()
{
    if(Lamp1 || Lamp1up)
        Cong_sentsavearray[0]=Hoistcontrolarray[0];

    if((Lamp2 && Way==0) || Lamp2up)
        Cong_sentsavearray[1]=Hoistcontrolarray[1];

    if((Lamp3 && Way==0) || Lamp3up)
        Cong_sentsavearray[2]=Hoistcontrolarray[2];

    if((Lamp4 && Way==0) || Lamp4up)
        Cong_sentsavearray[3]=Hoistcontrolarray[3];

    if(Lamp5 || Lamp5down)
        Cong_sentsavearray[4]=Hoistcontrolarray[4];

    if((Lamp4 && Way==4) || Lamp4down)
        Cong_sentsavearray[5]=Hoistcontrolarray[5];

    if((Lamp3 && Way==4) || Lamp3down)
        Cong_sentsavearray[6]=Hoistcontrolarray[6];

    if((Lamp2 && Way==4) || Lamp2down)
        Cong_sentsavearray[7]=Hoistcontrolarray[7];
}

void Cong_RequestSent(char Location, char Way)
{
    char i;
    for(i=0; i<8; i++)
    {
        if(Way==0 && (Cong_sentsavearray[(Location-1+i)%8] ^ 0xff))
        {
            Signal=Cong_sentsavearray[(Location-1+i)%8];
            break;
        }
        else if(Location==5 && (Cong_sentsavearray[(Location-1+i)%8] ^ 0xff))
        {
            Signal=Cong_sentsavearray[(Location-1+i)%8];
            break;
        }
        else if(Way==4 && (Cong_sentsavearray[(Way+5-Location+i)%8] ^ 0xff) && Location!=5)
        {
            Signal=Cong_sentsavearray[(Way+5-Location+i)%8];
            break;
        }
    }
}
void main()
{
	
	
	Lamp=0x00;
	Lamp0=0x00;
	Signal=0xff;
	while(1)
	{
		Lamp_light();
		Cong_RequestActivate();
	  Cong_RequestSent(Location,Way);
	  Cong_RequestAccept();	  
		
	}
	
}