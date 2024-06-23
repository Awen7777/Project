	#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sfr Seg=0x80;//��0x80�ĵ�ַӳ�䵽��ΪSeg�����⹦�ܼĴ��������ڿ�������ܵĶ�ѡ�ź�
sfr Motor=0xa0;//�Ĵ������ڿ��Ƶ��������
sfr Signal=0xb0;//���ڽ����ⲿ���͵��źţ���ʹ���˵���λ
sbit Opendoor=P1^0;//������ 0��Ч
sbit Closedoor=P1^1;//������ţ�0��Ч
sbit Warning=P1^2;//����,0��ʾ����(����������ͱ���)
sbit Up=P1^3;//����ָʾ��
sbit Down=P1^4;//����ָʾ��
sbit Switch=P1^5;//��ʾ�����Ƿ��������У��ڵ�·��������1��ʾ�����������У�0��ʾ����
sbit Overweight=P1^6;//�����ź���p1.6���ƣ�0��ʾ���أ���ⳬ�ر�����
sbit beep=P1^7;//������

char Floor_show[]={0xff,0xf9,0xa4,0xb0,0x99,0x92};//��������ܵ���ʾ����ʾ¥���1-5��Floor[0]��ʾ
char Door1[4]={0x40,0x80,0x10,0x20};//��ϵͳ���������Ĺ�����ʽ����ת����ʱ����֪����ô���ģ�
char Door2[4]={0x20,0x10,0x80,0x40};//��ת������
char Hoist1[4]={0x04,0x08,0x01,0x02};//���ػ��������Ĺ�����ʽ,��ת����
char Hoist2[4]={0x02,0x01,0x08,0x04};//��ת������
char Location=1;//��ʾ�������ڵ�¥��
char Way=0;//��ʾ�������з��� 0 4
char Layer=0;//��ʾ������Ҫ�ƶ���¥��������ʵ��Ӧ��������������ȫû�б�Ҫ������������������Ϊ�˺������
char Doorstatus=0;//��ʾ�����ŵ�״̬��0��ʾ�����Ź��ţ�1��ʾ�����ſ���
char Zhusendarray[2]={0x0f};//	���ʼ״̬Way=0 Location=1������Ϊ0x0f 
char Zhuacceptarray[]={0xFf};//��ʱ�洢���յ����ź�ֵ,��ʼ����1111 1111
char Lampcontrolarray[]={0x0f,0x1f,0x2f,0x3f,0xcf,0xbf,0xaf,0x9f};//����ʾ״̬�źţ�¥�����������ʾ��
char HoistControlArray[]={0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
void Zhu_requestsend();
void Zhu_status();

/*��ʱ����ʵ��*/
void Delay(uint type)//д��ô�����ʱ������Ϊ���������ػ����ټ�����׼��
{
    unsigned char a, b, c;
    switch (type)
    {
        case 1: // 1ms��ʱ
            c = 10;
            b = 42;
            a = 2;
            break;
        case 2: // 100ms��ʱ
            c = 19;
            b = 20;
            a = 130;
            break;
        case 3: // 62.5ms��ʱ
            b = 224;
            a = 138;
            break;
        case 4: // 31.25ms��ʱ
            c = 2;
            b = 220;
            a = 34;
            break;
        case 5: // 20.825ms��ʱ
            b = 58;
            a = 178;
            break;
        case 6: // 300ms��ʱ
            c = 5;
            b = 212;
            a = 140;
            break;
        case 7: // 2s��ʱ
            c = 127;
            b = 235;
            a = 32;
            break;
        default:
            // Ĭ��Ϊ1ms��ʱ
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
                    // ��ʱʵ��
                }
            }
        }
    }
    else // ��������62.5ms��ʱ
    {
        for (; b > 0; b--)
        {
            for (; a > 0; a--)
            {
                // ��ʱʵ��
            }
        }
    }
}


/*Switch���ؼ���ӳ���*/
void Switch_judge()
{
while(1)
    {
	 if(Switch==0)//�����������ô��������ѭ���������һֱ���˿�
	     break;
	  }
}





/*���ر����Ӻ���*/
void Over_weight()
{
	if(Overweight==0)//�ڴ˴����͵�ƽ��ʾ����
	{
		beep=~beep;
		Delay(1);
	}
}




