#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <conio.h>
#define N 5 //车库总量
#define HEAD1 "----------欢迎使用停车场管理系统----------\n"
#define HEAD2 "|--------请输入下列选项代码并回车--------|\n"
#define MENU1 "\n|1.新车入库登记                          |\n\n"
#define MENU2 "|2.出库并计费                            |\n\n"
#define MENU3 "|3.查询车辆在库状态                      |\n\n"
#define MENU4 "|4.查询停车位余量                        |\n\n"
#define MENU5 "|5.查看日志                              |\n\n"
#define MENU6 "|6.退出停车场管理系统                    |\n\n"
#define HEAD3 "------------------------------------------\n"


//函数的声明
void menu();
int FileStatus(FILE *fp);

typedef struct car      
{
	int timeHour;//停时HH（小时）
	int id;//车牌号码 
	int inTime[2];//车辆进入时间
	int totalTime;//总停车时间,以小时计
	int sum;//总金额
	int position;//位置剩余量
}car;


main(void)
{
	int position=5;//总车位
	int operate;
	char File[]={"停车场管理系统.bin"};	
	do
	{ 
		menu();
		while(1) //输入为1-6，执行对应的操作 
		{
			printf("请输入正确的选项后回车:");
			scanf("%d",&operate);
			if(operate>-1&&operate<7)
			{
				break;
			}
			else
			{
				printf("输入错误！\n");
			}
		}
		//输入正确执行对应操作 
		switch(operate)
		{
			case 1:
				{
				position--;
				ruku(File, position);
				break;
				}
			case 2:
				{
				position++;
				chuku(File);
				break;
				}
			case 3:
				{
				SearchNumber(File);
				break;
				}
			case 4:
				{
				positionElse(position);
				break;
				}
			case 5:
				{
				logout(File);		
				break;
				}
			case 6:
				{
				system("cls");
				exit(0);
				}
			default:
				{
				printf("选择错误，请重新选择\n");
				}
		}
		printf("\n");
		printf("按任意键返回主菜单!");
			{
				getch();
				system("cls");//清屏	
		}
	}
	while(operate != 0);
	return 1;
}


//以下是函数库

int ruku(char *File, int position)//新车入库登记
{
	int m;//位数侦测
	car temp,temp0;
	FILE *fp=fopen(File,"a+");//文件打开
	time_t t;
    struct tm * lt;
    time (&t);//获取时间
    lt = localtime (&t);//转为时间结构

	if(position<0)
		{
			printf("无可用车位!");
			getch();
			return 0;
		}
	if(fp==NULL)
	{
		printf("无法打开文件！请检查存储权限！\n",File);
		return;
	}
		Repeat:printf("请输入车牌号码(5位数字):\n");	
	    m=scanf("%05d",&temp.id);
	    if(m<=0||m>99999)
		{
			printf("您输入的车牌号码有误，请检查格式!\n");
			fflush(stdin);//清空chche 
		 goto Repeat;
		}
		fseek(fp,0,SEEK_SET);//移动指针到文件开头
		while(fread(&temp0,sizeof(car),1,fp))
		{
			if(temp0.id==temp.id)
			{
				printf("该车辆已经存在，请勿重复录入!\n");
				goto Repeat;
			}
		}
		
		temp.inTime[0]=lt->tm_hour;
		temp.inTime[1]=lt->tm_min;
		temp.inTime[2]=lt->tm_sec;

		if(!fwrite(&temp,sizeof(car),1,fp))//进行写入数据操作
		{
			printf("入库失败！\n向文件%s写入信息失败！\n",File);
			return;
		}
			printf( "车牌号为%d的车辆于%04d-%02d-%02d %02d:%02d:%02d入库成功！\n向文件%s写入信息成功！\n",temp.id,lt->tm_year+1900, ((lt->tm_mon)+1), lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec,File);
			fclose(fp);
			return 0;
}


