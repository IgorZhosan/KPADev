#include "library02061.h"

#pragma pack(1)
/*
typedef  struct {
DWORD  param;	//��� ���������
DWORD  timer;	// ��������� ������� ��� ����������� ���������
UCHAR  error;	//������ ������ ���������
} INPUTPARAM;*/

#define myDEBUG		1	// ����� �������

HANDLE	hECE0206_0;
HANDLE	hECE0206_1;
DWORD nOutput;
BOOL result;

#pragma pack(1)

UCHAR  bufOutput51;

void SI_pusk(UCHAR Device, UCHAR ChanNumber, UCHAR Mode, UCHAR Parity, UCHAR Freq)
{
    struct {
        UCHAR  ChanNumber;// =1;  ����� ������ (1..4)
        UCHAR  Mode;      // =0;  =0 - ������� �����; =1 - ����� ������������(������� �� ���������� ������ ����������)
        UCHAR  Parity;    // =1;  =0  - ��� �������� ����������; =1 - �������� ����������
        UCHAR  Freq;      // =0;  =0 - ����� �� �������� 36-100���; =1 - ����� �� �������� 11-14,5 ���
    } bufInput;

    bufInput.ChanNumber = ChanNumber;
    bufInput.Mode = Mode;
    bufInput.Parity = Parity;
    bufInput.Freq = Freq;

    if (Device==0) DeviceIoControl (hECE0206_0, ECE02061_XP_SI_PUSK, &bufInput, 4, &bufOutput51, 1, &nOutput, NULL  );
    else if (Device==1) DeviceIoControl (hECE0206_1, ECE02061_XP_SI_PUSK, &bufInput, 4, &bufOutput51, 1, &nOutput, NULL  );

    DWORD	tick = GetTickCount();
    while (( GetTickCount()- tick)<300) {};
}

void SO_pusk(UCHAR Device)
{
    struct {
        UCHAR  Err_en      =0;
        UCHAR  Parity      =1;
        UCHAR  Freq        =1;
        UCHAR  ArrayDim    =7;
        UCHAR  Delay       =0;
        UCHAR  ArrayNumber =1;
    } bufInput_0;

    struct {
        UCHAR  Err_en      =0;
        UCHAR  Parity      =1;
        UCHAR  Freq        =1;
        UCHAR  ArrayDim    =2;
        UCHAR  Delay       =0;
        UCHAR  ArrayNumber =1;
    } bufInput_1;


    if (Device==0)  DeviceIoControl (hECE0206_0, ECE02061_XP_SO_PUSK1, &bufInput_0, 6, NULL, 0, &nOutput, NULL);
    else if (Device==1)  DeviceIoControl (hECE0206_1, ECE02061_XP_SO_PUSK1, &bufInput_1, 6, NULL, 0, &nOutput, NULL);
}

void SI_clear_array(UCHAR Device, UCHAR ChanNumber)
{

    UCHAR  bufOutput;


    if (Device==0) DeviceIoControl (hECE0206_0, ECE02061_XP_CLEAR_SI_ARRAY, &ChanNumber, 1, &bufOutput, 1, &nOutput, NULL  );
    else if (Device==1) DeviceIoControl (hECE0206_1, ECE02061_XP_CLEAR_SI_ARRAY, &ChanNumber, 1, &bufOutput, 1, &nOutput, NULL  );


    DWORD     tick = GetTickCount();
    while (( GetTickCount()- tick)<1000) {};//
}

void SI_stop(UCHAR Device, UCHAR ChanNumber)
{

    UCHAR  bufOutput;

    if (Device==0) DeviceIoControl (hECE0206_0, ECE02061_XP_SI_STOP, &ChanNumber, 1, &bufOutput, 1, &nOutput , NULL  );
    else if (Device==1)  DeviceIoControl (hECE0206_1, ECE02061_XP_SI_STOP, &ChanNumber, 1, &bufOutput, 1, &nOutput , NULL  );

    DWORD	tick = GetTickCount();
    while (( GetTickCount()- tick)<300) {};

}

void SO_stop(UCHAR Device)
{
    if (Device==0) DeviceIoControl (hECE0206_0, ECE02061_XP_SO_STOP1, NULL, 0, NULL, 0, &nOutput ,NULL  );
    else if (Device==1) DeviceIoControl (hECE0206_1, ECE02061_XP_SO_STOP1, NULL, 0, NULL, 0, &nOutput ,NULL);

    DWORD	tick = GetTickCount();
    while (( GetTickCount()- tick)<300) {};
}


void BUF256x32_write(UCHAR Device, ULONG * paramArray, int Size)
{
    DWORD tick;

    struct {
        UCHAR   paramAddress =0;
        UCHAR   Comm= 0x80;
        ULONG   param[7];
    } outputParam;

    for (int i=0; i<Size; i++)
        outputParam.param[i]=*(paramArray+i);

    DWORD nbufInputSize = sizeof (outputParam);

    if (Device==0)  DeviceIoControl (hECE0206_0, ECE02061_XP_WRITE_ARRAY_1, &outputParam, nbufInputSize, NULL, 0 ,&nOutput, NULL);
    else if (Device==1) DeviceIoControl (hECE0206_1, ECE02061_XP_WRITE_ARRAY_1, &outputParam, nbufInputSize, NULL, 0 ,&nOutput, NULL);


    tick = GetTickCount();
    while (( GetTickCount()- tick)<11) {};
}

