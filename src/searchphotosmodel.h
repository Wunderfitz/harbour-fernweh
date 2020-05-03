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

#ifndef SEARCHPHOTOSMODEL_H
#define SEARCHPHOTOSMODEL_H

#include <QAbstractListModel>
#include <QVariantMap>
#include <QVariantList>
#include "flickrapi.h"

class SearchPhotosModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SearchPhotosModel(FlickrApi *flickrApi);
    ~SearchPhotosModel();

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Q_INVOKABLE void search(const QString &searchString);
    Q_INVOKABLE void loadMore();

signals:
    void searchPhotosStartUpdate();
    void searchPhotosUpdated();
    void searchPhotosError(const QString &errorMessage);
    void searchPhotosEndReached();
    void searchPhotosAppended();

public slots:
    void handleSearchPhotosSuccessful(const QVariantMap &result, const bool incrementalUpdate);
    void handleSearchPhotosError(const QString &errorMessage);

private:
    QVariantList searchResults;
    QVariantList incrementalUpdateResult;
    FlickrApi *flickrApi;
    int currentPage;
    int maxPages;
    QString searchString;
};

#endif // SEARCHPHOTOSMODEL_H
