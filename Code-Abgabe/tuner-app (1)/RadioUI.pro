QT += quick

SOURCES += \
        DabTableEntry.cpp \
        http.cpp \
        json.cpp \
        main.cpp \
        radio_hal.cpp \
        radio_methods.cpp \
        radio_properties.cpp \
        radio_station_model.cpp

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources \
    bilder.qrc \
    font.qrc \
    json.qrc \
    seiten.qrc

QT += core network

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    DabTableEntry.h \
    http.h \
    json.h \
    parameters.h \
    radio_hal.h \
    radio_methods.h \
    radio_properties.h \
    radio_station_model.h \
    radio_stations.h
