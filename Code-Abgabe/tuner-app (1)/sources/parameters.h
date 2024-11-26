#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <QList>
#include <QtGlobal> // For uint and quint64

class ProgramIdentifier {
public:
    uint type;
    quint64 value;

    ProgramIdentifier(uint type = 0, quint64 value = 0)
        : type(type), value(value) {}
};

class ProgramSelector {
public:
    ProgramIdentifier primaryId;
    QList<ProgramIdentifier> secondaryIds;

    ProgramSelector(const ProgramIdentifier &primaryId = ProgramIdentifier(),
                    const QList<ProgramIdentifier> &secondaryIds = QList<ProgramIdentifier>())
        : primaryId(primaryId), secondaryIds(secondaryIds) {}
};

#endif // PARAMETERS_H
