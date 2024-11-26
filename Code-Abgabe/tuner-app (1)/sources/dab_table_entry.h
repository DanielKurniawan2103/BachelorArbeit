#ifndef DABTABLEENTRY_H
#define DABTABLEENTRY_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QVariantMap>

struct DabTableEntry {
    QString label;
    uint frequency;

    static DabTableEntry fromVariant(const QVariant &variant) {
        DabTableEntry entry;
        QVariantMap map = variant.toMap();
        entry.label = map.value("label").toString();
        entry.frequency = map.value("frequency").toUInt();
        return entry;
    }
};

QList<DabTableEntry> fillDummyDABConfig();

#endif // DABTABLEENTRY_H
