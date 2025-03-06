#include "rtc.h"
#include "main.h"
#include <stdio.h>

uint8_t RTC_hour = 0x00;                       // 用于存储当前RTC时间的小时数,BCD码
uint8_t RTC_minute = 0x00;                     // 用于存储当前RTC时间的分钟数,BCD码
uint8_t Sleep_Status = 0;                      // 用于存储当前睡眠状态，0为正常工作，1为等待休眠，2为休眠中
uint16_t Difference_Waitsleep[3] = {0, 0, 0};  // 等待开始休眠计数，用于存储当前时间与开始休眠时间的差值，0为天数，1为小时数，当天数为0时,小时数为0时,分钟数为0时，休眠开始
uint16_t Difference_Waitawaken[3] = {0, 0, 0}; // 休眠计数，用于存储休眠开始时间与休眠结束时间的差值，0为天数，1为小时数，当天数为0时，小时数为0时，分钟数为0时，休眠结束拉高唤醒引脚
uint8_t End_Sleep[5] = {0, 0, 0, 0, 0};        // 用于存储休眠结束时间，年月日时，BCD码

extern uint8_t Message_Status;

uint16_t Bcd_To_Num(uint16_t bcd);                                                                                 // bcd转换为十进制数
uint16_t Num_To_Bcd(uint16_t num);                                                                                 // 十进制数转换为bcd
int daysInMonth(int year, int month);                                                                              // 获取指定年月的天数
int isLeapYear(int year);                                                                                          // 判断是否为润年
void RTC_Difference(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint16_t *difference); // 计算目标时间与当前RTC时间的天数与小时的差值存入返回数组中

void RTC_datetime_set(const uint8_t *data)
{
    RTC_InitTypeDef RTC_InitStruct = {0};

    RTC_InitStruct.DateStruct.Day = data[2]; // 设置日期，DAY、MONTH、YEAR必须为BCD方式，星期为0~6，代表星期日，星期一至星期六
    RTC_InitStruct.DateStruct.Month = data[1];
    RTC_InitStruct.DateStruct.Week = 0x01;
    RTC_InitStruct.DateStruct.Year = data[0];

    printf("-------Set Date as 20%x/%x/%x\r\n", RTC_InitStruct.DateStruct.Year, RTC_InitStruct.DateStruct.Month, RTC_InitStruct.DateStruct.Day);

    RTC_InitStruct.TimeStruct.Hour = data[3]; // 设置时间，HOUR、MINIUTE、SECOND必须为BCD方式，用户须保证HOUR、AMPM、H24之间的关联正确性
    RTC_InitStruct.TimeStruct.Minute = data[4];
    RTC_InitStruct.TimeStruct.Second = data[5];
    // RTC_InitStruct.TimeStruct.AMPM = 0;
    RTC_InitStruct.TimeStruct.H24 = 1;
    printf("-------Set Time as %02x:%02x:%02x\r\n", RTC_InitStruct.TimeStruct.Hour, RTC_InitStruct.TimeStruct.Minute, RTC_InitStruct.TimeStruct.Second);
    RTC_InitStruct.RTC_ClockSource = RTC_RTCCLK_FROM_LSE;
    RTC_Init(&RTC_InitStruct); //  用户需选定需要使用的时钟源，且必须使能对应的时钟源
}

