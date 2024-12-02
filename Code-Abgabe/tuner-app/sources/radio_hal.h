#ifndef RADIO_HAL_H
#define RADIO_HAL_H
#include <QObject>
#include <QSettings>
#include <QJsonObject>
#include <QTimer>
#include <QPixmap>
#include <QUrl>
#include "radio_stations.h"
#include "radio_station_model.h"
#include "radio_methods.h"
#include "http.h"

class Radio_HAL : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString song READ song WRITE setSong NOTIFY songChanged)
    Q_PROPERTY(QString band READ band WRITE setBand NOTIFY bandChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariantMap primaryId READ primaryId WRITE setPrimaryId NOTIFY primaryIdChanged)
    Q_PROPERTY(QList<QVariantMap> secondaryIds READ secondaryIds WRITE setSecondaryIds NOTIFY secondaryIdsChanged)  
    Q_PROPERTY(QUrl imageUrl READ imageUrl NOTIFY imageChanged)
    Q_PROPERTY(QUrl currentImageUrl READ currentImageUrl NOTIFY currentImageUrlChanged)
    Q_PROPERTY(qlonglong frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(QString info READ info WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(QVariantList memoryStations READ memoryStations NOTIFY memoryStationsChanged)
    Q_PROPERTY(bool connection READ connection NOTIFY connectionChanged)


public:
    explicit Radio_HAL(RadioStationModel *model, QObject *parent = nullptr);

    QString song() const;
    void setSong(const QString &song);

    QString band() const;
    void setBand(const QString &band);

    QString name() const;
    void setName(const QString &name);

    QVariantMap primaryId() const { return m_primaryId; }
    void setPrimaryId(const QVariantMap &primaryId){ m_primaryId = primaryId; }

    QList<QVariantMap> secondaryIds() const{ return m_secondaryIds; }
    void setSecondaryIds(const QList<QVariantMap> &secondaryIds){m_secondaryIds = secondaryIds;}

    QUrl imageUrl() const;
    void setImage(const QString &imagePath);

    QUrl currentImageUrl() const;
    void setCurrentImageUrl(const QString &ImagePath);

    qlonglong frequency() const { return m_freq; }
    void setFrequency(qlonglong frequency);

    QString info() const;
    void setInfo(const QString &info);


    void updateStationImage();
    void updateCurrentImage();

    struct MemoryStation {
            QString name;
            QString band;
            QString song;
            qlonglong frequency;
            QVariantMap primaryId;
            QList<QVariantMap> secondaryIds;
        };

    Q_INVOKABLE void setMemoryStation(int index);
    Q_INVOKABLE void loadMemoryStation(int index);

    QVariantList memoryStations() const;

    bool connection() const
    {
        return m_Connection;
    }

signals:
    void songChanged();
    void bandChanged();
    void nameChanged();
    void primaryIdChanged();
    void secondaryIdsChanged();
    void imageChanged();
    void currentImageUrlChanged();
    void frequencyChanged();
    void infoChanged();
    void memoryStationsChanged();
    void connectionChanged();

public slots:
    void onCountChanged();
    void onPropertiesChanged(const QString &name, const QString &newSong, const QString &newBand, const qlonglong &newFreq, const QString &newInfo);//für die Aktualisierung des Texts
    void onStart();

private slots:
    void onTimeout();
    void onConnectionChanged(bool connection);

private:
    void loadSettings();
    void saveSettings();

    RadioStationModel *m_stationModel;
    RadioStation m_station;

    QString m_currentStationName;
    QString m_currentStationSong;
    QString m_currentStationBand;
    QVariantMap m_primaryId;
    QList<QVariantMap> m_secondaryIds;

    QUrl m_currentStationImageUrl;
    QUrl m_currentSongUrl;

    qlonglong m_freq;
    QString m_info;

    QSettings m_settings;
    HttpClient m_httpClient;
    RadioMethods m_radioMethods;
    QTimer m_eventTimer;
    QTimer m_imageUpdateTimer;

    bool m_init = false;
    QVector<MemoryStation> m_memoryStations;

    bool m_Connection{false};

    int m_imageCounter = 0;

};

#endif // RADIO_HAL_H
