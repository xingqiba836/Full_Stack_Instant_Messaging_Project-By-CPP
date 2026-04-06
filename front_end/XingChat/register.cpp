#include "register.h"
#include "HttpMgr.h"
#include "ui_register.h"
#include <QDebug>
#include <QJsonDocument>
#include <QMessageBox>
#include <QStyle>
#include <QUrl>

namespace {

void repolish(QWidget *w)
{
    w->style()->unpolish(w);
    w->style()->polish(w);
}

} // namespace

Register::Register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    initHttpHandlers();
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
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &Register::slot_reg_mod_finish);
}

void Register::initHttpHandlers()
{
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject &jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误"), false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱，注意查收"), true);
        qDebug() << "email is " << email;
    });
}

void Register::showTip(QString str, bool b_ok)
{
    if (b_ok) {
        ui->err_tip->setProperty("state", "err");
    } else {
        ui->err_tip->setProperty("state", "normal");
    }

    ui->err_tip->setText(str);

    repolish(ui->err_tip);
}

void Register::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if (err != ErrorCodes::SUCCESS) {
        showTip(tr("网络请求错误"), false);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isNull()) {
        showTip(tr("json解析错误"), false);
        return;
    }

    if (!jsonDoc.isObject()) {
        showTip(tr("json解析错误"), false);
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    auto it = _handlers.find(id);
    if (it == _handlers.end()) {
        return;
    }
    it.value()(jsonObj);

    return;
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
    QString email = ui->emailInput->text().trimmed();
    if (email.isEmpty()) {
        showTip(tr("请输入邮箱"), false);
        ui->emailInput->setFocus();
        return;
    }

    QJsonObject json;
    json["email"] = email;
    QString request_url = gate_url_prefix + "/get_varifycode";
    QUrl url(request_url);
    HttpMgr::GetInstance()->PostHttpReq(url, json, ReqId::ID_GET_VARIFY_CODE, Modules::REGISTERMOD);
}
