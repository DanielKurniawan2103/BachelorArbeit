#ifndef RADIO_STATION_MODEL_H
#define RADIO_STATION_MODEL_H
#include <QAbstractListModel>
#include "radio_stations.h"

class RadioMethods;

class RadioStationModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    static RadioStationModel* instance();
    enum RadioStationRoles {
        NameRole = Qt::UserRole + 1,
        SongRole,
        BandRole,
        PrimaryIdRole,
        SecondaryIdsRole,
        FrequencyRole,
        InfoRole

    };

    explicit RadioStationModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addStation(const RadioStation &station);
    void modify(const QString &name, const QString &newSong, const QString &newBand, const qlonglong &newFreq, const QString &newInfo);


signals:
    void countChanged();
    void parameterChanged(const QString &name, const QString &newSong, const QString &newBand, const qlonglong &newFreq, const QString &info);

private:

    void removeStationAfterTimeout(const QString &stationName, int timeout);
    void resetStationTimeout(const QString &stationName);
    void startStationTimeout(const QString &stationName);
    bool stationExists(const RadioStation &station) const;
    QList<RadioStation> m_stations;
    static RadioStationModel* m_instance;
    RadioMethods *m_radioMethods;

    QHash<QString, QTimer*> m_stationTimers;
};

#endif // RADIO_STATION_MODEL_H
