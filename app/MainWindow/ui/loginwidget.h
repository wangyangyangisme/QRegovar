#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QStringList>
#include <QDebug>

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget* parent = 0);

    const QString username() const;
    const QString password() const;


    void loginFailed();
    void clear();

Q_SIGNALS:
    void accepted();


private:
    QLabel* mErrorMessage;
    QComboBox* mComboUsername;
    QLineEdit* mEditPassword;



};

#endif // LOGINWIDGET_H
