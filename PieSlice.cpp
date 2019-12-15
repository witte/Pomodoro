#include "PieSlice.h"

#include <QStyleHints>
#include <QGuiApplication>
#include <QPainter>
#include <QtDebug>

#include <math.h>

PieSlice::PieSlice (QQuickItem* parent) : QQuickPaintedItem (parent)
{
    setAcceptHoverEvents (true);
    setAcceptedMouseButtons (Qt::LeftButton);
}

QColor PieSlice::color() const
{
    return m_color;
}

void PieSlice::setColor (const QColor& color)
{
    if (color == m_color) return;

    m_color = color;
    update();
}

double PieSlice::fromAngle() const
{
    return m_fromAngle;
}

void PieSlice::setFromAngle (double angle)
{
    if ( (angle < -360) || (angle > 360) ) angle -= 360;
    if (qFuzzyCompare (angle, m_fromAngle)) return;

    m_fromAngle = angle;
    update();
    emit fromAngleChanged (angle);
}

double PieSlice::toAngle() const
{
    return m_toAngle;
}

void PieSlice::setToAngle (double angle)
{
    if ( (angle < -360) || (angle > 360) ) angle -= 360;
    if (qFuzzyCompare (angle, m_toAngle)) return;

    m_toAngle = angle;
    update();
    emit fromAngleChanged (angle);
}

void PieSlice::paint (QPainter* painter)
{
    QPen pen (m_color, 0.01);
    painter->setPen (pen);
    painter->setBrush ({m_color});
    painter->setRenderHints (QPainter::Antialiasing, true);
    QRectF a = boundingRect().adjusted (1, 1, -1, -1);

    double from = m_fromAngle - 90;
    double span = m_toAngle - m_fromAngle;

    if (span < 0.0) span += 360;

    painter->drawPie (a, -int (from * 16.0), -int (span * 16.0));
}

void PieSlice::setPressed (bool pressed)
{
    if (m_pressed == pressed) return;

    m_pressed = pressed;
    emit pressedChanged();
}

void PieSlice::setContainsMouse (bool containsMouse)
{
    if (m_containsMouse == containsMouse) return;

    m_containsMouse = containsMouse;
    emit containsMouseChanged();
}

bool PieSlice::contains (const QPointF& point) const
{
    if (!QQuickItem::contains (point)) return false;

    double centerPointX = width() / 2;

    double x = point.x() - centerPointX;
    double y = point.y() - centerPointX;

    double distanceFromCenter = (x*x + y*y);
    double radius = pow (centerPointX, 2);
    if (distanceFromCenter > radius) return false;

    double angle = angleAt (point);

    return ( (angle > m_fromAngle) && (angle < m_toAngle) );
}

double PieSlice::angleAt (const QPointF& point) const
{
    double centerPointX = width() / 2;

    double x = (point.x() - centerPointX);
    double y = (point.y() - centerPointX);

    double angle = (atan2 (y, x) * 180 / M_PI) + 90;
    if (angle < 0) angle += 360;

    return angle;
}

void PieSlice::mousePressEvent (QMouseEvent* event)
{
    setPressed (true);
    m_pressPoint = event->pos();
    m_lastAngleMove = angleAt (event->pos());
    emit pressed (event);
}


void PieSlice::mouseMoveEvent (QMouseEvent* event)
{
    if (!m_pressed) return;

    double eventAngle = angleAt (event->pos());

    emit moved (eventAngle);
    m_lastAngleMove = eventAngle;
}

void PieSlice::mouseReleaseEvent (QMouseEvent* event)
{
    setPressed (false);
    emit released();

    const int threshold = qApp->styleHints()->startDragDistance();
    const bool isClick = (threshold >= qAbs (event->x() - m_pressPoint.x()) &&
                          threshold >= qAbs (event->y() - m_pressPoint.y()));

    if (isClick) emit clicked (event);
}

void PieSlice::mouseUngrabEvent()
{
    setPressed (false);
    emit canceled();
}

void PieSlice::hoverEnterEvent (QHoverEvent*)
{
    setContainsMouse (true);
}

void PieSlice::hoverLeaveEvent (QHoverEvent*)
{
    setContainsMouse (false);
}


