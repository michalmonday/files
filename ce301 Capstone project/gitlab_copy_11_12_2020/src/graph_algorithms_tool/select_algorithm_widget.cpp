#include "select_algorithm_widget.h"
#include "ui_select_algorithm_widget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QSettings>
#include <QWindow>

SelectAlgorithmWidget *select_algorithm_widget = nullptr;
bool use_suggested_graphs = false;

SelectAlgorithmWidget::SelectAlgorithmWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectAlgorithmWidget)
{
    ui->setupUi(this);

    QSettings settings("settings.ini", QSettings::IniFormat);
    QString prefix = windowTitle() + "/";
    if (settings.value(prefix + "was_saved", false).toBool()) {
        restoreGeometry( settings.value(prefix + "geometry").toByteArray() );
        useSuggestedGraphs( settings.value(prefix + "use_suggested_graphs", false).toBool() );
    }

    setWindowFlag(Qt::WindowStaysOnTopHint);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &SelectAlgorithmWidget::onItemSelected);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowFlag(Qt::WindowType::WindowCloseButtonHint, false);
    //setWindowFlag(Qt::WindowType::WindowDoesNotAcceptFocus, true);
}

SelectAlgorithmWidget::~SelectAlgorithmWidget()
{
    // store position and size
    QSettings settings("settings.ini", QSettings::IniFormat);
    const QString prefix = windowTitle() + "/";
    settings.setValue(prefix + "was_saved", true);
    settings.setValue(prefix + "geometry", saveGeometry());
    settings.setValue(prefix + "use_suggested_graphs", use_suggested_graphs);

    delete ui;
}

void SelectAlgorithmWidget::reset() {
    //ui->listWidget->addItems({"a","b"});
    ui->listWidget->clear();
    ui->textBrowser->clear();
    descriptions.clear();
}

void SelectAlgorithmWidget::addStep(const QString &list_entry, const QString &description)
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

void SelectAlgorithmWidget::setAlgorithmBrief(QString brief)
{
    ui->label_algorithm_brief->setText(brief);
}

void SelectAlgorithmWidget::useSuggestedGraphs(bool state)
{
    ui->checkBox_use_suggested_graphs->setChecked(state);
}

QString SelectAlgorithmWidget::selectedAlgorithm()
{
    return ui->comboBox_algorithm->currentText();
}

// slot
void SelectAlgorithmWidget::onItemSelected(QListWidgetItem *item)
{
    ui->textBrowser->setHtml(
                descriptions[ui->listWidget->row(item)]
            );
}

void SelectAlgorithmWidget::on_comboBox_algorithm_currentTextChanged(const QString &algorithm_name)
{
    main_window->ui->graphicsView->setAlgorithm(algorithm_name);
    main_window->ui->graphicsView->setFocus();
}


void SelectAlgorithmWidget::mousePressEvent(QMouseEvent *event)
{
    if (QWindow *window = windowHandle())
        window->startSystemMove();
}


void SelectAlgorithmWidget::focusInEvent(QFocusEvent *event)
{
}


void SelectAlgorithmWidget::keyPressEvent(QKeyEvent *event)
{
    QKeyEvent *e = new QKeyEvent ( event->type(), event->key(), event->modifiers());
    QCoreApplication::postEvent (main_window->ui->graphicsView->scene(), e);
}

void SelectAlgorithmWidget::keyReleaseEvent(QKeyEvent *event)
{
    QKeyEvent *e = new QKeyEvent ( event->type(), event->key(), event->modifiers());
    QCoreApplication::postEvent (main_window->ui->graphicsView->scene(), e);
}

void SelectAlgorithmWidget::on_checkBox_use_suggested_graphs_stateChanged(int checked)
{
    use_suggested_graphs = checked;
    if (checked) {
        main_window->ui->graphicsView->setAlgorithm(selectedAlgorithm());
        main_window->ui->graphicsView->setFocus();
    }
}
