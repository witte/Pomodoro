#ifndef PIESLICE_H
#define PIESLICE_H

#include <QtQuick/QQuickPaintedItem>
#include <QMouseEvent>
#include <QColor>

class PieSlice : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)
    Q_PROPERTY(bool containsMouse READ containsMouse NOTIFY containsMouseChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(qreal fromAngle READ fromAngle WRITE setFromAngle NOTIFY fromAngleChanged)
    Q_PROPERTY(qreal angleSpan READ angleSpan WRITE setAngleSpan NOTIFY angleSpanChanged)

public:
    PieSlice(QQuickItem *parent = 0);

    bool contains(const QPointF &point) const;
    bool isPressed() const { return m_pressed; }
    bool containsMouse() const { return m_containsMouse; }
    qreal angleAt(const QPointF &point) const;

    QColor color() const;
    void setColor(const QColor &color);

    qreal fromAngle() const;
    void setFromAngle(qreal angle);

    qreal angleSpan() const;
    void setAngleSpan(qreal span);

    void paint(QPainter *painter);

signals:
    void pressed(QMouseEvent *event);
    void moved(qreal deltaAngle);

    void released();
    void clicked(QMouseEvent *event);
    void canceled();
    void pressedChanged();
    void containsMouseChanged();

    void fromAngleChanged(qreal);
    void angleSpanChanged(qreal);

protected:
    void setPressed(bool pressed);
    void setContainsMouse(bool containsMouse);
    void mousePressEvent(QMouseEvent *event);
//    void positionChanged(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event);
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);
    void mouseUngrabEvent();

private:
    QColor m_color;
    qreal m_fromAngle;
    qreal m_angleSpan;

    bool m_pressed;
    QPointF m_pressPoint;
    bool m_containsMouse;

    qreal m_lastAngleMove;

    int m_iterator;
    qreal getRemainderAngle(qreal angle);
};

#endif // PIESLICE_H
