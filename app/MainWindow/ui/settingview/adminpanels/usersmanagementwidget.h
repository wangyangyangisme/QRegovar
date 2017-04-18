#ifndef USERSMANAGEMENTWIDGET_H
#define USERSMANAGEMENTWIDGET_H

#include <QWidget>
#include <QTableView>
#include "../settingpanels/myprofilewidget.h"


class UsersManagementWidget : public AbstractSettingsWidget
{
    Q_OBJECT
public:
    explicit UsersManagementWidget(QWidget *parent = 0);


public Q_SLOTS:
    bool save();
    bool load();
    void onChanged();

private:
    QTableView* mUsersList;
    MyProfileWidget* mUserDetails;



};

#endif // USERSMANAGEMENTWIDGET_H
