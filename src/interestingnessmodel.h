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

#ifndef INTERESTINGNESSMODEL_H
#define INTERESTINGNESSMODEL_H

#include <QAbstractListModel>
#include <QVariantMap>
#include <QVariantList>
#include "flickrapi.h"

class InterestingnessModel : public QAbstractListModel
{
    Q_OBJECT
public:
    InterestingnessModel(FlickrApi *flickrApi);
    ~InterestingnessModel();

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Q_INVOKABLE void update();
    Q_INVOKABLE void loadMore();

signals:
    void interestingnessStartUpdate();
    void interestingnessUpdated(int modelIndex);
    void interestingnessError(const QString &errorMessage);
    void interestingnessEndReached();
    void interestingnessAppended();

public slots:
    void handleInterestingnessSuccessful(const QVariantMap &result, const bool incrementalUpdate);
    void handleInterestingnessError(const QString &errorMessage);

private:
    QVariantList interestingness;
    QVariantList incrementalUpdateResult;
    FlickrApi *flickrApi;
    int currentPage;
    int maxPages;
};

#endif // INTERESTINGNESSMODEL_H
