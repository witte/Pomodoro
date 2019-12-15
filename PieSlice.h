#pragma once

#include <QtQuick/QQuickPaintedItem>
#include <QMouseEvent>
#include <QColor>

class PieSlice : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY (bool   pressed       READ isPressed                        NOTIFY pressedChanged      )
    Q_PROPERTY (bool   containsMouse READ containsMouse                    NOTIFY containsMouseChanged)
    Q_PROPERTY (QColor color         READ color         WRITE setColor                                )

    Q_PROPERTY (double fromAngle     READ fromAngle     WRITE setFromAngle NOTIFY fromAngleChanged    )
    Q_PROPERTY (double toAngle       READ toAngle       WRITE setToAngle   NOTIFY toAngleChanged      )


public:
    PieSlice (QQuickItem* parent = nullptr);

    bool contains (const QPointF& point) const override;
    bool isPressed() const { return m_pressed; }
    bool containsMouse() const { return m_containsMouse; }
    double angleAt (const QPointF& point) const;

    QColor color() const;
    void setColor (const QColor& color);

    double fromAngle() const;
    void setFromAngle (double angle);

    double toAngle() const;
    void setToAngle (double angle);

    void paint (QPainter* painter) override;

signals:
    void pressed (QMouseEvent* event);
    void moved (double angle);

    void released();
    void clicked (QMouseEvent* event);
    void canceled();
    void pressedChanged();
    void containsMouseChanged();

    void fromAngleChanged (double);
    void toAngleChanged   (double);


protected:
    void setPressed (bool pressed);
    void setContainsMouse (bool containsMouse);

    void mousePressEvent   (QMouseEvent* event) override;
    void mouseMoveEvent    (QMouseEvent* event) override;
    void mouseReleaseEvent (QMouseEvent* event) override;
    void mouseUngrabEvent() override;

    void hoverEnterEvent (QHoverEvent*) override;
    void hoverLeaveEvent (QHoverEvent*) override;


private:
    QColor m_color;
    double m_fromAngle;
    double m_toAngle;

    bool m_pressed;
    QPointF m_pressPoint;
    bool m_containsMouse;

    double m_lastAngleMove;
};