// RTC等待开始休眠闹钟与结束时间设置 ，传入BCD码
void RTC_Sleep_Set(const uint8_t *data1, const uint8_t *data2)
{

    // 检测传入数据是否合法

    // 设置休眠结束时间
    End_Sleep[0] = data2[0];
    End_Sleep[1] = data2[1];
    End_Sleep[2] = data2[2];
    End_Sleep[3] = data2[3];
    End_Sleep[4] = data2[4];

    printf("=====Difference_Waitsleep[0]:%d\r\n", Difference_Waitsleep[0]);
    printf("=====Difference_Waitsleep[1]:%d\r\n", Difference_Waitsleep[1]);
    printf("=====Difference_Waitsleep[2]:%d\r\n", Difference_Waitsleep[2]);

    // 打印传入的时间
    printf("year,month,day,hour:%x,%x,%x,%x,%x\r\n", data1[0], data1[1], data1[2], data1[3], data1[4]);

    // 计算当前时间与开始休眠时间的差值,返回值存入Difference数组中
    RTC_Difference(data1[0], data1[1], data1[2], data1[3], data1[4], Difference_Waitsleep);
    printf("=====Difference_Waitsleep[0]:%d\r\n", Difference_Waitsleep[0]);
    printf("=====Difference_Waitsleep[1]:%d\r\n", Difference_Waitsleep[1]);
    printf("=====Difference_Waitsleep[2]:%d\r\n", Difference_Waitsleep[2]);

    // 判断天数是否为0，如果为0只设置小时与分钟
    if (Difference_Waitsleep[0] == 0)
    {
        uint8_t alarm_hour = 0;
        uint8_t alarm_minute = 0;
        printf("=====RTC_hour:%x\r\n", RTC_hour);
        printf("=====RTC_minute:%x\r\n", RTC_minute);

        // 根据时间与分钟 计算出下次的闹钟时间
        // 先计算分钟
        if ((Difference_Waitsleep[2] + Bcd_To_Num(RTC_minute)) >= 60)
        {
            alarm_minute = (Difference_Waitsleep[2] + Bcd_To_Num(RTC_minute)) - 60;
            //  小时加1
            Difference_Waitsleep[1] = Difference_Waitsleep[1] + 1;
        }
        else
        {
            alarm_minute = Difference_Waitsleep[2] + Bcd_To_Num(RTC_minute);
        }
        //  再计算小时
        if ((Difference_Waitsleep[1] + Bcd_To_Num(RTC_hour)) >= 24)
        {
            alarm_hour = (Difference_Waitsleep[1] + Bcd_To_Num(RTC_hour)) - 24;
        }
        else
        {
            alarm_hour = Difference_Waitsleep[1] + Bcd_To_Num(RTC_hour);
        }

        printf("=====alarm_hour:%x\r\n", Num_To_Bcd(alarm_hour));
        printf("=====alarm_minute:%x\r\n", Num_To_Bcd(alarm_minute));
        //  设置下次闹钟时间
        RTC_AlarmA_Set(Num_To_Bcd(alarm_hour), Num_To_Bcd(alarm_minute), 0x59);

        Difference_Waitsleep[1] = 0;
        Difference_Waitsleep[2] = 0;
    }
    else if (Difference_Waitsleep[0] != 0)
    {
        //  设置下次闹钟时间,为明天的现在时间，共24小时
        RTC_AlarmA_Set(RTC_hour, RTC_minute, 0x59);
    }
}

// RTC闹钟设置 ，传入BCD码
void RTC_AlarmA_Set(uint8_t hour, uint8_t minute, uint8_t second)
{
    //
    RTC_AlarmTypeDef RTC_AlarmStruct = {0};

    // 检测传入数据是否合法
    if (hour > 0x23 || minute > 0x59)
    {
        printf("=====RTC_AlarmA_Set error\r\n");
        return;
    }

    // 闹钟屏蔽设置，屏蔽掉星期
    RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_WeekMON | RTC_AlarmMask_WeekTUE | RTC_AlarmMask_WeekWED | RTC_AlarmMask_WeekTHU | RTC_AlarmMask_WeekFRI | RTC_AlarmMask_WeekSAT | RTC_AlarmMask_WeekSun;

    // 闹钟时间设置
    RTC_AlarmStruct.RTC_AlarmTime.Hour = hour;
    RTC_AlarmStruct.RTC_AlarmTime.Minute = minute;
    RTC_AlarmStruct.RTC_AlarmTime.Second = second;

    RTC_SetAlarm(RTC_Alarm_A, &RTC_AlarmStruct);
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
    RTC_ITConfig(RTC_IT_ALARMA, ENABLE);
    printf("=====Set alarm hour:%x\r\n", hour);
    printf("=====Set alarm minute:%x\r\n", minute);
}

