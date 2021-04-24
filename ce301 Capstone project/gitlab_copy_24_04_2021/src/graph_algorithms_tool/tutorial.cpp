#include "tutorial.h"
#include "ui_tutorial.h"

#include <QLabel>
#include <QMovie>
#include <QDebug>
#include "mainwindow.h"

// single global object used throughout the whole program
// single window is reused instead of creating new window for different guides/tutorials/info-pupup-windows
Tutorial *tutorial = nullptr;


Tutorial::Tutorial(const QList<QPair<QString, QString>> slides_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tutorial)
{
    ui->setupUi(this);
    setSlides(slides_);
    ui->checkBox_dont_show_again->show();
}

Tutorial::~Tutorial()
{
    delete ui;
}

/*
static QList<QPair<QString, QString>> slides = {
    //{"test.gif", "Testing description"},            // test.gif is located in: resources/tutorial/
    //{"resizable_window.gif", "Another description"},
    //{"book pic.jpg", "test"}
    {"", "This program is designed to help with understanding graph algorithms. It allows to visualise what is going on at each step of an algorithm, showing "},
    {"authors.png", "2 of the minimum cost spanning tree algorithms presented in this program are Kruskal's algorithm developed by Joseph Kruskal (left), and Prim's algorithm, initially developed by Vojtech Jarnik and later rediscovered by Robert Clay Prim (right)."}
};*/

void Tutorial::on_pushButton_next_clicked()
{
    static int current_slide = 0;
    if (++current_slide >= slides.size()) {
        qDebug() << "Provided slide number is higher than the number of prepared slides. (slide number = " << current_slide << ")";
        current_slide %= slides.size();
    }
    setCurrentSlide(current_slide);

}

void Tutorial::setCurrentSlide(int num) {
    // cleanup old gif
    if (QMovie *old_movie = ui->label_gifholder->movie()) delete old_movie;

    QString gif_name = slides[num].first;
    QString description = slides[num].second;

    if (gif_name.isEmpty()) {
        //ui->label_gifholder->setVisible(false);
        ui->label_gifholder->setHidden(true);
    } else {
        ui->label_gifholder->setHidden(false);

        QMovie *movie = new QMovie(":/resources/tutorial/" + gif_name); //QMovie("resources/tutorial/" + gif_name);
        movie->setScaledSize(ui->label_gifholder->size());

        if (!movie->isValid()) { qDebug() << gif_name << " gif couldn't be loaded"; return; }

        ui->label_gifholder->setMovie(movie);
        movie->start();
    }

    // set description
    ui->label_textholder->setText(description);

    // set "Next" button text
    ui->pushButton_next->setText(QString().sprintf("Next %d/%d", num+1, slides.size()));
}

void Tutorial::setSlides(const QList<QPair<QString, QString> > slides_) {
    slides = slides_;
    setCurrentSlide(0);
    ui->checkBox_dont_show_again->hide();
}

void Tutorial::on_pushButton_close_clicked()
{
    //delete this;
    hide();
}

void Tutorial::on_checkBox_dont_show_again_toggled(bool checked)
{
    settings->setValue("dont_show_tutorial_again", checked);
}

void Tutorial::on_pushButton_clicked()
{

}
