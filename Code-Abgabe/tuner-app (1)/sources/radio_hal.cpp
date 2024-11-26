#include <QJsonObject>
#include <QStandardPaths>
#include <QFile>
#include "radio_methods.h"
#include "radio_hal.h"
#include "radio_station_model.h"

Radio_HAL::Radio_HAL(RadioStationModel *model, QObject *parent)
    : QObject(parent), m_stationModel(model),m_station("","","", QVariantMap(), QList<QVariantMap>(),0, ""),m_settings("IAV", "Radio"),m_radioMethods(model,&m_httpClient) {

    connect(&m_eventTimer, &QTimer::timeout, this, &Radio_HAL::onTimeout);
    m_eventTimer.start(1000);

    connect(&m_imageUpdateTimer, &QTimer::timeout, this, &Radio_HAL::updateCurrentImage);
        m_imageUpdateTimer.start(3000);
    m_memoryStations.resize(5);

    connect(m_stationModel, &RadioStationModel::countChanged, this, &Radio_HAL::onCountChanged);
    connect(m_stationModel, &RadioStationModel::parameterChanged, this, &Radio_HAL::onPropertiesChanged);
    loadSettings();

    HttpClient::setCallbackConnection([this](bool connection)
    {
       m_Connection = connection;
       emit connectionChanged();
    });

}
void Radio_HAL::onStart(){
    //loadSettings();
}

QString Radio_HAL::song() const {
    return m_currentStationSong;
}

void Radio_HAL::setSong(const QString &song) {
    QString songStr = song;
        if (m_currentStationSong != songStr) {
            m_currentStationSong = songStr;
            emit songChanged();
            saveSettings();
    }
}

QString Radio_HAL::band() const {
    return m_currentStationBand;
}

void Radio_HAL::setBand(const QString &band) {
    if (m_currentStationBand != band) {
        m_currentStationBand = band;
        emit bandChanged();
        saveSettings();
    }
}

QString Radio_HAL::name() const {
    return m_currentStationName;
}

void Radio_HAL::setName(const QString &name) {    
    if (m_currentStationName != name) {
        m_currentStationName = name;
        if (!m_primaryId.isEmpty() && !m_secondaryIds.isEmpty()) {
            m_radioMethods.tune(m_primaryId, m_secondaryIds);
            updateStationImage();
            updateCurrentImage();
        } else {
            qDebug() << "Primary ID or Secondary IDs are empty!";
        }
        emit nameChanged();
        saveSettings();
    }
}

void Radio_HAL::onCountChanged() {}

void Radio_HAL::loadSettings() {
    m_currentStationSong = m_settings.value("song", "").toString();
    m_currentStationBand = m_settings.value("band", "").toString();
    m_currentStationName = m_settings.value("name", "").toString();
    m_primaryId = m_settings.value("primaryId").toMap();
    QVariantList secondaryIdsList = m_settings.value("secondaryIds").toList();
    m_secondaryIds.clear();
    for (const QVariant &item : secondaryIdsList) {
            m_secondaryIds.append(item.toMap());
        }
    if (m_settings.contains("image")) {
        m_currentStationImageUrl = m_settings.value("image").toUrl();
    } else {
        m_currentStationImageUrl = QUrl(":/bilder/icons/radio.PNG");
    }
    m_freq = m_settings.value("frequency","").toDouble();
    m_info = m_settings.value("info", "").toString();
    emit imageChanged();

    for (int i = 0; i < m_memoryStations.size(); ++i) {
        m_memoryStations[i].name = m_settings.value(QString("memoryStation%1/name").arg(i)).toString();
        m_memoryStations[i].frequency = m_settings.value(QString("memoryStation%1/frequency").arg(i)).toLongLong();
        m_memoryStations[i].song = m_settings.value(QString("memoryStation%1/song").arg(i)).toString();
        m_memoryStations[i].band = m_settings.value(QString("memoryStation%1/band").arg(i)).toString();
        m_memoryStations[i].primaryId = m_settings.value(QString("memoryStation%1/primaryId").arg(i)).toMap();
        QVariantList variantList = m_settings.value(QString("memoryStation%1/secondaryIds").arg(i)).toList();
             QList<QVariantMap> secondaryIds;
            for (const QVariant &variant : variantList) {
                secondaryIds.append(variant.toMap());
            }
        m_memoryStations[i].secondaryIds = secondaryIds;
    }
}

void Radio_HAL::saveSettings() {
    m_settings.setValue("song", m_currentStationSong);
    m_settings.setValue("band", m_currentStationBand);
    m_settings.setValue("name", m_currentStationName);
    m_settings.setValue("primaryId", m_primaryId);
    QVariantList secondaryIdsList;
        for (const QVariantMap &map : m_secondaryIds) {
            secondaryIdsList.append(map);
        }
    m_settings.setValue("secondaryIds", secondaryIdsList);
    m_settings.setValue("image", m_currentStationImageUrl);
    m_settings.setValue("frequency", m_freq);
    m_settings.setValue("info", m_info);

    for (int i = 0; i < m_memoryStations.size(); ++i) {
        m_settings.setValue(QString("memoryStation%1/name").arg(i), m_memoryStations[i].name);
        m_settings.setValue(QString("memoryStation%1/frequency").arg(i), m_memoryStations[i].frequency);
        m_settings.setValue(QString("memoryStation%1/song").arg(i), m_memoryStations[i].song);
        m_settings.setValue(QString("memoryStation%1/band").arg(i), m_memoryStations[i].band);
        m_settings.setValue(QString("memoryStation%1/primaryId").arg(i), m_memoryStations[i].primaryId);

        QVariantList variantList;
        for (const QVariantMap &map : m_memoryStations[i].secondaryIds) {
            variantList.append(map);
        }
    m_settings.setValue(QString("memoryStation%1/secondaryIds").arg(i), variantList);
    }
}