void ShowTime(void)
{
    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_DateTypeDef RTC_DateStruct = {0};
    // static uint8_t *WeekdayStr[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    static uint8_t *H12AMPMStr[2][2] = {{"AM", "PM"}, {"", ""}};

    RTC_GetDate(&RTC_DateStruct);
    RTC_GetTime(&RTC_TimeStruct);

    printf(".Date is 20%02x/%02x/%02x.Time is %02x%s:%02x:%02x\r\n", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Day, RTC_TimeStruct.Hour, H12AMPMStr[RTC_TimeStruct.H24][RTC_TimeStruct.AMPM], RTC_TimeStruct.Minute, RTC_TimeStruct.Second);
}

void RTC_IRQHandlerCallBack(void)
{
    if (RTC_GetITState(RTC_IT_ALARMA))
    {
        RTC_ClearITPendingBit(RTC_IT_ALARMA);
        printf("*********Alarm!!!!\r\n");

        // 判断当前状态是否为等待休眠状态
        if (Sleep_Status == 1)
        {
            // 判断天数是否为大于0，如果大于0，设置下次闹钟时间为明天的现在时间，共24小时
            if (Difference_Waitsleep[0] > 0)
            {
                RTC_AlarmA_Set(RTC_hour, RTC_minute, 0x59);
                Difference_Waitsleep[0] = Difference_Waitsleep[0] - 1;
            }
            else if (Difference_Waitsleep[0] == 0 && (Difference_Waitsleep[1] != 0 || Difference_Waitsleep[2] != 0))
            {
                uint8_t alarm_hour = 0;
                uint8_t alarm_minute = 0;

                // 计算下次的闹钟时间
                //  先计算分钟
                if ((Difference_Waitsleep[2] + Bcd_To_Num(RTC_minute)) >= 60)
                {
                    alarm_minute = (Difference_Waitsleep[2] + Bcd_To_Num(RTC_minute)) - 60;
                    //  小时加1
                    Difference_Waitsleep[1] = Difference_Waitsleep[1] + 1;
                }
                else
                {
                    alarm_minute = Difference_Waitsleep[2] + Bcd_To_Num(RTC_minute);
                }
                //  再计算小时
                if ((Difference_Waitsleep[1] + Bcd_To_Num(RTC_hour)) >= 24)
                {
                    alarm_hour = (Difference_Waitsleep[1] + Bcd_To_Num(RTC_hour)) - 24;
                }
                else
                {
                    alarm_hour = Difference_Waitsleep[1] + Bcd_To_Num(RTC_hour);
                }

                //  设置下次闹钟时间
                RTC_AlarmA_Set(Num_To_Bcd(alarm_hour), Num_To_Bcd(alarm_minute), 0x59);
                Difference_Waitsleep[1] = 0;
                Difference_Waitsleep[2] = 0;
            }
            else if (Difference_Waitsleep[0] == 0 && Difference_Waitsleep[1] == 0 && Difference_Waitsleep[2] == 0)
            {
                printf("=====No need to wait sleep,enter sleep\r\n");

                // 获取当前时间与休眠结束时间的差值
                RTC_Difference(End_Sleep[0], End_Sleep[1], End_Sleep[2], End_Sleep[3], End_Sleep[4], Difference_Waitawaken);

                // 更新Sleep_Status状态
                // 更新Sleep_Status状态为2 紧接着if判断，并进入休眠状态
                Sleep_Status = 2;
            }
        }

        // 判断当前状态是否为休眠状态
        if (Sleep_Status == 2)
        {
            // 判断天数是否为大于0，如果大于0，设置下次闹钟时间为明天的现在时间，共24小时
            if (Difference_Waitawaken[0] > 0)
            {
                RTC_AlarmA_Set(RTC_hour, RTC_minute, 0x59);
                Difference_Waitawaken[0] = Difference_Waitawaken[0] - 1;
            }
            else if (Difference_Waitawaken[0] == 0 && (Difference_Waitawaken[1] != 0 || Difference_Waitawaken[2] != 0))
            {
                uint8_t alarm_hour = 0;
                uint8_t alarm_minute = 0;

                // 计算下次的闹钟时间
                //  先计算分钟
                if ((Difference_Waitawaken[2] + Bcd_To_Num(RTC_minute)) >= 60)
                {
                    alarm_minute = (Difference_Waitawaken[2] + Bcd_To_Num(RTC_minute)) - 60;
                    //  小时加1
                    Difference_Waitawaken[1] = Difference_Waitawaken[1] + 1;
                }
                else
                {
                    alarm_minute = Difference_Waitawaken[2] + Bcd_To_Num(RTC_minute);
                }
                //  再计算小时
                if ((Difference_Waitawaken[1] + Bcd_To_Num(RTC_hour)) >= 24)
                {
                    alarm_hour = (Difference_Waitawaken[1] + Bcd_To_Num(RTC_hour)) - 24;
                }
                else
                {
                    alarm_hour = Difference_Waitawaken[1] + Bcd_To_Num(RTC_hour);
                }

                //  设置下次闹钟时间
                RTC_AlarmA_Set(Num_To_Bcd(alarm_hour), Num_To_Bcd(alarm_minute), 0x00);
                Difference_Waitawaken[1] = 0;
                Difference_Waitawaken[2] = 0;
            }

            else if (Difference_Waitawaken[0] == 0 && Difference_Waitawaken[1] == 0 && Difference_Waitawaken[2] == 0)
            {

                // 休眠结束，拉高唤醒引脚
                printf("=====Sleep end,awaken\r\n");
                PA00_SETHIGH();
                //  更新Sleep_Status状态
                Sleep_Status = 0;
                Message_Status = 0;

                // 清零相关变量
                for (int i = 0; i < 3; i++)
                {
                    Difference_Waitsleep[i] = 0;
                    Difference_Waitawaken[i] = 0;
                }
                for (int i = 0; i < 5; i++)
                {
                    End_Sleep[i] = 0;
                }
                RTC_hour = 0x00;
                RTC_minute = 0x00;
            }
        }
    }

    if (RTC_GetITState(RTC_IT_INTERVAL))
    {
        RTC_ClearITPendingBit(RTC_IT_INTERVAL);
        ShowTime();
    }
}

