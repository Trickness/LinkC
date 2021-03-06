/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 6, 2014
 */
#include "MainWindow.h"
#include "linkc_types.h"
#include <unistd.h>
#include <string.h>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent){
    usrLabel      = new QLabel(tr("UserName:"));
    pwdLabel      = new QLabel(tr("PassWord:"));
    UsernameInput = new QLineEdit;
    PasswordInput = new QLineEdit;
    PasswordInput->setEchoMode(QLineEdit::Password);
    gridLayout = new QGridLayout;
    gridLayout->addWidget(usrLabel, 0, 0, 1, 1);
    gridLayout->addWidget(UsernameInput, 0, 1, 1, 3);
    gridLayout->addWidget(pwdLabel, 1, 0, 1, 1);
    gridLayout->addWidget(PasswordInput, 1, 1, 1, 3);
    LoginButton = new QPushButton(tr("Login"));
    CancelButton = new QPushButton(tr("Cancel"));
    btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(50);
    btnLayout->addWidget(LoginButton);
    btnLayout->addWidget(CancelButton);
    dlgLayout = new QVBoxLayout;
    dlgLayout->setMargin(40);
    dlgLayout->addLayout(gridLayout);
    dlgLayout->addStretch(40);
    dlgLayout->addLayout(btnLayout);
    setLayout(dlgLayout);
    connect(LoginButton, SIGNAL(clicked()), this, SLOT(LoginClick()));
    connect(CancelButton, SIGNAL(clicked()), this, SLOT(QuitClick()));
    setWindowTitle(tr("Login"));
    resize(300, 200);
    setMinimumSize(300,200);
    setMaximumSize(300,200);
}

LoginWindow::~LoginWindow(){
    delete usrLabel;
    delete pwdLabel;
    delete UsernameInput;
    delete PasswordInput;
    delete gridLayout;
    delete LoginButton;
    delete CancelButton;
    delete btnLayout;
    delete dlgLayout;
    LinkC_Debug("Login Window",LINKC_EXITED);
}

void LoginWindow::QuitClick(void){
    QString response = "确定退出？";
    int answer = 0;
    answer = QMessageBox::question(0,"询问",response,QMessageBox::Yes|QMessageBox::No);
    if (answer == QMessageBox::Yes){
        delete usrLabel;
        delete pwdLabel;
        delete UsernameInput;
        delete PasswordInput;
        delete gridLayout;
        delete LoginButton;
        delete CancelButton;
        delete btnLayout;
        delete dlgLayout;
        LinkC_Debug("Login Window",LINKC_EXITED);
        emit Exit();
    }
}

void LoginWindow::closeEvent(QEvent *){
    emit Exit();
}

void LoginWindow::LoginClick(void){

    accept();
}

int LoginWindow::GetLoginData(struct login_data& s){
    QString aa = UsernameInput->text();
    QString ab = PasswordInput->text();
    if (aa == ""){
        QMessageBox::warning(0,"警告！","用户名不能为空",QMessageBox::Yes);
        return -1;
    }
    else if (ab == ""){
        QMessageBox::warning(0,"警告！","密码不能为空",QMessageBox::Yes);
        return -1;
    }
    QByteArray ba = aa.toLatin1();
    UserName = ba.data();
    strncpy (st.user_name,ba.data(),12);
    ba = ab.toLatin1();
    PassWord = ba.data();
    strncpy (st.pass_word,ba.data(),20);
    s = st;
    return 0;
}
