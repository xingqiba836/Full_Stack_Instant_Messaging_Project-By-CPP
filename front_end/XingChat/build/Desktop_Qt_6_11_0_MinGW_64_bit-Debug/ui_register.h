/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *usernameInput;
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QLineEdit *confirmPasswordInput;
    QHBoxLayout *verificationLayout;
    QLineEdit *verificationInput;
    QPushButton *getCodeButton;
    QPushButton *registerButton;
    QPushButton *cancelButton;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName("Register");
        Register->resize(320, 540);
        Register->setMinimumSize(QSize(320, 540));
        Register->setMaximumSize(QSize(320, 540));
        Register->setStyleSheet(QString::fromUtf8("Register {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fafbfc, stop:1 #f1f3f6);\n"
"}\n"
"QLineEdit {\n"
"    border: none;\n"
"    border-bottom: 1px solid #d1d9e6;\n"
"    border-radius: 0px;\n"
"    padding: 10px 0px;\n"
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
"QPushButton#registerButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2ecc71, stop:1 #27ae60);\n"
"    color: white;\n"
"}\n"
"QPushButton#registerButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #27ae60, stop:1 #229954);\n"
"}\n"
"QPushButton#registerButton:pr"
                        "essed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #229954, stop:1 #1e8449);\n"
"}\n"
"QPushButton#getCodeButton {\n"
"    background-color: #ffffff;\n"
"    color: #2c3e50;\n"
"    border: 1px solid #d1d9e6;\n"
"}\n"
"QPushButton#getCodeButton:hover {\n"
"    background-color: #f4f6f8;\n"
"}\n"
"QPushButton#cancelButton {\n"
"    background-color: transparent;\n"
"    color: #2ecc71;\n"
"    border: 1px solid #2ecc71;\n"
"}\n"
"QPushButton#cancelButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ecf9f1, stop:1 #dff2e8);\n"
"    border: 1px solid #27ae60;\n"
"    color: #27ae60;\n"
"}\n"
"QLabel {\n"
"    color: #2c3e50;\n"
"}\n"
""));
        verticalLayout = new QVBoxLayout(Register);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(30, 20, 30, 20);
        usernameInput = new QLineEdit(Register);
        usernameInput->setObjectName("usernameInput");
        usernameInput->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(usernameInput);

        emailInput = new QLineEdit(Register);
        emailInput->setObjectName("emailInput");
        emailInput->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(emailInput);

        passwordInput = new QLineEdit(Register);
        passwordInput->setObjectName("passwordInput");
        passwordInput->setMinimumSize(QSize(0, 40));
        passwordInput->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(passwordInput);

        confirmPasswordInput = new QLineEdit(Register);
        confirmPasswordInput->setObjectName("confirmPasswordInput");
        confirmPasswordInput->setMinimumSize(QSize(0, 40));
        confirmPasswordInput->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(confirmPasswordInput);

        verificationLayout = new QHBoxLayout();
        verificationLayout->setObjectName("verificationLayout");
        verificationInput = new QLineEdit(Register);
        verificationInput->setObjectName("verificationInput");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(verificationInput->sizePolicy().hasHeightForWidth());
        verificationInput->setSizePolicy(sizePolicy);
        verificationInput->setMinimumSize(QSize(0, 40));

        verificationLayout->addWidget(verificationInput);

        getCodeButton = new QPushButton(Register);
        getCodeButton->setObjectName("getCodeButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(getCodeButton->sizePolicy().hasHeightForWidth());
        getCodeButton->setSizePolicy(sizePolicy1);
        getCodeButton->setMinimumSize(QSize(80, 40));
        getCodeButton->setMaximumSize(QSize(100, 40));
        getCodeButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        verificationLayout->addWidget(getCodeButton);


        verticalLayout->addLayout(verificationLayout);

        registerButton = new QPushButton(Register);
        registerButton->setObjectName("registerButton");
        registerButton->setMinimumSize(QSize(0, 45));
        registerButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        verticalLayout->addWidget(registerButton);

        cancelButton = new QPushButton(Register);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setMinimumSize(QSize(0, 45));
        cancelButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        verticalLayout->addWidget(cancelButton);


        retranslateUi(Register);

        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QWidget *Register)
    {
        Register->setWindowTitle(QCoreApplication::translate("Register", "XingChat - \346\263\250\345\206\214", nullptr));
        usernameInput->setPlaceholderText(QCoreApplication::translate("Register", "\347\224\250\346\210\267", nullptr));
        emailInput->setPlaceholderText(QCoreApplication::translate("Register", "\351\202\256\347\256\261", nullptr));
        passwordInput->setPlaceholderText(QCoreApplication::translate("Register", "\345\257\206\347\240\201", nullptr));
        confirmPasswordInput->setPlaceholderText(QCoreApplication::translate("Register", "\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        verificationInput->setPlaceholderText(QCoreApplication::translate("Register", "\351\252\214\350\257\201\347\240\201", nullptr));
        getCodeButton->setText(QCoreApplication::translate("Register", "\350\216\267\345\217\226", nullptr));
        registerButton->setText(QCoreApplication::translate("Register", "\346\263\250\345\206\214", nullptr));
        cancelButton->setText(QCoreApplication::translate("Register", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