// BCD码加1
uint8_t BCD_Add(uint8_t bcd)
{
    uint8_t temp = 0;
    temp = (bcd & 0x0f) + 1;
    if (temp >= 10)
    {
        temp = temp - 10;
        bcd = (bcd & 0xf0) + temp;
        temp = (bcd >> 4) + 1;
        if (temp >= 10)
        {
            temp = temp - 10;
            bcd = (temp << 4) + 0x10;
        }
        else
        {
            bcd = (temp << 4) + 0x00;
        }
    }
    else
    {
        bcd = (bcd & 0xf0) + temp;
    }
    return bcd;
}

// BCD码减1
uint8_t BCD_Sub(uint8_t bcd)
{
    uint8_t temp = 0;
    temp = (bcd & 0x0f) - 1;
    if (temp >= 0x0f)
    {
        temp = temp + 10;
        bcd = (bcd & 0xf0) + temp;
        temp = (bcd >> 4) - 1;
        if (temp >= 0x0f)
        {
            temp = temp + 10;
            bcd = (temp << 4) + 0x90;
        }
        else
        {
            bcd = (temp << 4) + 0x80;
        }
    }
    else
    {
        bcd = (bcd & 0xf0) + temp;
    }
    return bcd;
}

uint16_t Bcd_To_Num(uint16_t bcd)
{
    uint16_t temp = 0;
    temp = (bcd & 0x000f) + ((bcd & 0x00f0) >> 4) * 10 + ((bcd & 0x0f00) >> 8) * 100 + ((bcd & 0xf000) >> 12) * 1000;
    return temp;
}

uint16_t Num_To_Bcd(uint16_t num)
{
    uint16_t temp = 0;
    temp = (num / 1000) << 12 | ((num % 1000) / 100) << 8 | (((num % 1000) % 100) / 10) << 4 | (((num % 1000) % 100) % 10);
    return temp;
}
// 获取指定年月的天数
int daysInMonth(int year, int month)
{
    int days;

    switch (month)
    {
    case 4:
    case 6:
    case 9:
    case 11:
        days = 30;
        break;
    case 2:
        days = isLeapYear(year) ? 29 : 28;
        break;
    default:
        days = 31;
    }

    return days;
}

