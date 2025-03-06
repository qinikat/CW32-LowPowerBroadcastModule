#ifndef __RTC_H
#define __RTC_H

#include "main.h"

extern uint8_t alarm_hour;
extern uint8_t alarm_minute;
extern uint8_t alarm_second;
extern uint8_t sleep_flag;

void ShowTime(void);
void RTC_IRQHandlerCallBack(void);
void RTC_datetime_set(const uint8_t *data);
void RTC_AlarmA_Set(uint8_t hour, uint8_t minute, uint8_t second);
void RTC_Sleep_Set(const uint8_t *data1, const uint8_t *data2);
uint8_t BCD_Add(uint8_t bcd);
uint8_t BCD_Sub(uint8_t bcd);
uint8_t RTC_Time_Check(const uint8_t *time, uint8_t length);
uint8_t RTC_Time_Compare(const uint8_t *time1, const uint8_t *time2, uint8_t condition);

#endif
