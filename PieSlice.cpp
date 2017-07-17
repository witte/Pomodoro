#include "PieSlice.h"

#include <QStyleHints>
#include <QGuiApplication>
#include <QPainter>
#include <QtDebug>

PieSlice::PieSlice(QQuickItem *parent) : QQuickPaintedItem(parent),
    m_fromAngle{0},
    m_angleSpan{0},
    m_pressed{false},
    m_containsMouse{false}
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QColor PieSlice::color() const
{
    return m_color;
}

void PieSlice::setColor(const QColor &color)
{
    if (color != m_color)
    {
        m_color = color;
        update();
    }
}

qreal PieSlice::fromAngle() const
{
    return m_fromAngle;
}

void PieSlice::setFromAngle(qreal angle)
{
    if ( (angle < -360) || (angle > 360) ) angle -= 360;

    if (angle != m_fromAngle)
    {
        m_fromAngle = angle;
        update();
        emit fromAngleChanged(angle);
    }
}

qreal PieSlice::angleSpan() const
{
    return m_angleSpan;
}

void PieSlice::setAngleSpan(qreal angle)
{
    if (angle != m_angleSpan)
    {
        m_iterator = 0;
        while (angle < 0)    angle += 360;
        while (angle >= 360) angle -= 360;

        m_angleSpan = angle;
        update();
        emit angleSpanChanged(m_angleSpan);
    }
}

void PieSlice::paint(QPainter *painter)
{
    QPen pen(m_color, 0.01);
    painter->setPen(pen);
    painter->setBrush(QBrush(m_color));
    painter->setRenderHints(QPainter::Antialiasing, true);
    QRectF a = boundingRect().adjusted(1, 1, -1, -1);
    painter->drawPie(a, (-m_fromAngle+90) * 16, -m_angleSpan * 16);
}

void PieSlice::setPressed(bool pressed)
{
    if (m_pressed != pressed) {
        m_pressed = pressed;
        emit pressedChanged();
    }
}

void PieSlice::setContainsMouse(bool containsMouse)
{
    if (m_containsMouse != containsMouse) {
        m_containsMouse = containsMouse;
        emit containsMouseChanged();
    }
}

bool PieSlice::contains(const QPointF &point) const
{
    if (!QQuickItem::contains(point))
        return false;

    qreal centerPointX = width() / 2;
//    qreal centerPointY = height() / 2; // no need in circles

    qreal x = point.x() - centerPointX;
    qreal y = (point.y() - centerPointX);

    qreal distanceFromCenter = (x*x + y*y);
    qreal radius = pow(centerPointX, 2);
    if (distanceFromCenter > radius)
        return false;

    qreal angle = angleAt(point);

    return ( (angle > m_fromAngle) && (angle < (m_fromAngle + m_angleSpan)) );
}

qreal PieSlice::angleAt(const QPointF &point) const
{
    qreal centerPointX = width() / 2;
//    qreal centerPointY = height() / 2; // no need in circles

    qreal x = (point.x() - centerPointX);
    qreal y = (point.y() - centerPointX);

    qreal angle = (atan2 (y,x) * 180 / M_PI) + 90;
    if (angle < 0) angle += 360;

    return angle;
}

void PieSlice::mousePressEvent(QMouseEvent *event)
{
    setPressed(true);
    m_pressPoint = event->pos();
    m_lastAngleMove = angleAt(event->pos());
    emit pressed(event);
}


void PieSlice::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pressed)
    {
        qreal eventAngle = angleAt(event->pos());

        emit moved(eventAngle - m_lastAngleMove);
        m_lastAngleMove = eventAngle;
    }

// I'm leaving this here for now as a little reminder of some things I'd like to implement later on,
// like snaping and the position/angle update (which already happens right now but made on the javascript side)
//
//    const qreal oldPos = position;
//    qreal pos = positionAt(point);
//    if (snapMode == QQuickDial::SnapAlways)
//        pos = snapPosition(pos);

//    if (wrap || (!wrap && !isLargeChange(point, pos))) {
//        if (live)
//            q->setValue(valueAt(pos));
//        else
//            setPosition(pos);
//        if (!qFuzzyCompare(pos, oldPos))
//            emit q->moved();
//    }
}

void PieSlice::mouseReleaseEvent(QMouseEvent *event)
{
    setPressed(false);
    emit released();

    const int threshold = qApp->styleHints()->startDragDistance();
    const bool isClick = (threshold >= qAbs(event->x() - m_pressPoint.x()) &&
                          threshold >= qAbs(event->y() - m_pressPoint.y()));

    if (isClick)
        emit clicked(event);
}

void PieSlice::mouseUngrabEvent()
{
    setPressed(false);
    emit canceled();
}

void PieSlice::hoverEnterEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    setContainsMouse(true);
}

void PieSlice::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    setContainsMouse(false);
}


