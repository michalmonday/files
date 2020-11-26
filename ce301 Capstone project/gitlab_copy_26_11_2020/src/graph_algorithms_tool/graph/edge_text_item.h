#ifndef EDGE_TEXT_ITEM_H
#define EDGE_TEXT_ITEM_H

#include <QGraphicsTextItem>

class Edge;

class EdgeTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    EdgeTextItem(QString text, Edge *parent = nullptr);

    void setColor(QColor c);
    QColor color() { return color_; }
    void resetColor();

signals:
    void textEntered(QString text);



    // QGraphicsItem interface
protected:
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    Edge *edge;
    QString previous_value;
    QColor color_;

};

#endif // EDGE_TEXT_ITEM_H
