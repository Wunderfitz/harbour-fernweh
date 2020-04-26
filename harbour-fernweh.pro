# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-fernweh

CONFIG += sailfishapp sailfishapp_i18n c++11

QT += core network dbus positioning sql

include(src/o2/o2.pri)

SOURCES += src/harbour-fernweh.cpp \
    src/flickraccount.cpp \
    src/flickrapi.cpp \
    src/ownphotosmodel.cpp

DISTFILES += qml/harbour-fernweh.qml \
    qml/pages/*.qml \
    qml/components/*.qml \
    qml/pages/SettingsPage.qml \
    rpm/harbour-fernweh.changes.in \
    rpm/harbour-fernweh.spec \
    rpm/harbour-fernweh.yaml \
    translations/*.ts \
    harbour-fernweh.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

gui.files = qml
gui.path = /usr/share/$${TARGET}

images.files = images
images.path = /usr/share/$${TARGET}

ICONPATH = /usr/share/icons/hicolor

86.png.path = $${ICONPATH}/86x86/apps/
86.png.files += icons/86x86/harbour-fernweh.png

108.png.path = $${ICONPATH}/108x108/apps/
108.png.files += icons/108x108/harbour-fernweh.png

128.png.path = $${ICONPATH}/128x128/apps/
128.png.files += icons/128x128/harbour-fernweh.png

172.png.path = $${ICONPATH}/172x172/apps/
172.png.files += icons/172x172/harbour-fernweh.png

256.png.path = $${ICONPATH}/256x256/apps/
256.png.files += icons/256x256/harbour-fernweh.png

fernweh.desktop.path = /usr/share/applications/
fernweh.desktop.files = harbour-fernweh.desktop

INSTALLS += 86.png 108.png 128.png 172.png 256.png \
            fernweh.desktop gui images

TRANSLATIONS += translations/harbour-fernweh-de.ts

HEADERS += \
    src/flickraccount.h \
    src/flickrapi.h \
    src/ownphotosmodel.h
