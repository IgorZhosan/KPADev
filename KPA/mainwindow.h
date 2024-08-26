#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>

extern QCheckBox *kpaCheckBox;
extern QCheckBox *priemCheckBox;

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
extern QCheckBox *out_kk1_checkbox;
extern QCheckBox *out_kk2_checkbox;
extern QCheckBox *out_kk3_checkbox;
extern QCheckBox *out_kk4_checkbox;
extern QCheckBox *out_kk5_checkbox;
extern QCheckBox *out_kk6_checkbox;
extern QCheckBox *out_kk7_checkbox;
extern QCheckBox *out_kk8_checkbox;
extern QCheckBox* AD9M2;
extern QCheckBox* broadcast;



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
