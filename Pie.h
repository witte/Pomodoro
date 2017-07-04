#ifndef PIE_H
#define PIE_H

#include <QQuickItem>

class PieSlice;

class Pie : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<PieSlice> slices READ slices)

public:
    Pie(QQuickItem *parent = 0);

    QQmlListProperty<PieSlice> slices();

private:
    static void append_slice(QQmlListProperty<PieSlice> *list, PieSlice *slice);

    QList<PieSlice *> m_slices;
};

#endif // PIE_H
