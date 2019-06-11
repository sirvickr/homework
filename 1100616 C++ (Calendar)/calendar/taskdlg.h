#ifndef TASKDLG_H
#define TASKDLG_H

#include <QDialog>

namespace Ui {
class TaskDlg;
}

class Task;

class TaskDlg : public QDialog
{
    Q_OBJECT

public:
    enum TaskType {
        ttCommon,
        ttMeeting,
        ttCall,
        ttBirthday
    };
    // для добавления
    explicit TaskDlg(QWidget *parent = 0);
    // для редактирования дела (Task)
    explicit TaskDlg(const QDateTime& when, const QString& name, QWidget *parent = 0);
    // для редактирования звонка (Interaction)
    explicit TaskDlg(const QDateTime& when, const QString& name, const QString& who, QWidget *parent = 0);
    // для редактирования встречи (Meeting)
    explicit TaskDlg(const QDateTime& when, const QString& name, const QString& who, const QString& place, QWidget *parent = 0);
    // для редактирования ДР (Birthday)
    explicit TaskDlg(const QDateTime& when, const QString& name, const QString& who, const QList<QString>& presents, QWidget *parent = 0);
    ~TaskDlg();

    Task* task() const {
        return m_task;
    }

private slots:
    void on_cmdAdd_clicked();

    void on_cmdDelete_clicked();

    void on_optTask_clicked();

    void on_optMeeting_clicked();

    void on_optCall_clicked();

    void on_optBirthday_clicked();

    void on_buttonBox_accepted();

private:
    Ui::TaskDlg *ui;
    Task* m_task = 0;
    TaskType m_taskType = ttCommon;

private:
    void init();
};

#endif // TASKDLG_H
