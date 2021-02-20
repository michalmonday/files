#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <QDialog>

namespace Ui {
class Tutorial;
}

class Tutorial : public QDialog
{
    Q_OBJECT

public:
    explicit Tutorial(const QList<QPair<QString, QString>> slides, QWidget *parent = nullptr);
    ~Tutorial();

    void setCurrentSlide(int num);

    void setSlides(const QList<QPair<QString, QString>> slides_);

private slots:
    void on_pushButton_next_clicked();

    void on_pushButton_close_clicked();

    void on_checkBox_dont_show_again_toggled(bool checked);

    void on_pushButton_clicked();

private:
    Ui::Tutorial *ui;  

    QList<QPair<QString, QString>> slides;
};

extern Tutorial *tutorial;

#endif // TUTORIAL_H
