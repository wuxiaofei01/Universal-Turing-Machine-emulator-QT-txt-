#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    n=0;
    dely=1000;
    timer=new QTimer;
    state_file=0;
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    state=state_file;
    ui->horizontalSlider->setMaximum(1000);
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setValue(500);
    pos=5;
    pos2=1;
    check_ans=0;
    delete  timer;
    timer=new QTimer;
    flag=0;
    for(int i=0;i<22;i++)ans.push_back('B');
}
void MainWindow::on_pushButton_clicked()//文件选取按钮
{


    QString file;

    file = QFileDialog::getOpenFileName(this);

     QFile f(file);

     if (!f.open(QIODevice::ReadOnly))//打开指定文件
     {
         QMessageBox::about(NULL, "文件", "文件打开失败");
         return;
     }

         QTextStream txtInput(&f);//读入文件内容;
        txtInput.setCodec( QTextCodec::codecForName("UTF-8") );

         ui->text->clear();//初始化
         end.clear();
         for(int i=0;i<1000;i++)rule[i].clear();
         for(int i=0;i<1000;i++)//分配空间
         {
             rule2[i].dir.resize(100);
             rule2[i].c_now.resize(100);
             rule2[i].c_next.resize(100);
             rule2[i].pos_now.resize(100);
             rule2[i].pos_next.resize(100);
         }



     for(int i=1;i<=5;i++)//将数据展示到界面上
     {
         QString lineStr = txtInput.readLine();  //读取数据
         if(i==2)//得到合法输入字符
         {
            string temp=lineStr.toStdString();
            for(int i=4;i<temp.size();i+=3)
            {
                legal_chatacter.push_back(temp[i]);//将合法字符存入
            }
         }
         if(i==4)
         {
             string temp=lineStr.toStdString();
             string temp2;
             for(int i=5;i<temp.size();i++)temp2.push_back(temp[i]);
             state_file=atoi(temp2.c_str());
         }
         if(i==5)
         {
             string temp=lineStr.toStdString();
             for(int i=4;i<temp.size();i++)end.push_back(temp[i]);
         }
         ui->text->insertPlainText(lineStr);
         ui->text->insertPlainText("\n");
         ui->text->textCursor().movePosition(QTextCursor::Down);
     }


     n=0;//记录Node数组的位置
     while(!txtInput.atEnd())
     {
         QString temp=txtInput.readLine();
         string temp2=temp.toStdString();

         rule[++n]=temp2;

         istringstream is(temp2);
         is>>rule2[n].pos_now>>rule2[n].c_now>>rule2[n].pos_next>>rule2[n].c_next>>rule2[n].dir;

     }

     f.close();

}


void MainWindow::paintEvent(QPaintEvent *event)
{
    ui->text3->clear();//当前要显示的字符串
    ui->text2->clear();
    string temp;
    for(int i=0;i<pos;i++)temp.push_back(ans[i]);

    ui->text3->insertPlainText(QString::fromStdString(temp));//前缀字符串

    ui->text3->setTextColor(Qt::red);     //高亮
    temp.clear();
    temp.push_back(ans[pos]);
    ui->text3->insertPlainText(QString::fromStdString(temp));
    ui->text3->setTextColor(Qt::black);     //变黑

    temp.clear();
    for(int i=pos+1;i<ans.size();i++)temp.push_back(ans[i]);
    ui->text3->insertPlainText(QString::fromStdString(temp));//后缀字符串

    ui->text3->insertPlainText("\n");
    ui->text3->textCursor().movePosition(QTextCursor::Down);

    if(n>=25)
    {   QFont front("Arial",9);
        ui->text2->setFont(front);
    }
    else
    {
        QFont front("Arial",15);
                ui->text2->setFont(front);
    }
    //滚动状态栏
    for(int i=1;i<=n;i++)
    {
        if(i==pos2)ui->text2->setTextColor(Qt::red);
        ui->text2->insertPlainText(QString::fromStdString(rule[i]));
        ui->text2->insertPlainText("\n");
        //ui->text2->setTextColor(Qt::yellow);     可以实现文本高亮了
        ui->text2->textCursor().movePosition(QTextCursor::Down);
        if(i==pos2)ui->text2->setTextColor(Qt::black);
    }
}

