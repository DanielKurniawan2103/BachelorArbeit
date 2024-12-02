#ifndef RADIO_METHODS_H
#define RADIO_METHODS_H

#include <QObject>
#include <QJsonObject>
#include <QMutex>
#include <QProcess>
#include "http.h"
#include "radio_station_model.h"

class RadioMethods : public QObject {
    Q_OBJECT

public:
    explicit RadioMethods(RadioStationModel *model, HttpClient *client, QObject *parent = nullptr);
    void startProgramListUpdates();
    void getProperties();
    void getAmFmRegionConfig(bool full);
    void getDabRegionConfig();
    void registerAnnouncementListener();
    void openSession();
    void getParameters();
    Q_INVOKABLE void tune(const QVariantMap &primaryId, const QList<QVariantMap> &secondaryIds);
    void getImage(int id, const QString name, bool stationImage);
    void getEvents();

signals:
    void imageReceived(const QByteArray &imageData);

private:  
    QString const m_filePath;
    QProcess m_process;
    HttpClient m_httpClient;
    QString m_errorString;
    qint64 m_sessionID;
    int m_stationID;
    int m_currentImageId;
    RadioStationModel *m_stationModel;
    QMutex m_mutex;

};

#endif // RADIO_METHODS_H
