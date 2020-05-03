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

#include "searchphotosmodel.h"

SearchPhotosModel::SearchPhotosModel(FlickrApi *flickrApi)
{
    this->flickrApi = flickrApi;

    connect(flickrApi, &FlickrApi::photosSearchError, this, &SearchPhotosModel::handleSearchPhotosError);
    connect(flickrApi, &FlickrApi::photosSearchSuccessful, this, &SearchPhotosModel::handleSearchPhotosSuccessful);
}

SearchPhotosModel::~SearchPhotosModel()
{
}

int SearchPhotosModel::rowCount(const QModelIndex &) const
{
    return searchResults.size();
}

QVariant SearchPhotosModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role == Qt::DisplayRole) {
        return QVariant(searchResults.value(index.row()));
    }
    return QVariant();
}

bool SearchPhotosModel::insertRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "SearchPhotosModel::loadMore" << row << count;
    beginInsertRows(parent, rowCount(QModelIndex()), rowCount(QModelIndex()) + count - 1);
    searchResults.append(this->incrementalUpdateResult);
    endInsertRows();
    emit searchPhotosAppended();
    return true;
}

void SearchPhotosModel::search(const QString &searchString)
{
    qDebug() << "SearchPhotosModel::search" << searchString;
    emit searchPhotosStartUpdate();
    this->searchString = searchString;
    if (this->searchString.isEmpty()) {
        beginResetModel();
        this->searchResults.clear();
        endResetModel();
        emit searchPhotosUpdated();
    } else {
        flickrApi->photosSearch(searchString);
    }
}

void SearchPhotosModel::loadMore()
{
    qDebug() << "SearchPhotosModel::loadMore";
    flickrApi->photosSearch(this->searchString, this->currentPage + 1);
}

void SearchPhotosModel::handleSearchPhotosSuccessful(const QVariantMap &result, const bool incrementalUpdate)
{
    qDebug() << "SearchPhotosModel::handleSearchPhotosSuccessful";

    this->currentPage = result.value("photos").toMap().value("page").toInt();
    this->maxPages = result.value("photos").toMap().value("pages").toInt();
    qDebug() << "Current page: " << this->currentPage << ", maximum pages: " << this->maxPages;
    if (incrementalUpdate) {
        qDebug() << "User wanted to search more photos";
        if (result.value("photos").toMap().value("photo").toList().size() > 1) {
            this->incrementalUpdateResult.clear();
            this->incrementalUpdateResult = result.value("photos").toMap().value("photo").toList();
            insertRows(rowCount(QModelIndex()), this->incrementalUpdateResult.size());
        } else {
            emit searchPhotosEndReached();
        }
    } else {
        beginResetModel();
        qDebug() << "Complete update of searched photos";
        if (result.isEmpty()) {
            emit searchPhotosEndReached();
        } else {
            searchResults.clear();
            searchResults.append(result.value("photos").toMap().value("photo").toList());
        }
        endResetModel();

        emit searchPhotosUpdated();
    }
}

void SearchPhotosModel::handleSearchPhotosError(const QString &errorMessage)
{
    emit searchPhotosError(errorMessage);
}

