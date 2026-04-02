#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login() override;

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    void connectSignals();

    Ui::Login *ui;
};

#endif // LOGIN_H