int chuku(char *File)//出库并计费
{
	int a=0,len,del;
	car s[N];
	FILE *fp=fopen(File,"r");
	time_t t;
    struct tm * lt;
    time (&t);//获取时间
    lt = localtime (&t);//转为时间结构

	if(fp==NULL)
	{
		printf("未创建文件，请先录入车辆信息！");
		return;
	}
	if(FileStatus(fp))
	{
		printf("文件为空，请先录入信息!");
		return;
	}
	printf("请输入出库车辆车牌号码(5位):");
	scanf("%d",&del);
	fseek(fp,0,SEEK_SET);
	while(fread(s+a,sizeof(car),1,fp))
	{
		if(s[a].id==del)
		{
			s[a].totalTime=lt->tm_sec-s[a].inTime[2];
			s[a].sum=5+abs(lt->tm_hour-s[a].inTime[0])*60+abs(lt->tm_min-s[a].inTime[1])*1;//+abs(lt->tm_sec-s[a].inTime[2]);
			//总金额计算公式，入场费5元，每分钟1元，不足1分钟按1d分钟计算
			printf("下面的车牌号为%d的车辆信息:\n",del);
			printf("车牌号：%d\n",s[a].id);
			printf("入库时间：%02d:%02d:%02d\n",abs(s[a].inTime[0]),abs(s[a].inTime[1]),abs(s[a].inTime[2]));
			printf("出库时间：%04d-%02d-%02d %02d:%02d:%02d\n",lt->tm_year+1900, ((lt->tm_mon)+1), lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
			printf("已停时长(精确到分钟)：%d 分钟\n",abs(lt->tm_hour-s[a].inTime[0])*60+abs(lt->tm_min-s[a].inTime[1])*1);//+abs(lt->tm_sec-s[a].inTime[2]));
			printf("应缴金额：%d 元。",s[a].sum);
			printf("缴费成功请按任意键!");
			a-=1;
		}
		a++;
	}
	fclose(fp);
	len=a;
	fp=fopen(File,"w");
	fseek(fp,0,SEEK_SET);
	for(a=0;a<len;a++)			//删除后重写文件
		fwrite(s+a,sizeof(car),1,fp);
	fclose(fp);
	return 0;
}


int SearchNumber(char *File)//按车牌号查找
{
	int Number;
	car temp;//定义结构体变量 temp 
	FILE *fp = fopen(File,"r");
	if(fp == NULL)
	{
		printf("无法打开文件");
		return;
	}
	if( FileStatus(fp) )
	{
		printf("停车场空！");
		return;
	}
	printf("请输入车号:");
	scanf("%d",&Number);
	fseek(fp,0,0);//从文件开始进行查找
	while(fread(&temp,sizeof(car),1,fp))//从fp所指向的文件读取一个数据块(sizeof(car)) ，并放到*temp所指向的内存地址中 
	{
		if( temp.id == Number )
		{
			printf("车号为%d的车辆在此停车场中！\n",Number);
			return;//结束程序
		}	
	}
	printf("没有找到车号为%d的车辆信息!",Number);
	fclose(fp);
	return;
}


int positionElse(position)
{
	printf("剩余车位：%d 个。",position);
	return 0;
}


int logout(char *File)//输出文件 
{
	int a,b,c;
	car S;
	FILE *fp=fopen(File,"r");//进行读入
	time_t t;
    struct tm * lt;
    time (&t);//获取时间
    lt = localtime (&t);//转为时间结构
	a=S.inTime[0];
	b=S.inTime[1];
	c=S.inTime[2];
	S.totalTime=((lt->tm_sec)-c);
	S.sum=5+S.totalTime;
	if(fp==NULL)
	{
		printf("无法打开\n",File);
		return;
	}

	fseek(fp,0,SEEK_SET);
		system("cls");
		printf(" 车号     进场时间     已停时间      总价\n");
	while(fread(&S,sizeof(car),1,fp))//从文件读入一个结构体大小的空间给S 
		printf(" %d    %02d:%02d:%02d      %02d:%02d        %d 元\n",S.id,S.inTime[0],S.inTime[1],S.inTime[2], abs(lt->tm_hour-S.inTime[0]),abs(lt->tm_min-S.inTime[1]),5+abs(lt->tm_hour-S.inTime[0])*60+abs(lt->tm_min-S.inTime[1])*1);
		printf("\n");
		fclose(fp);
}


void menu()  //输出主程序选择菜单
{
	printf(HEAD1);
	printf(HEAD2);
	printf(MENU1);
	printf(MENU2);
	printf(MENU3);
	printf(MENU4);
	printf(MENU5);
	printf(MENU6);
	printf(HEAD3);
}


int FileStatus(FILE *fp)//判断文件是否为空文件
{
	int len; 
	int a;
	fseek(fp,0,SEEK_END);		//将指针定位到文件尾部
	len = ftell(fp);	
	if (len==0)		
	a = 1;
	else
	a = 0;		
	return a;
}