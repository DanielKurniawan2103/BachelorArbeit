#include "radio_station_model.h"
#include "radio_methods.h"
#include <QDebug>

RadioStationModel* RadioStationModel::m_instance = nullptr;

RadioStationModel* RadioStationModel::instance() {
    if (!m_instance) {
        m_instance = new RadioStationModel();
    }
    return m_instance;
}

RadioStationModel::RadioStationModel(QObject *parent)
    : QAbstractListModel(parent), m_radioMethods(nullptr) {
}

int RadioStationModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_stations.count();
}

QVariant RadioStationModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= m_stations.count())
        return QVariant();

    const RadioStation &station = m_stations[index.row()];
    if (role == NameRole)
        return station.name();
    else if (role == BandRole)
        return station.band();
    else if (role == SongRole)
        return station.song();
    else if (role == PrimaryIdRole)
        return station.primaryId();
    else if (role == SecondaryIdsRole)
        return QVariant::fromValue(station.secondaryIds());
    else if (role == FrequencyRole)
        return station.frequency();
    else if (role == InfoRole)
        return station.info();
    return QVariant();
}

QHash<int, QByteArray> RadioStationModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SongRole] = "song";
    roles[BandRole] = "band"; 
    roles[PrimaryIdRole] = "primaryId";
    roles[SecondaryIdsRole] = "secondaryIds";
    roles[FrequencyRole] = "frequency";
    roles[InfoRole]="info";
    return roles;
}

void RadioStationModel::addStation(const RadioStation &station) {
    if (stationExists(station)) { 
        resetStationTimeout(station.name());
        return;   
    }
    RadioStation newStation = station;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_stations << newStation;
    endInsertRows();  
    emit countChanged();
    startStationTimeout(station.name());
}

void RadioStationModel::modify(const QString &name,
                               const QString &newSong,
                               const QString &newBand,
                               const qlonglong &newFreq,
                               const QString &newInfo) {
    for (int i = 0; i < m_stations.size(); ++i) {
        if (m_stations[i].name() == name) {
            m_stations[i].setSong(newSong);
            m_stations[i].setBand(newBand);
            m_stations[i].setFreq(newFreq);
            m_stations[i].setInfo(newInfo);
            QModelIndex topLeft = index(i);
            QModelIndex bottomRight = index(i);
            emit dataChanged(topLeft, bottomRight, {SongRole});
            emit parameterChanged(name, newSong, newBand, newFreq, newInfo);
            break;
        }
    }
}

bool RadioStationModel::stationExists(const RadioStation &station) const {
    for (const RadioStation &existingStation : m_stations) {
        if (existingStation.name() == station.name()){
            return true;
        }
    }
    return false;
}

void RadioStationModel::resetStationTimeout(const QString &stationName) {
    if (m_stationTimers.contains(stationName)) {
        m_stationTimers[stationName]->start(5000);
    }
}

void RadioStationModel::startStationTimeout(const QString &stationName) {
    removeStationAfterTimeout(stationName, 5000);
}

void RadioStationModel::removeStationAfterTimeout(const QString &stationName, int timeout) {
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, [this, stationName, timer]() {
        for (int i = 0; i < m_stations.size(); ++i) {
            if (m_stations[i].name() == stationName) {
                beginRemoveRows(QModelIndex(), i, i);
                m_stations.removeAt(i);
                endRemoveRows();
                emit parameterChanged(stationName, ""," ", 0, "MUTED");
                emit countChanged();
                break;
            }
        }
        timer->deleteLater();
        m_stationTimers.remove(stationName);
    });
    timer->start(timeout);
    m_stationTimers[stationName] = timer;
}


