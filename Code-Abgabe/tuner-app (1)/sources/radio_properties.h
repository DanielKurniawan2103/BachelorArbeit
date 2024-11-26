#ifndef RADIO_PROPERTIES_H
#define RADIO_PROPERTIES_H

#include <QVariant>
#include <QString>
#include <QList>
#include <QMap>
#include "dab_table_entry.h"
#include "radio_hal.h"


// ############################################### getProperties() ####################################################################################

class VendorKeyValue {
public:
    QString key;
    QString value;
};

class Properties {
public:
    QString maker;
    QString product;
    QString version;
    QString serial;
    QList<quint32> supportedIdentifierTypes;
    QList<VendorKeyValue> vendorInfo;

    static Properties fromVariant(const QVariant &variant);
};

// ################################################# Tune(), Scan(), Step() ##########################################################################################

class TunerSessionResult{
public:
    QString result;

    static TunerSessionResult fromVariant(const QVariant &variant);
    };

// ################################################## GetAmFmRegionConfig() ##########################################################################
struct AmFmBandRange
{
public:
    uint lowerBound;
    uint upperBound;
    uint spacing;
    uint scanSpacing;

    static AmFmBandRange fromVariant(const QVariant &variant) {
        AmFmBandRange bandRange;
        QVariantMap map = variant.toMap();
        bandRange.lowerBound = map.value("lowerBound").toUInt();
        bandRange.upperBound = map.value("upperBound").toUInt();
        bandRange.spacing = map.value("spacing").toUInt();
        bandRange.scanSpacing = map.value("scanSpacing").toUInt();
        return bandRange;
    }

};

class AmFmRegionConfig
{
public:
    uint fmDeemphasis;
    uint fmRds;
    QList<AmFmBandRange> ranges;

    static AmFmRegionConfig fromVariant(const QVariant &variant) {
        AmFmRegionConfig config;
        QVariantMap map = variant.toMap();
        config.fmDeemphasis = map.value("fmDeemphasis").toUInt();
        config.fmRds = map.value("fmRds").toUInt();

        QVariantList rangesList = map.value("ranges").toList();
        for (const QVariant &item : rangesList) {
            config.ranges.append(AmFmBandRange::fromVariant(item));
        }
        return config;
    }
};

class AmFmRegionAndResult
{
public:
    AmFmRegionConfig config;
    QString Result;
    static AmFmRegionAndResult fromVariant(const QVariant &variant);

};

// ######################################################## getDabRegionConfig() ############################################################################

class DabConfigResult
{
public:
    QList<DabTableEntry> config;
    QString result;

    static DabConfigResult fromVariant(const QVariant &variant);
};

// ######################################################## openSession() ###################################################################################

class OpenSessionResult
{
public:
    QString result;
    qint64 session;
    static OpenSessionResult fromVariant(const QVariant &variant);

};

// ######################################################### events() ################################################################################

class Events
{
public:

};

#endif // RADIO_PROPERTIES_H
