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

#include "ownphotosmodel.h"

#include <QListIterator>

const char SETTINGS_CURRENT_PHOTO[] = "photos/currentId";

OwnPhotosModel::OwnPhotosModel(FlickrApi *flickrApi)
    : settings("harbour-piepmatz", "settings")
{
    this->flickrApi = flickrApi;

    connect(flickrApi, &FlickrApi::ownPhotosError, this, &OwnPhotosModel::handleOwnPhotosError);
    connect(flickrApi, &FlickrApi::ownPhotosSuccessful, this, &OwnPhotosModel::handleOwnPhotosSuccessful);
}

OwnPhotosModel::~OwnPhotosModel()
{
}

int OwnPhotosModel::rowCount(const QModelIndex &) const
{
    return ownPhotos.size();
}

QVariant OwnPhotosModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role == Qt::DisplayRole) {
        return QVariant(ownPhotos.value(index.row()));
    }
    return QVariant();
}

void OwnPhotosModel::update()
{
    qDebug() << "OwnPhotosModel::update";
    emit ownPhotosStartUpdate();
    flickrApi->peopleGetPhotos("me");
}

void OwnPhotosModel::loadMore()
{
    qDebug() << "OwnPhotosModel::loadMore";
    emit ownPhotosStartUpdate();
    QString maxId;
    // TODO: Implement paging!!
    if (!ownPhotos.isEmpty()) {
        QVariantMap lastItem = ownPhotos.last().toMap();
        maxId = lastItem.value("id_str").toString();
    }
    flickrApi->peopleGetPhotos("me");
}

void OwnPhotosModel::setCurrentPhotoId(const QString &photoId)
{
    qDebug() << "OwnPhotosModel::setCurrentPhotoId" << photoId;
    settings.setValue(SETTINGS_CURRENT_PHOTO, photoId);
}

void OwnPhotosModel::handleOwnPhotosSuccessful(const QVariantMap &result, const bool incrementalUpdate)
{
    qDebug() << "OwnPhotosModel::handleOwnPhotosSuccessful";
    beginResetModel();
    if (incrementalUpdate) {
        qDebug() << "User wanted to load more photos";
        if (result.value("photos").toMap().value("photo").toList().size() > 1) {
            QVariantList incrementalUpdateResult = result.value("photos").toMap().value("photo").toList();
            incrementalUpdateResult.removeFirst();
            ownPhotos.append(incrementalUpdateResult);
        } else {
            emit ownPhotosEndReached();
        }
    } else {
        qDebug() << "Complete update of own photos";
        if (result.isEmpty()) {
            emit ownPhotosEndReached();
        } else {
            ownPhotos.clear();
            ownPhotos.append(result.value("photos").toMap().value("photo").toList());
        }
    }
    endResetModel();

    QListIterator<QVariant> photoIterator(ownPhotos);
    int i = 0;
    int modelIndex = 0;
    QString lastPhotoId = settings.value(SETTINGS_CURRENT_PHOTO).toString();
    while (photoIterator.hasNext()) {
        QVariantMap singlePhoto = photoIterator.next().toMap();
        if (singlePhoto.value("id").toString() == lastPhotoId) {
            modelIndex = i;
        }
        i++;
    }

    emit ownPhotosUpdated(modelIndex);
}

void OwnPhotosModel::handleOwnPhotosError(const QString &errorMessage)
{
    emit ownPhotosError(errorMessage);
}

