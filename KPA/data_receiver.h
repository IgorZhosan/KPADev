#ifndef DATA_RECEIVER_H
#define DATA_RECEIVER_H
#include <QCheckBox>


extern QCheckBox *kpaCheckBox;
extern QCheckBox *priemCheckBox;
extern QCheckBox *AD9M2;
extern QCheckBox *broadcast;
extern QCheckBox *TM;

extern QCheckBox *out_otkaz_checkbox;
extern QCheckBox *out_po_checkbox;
extern QCheckBox *out_k0_checkbox;
extern QCheckBox *out_tk_checkbox;
extern QCheckBox *out_kvs_checkbox;
extern QCheckBox *out_tsn_checkbox;
extern QCheckBox *out_nkk_checkbox;
extern QCheckBox *out_mgn_checkbox;
extern QCheckBox *out_pp_checkbox;
extern QCheckBox *out_ppp_checkbox;
extern QCheckBox *out_nk_checkbox;
extern QCheckBox *out_itp_checkbox;
extern QCheckBox *out_pr_checkbox;

extern QCheckBox *out_lk1_checkbox;
extern QCheckBox *out_lk2_checkbox;
extern QCheckBox *out_lk3_checkbox;
extern QCheckBox *out_lk4_checkbox;

extern QCheckBox *out_lt1_checkbox;
extern QCheckBox *out_lt2_checkbox;
extern QCheckBox *out_lt3_checkbox;
extern QCheckBox *out_lt4_checkbox;
extern QCheckBox *out_lt5_checkbox;
extern QCheckBox *out_lt6_checkbox;
extern QCheckBox *out_lt7_checkbox;

extern QCheckBox *out_kk1_checkbox;
extern QCheckBox *out_kk2_checkbox;
extern QCheckBox *out_kk3_checkbox;
extern QCheckBox *out_kk4_checkbox;
extern QCheckBox *out_kk5_checkbox;
extern QCheckBox *out_kk6_checkbox;
extern QCheckBox *out_kk7_checkbox;
extern QCheckBox *out_kk8_checkbox;
extern bool isStatePause;



// Объявление функции
void receiveDataAndDisplay();
void coder_ch1();
void checkAndSendAD9M2Broadcast();
void initSerialPort(const wchar_t* portName);
void  readFromSerialPort();
void Timer_Event();

#endif // DATA_RECEIVER_H
