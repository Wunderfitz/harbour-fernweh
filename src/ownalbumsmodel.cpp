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

#include "ownalbumsmodel.h"

#include <QListIterator>

const char SETTINGS_CURRENT_ALBUM[] = "albums/currentId";

OwnAlbumsModel::OwnAlbumsModel(FlickrApi *flickrApi)
    : settings("harbour-fernweh", "settings")
{
    this->flickrApi = flickrApi;

    connect(flickrApi, &FlickrApi::ownPhotosetsError, this, &OwnAlbumsModel::handleOwnAlbumsError);
    connect(flickrApi, &FlickrApi::ownPhotosetsSuccessful, this, &OwnAlbumsModel::handleOwnAlbumsSuccessful);
}

OwnAlbumsModel::~OwnAlbumsModel()
{
}

int OwnAlbumsModel::rowCount(const QModelIndex &) const
{
    return ownAlbums.size();
}

QVariant OwnAlbumsModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role == Qt::DisplayRole) {
        return QVariant(ownAlbums.value(index.row()));
    }
    return QVariant();
}

bool OwnAlbumsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "OwnAlbumsModel::loadMore" << row << count;
    beginInsertRows(parent, rowCount(QModelIndex()), rowCount(QModelIndex()) + count - 1);
    ownAlbums.append(this->incrementalUpdateResult);
    endInsertRows();
    emit ownAlbumsAppended();
    return true;
}

void OwnAlbumsModel::update()
{
    qDebug() << "OwnAlbumsModel::update";
    emit ownAlbumsStartUpdate();
    flickrApi->photosetsGetList("");
}

void OwnAlbumsModel::loadMore()
{
    qDebug() << "OwnAlbumsModel::loadMore";
    emit ownAlbumsStartUpdate();
    flickrApi->photosetsGetList("", this->currentPage + 1);
}

void OwnAlbumsModel::setCurrentAlbumId(const QString &albumId)
{
    qDebug() << "OwnAlbumsModel::setCurrentPhotoId" << albumId;
    settings.setValue(SETTINGS_CURRENT_ALBUM, albumId);
}

void OwnAlbumsModel::handleOwnAlbumsSuccessful(const QVariantMap &result, const bool incrementalUpdate)
{
    qDebug() << "OwnAlbumsModel::handleOwnAlbumsSuccessful";

    this->currentPage = result.value("photosets").toMap().value("page").toInt();
    this->maxPages = result.value("photosets").toMap().value("pages").toInt();
    qDebug() << "Current page: " << this->currentPage << ", maximum pages: " << this->maxPages;
    if (incrementalUpdate) {
        qDebug() << "User wanted to load more albums";
        if (result.value("photosets").toMap().value("photoset").toList().size() > 1) {
            this->incrementalUpdateResult.clear();
            this->incrementalUpdateResult = result.value("photosets").toMap().value("photoset").toList();
            insertRows(rowCount(QModelIndex()), this->incrementalUpdateResult.size());
        } else {
            emit ownAlbumsEndReached();
        }
    } else {
        beginResetModel();
        qDebug() << "Complete update of own albums";
        if (result.isEmpty()) {
            emit ownAlbumsEndReached();
        } else {
            ownAlbums.clear();
            ownAlbums.append(result.value("photosets").toMap().value("photoset").toList());
        }
        endResetModel();

        QListIterator<QVariant> albumIterator(ownAlbums);
        int i = 0;
        int modelIndex = 0;
        QString lastAlbumId = settings.value(SETTINGS_CURRENT_ALBUM).toString();
        while (albumIterator.hasNext()) {
            QVariantMap singleAlbum = albumIterator.next().toMap();
            if (singleAlbum.value("id").toString() == lastAlbumId) {
                modelIndex = i;
            }
            i++;
        }

        emit ownAlbumsUpdated(modelIndex);
    }
}

void OwnAlbumsModel::handleOwnAlbumsError(const QString &errorMessage)
{
    emit ownAlbumsError(errorMessage);
}
