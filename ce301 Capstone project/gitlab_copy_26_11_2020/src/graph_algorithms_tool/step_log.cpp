#include "step_log.h"
#include "ui_step_log.h"

#include <QSettings>

StepLog::StepLog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StepLog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &StepLog::onItemSelected);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    // load position and size
    QSettings settings("settings.ini", QSettings::IniFormat);
    const QString prefix = windowTitle() + "/";
    if (settings.value(prefix + "was_saved", false).toBool())
        restoreGeometry( settings.value(prefix + "geometry").toByteArray() );
}

StepLog::~StepLog()
{
    // store position and size
    QSettings settings("settings.ini", QSettings::IniFormat);
    const QString prefix = windowTitle() + "/";
    settings.setValue(prefix + "was_saved", true);
    settings.setValue(prefix + "geometry", saveGeometry());

    delete ui;
}

void StepLog::reset() {
    //ui->listWidget->addItems({"a","b"});
    ui->listWidget->clear();
    ui->textBrowser->clear();
    descriptions.clear();
}

void StepLog::addStep(const QString &list_entry, const QString &description)
{
    QListWidget *list = ui->listWidget;
    QTextBrowser *text_browser = ui->textBrowser;

    list->addItem(list_entry);
    text_browser->setHtml(description);
    descriptions.append(description);


    for (QListWidgetItem *selected_item : list->selectedItems())
        selected_item->setSelected(false);

    QListWidgetItem *new_item = list->item(list->count()-1);
    new_item->setSelected(true);
    list->scrollToBottom();
}

// slot
void StepLog::onItemSelected(QListWidgetItem *item)
{
    ui->textBrowser->setHtml(
                descriptions[ui->listWidget->row(item)]
            );
}