// 判断是否为润年
int isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 计算目标时间与当前RTC时间的天数与小时与分钟的差值并存入返回数组中
// 传入目标时间，传入返回数组地址。无返回
void RTC_Difference(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint16_t *difference)
{
    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_DateTypeDef RTC_DateStruct = {0};

    // 计算目标时间与当前RTC时间的天数和小时差值
    uint16_t daysDifference = 0;
    uint16_t hoursDifference = 0;
    uint16_t minutesDifference = 0;

    // 获取当前RTC时间，此为BCD码
    RTC_GetDate(&RTC_DateStruct);
    RTC_GetTime(&RTC_TimeStruct);

    // 打印当前时间
    printf("Now time Year:%x,Month:%x,Day:%x,hour:%x,minute:%x\r\n", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Day, RTC_TimeStruct.Hour, RTC_TimeStruct.Minute);
    // RTC_DateStruct.Year = 0x23;
    // RTC_DateStruct.Month = 0x06;
    // RTC_DateStruct.Day = 0x21;
    // RTC_TimeStruct.Hour = 0x06;
    // RTC_TimeStruct.Minute = 0x20;

    // 更新暂存值RTC_hour,RTC_minute
    RTC_hour = RTC_TimeStruct.Hour;
    RTC_minute = RTC_TimeStruct.Minute;

    // BCD码转换为十进制数
    RTC_DateStruct.Year = Bcd_To_Num(RTC_DateStruct.Year);
    RTC_DateStruct.Month = Bcd_To_Num(RTC_DateStruct.Month);
    RTC_DateStruct.Day = Bcd_To_Num(RTC_DateStruct.Day);
    RTC_TimeStruct.Hour = Bcd_To_Num(RTC_TimeStruct.Hour);
    RTC_TimeStruct.Minute = Bcd_To_Num(RTC_TimeStruct.Minute);

    year = Bcd_To_Num(year);
    month = Bcd_To_Num(month);
    day = Bcd_To_Num(day);
    hour = Bcd_To_Num(hour);
    minute = Bcd_To_Num(minute);

    // 判断目标年月日时分是否大于当前年月日时分，如果小于等于，直接返回
    if (year < RTC_DateStruct.Year ||
        (year == RTC_DateStruct.Year && month < RTC_DateStruct.Month) ||
        (year == RTC_DateStruct.Year && month == RTC_DateStruct.Month && day < RTC_DateStruct.Day) ||
        (year == RTC_DateStruct.Year && month == RTC_DateStruct.Month && day == RTC_DateStruct.Day && hour < RTC_TimeStruct.Hour) ||
        (year == RTC_DateStruct.Year && month == RTC_DateStruct.Month && day == RTC_DateStruct.Day && hour == RTC_TimeStruct.Hour && minute <= RTC_TimeStruct.Minute))
    {
        difference[0] = 0;
        difference[1] = 0;
        difference[2] = 0;
        return;
    }

    // 如果目标时间年月日等于当前时间年月日，直接计算小时与分钟差值,并更新difference数组后返回
    if (year == RTC_DateStruct.Year && month == RTC_DateStruct.Month && day == RTC_DateStruct.Day)
    {
        // 计算小时差值
        if (hour >= RTC_TimeStruct.Hour)
        {
            hoursDifference = hour - RTC_TimeStruct.Hour;
        }
        else
        {
            hoursDifference = 24 - RTC_TimeStruct.Hour + hour;
        }

        // 计算分钟差值
        if (minute >= RTC_TimeStruct.Minute)
        {
            minutesDifference = minute - RTC_TimeStruct.Minute;
        }
        else
        {
            minutesDifference = 60 - RTC_TimeStruct.Minute + minute;
            hoursDifference--;
        }

        // 存入返回数组
        difference[0] = 0;
        difference[1] = hoursDifference;
        difference[2] = minutesDifference;
        return;
    }

    // 计算总天数差值
    {
        // 计算年份差值
        uint16_t yearDifference = year - RTC_DateStruct.Year;

        // 计算月份差值
        uint16_t monthDifference = 0;
        if (month >= RTC_DateStruct.Month)
        {
            monthDifference = month - RTC_DateStruct.Month;
        }
        else
        {
            monthDifference = 12 - RTC_DateStruct.Month + month;
            yearDifference--;
        }

        // 计算天数差值
        uint16_t dayDifference = 0;
        if (day >= RTC_DateStruct.Day)
        {
            dayDifference = day - RTC_DateStruct.Day;
        }
        else
        {
            dayDifference = daysInMonth(RTC_DateStruct.Year, RTC_DateStruct.Month) - RTC_DateStruct.Day + day;
            monthDifference--;
        }

        // 计算总天数差值
        daysDifference = yearDifference * 365 + monthDifference * 30 + dayDifference;
    }

    // 计算小时差值
    if (hour >= RTC_TimeStruct.Hour)
    {
        hoursDifference = hour - RTC_TimeStruct.Hour;
    }
    else
    {
        hoursDifference = 24 - RTC_TimeStruct.Hour + hour;
        daysDifference--;
    }

    // 计算分钟差值
    if (minute >= RTC_TimeStruct.Minute)
    {
        minutesDifference = minute - RTC_TimeStruct.Minute;
    }
    else
    {
        minutesDifference = 60 - RTC_TimeStruct.Minute + minute;
        if (hoursDifference == 0)
        {
            daysDifference--;
            hoursDifference = 24;
        }
        hoursDifference--;
    }

    // 存入返回数组
    difference[0] = daysDifference;
    difference[1] = hoursDifference;
    difference[2] = minutesDifference;
}

