/*
    Copyright (C) 2020 Sebastian J. Wolf

    This file is part of Fernweh.

    Fernweh is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fernweh is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fernweh. If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QScopedPointer>
#include <QQuickView>
#include <QtQml>
#include <QQmlContext>
#include <QQmlEngine>
#include <QGuiApplication>

#include "flickraccount.h"
#include "flickrapi.h"
#include "ownphotosmodel.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    QQmlContext *context = view.data()->rootContext();

    FlickrAccount flickrAccount;
    context->setContextProperty("flickrAccount", &flickrAccount);

    FlickrApi *flickrApi = flickrAccount.getFlickrApi();
    context->setContextProperty("flickrApi", flickrApi);

    OwnPhotosModel ownPhotosModel(flickrApi);
    context->setContextProperty("ownPhotosModel", &ownPhotosModel);

    view->setSource(SailfishApp::pathTo("qml/harbour-fernweh.qml"));
    view->show();
    return app->exec();
}
