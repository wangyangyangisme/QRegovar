#ifndef RESUMEWIDGET_H
#define RESUMEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>

namespace projectview
{



class ResumeWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel* mCommentLabel;
    QLabel* mSharedUserLabel;
    QTableWidget* mEventsTable;
    QTableWidget* mSubjectsTable;
    QTableWidget* mTasksTable;
    QTableWidget* mFilesTable;



public:
    explicit ResumeWidget(QWidget *parent = 0);


Q_SIGNALS:


public Q_SLOTS:



};
} // END namespace projectview
#endif // RESUMEWIDGET_H