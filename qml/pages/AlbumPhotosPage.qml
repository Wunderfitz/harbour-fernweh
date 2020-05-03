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
import QtQuick 2.0
import QtGraphicalEffects 1.0
import Sailfish.Silica 1.0
import "../components"
import "../js/functions.js" as Functions

Page {
    id: friendsPage
    allowedOrientations: Orientation.All

    focus: true
    Keys.onLeftPressed: {
        pageStack.pop();
    }
    Keys.onEscapePressed: {
        pageStack.pop();
    }
    Keys.onDownPressed: {
        albumPhotosGridView.flick(0, - parent.height);
    }
    Keys.onUpPressed: {
        albumPhotosGridView.flick(0, parent.height);
    }
    Keys.onPressed: {
        if (event.key === Qt.Key_T) {
            albumPhotosGridView.scrollToTop();
            event.accepted = true;
        }
        if (event.key === Qt.Key_B) {
            albumPhotosGridView.scrollToBottom();
            event.accepted = true;
        }
        if (event.key === Qt.Key_PageDown) {
            albumPhotosGridView.flick(0, - parent.height * 2);
            event.accepted = true;
        }
        if (event.key === Qt.Key_PageUp) {
            albumPhotosGridView.flick(0, parent.height * 2);
            event.accepted = true;
        }
    }

    property variant albumPhotosModel;
    property variant albumModel;
    property bool loaded : false;

    Component.onCompleted: {
        if (!albumPhotosModel) {
            console.log("Loading photos for " + albumModel.id);
            flickrApi.photosetsGetPhotos(albumModel.id);
        } else {
            loaded = true;
        }
    }

    AppNotification {
        id: albumPhotosNotification
    }

    Connections {
        target: flickrApi
        onPhotosetsGetPhotosSuccessful: {
            if (!albumPhotosModel) {
                albumPhotosModel = result.photoset.photo;
                loaded = true;
            }
        }
        onPhotosetsGetPhotosError: {
            if (!albumPhotosModel) {
                loaded = true;
                albumPhotosNotification.show(errorMessage);
            }
        }
    }

    SilicaFlickable {
        id: albumPhotosContainer
        width: parent.width
        height: parent.height

        LoadingIndicator {
            id: albumPhotosLoadingIndicator
            visible: !loaded
            Behavior on opacity { NumberAnimation {} }
            opacity: loaded ? 0 : 1
            height: parent.height
            width: parent.width
        }

        Column {
            anchors.fill: parent

            PageHeader {
                id: albumPhotosHeader
                title: albumModel.title._content
            }

            SilicaGridView {
                id: albumPhotosGridView


                anchors.left: parent.left
                anchors.right: parent.right
                height: parent.height - albumPhotosHeader.height
                cellWidth: width / 3;
                cellHeight: width / 3;

                clip: true

                model: albumPhotosModel
                delegate: PhotoThumbnail {
                    photoData: modelData
                    width: albumPhotosGridView.cellWidth
                    height: albumPhotosGridView.cellHeight
                }
                VerticalScrollDecorator {}
            }

        }

    }
}
