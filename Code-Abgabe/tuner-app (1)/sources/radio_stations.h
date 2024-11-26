#ifndef RADIO_STATIONS_H
#define RADIO_STATIONS_H

#include <QString>
#include <QList>
#include <QVariantMap>
#include <QPixmap>

class RadioStation {
public:
    RadioStation(const QString &name, const QString &song, const QString &band, const QVariantMap &primaryId, const QList<QVariantMap> &secondaryIds, qlonglong frequency, const QString &info)
        : m_name(name), m_song(song), m_band(band), m_primaryId(primaryId), m_secondaryIds(secondaryIds), m_freq(frequency), m_info(info) {}

    QString name() const { return m_name; }
    QString song() const { return m_song; }
    QString band() const { return m_band; }

    QVariantMap primaryId() const { return m_primaryId;}
    QList<QVariantMap> secondaryIds() const { return m_secondaryIds;}

    qlonglong frequency() const {return m_freq;}
    QString info() const {return m_info; }


    void setSong(const QString &song) {
        m_song = song;
    }
    void setBand(const QString &band){
        m_band = band;
    }
    void setFreq(const qlonglong &freq){
        m_freq = freq;
    }
    void setInfo(const QString &info){
        m_info = info;
    }


private:
    QString m_name;
    QString m_song;
    QString m_band;
    QVariantMap m_primaryId;
    QList<QVariantMap> m_secondaryIds;
    qlonglong m_freq;
    QString m_info;
};

#endif // RADIO_STATIONS_H
