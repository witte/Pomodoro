#include "Pie.h"
#include "PieSlice.h"

Pie::Pie(QQuickItem *parent) : QQuickItem(parent) {}

QQmlListProperty<PieSlice> Pie::slices()
{
    return QQmlListProperty<PieSlice>(this, 0, &Pie::append_slice, 0, 0, 0);
}

void Pie::append_slice(QQmlListProperty<PieSlice> *list, PieSlice *slice)
{
    Pie *chart = qobject_cast<Pie *>(list->object);
    if (chart)
    {
        slice->setParentItem(chart);
        chart->m_slices.append(slice);
    }
}
