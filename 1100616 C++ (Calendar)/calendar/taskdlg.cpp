#include "taskdlg.h"
#include "ui_taskdlg.h"
#include "calendar.h"
#include "birthday.h"
#include "meeting.h"

#include <QDebug>
#include <QInputDialog>
#include <QDateTime>

void TaskDlg::init()
{
    ui->setupUi(this);
}

// для добавления любой задачи
TaskDlg::TaskDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDlg)
{
    init();
    ui->optTask->animateClick();
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

// для редактирования дела (Task)
TaskDlg::TaskDlg(const QDateTime& when, const QString& name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDlg)
{
    init();
    ui->optTask->animateClick();
    ui->txtName->setText(name);
    ui->dateTimeEdit->setDateTime(when);
}

// для редактирования звонка (Interaction)
TaskDlg::TaskDlg(const QDateTime& when, const QString& name, const QString& who, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDlg)
{
    init();
    ui->optCall->animateClick();
    ui->txtName->setText(name);
    ui->txtWho->setText(who);
    ui->dateTimeEdit->setDateTime(when);
}

// для редактирования встречи (Meeting)
TaskDlg::TaskDlg(const QDateTime& when, const QString& name, const QString& who, const QString& place, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDlg)
{
    init();
    ui->optMeeting->animateClick();
    ui->txtName->setText(name);
    ui->txtWho->setText(who);
    ui->txtWhere->setText(place);
    ui->dateTimeEdit->setDateTime(when);
}

// для редактирования ДР (Birthday)
TaskDlg::TaskDlg(const QDateTime& when, const QString& name, const QString& who, const QList<QString>& presents, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDlg)
{
    init();
    ui->optBirthday->animateClick();
    ui->txtName->setText(name);
    ui->txtWho->setText(who);
    for(const auto& present : presents) {
        ui->listWidget->addItem(present);
    }
    ui->dateTimeEdit->setDateTime(when);
}

TaskDlg::~TaskDlg()
{
    qDebug() << "TaskDlg::destructor";
    delete ui;
}

void TaskDlg::on_cmdAdd_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(nullptr, "Новый подарок", "Подарок: ", QLineEdit::Normal, "", &ok);
    if(ok && !name.isEmpty()) {
        auto item = new QListWidgetItem(name);
        ui->listWidget->addItem(item);
    }
}

void TaskDlg::on_cmdDelete_clicked()
{
    ;
}

void TaskDlg::on_optTask_clicked()
{
    qDebug() << "TASK";
    m_taskType = ttCommon;
    ui->txtName->setText("Дело");
    ui->gbxList->setVisible(false);
    ui->txtWhere->setEnabled(false);
    ui->lblWhere->setEnabled(false);
    ui->txtWho->setEnabled(false);
    ui->lblWho->setEnabled(false);
    ui->lblWho->setText("С кем");
}

void TaskDlg::on_optMeeting_clicked()
{
    qDebug() << "MEETING";
    m_taskType = ttMeeting;
    ui->txtName->setText("Встреча");
    ui->gbxList->setVisible(false);
    ui->txtWhere->setEnabled(true);
    ui->lblWhere->setEnabled(true);
    ui->txtWho->setEnabled(true);
    ui->lblWho->setEnabled(true);
    ui->lblWho->setText("С кем");
}

void TaskDlg::on_optCall_clicked()
{
    qDebug() << "CALL";
    m_taskType = ttCall;
    ui->txtName->setText("Звонок");
    ui->gbxList->setVisible(false);
    ui->txtWhere->setEnabled(false);
    ui->lblWhere->setEnabled(false);
    ui->txtWho->setEnabled(true);
    ui->lblWho->setEnabled(true);
    ui->lblWho->setText("Кому");
}

void TaskDlg::on_optBirthday_clicked()
{
    qDebug() << "BIRTHDAY";
    m_taskType = ttBirthday;
    ui->txtName->setText("ДР");
    ui->gbxList->setVisible(true);
    ui->txtWhere->setEnabled(false);
    ui->lblWhere->setEnabled(false);
    ui->txtWho->setEnabled(true);
    ui->lblWho->setEnabled(true);
    ui->lblWho->setText("У кого");
}

void TaskDlg::on_buttonBox_accepted()
{
    if(m_task)
        delete m_task;
    switch(m_taskType) {
    case ttCommon:
        m_task = new Task(ui->dateTimeEdit->dateTime(), ui->txtName->text());
        break;
    case ttMeeting:
        m_task = new Meeting(ui->dateTimeEdit->dateTime(), ui->txtName->text(),
                             ui->txtWho->text(), ui->txtWhere->text());
        break;
    case ttCall:
        m_task = new Interaction(ui->dateTimeEdit->dateTime(), ui->txtName->text(),
                                 ui->txtWho->text());
        break;
    case ttBirthday:
        m_task = new Birthday(ui->dateTimeEdit->dateTime(), ui->txtName->text(),
                              ui->txtWho->text());
        for(int i = 0; i < ui->listWidget->count(); ++i) {
            static_cast<Birthday*>(m_task)->addPresent(ui->listWidget->item(i)->text());
        }
        break;
    default:
        m_task = 0;
    }
}
