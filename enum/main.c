#include <stdio.h>

typedef enum DAY { MON=0x800200, TUE, WED=0x400, THU, FRI, SAT, SUN }DAY;

void main()
{
     DAY yesterday, today, tomorrow;
    printf("%d %d %d %x \n", MON, TUE, WED,THU); 
    yesterday = TUE;
    today = (enum DAY) (yesterday + 1); //类型转换
    tomorrow = (enum DAY) 30; //类型转换
    //tomorrow = 3; //错误

    printf("%d %d %d \n", yesterday, today, tomorrow); //输出：2 3 30
}