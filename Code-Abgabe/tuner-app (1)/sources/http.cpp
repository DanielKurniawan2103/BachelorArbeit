#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QThread>
#include "http.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent){}

HttpClient::~HttpClient() {}

QVariant HttpClient::makeRequest(const QJsonObject &json, const QString &endPoint) {

    QVariant responseData;
    if (m_serverStrUri.isEmpty()) {
        return responseData;
    }

    QUrl url(m_serverStrUri + endPoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const int retryDelayMs = 1000;
    bool success = false;
    QString lastHttpError="";
    while (!success) {
        QNetworkReply *reply = nullptr;
        QEventLoop eventLoop;

        if (endPoint == "process") {
            QJsonDocument doc(json);
            QByteArray data = doc.toJson();
            reply = m_manager.post(request, data);
        } else if (endPoint == "events") {
            reply = m_manager.get(request);
        } else {
            return responseData;
        }

        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();


        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            if (!response.isEmpty()) {
                QJsonDocument responseDoc = QJsonDocument::fromJson(response);
                responseData = responseDoc.toVariant();
                success = true;
                m_connection = true;
                lastHttpError="";
                emit connectionChanged(m_connection);
                if (m_Callback)
                {
                    m_Callback(m_connection);
                }
            }
        } else {
            QThread::msleep(100);
            if (lastHttpError != reply->errorString()){
                //do not spam: only trace when errorString has been changed
                lastHttpError = reply->errorString();
                qDebug() << "HTTP Error:" << reply->errorString();
            }
            success = false;
            m_connection = false;
            if (m_Callback)
            {
                m_Callback(m_connection);
            }
        }
        reply->deleteLater();
    }
    return responseData;
}


QVariant HttpClient::makeRequest_noReply(const QJsonObject &json, const QString &endPoint) {
    QVariant responseData;
    if (m_serverStrUri.isEmpty()) {
        return responseData;
    }

    QUrl url(m_serverStrUri + endPoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nullptr;
    QEventLoop eventLoop;

    if (endPoint == "process") {
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();
        reply = m_manager.post(request, data);
    } else if (endPoint == "events") {
        reply = m_manager.get(request);
    } else {
        return responseData;
    }

    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        if (!response.isEmpty()) {
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            responseData = responseDoc.toVariant();
        }
    }
    reply->deleteLater();
    return responseData;
}

void HttpClient::setCallbackConnection(std::function<void (bool)> cb)
{
    m_Callback = std::move(cb);
}
