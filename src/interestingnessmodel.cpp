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

#include "interestingnessmodel.h"

InterestingnessModel::InterestingnessModel(FlickrApi *flickrApi)
{
    this->flickrApi = flickrApi;

    connect(flickrApi, &FlickrApi::interestingnessGetListError, this, &InterestingnessModel::handleInterestingnessError);
    connect(flickrApi, &FlickrApi::interestingnessGetListSuccessful, this, &InterestingnessModel::handleInterestingnessSuccessful);
}

InterestingnessModel::~InterestingnessModel()
{
}

int InterestingnessModel::rowCount(const QModelIndex &) const
{
    return interestingness.size();
}

QVariant InterestingnessModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role == Qt::DisplayRole) {
        return QVariant(interestingness.value(index.row()));
    }
    return QVariant();
}

bool InterestingnessModel::insertRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "InterestingnessModel::loadMore" << row << count;
    beginInsertRows(parent, rowCount(QModelIndex()), rowCount(QModelIndex()) + count - 1);
    interestingness.append(this->incrementalUpdateResult);
    endInsertRows();
    emit interestingnessAppended();
    return true;
}

void InterestingnessModel::update()
{
    qDebug() << "InterestingnessModel::update";
    emit interestingnessStartUpdate();
    flickrApi->interestingnessGetList();
}

void InterestingnessModel::loadMore()
{
    qDebug() << "InterestingnessModel::loadMore";
    flickrApi->interestingnessGetList(this->currentPage + 1);
}

void InterestingnessModel::handleInterestingnessSuccessful(const QVariantMap &result, const bool incrementalUpdate)
{
    qDebug() << "InterestingnessModel::handleOwnAlbumsSuccessful";

    this->currentPage = result.value("photos").toMap().value("page").toInt();
    this->maxPages = result.value("photos").toMap().value("pages").toInt();
    qDebug() << "Current page: " << this->currentPage << ", maximum pages: " << this->maxPages;
    if (incrementalUpdate) {
        qDebug() << "User wanted to load more interesting photos";
        if (result.value("photos").toMap().value("photo").toList().size() > 1) {
            this->incrementalUpdateResult.clear();
            this->incrementalUpdateResult = result.value("photos").toMap().value("photo").toList();
            insertRows(rowCount(QModelIndex()), this->incrementalUpdateResult.size());
        } else {
            emit interestingnessEndReached();
        }
    } else {
        beginResetModel();
        qDebug() << "Complete update of interesting photos";
        if (result.isEmpty()) {
            emit interestingnessEndReached();
        } else {
            interestingness.clear();
            interestingness.append(result.value("photos").toMap().value("photo").toList());
        }
        endResetModel();

        emit interestingnessUpdated(0);
    }
}

void InterestingnessModel::handleInterestingnessError(const QString &errorMessage)
{
    emit interestingnessError(errorMessage);
}