void MainWindow::out(int flag)//这是第几条规则
{
    if(check_ans==1||rule2[flag].pos_next==end)
    {
        QMessageBox msg(this);//对话框设置父组件
        msg.setStyleSheet("QLabel{"
                              "min-width:150px;"
                              "min-height:40px; "
                              "font-size:16px;"
                              "}");
        msg.setWindowTitle("input is legal");//对话框标题
        msg.setText("accept");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok );//对话框上包含的按钮
        if(msg.exec() == QMessageBox::Ok)//模态调用
        {
           qDebug() << " Ok is clicked!";//数据处理
        }
        check_ans=1;
        timer->stop();
    }
    else if(flag==0)
    {
        qDebug()<<flag;
        QMessageBox msg(this);//对话框设置父组件
        msg.setStyleSheet("QLabel{"
                              "min-width:150px;"
                              "min-height:40px; "
                              "font-size:16px;"
                              "}");
        msg.setWindowTitle("input is invalided");//对话框标题
        msg.setText("rejected");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok );//对话框上包含的按钮
        if(msg.exec() == QMessageBox::Ok)//模态调用
        {
           qDebug() << " Ok is clicked!";//数据处理
        }
        timer->stop();
    }
}
void MainWindow::run()
{
    //更改ans字符串 这样显示出来就是不一样的了 同时需要高亮 状态位置
     //0 0 1 X R
     int flag=0;//判断是否有成立的条件 如果没有 那么自然不符合了
     string temp;
     temp.push_back(ans[pos]);//为了下续判断 string 调整数据结构
     for(int i=1;i<=n;i++)//遍历所有的规则
     {
         if(atoi(rule2[i].pos_now.c_str())==state&&rule2[i].c_now==temp)//当前的状态合法 而且读取的字符合法
         {
             flag=i;
             ans[pos]=rule[i][6];
             if(rule[i][8]=='R')pos++;
             else if(rule[i][8]=='L')pos--;
             state=rule[i][4]-'0';
             pos2=i;
             break;
         }
     }
     out(flag);
     update();
}
void MainWindow::on_pushButton_2_clicked()//运行按钮
{
    timer->stop();
    run();
}

bool MainWindow::check(string s)
{
    for(int i=0;i<s.size();i++)
    {
        int flag=0;
        for(int j=0;j<legal_chatacter.size();j++)
        {
            if(s[i]==legal_chatacter[j])flag=1;
        }
        if(flag==0)return 0;
    }
    return 1;
}
void MainWindow::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
    QString input=ui->lineEdit->text();
    QMessageBox msg(this);//对话框设置父组件
    msg.setWindowTitle("input is not invailid");//对话框标题
    msg.setText("This is a wrong input!");//对话框提示文本
    msg.setIcon(QMessageBox::Information);//设置图标类型
    msg.setStandardButtons(QMessageBox::Ok );//对话框上包含的按钮

    int flag=check(input.toStdString());


    if((flag==0)&&msg.exec() == QMessageBox::Ok)//模态调用
    {
       qDebug() << " Ok is clicked!";//数据处理
    }

    init();
    string temp=input.toStdString();
    ans.clear();


    for(int i=0;i<=4;i++)ans.push_back('B');

    for(int i=0;i<input.size();i++)ans.push_back(temp[i]);

    for(int i=0;i<=4;i++)ans.push_back('B');

}

void MainWindow::on_pushButton_3_clicked()
{
    if(flag==0)//还没链接过
    {
        connect(timer, SIGNAL(timeout()), this, SLOT(run()));
        flag=1;
    }

    timer->start(dely);

}

void MainWindow::on_pushButton_4_clicked()
{
    timer->stop();
}



void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    dely=ui->horizontalSlider->maximum() - ui->horizontalSlider->value();

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_pushButton_3_clicked()));


}
