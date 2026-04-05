#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register() override;

signals:
    void backToLogin();

private slots:
    void onRegisterClicked();
    void onCancelClicked();
    void onGetCodeClicked();

private:
    void connectSignals();
    Ui::Register *ui;
};

#endif // REGISTER_H