/*����(���ػ�)���ٻ��߼��������ӳ���*/
void Hoist_speed1(char Way)					 //���0m/s--0.5m/s�� ���ٻ������ʻ�ӳ��򣨼��ٶ� 0.75��
{
char time,rotate;
	if(Warning==0)//���ݵ�·ͼ�����͵�ƽʱ��������
	   {
	  	Switch=1;
		  while(1)
		    {
		     beep=~beep;
	         Delay(1);
		    }
	   }
switch(Way)
    {							 // Way:����ת�ж�	0 ���������� ��ת
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

void Hoist_speed2(char Way)// ��� 0.5m/s--1.0m/s�� ���ٻ������ʻ�ӳ��򣨼��ٶ� 0.75��
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
    case 0://Way0�����ת����������
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
void Hoist_speed3(char Way)//��� 1.0m/s--1.5m/s�� ���ٻ������ʻ�ӳ��򣨼��ٶ� 0.75��
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
void Hoist_speed4(char Way,char one,char two )// 1.5m/s ������ʻ�ӳ��򣨼��ٶ� 0����ע����Ϊ0.5s��������ʻ��
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
					  if(Layer!=1 && two==1 && one==3 && time*rotate==12 )		  //����λ��ȷ��
					      {
					      Seg=Floor_show[++Location];//�˴�Flow_show��ʾ�������ʾ����
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
/*����������ָʾ�������ӳ���*/
void Lamp_of_updown()
{
	if(Way==0&Layer!=0)//Way=0ʱ��ʾ����Way=4ʱ��ʾ���У�����0��1Ҳ��Ϊ������ļ���ʵ�֣�
	{
		Up=1;//�ߵ�ƽ��Ч
		Down=0;
	}
	else if(Way==4&Layer!=0)
	{
		Up=0;
		Down=1;
	}
	else if(Layer==0)//����������ǵ����Ŵ򿪵��ǲ�����������ԼȲ�����Ҳ������
	{
		Up=0;
		Down=0;
	}
}

/*ȷ��Layer��Way�ӳ���*/
void Judge()
{
	switch(Zhuacceptarray[0]<<4)//���ݽ��յ��������ź�,�����������Ҫ�ƶ���¥����(Layer)���ƶ�����(Way)
	{
		case 0x00://�����Layer��Way��ֵλ�����ĵ�����ƺ�ָʾ�ƿ����ṩ����
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

/*���ݵ�������ӳ���*/
void Arrive_sound()//���ݵ����ӳ��򣬵����ݵ���˿�ָ��¥��󣬷���������������ʾ�˿͵���
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
/*���ص�������ӳ�����Ҫ�������洴���ļ��ټ������ٳ�����ʵ�֣�*/
void Hoist()
{
	char i;//���ٽ׶��ӳ���ѭ������
	char One=0, Two=1;
	Lamp_of_updown();//����������ָʾ��
	Hoist_speed1(Way);//�������м��㻹�����м��㣬���Ⱦ�����һ���Ǽ��ٹ���
	Hoist_speed2(Way);//�ܵļ��ټ��ٽ׶�
	Hoist_speed3(Way);
	//���ٹ���
  for(i=0;i<3+6*(Layer-1);i++)
	{
		Hoist_speed4(Way,One,Two);
		One++;
		if(i%3==0)//ÿ��������������1.5s����ɨ������״̬
		{
			if((Signal<<4)^(Zhuacceptarray[0]<<4))//��������仯�ͽ��и���
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
	Hoist_speed3(Way);//ʵ�ּ��ٹ���
	Hoist_speed2(Way);
	Hoist_speed1(Way);
	Switch_judge();//�жϵ����ܷ���������
	Arrive_sound();//�����ŵ��������
	Zhu_status();
	if(Way==0)
	
		Seg=Floor_show[++Location];//�ı�Location��ֵ�������Ĵ���Seg��
	else
		Seg=Floor_show[--Location];
}
/*�����Ŵ򿪺���ʵ��*/
void Door_open()//�����ŵĿ����͹ر���ͨ��һ�����������,��������������ĵķ�ʽ����,ÿ��ת�� 90 �ȡ���������ʵ�ֵ����ŵĻ������ϡ�
{
	char a;
	for(a=0;a<8;a++)
 {
	Motor=Door2[a%4];//�Ĵ�ѭ���𲽽������Ŵ�
	Delay(6);//��ʱ300ms
	
 }
}
/*�����Źرպ���ʵ��*/
void Door_close()
{
	char a;
	if(Doorstatus==1)
	{
		for(a=0;a<8;a++)
	{
		Motor=Door1[a%4];
		Delay(6);//��ʱ300ms
	}
	}
	
}


/*�����ڲ�����ʵ��*/
void Indoor_requst(char a)//aΪ����Ĵ���,��Ҫ��������ʱ�䣬���ʱ���������������ŵ�״̬��
{                       	//��ʵ���о��ǵ����ſ���ʱ����
	char b;
	for(b=0;b<a;b++)
	{
		//��ⳬ�ر���
		Over_weight();
		//����������ͱ���
		if(Warning==0)
		{
			Switch=1;
			while(1)
			{
				beep=~beep;//������
				Delay(1);
			}
		}
		if(Opendoor==0&&Doorstatus==1)//�п����������Ŵ��ڿ�״̬
		{
			Signal=0x4f;//�޸�Signal
		}
		if(Closedoor==0&&Doorstatus==0)//�й������������Ѿ����ڹر�״̬
		{Signal=0x5f;}
		if(Opendoor==0&&Doorstatus==0)//�п������������Ŵ��ڹر�״̬
		{
			Door_open();//�Ƚ��Ŵ�
			Signal=0x4f;//Ȼ�����޸��ź�
			Doorstatus=1;//Ȼ���ŵ�״̬��Ϊ�ߵ�ƽ,�Ŵ��ڴ�״̬
		}
		if(Closedoor==0&&Doorstatus==1)
		{
			Door_close();
			Signal=0x5f;
			Doorstatus=0;
		}
		Delay(2);//�ӳ�100ms
	}
}
/*�����ſ����ӳ���*/
void Door_control()
{
	
	Door_open();//�����ſ���
	Doorstatus=1;
	Indoor_requst(50);//50*100ms=5s,�趨5s����ʱ��������Զ��ر�
	Door_close();//�������Զ��ر�
	Doorstatus=0;
	Indoor_requst(5);//����5*100ms��ʱ��˿������ʱ���ڿ��Խ��п��ŵ���������
}

/*�����źŴ����ӳ���*/
void Zhu_requestsend()//��Ҫ�����Ǹ��ݵ��ݵ�λ�ú����з���������Ӧ���źŷ��ͳ�ȥ
{
	if(Location==1)//������1¥
	{
		Zhusendarray[0]=Lampcontrolarray[0];
		
		Signal=Zhusendarray[0];
	}
	else if(Location==2&&Way==0)//��¥������
	{
		Zhusendarray[0]=Lampcontrolarray[1];//Lampcontrolarray���λ��ʾ�������з���0���У��ε���λ��ʾ¥��
	  Signal=Zhusendarray[0];
	}
	else if(Location==3&&Way==0)//��¥����
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
	else if(Location==3&&Way==4)//��¥����
	{
		Zhusendarray[0]=Lampcontrolarray[6];
	  Signal=Zhusendarray[0];
	}
	else if(Location==2&&Way==4)//��¥������
	{
		Zhusendarray[0]=Lampcontrolarray[7];//Lampcontrolarray���λ��ʾ�������з���0���У��ε���λ��ʾ¥��
	  Signal=Zhusendarray[0];
	}
}
/*�û������źŽ����ӳ���*/
void Zhu_requestaccept1()//�û�������ݺ������Ӧ��û��ʱ
{
	while(1)
	{
		Switch_judge();
		Zhu_requestsend();//���͵�ǰλ�õ������ź�
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
	if((Signal<<4) ^ (Zhuacceptarray[0]<<4))//��򣬲�ͬΪ�棬��ͬ˵�����µ��ź���Ҫ����
	{
		if(Doorstatus==1) Door_close();
		if((Signal<<4)^0x80) Zhuacceptarray[0]=Signal;
		Judge();//�ж���Way��Layer��ֵ
		if(Layer==0)//�����ݴ������¥��ʱֱ�ӿ���
		{
			Door_control();
			Zhu_requestaccept1();
		}
		else//�����ݲ������¥��ʱ
		{
			Hoist();//���ݵ������¥����ٴ�
		  Door_control();//���Ƶ����ſ���
			Judge();//���ݵ����������Layer��Way��ֵ
			Lamp_of_updown();//���µ���������ָʾ��
		}
	}
}
void Zhu_status()
{
	Layer=0;
}

void main()
{
	Seg=Floor_show[Location];//����ǰ¥�����ʾ��Ϣ��ֵ���Ĵ���Seg
	Up=0;
	Down=0;//�Ƚ�������ָʾ�ƹر�
	Switch=1;
	Signal=0x0f;
	Motor=0xdd;
	beep=0;
	while(1)
	{
		Switch_judge();//������������תʱ�����������ѭ��ִ������ĳ���
		Zhu_requestaccept();//���������ź�,Ȼ������ź���ȷ�����Ÿ��µȲ���
		Indoor_requst(5);//�ڲ��������ڴ��������ݺ�ĸ������
		if(Layer!=0)//��ʾ������ĳһ����
		{
			Hoist();//������ת
			Door_control();//�����ſ���
			Judge();////���ݵ����������Layer��Way��ֵ
			Lamp_of_updown();//���µ���������ָʾ��
		}
		Zhu_requestsend();//���͵�ǰλ�õ������ź�
	}
}
