#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<vector>
#include<bits/stdc++.h>
#include<QFileDialog>
#include<QFile>
#include<QMessageBox>
#include<QTextStream>
#include<QDebug>
#include<QTimer>
#include<QTextCodec>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


using namespace  std;
struct Node//当前的状态 接下来的状态 当前的字符 接下来的字符 方向    001Xl 当前状态0 当前字符 0 变为状态1 变为字符X 往左
{
    string pos_now,pos_next;
    string c_now,c_next;
    string dir;//l r s
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    string end;//结束标识符
    string rule[1000];//存放图灵机规则
    Node rule2[1000];
    string legal_chatacter;//合法字符
    string ans;

    QTimer *timer;

    int n;//记录有多少条规则
    int dely;//延迟时间发射
    int pos;//当前的位置
    int pos2;//记录应用的是哪一条规则
    int state;//状态标识符
    int state_file;//文件输入的初始状态
    int check_ans;//判断当前位置是否已经满足了
    bool flag;//判断时间触发器是否存在

    void init();
    bool check(string s);//检查字符串是否合法
    void out(int flag);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_pushButton_3_clicked();

    void run();

    void on_pushButton_4_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *event);
};
#endif // MAINWINDOW_H
