#pragma once
#include <QQuickItem>

class PieSlice;

class Pie : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY (QQmlListProperty<PieSlice> slices READ slices)


public:
    Pie (QQuickItem* parent = nullptr);

    QQmlListProperty<PieSlice> slices();


private:
    static void append_slice (QQmlListProperty<PieSlice>* list, PieSlice* slice);

    QList<PieSlice*> m_slices;
};
