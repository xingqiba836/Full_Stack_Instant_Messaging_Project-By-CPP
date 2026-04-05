#include "login.h"
#include "ui_login.h"
#include "register.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    
    // 从资源系统加载图像
    QPixmap pixmap(":/resources/wechat.png");
    
    if (!pixmap.isNull()) {
        ui->logoLabel->setPixmap(pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qDebug() << "Could not load image from resource";
        ui->logoLabel->setText("💬");
        ui->logoLabel->setStyleSheet("QLabel { font-size: 60px; }");
    }
    
    connectSignals();
}

Login::~Login()
{
    delete ui;
}

void Login::connectSignals()
{
    connect(ui->loginButton, &QPushButton::clicked, this, &Login::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &Login::onRegisterClicked);

    // 支持回车键登录
    connect(ui->usernameInput, &QLineEdit::returnPressed, this, &Login::onLoginClicked);
    connect(ui->passwordInput, &QLineEdit::returnPressed, this, &Login::onLoginClicked);
}

void Login::onLoginClicked()
{
    QString username = ui->usernameInput->text().trimmed();
    QString password = ui->passwordInput->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入用户名");
        ui->usernameInput->setFocus();
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入密码");
        ui->passwordInput->setFocus();
        return;
    }

    // 这里添加登录逻辑
    QMessageBox::information(this, "登录", 
        QString("用户名: %1\n密码: %2\n\n(这是演示，请添加实际的登录逻辑)").arg(username, password));
}

void Login::onRegisterClicked()
{
    Register *reg = new Register;
    reg->setAttribute(Qt::WA_DeleteOnClose);
    reg->setGeometry(this->geometry());
    reg->setFixedSize(this->size());

    connect(reg, &Register::backToLogin, this, [this]() {
        this->show();
    });
    connect(reg, &QObject::destroyed, this, [this]() {
        if (!this->isVisible()) {
            this->show();
        }
    });
    reg->show();
    hide();
}