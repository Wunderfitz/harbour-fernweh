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

#ifndef OWNPHOTOSMODEL_H
#define OWNPHOTOSMODEL_H

#include <QAbstractListModel>
#include <QSettings>
#include <QVariantMap>
#include <QVariantList>
#include "flickrapi.h"

class OwnPhotosModel : public QAbstractListModel
{
    Q_OBJECT
public:
    OwnPhotosModel(FlickrApi *flickrApi);
    ~OwnPhotosModel();

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void update();
    Q_INVOKABLE void loadMore();
    Q_INVOKABLE void setCurrentPhotoId(const QString &photoId);

signals:
    void ownPhotosStartUpdate();
    void ownPhotosUpdated(int modelIndex);
    void ownPhotosError(const QString &errorMessage);
    void ownPhotosEndReached();

public slots:
    void handleOwnPhotosSuccessful(const QVariantMap &result, const bool incrementalUpdate);
    void handleOwnPhotosError(const QString &errorMessage);

private:
    QVariantList ownPhotos;
    QSettings settings;
    FlickrApi *flickrApi;

};

#endif // OWNPHOTOSMODEL_H