// 时间数据合法性判断，检测是否为BCD码，且是否为合法时间
// 传入时间数据，返回1为合法，返回0为不合法
uint8_t RTC_Time_Check(const uint8_t *time, uint8_t length)
{
    // 判断是否为BCD码
    for (uint8_t i = 0; i < length; i++)
    {
        if ((time[i] & 0x0f) > 0x09 || ((time[i] >> 4) & 0x0f) > 0x09)
        {
            printf("=====Time is not BCD\r\n");
            return 0;
        }
    }
    // 判断是否为合法时间,依照年月日时分秒的顺序，并根据length判断是否需要判断秒
    if (length == 5)
    {
        if (time[0] > 0x99 || time[1] > 0x12 || time[2] > 0x31 || time[3] > 0x23 || time[4] > 0x59)
        {
            printf("=====Time is not legal\r\n");
            return 0;
        }
        return 1;
    }
    else if (length == 6)
    {
        if (time[0] > 0x99 || time[1] > 0x12 || time[2] > 0x31 || time[3] > 0x23 || time[4] > 0x59 || time[5] > 0x59)
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

// 传入两组时间数据，BCD码，年月日时分，判断第一组是否小于第二组，返回1为小于，返回0为大于，根据condition是否可以相等判断是否可以相等0为不可以，1为可以
uint8_t RTC_Time_Compare(const uint8_t *time1, const uint8_t *time2, uint8_t condition)
{
    // 判断年份
    if (time1[0] < time2[0])
    {
        printf("test1\r\n");
        return 1;
    }
    else if (time1[0] > time2[0])
    {
        return 0;
    }
    // 判断月份
    if (time1[1] < time2[1])
    {
        printf("time1[1],time2[1]:%x,%x\r\n", time1[1], time2[1]);
        printf("test2\r\n");
        return 1;
    }
    else if (time1[1] > time2[1])
    {
        return 0;
    }
    // 判断日期
    if (time1[2] < time2[2])
    {
        printf("test3\r\n");
        return 1;
    }
    else if (time1[2] > time2[2])
    {
        return 0;
    }
    // 判断小时
    if (time1[3] < time2[3])
    {
        printf("test4\r\n");
        return 1;
    }
    else if (time1[3] > time2[3])
    {
        return 0;
    }
    // 判断分钟
    if (time1[4] < time2[4])
    {
        printf("test5\r\n");
        return 1;
    }
    else if (time1[4] > time2[4])
    {
        return 0;
    }
    // 如果年月日时分都相等
    if (condition == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
