#ifndef REGISTER_H
#define REGISTER_H

#include "global.h"
#include <QJsonObject>
#include <QMap>
#include <QString>
#include <QWidget>
#include <functional>

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
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

private:
    void connectSignals();
    void initHttpHandlers();
    void showTip(QString str, bool b_ok);

    Ui::Register *ui;
    QMap<ReqId, std::function<void(const QJsonObject &)>> _handlers;
};

#endif // REGISTER_H
