#ifndef STEP_LOG_H
#define STEP_LOG_H

#include <QListWidgetItem>
#include <QWidget>

namespace Ui {
class SelectAlgorithmWidget;
}

class SelectAlgorithmWidget : public QWidget
{
    Q_OBJECT

public:
    Ui::SelectAlgorithmWidget *ui;

    explicit SelectAlgorithmWidget(QWidget *parent = nullptr);
    virtual ~SelectAlgorithmWidget() override;

    void reset();

    void addStep(const QString &list_entry, const QString &description);

    void setAlgorithmBrief(QString brief);

    QString selectedAlgorithm();

    void useSuggestedGraphs(bool state);

public slots:
    void onItemSelected(QListWidgetItem *item);

private slots:
    void on_comboBox_algorithm_currentTextChanged(const QString &arg1);
    void on_checkBox_use_suggested_graphs_stateChanged(int arg1);

private:
    QStringList descriptions;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
};

extern SelectAlgorithmWidget *select_algorithm_widget;
extern bool use_suggested_graphs;

#endif // STEP_LOG_H
