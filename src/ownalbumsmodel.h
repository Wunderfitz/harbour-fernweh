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

#ifndef OWNALBUMSMODEL_H
#define OWNALBUMSMODEL_H

#include <QAbstractListModel>
#include <QSettings>
#include <QVariantMap>
#include <QVariantList>
#include "flickrapi.h"

class OwnAlbumsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    OwnAlbumsModel(FlickrApi *flickrApi);
    ~OwnAlbumsModel();

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Q_INVOKABLE void update();
    Q_INVOKABLE void loadMore();
    Q_INVOKABLE void setCurrentAlbumId(const QString &albumId);

signals:
    void ownAlbumsStartUpdate();
    void ownAlbumsUpdated(int modelIndex);
    void ownAlbumsError(const QString &errorMessage);
    void ownAlbumsEndReached();
    void ownAlbumsAppended();

public slots:
    void handleOwnAlbumsSuccessful(const QVariantMap &result, const bool incrementalUpdate);
    void handleOwnAlbumsError(const QString &errorMessage);

private:
    QVariantList ownAlbums;
    QVariantList incrementalUpdateResult;
    QSettings settings;
    FlickrApi *flickrApi;
    int currentPage;
    int maxPages;
};

#endif // OWNALBUMSMODEL_H
