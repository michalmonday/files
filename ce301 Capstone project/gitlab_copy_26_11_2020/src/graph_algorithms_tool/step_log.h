#ifndef STEP_LOG_H
#define STEP_LOG_H

#include <QListWidgetItem>
#include <QWidget>

namespace Ui {
class StepLog;
}

class StepLog : public QWidget
{
    Q_OBJECT

public:
    explicit StepLog(QWidget *parent = nullptr);
    virtual ~StepLog() override;

    void reset();

    void addStep(const QString &list_entry, const QString &description);

public slots:
    void onItemSelected(QListWidgetItem *item);

private:
    Ui::StepLog *ui;
    QStringList descriptions;
};

#endif // STEP_LOG_H
