#ifndef HTTP_H
#define HTTP_H

#include <QtQml>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonArray>
#include <QEventLoop>
#include <QUrl>

#include <functional>

class HttpClient : public QObject{
    Q_OBJECT

public:
    explicit HttpClient(QObject *parent = nullptr);
    ~HttpClient();

    bool connection()const {
        return m_connection;
    };

    QVariant makeRequest(const QJsonObject &json, const QString &endPoint);
    QVariant makeRequest_noReply(const QJsonObject &json, const QString &endPoint);

    static void setCallbackConnection(std::function<void(bool)> cb);

signals:
    void connectionChanged(const bool &connection);

private:
    bool m_connection = false;
    inline static std::function<void(bool)> m_Callback{nullptr};
    QNetworkAccessManager m_manager;
    QString m_errorString;
    QString m_serverStrUri = "http://localhost:55587/"; //Host und Port
    //192.168.199.1     //Host rechner
    //172.16.250.1      //Raspi
    //192.168.1.5       //GEMINI
};

#endif // HTTP_H
