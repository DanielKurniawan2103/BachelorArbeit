#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJsonObject>
#include <QQmlContext>
#include "radio_station_model.h"
#include "radio_hal.h"
#include "http.h"

#include "radio_main_widget.h"



int main(int a_Argc, char *a_Argv[])
{
    auto service = CREATE_GEMINI_SERVICE(a_Argc, a_Argv);
    auto wOption = new RadioWidget();
    Helper::SetBackgroundColour(wOption, Colour::Type::kBackgroundDefault);

    RadioStationModel *radioStationModel = RadioStationModel::instance();
    Radio_HAL radioHal(radioStationModel);
    HttpClient httpClient;
    RadioMethods radioMethods(radioStationModel, &httpClient);

    wOption->rootContext()->setContextProperty("radioStationModel", radioStationModel);
    wOption->rootContext()->setContextProperty("radio_HAL",&radioHal);
    wOption->rootContext()->setContextProperty("radioMethods", &radioMethods);
    wOption->rootContext()->setContextProperty("radioHttp", &httpClient);
    qmlRegisterType<RadioMethods>("RADIO_METHODS", 1, 0, "RadioMethods");
    wOption->setSource(QUrl(u"qrc:/qml/main.qml"_qs));
    QObject::connect(wOption,&RadioWidget::shown,&radioHal,&Radio_HAL::onStart);
    service.addView(wOption);
    return service.exec();

}
