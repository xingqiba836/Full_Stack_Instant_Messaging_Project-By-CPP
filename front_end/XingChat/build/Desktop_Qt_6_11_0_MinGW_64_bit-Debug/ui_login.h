/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *logoLabel;
    QSpacerItem *verticalSpacer_top;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QSpacerItem *verticalSpacer_mid;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(300, 500);
        Login->setMinimumSize(QSize(300, 500));
        Login->setMaximumSize(QSize(300, 500));
        Login->setStyleSheet(QString::fromUtf8("Login {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fafbfc, stop:1 #f1f3f6);\n"
"}\n"
"QLineEdit {\n"
"    border: none;\n"
"    border-bottom: 1px solid #d1d9e6;\n"
"    border-radius: 0px;\n"
"    padding: 15px 0px;\n"
"    background-color: transparent;\n"
"    color: #2c3e50;\n"
"    font-size: 14px;\n"
"    selection-background-color: #3498db;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-bottom: 2px solid #3498db;\n"
"    background-color: transparent;\n"
"}\n"
"QLineEdit::placeholder {\n"
"    color: #95a5a6;\n"
"}\n"
"QPushButton {\n"
"    border-radius: 8px;\n"
"    padding: 12px;\n"
"    font-weight: 500;\n"
"    font-size: 14px;\n"
"    border: none;\n"
"}\n"
"QPushButton#loginButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2ecc71, stop:1 #27ae60);\n"
"    color: white;\n"
"}\n"
"QPushButton#loginButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #27ae60, stop:1 #229954);\n"
"}\n"
"QPushButton#loginButton:pressed {\n"
""
                        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #229954, stop:1 #1e8449);\n"
"}\n"
"QPushButton#registerButton {\n"
"    background-color: transparent;\n"
"    color: #2ecc71;\n"
"    border: 1px solid #2ecc71;\n"
"}\n"
"QPushButton#registerButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ecf9f1, stop:1 #dff2e8);\n"
"    border: 1px solid #27ae60;\n"
"    color: #27ae60;\n"
"}\n"
"QPushButton#registerButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #d1f0db, stop:1 #b8dfc8);\n"
"    border: 1px solid #1e8449;\n"
"}\n"
"QLabel {\n"
"    color: #2c3e50;\n"
"}"));
        verticalLayout = new QVBoxLayout(Login);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(30, 20, 30, 20);
        logoLabel = new QLabel(Login);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setMinimumSize(QSize(120, 120));
        logoLabel->setMaximumSize(QSize(120, 120));
        logoLabel->setPixmap(QPixmap(QString::fromUtf8(":/resources/wechat.png")));
        logoLabel->setScaledContents(true);
        logoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(logoLabel, 0, Qt::AlignHCenter);

        verticalSpacer_top = new QSpacerItem(20, 15, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_top);

        usernameInput = new QLineEdit(Login);
        usernameInput->setObjectName("usernameInput");
        usernameInput->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(usernameInput);

        passwordInput = new QLineEdit(Login);
        passwordInput->setObjectName("passwordInput");
        passwordInput->setMinimumSize(QSize(0, 40));
        passwordInput->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(passwordInput);

        verticalSpacer_mid = new QSpacerItem(20, 15, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_mid);

        loginButton = new QPushButton(Login);
        loginButton->setObjectName("loginButton");
        loginButton->setMinimumSize(QSize(0, 45));
        loginButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        verticalLayout->addWidget(loginButton);

        registerButton = new QPushButton(Login);
        registerButton->setObjectName("registerButton");
        registerButton->setMinimumSize(QSize(0, 45));
        registerButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        verticalLayout->addWidget(registerButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "XingChat - \347\231\273\345\275\225", nullptr));
        usernameInput->setPlaceholderText(QCoreApplication::translate("Login", "\347\224\250\346\210\267\345\220\215\346\210\226\351\202\256\347\256\261", nullptr));
        passwordInput->setPlaceholderText(QCoreApplication::translate("Login", "\345\257\206\347\240\201", nullptr));
        loginButton->setText(QCoreApplication::translate("Login", "\347\231\273\345\275\225", nullptr));
        registerButton->setText(QCoreApplication::translate("Login", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