void Radio_HAL::onPropertiesChanged(const QString &name, const QString &newSong, const QString &newBand, const qlonglong &newFreq, const QString &newInfo) {
    //qDebug() << "HAL updated for station:" << name << "with new song:" << newSong <<"with new Band:"<< newBand <<"and new Frequency: "<< newFreq;
    if (m_currentStationName == name) {
        setSong(newSong);
        setBand(newBand);
        setFrequency(newFreq);
        setInfo(newInfo);
    }
}

void Radio_HAL::setFrequency(qlonglong frequency) {
    if (m_freq != frequency) {
        m_freq = frequency;
        emit frequencyChanged();
    }
}

QString Radio_HAL::info() const {
    return m_info;
}

void Radio_HAL::setInfo(const QString &info) {
    if (m_info != info)
    m_info = info;
    emit infoChanged();
    saveSettings();
}

QUrl Radio_HAL::imageUrl() const {
    return m_currentStationImageUrl;
}

void Radio_HAL::setImage(const QString &imagePath) {
    QString finalImagePath = imagePath;
    if (QFile::exists(QCoreApplication::applicationDirPath() + "/radio/" + imagePath)) {
        finalImagePath = "file://" + QCoreApplication::applicationDirPath() + "/radio/" + imagePath;
    } else {
        finalImagePath = "qrc:/bilder/icons/radio.PNG";
    }
    m_currentStationImageUrl = finalImagePath;
    emit imageChanged();
}

void Radio_HAL::updateStationImage() {
    QString imagePath = m_currentStationName+".png";
    setImage(imagePath);
}

QUrl Radio_HAL::currentImageUrl() const{
    return m_currentSongUrl;
}

void Radio_HAL::setCurrentImageUrl(const QString &ImagePath){
    QString finalImagePath = ImagePath;
    m_currentSongUrl = "file://"+QCoreApplication::applicationDirPath()+"/radio/"+finalImagePath;
    emit currentImageUrlChanged();
}

void Radio_HAL::updateCurrentImage(){
    m_imageCounter++;
    QString imagePath;
    if (m_imageCounter % 2 == 0){
        imagePath = "current"+m_currentStationName+".png";
    }else{
        imagePath = "current1"+m_currentStationName+".png";
    }
    setCurrentImageUrl(imagePath);
}

void Radio_HAL::onTimeout() {
    if(!m_init){
        m_radioMethods.getProperties();
        m_radioMethods.getAmFmRegionConfig(true);
        m_radioMethods.getDabRegionConfig();
        m_radioMethods.registerAnnouncementListener();
        m_radioMethods.openSession();
        m_radioMethods.getParameters();

        if (!m_currentStationName.isEmpty()) {
            m_radioMethods.tune(m_primaryId, m_secondaryIds);
        }
        m_init =true;
    }
    m_radioMethods.startProgramListUpdates();
    m_radioMethods.getEvents();

}

void Radio_HAL::onConnectionChanged(bool connection)
{
    emit connectionChanged();
}

QVariantList Radio_HAL::memoryStations() const {
    QVariantList list;
    for (const auto &station : m_memoryStations) {
        QVariantMap map;
        map["name"] = station.name;
        map["frequency"] = station.frequency;
        map["song"] = station.song;
        map["band"] = station.band;
        map["primaryId"] = station.primaryId;
        QVariantList secondaryIdsList;
            for (const auto &secondaryId : station.secondaryIds) {
                secondaryIdsList.append(secondaryId);
            }
        map["secondaryIds"] = secondaryIdsList;

        list.append(map);
    }
    return list;
}

void Radio_HAL::setMemoryStation(int index) {
    if (index < 0 || index >= m_memoryStations.size()) return;
    m_memoryStations[index].name = m_currentStationName;
    m_memoryStations[index].frequency = m_freq;
    m_memoryStations[index].song = m_currentStationSong;
    m_memoryStations[index].band = m_currentStationBand;
    m_memoryStations[index].primaryId = m_primaryId;
    m_memoryStations[index].secondaryIds = m_secondaryIds;
    emit memoryStationsChanged();
    saveSettings();
}

void Radio_HAL::loadMemoryStation(int index) {
    if (index < 0 || index >= m_memoryStations.size()) return;
    setSong(m_memoryStations[index].song);
    setBand(m_memoryStations[index].band);
    setPrimaryId(m_memoryStations[index].primaryId);
    setSecondaryIds(m_memoryStations[index].secondaryIds);
    setName(m_memoryStations[index].name);
}



