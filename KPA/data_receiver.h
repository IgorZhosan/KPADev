#ifndef DATA_RECEIVER_H
#define DATA_RECEIVER_H
#include <QCheckBox>

extern QCheckBox *kpaCheckBox;
extern QCheckBox *priemCheckBox;

// Объявление функции
void receiveDataAndDisplay();
void ifCheckBoxesIsTrue();
void coder_ch1();
void checkAndSendAD9M2Broadcast();

#endif // DATA_RECEIVER_H
