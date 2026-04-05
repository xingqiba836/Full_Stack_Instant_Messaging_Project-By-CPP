#include "register.h"
#include "ui_register.h"
#include <QMessageBox>

Register::Register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    connectSignals();
}

Register::~Register()
{
    delete ui;
}

void Register::connectSignals()
{
    connect(ui->registerButton, &QPushButton::clicked, this, &Register::onRegisterClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &Register::onCancelClicked);
    connect(ui->getCodeButton, &QPushButton::clicked, this, &Register::onGetCodeClicked);
}

void Register::onRegisterClicked()
{
    QString username = ui->usernameInput->text().trimmed();
    QString email = ui->emailInput->text().trimmed();
    QString password = ui->passwordInput->text();
    QString confirmPassword = ui->confirmPasswordInput->text();
    QString code = ui->verificationInput->text().trimmed();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入用户名");
        ui->usernameInput->setFocus();
        return;
    }

    if (email.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入邮箱");
        ui->emailInput->setFocus();
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入密码");
        ui->passwordInput->setFocus();
        return;
    }

    if (confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "提示", "请确认密码");
        ui->confirmPasswordInput->setFocus();
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致");
        ui->confirmPasswordInput->setFocus();
        return;
    }

    if (code.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入验证码");
        ui->verificationInput->setFocus();
        return;
    }

    QMessageBox::information(this, "注册成功", QString("用户名: %1\n邮箱: %2\n\n(注册成功，请添加实际注册逻辑)").arg(username, email));
    emit backToLogin();
    close();
}

void Register::onCancelClicked()
{
    emit backToLogin();
    close();
}

void Register::onGetCodeClicked()
{
    QMessageBox::information(this, "验证码", "验证码已发送，请在邮箱中查收（仅演示）");
}
