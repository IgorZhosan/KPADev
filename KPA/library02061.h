#ifndef __LIBRARY02061_H__
#define __LIBRARY02061_H__

#include <windows.h>
#include "ioctlECE0206.h"
#include <winioctl.h>
#include <stdio.h>
#include <conio.h>

// Функции для управления каналами и массивами данных
void SI_pusk(UCHAR Device, UCHAR ChanNumber, UCHAR Mode, UCHAR Parity, UCHAR Freq);
void SO_pusk(UCHAR Device);
void SI_clear_array(UCHAR Device, UCHAR ChanNumber);
void SI_stop(UCHAR Device, UCHAR ChanNumber);
void SO_stop(UCHAR Device);
void BUF256x32_write(UCHAR Device, ULONG* paramArray, int Size);

// Внешние переменные, если они используются в других частях программы
extern HANDLE hECE0206_0;
extern HANDLE hECE0206_1;
extern DWORD nOutput;
extern BOOL result;

#endif // __LIBRARY02061_H__
