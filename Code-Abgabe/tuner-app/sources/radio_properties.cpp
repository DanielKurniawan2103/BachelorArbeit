#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include "radio_properties.h"

Properties Properties::fromVariant(const QVariant &variant) {
    Properties properties;
    QVariantMap map = variant.toMap();
    properties.maker = map.value("maker").toString();
    properties.product = map.value("product").toString();
    properties.version = map.value("version").toString();
    properties.serial = map.value("serial").toString();

    QVariantList supportedIdentifierTypesList = map.value("supportedIdentifierTypes").toList();
    for (const QVariant &item : supportedIdentifierTypesList) {
        properties.supportedIdentifierTypes.append(item.toUInt());
    }

    QVariantList vendorInfoList = map.value("vendorInfo").toList();
    for (const QVariant &item : vendorInfoList) {
        QVariantMap vendorMap = item.toMap();
        VendorKeyValue vendorKeyValue;
        vendorKeyValue.key = vendorMap.value("key").toString();
        vendorKeyValue.value = vendorMap.value("value").toString();
        properties.vendorInfo.append(vendorKeyValue);
    }
    return properties;
}

TunerSessionResult TunerSessionResult::fromVariant(const QVariant &variant) {
    TunerSessionResult tunerSessionResult;
    QVariantMap map = variant.toMap();
    tunerSessionResult.result = map.value("Result").toString();
    return tunerSessionResult;
}

AmFmRegionAndResult AmFmRegionAndResult::fromVariant(const QVariant &variant){
    AmFmRegionAndResult regionAndResult;
    QVariantMap map = variant.toMap();
    regionAndResult.config = AmFmRegionConfig::fromVariant(map.value("config"));
    regionAndResult.Result = map.value("result").toString();
    return regionAndResult;
}

DabConfigResult DabConfigResult::fromVariant(const QVariant &variant){
    DabConfigResult configResult;
    QVariantMap map = variant.toMap();
    QVariantList configList = map.value("config").toList();
    for (const QVariant &item : configList) {
        configResult.config.append(DabTableEntry::fromVariant(item));
    }
    configResult.result = map.value("result").toString();
    return configResult;

}

OpenSessionResult OpenSessionResult::fromVariant(const QVariant &variant){
    OpenSessionResult openSessionResult;
    QVariantMap map = variant.toMap();
    openSessionResult.result = map.value("result").toString();
    openSessionResult.session = map.value("session").toLongLong();    
    return openSessionResult;

}
