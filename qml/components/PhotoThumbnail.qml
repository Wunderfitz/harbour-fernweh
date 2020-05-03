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
import QtQuick 2.5
import QtGraphicalEffects 1.0
import Sailfish.Silica 1.0
import "../js/functions.js" as Functions

Item {

    id: photoThumbnail

    property variant photoData;

    Connections {
        target: flickrApi
        onDownloadSuccessful: {
            if (String(downloadIds.farm) === String(photoData.farm) &&
                String(downloadIds.server) === String(photoData.server) &&
                String(downloadIds.id) === String(photoData.id) &&
                String(downloadIds.photoSize) === "n") {
                singleImage.source = filePath;
            }
        }
        onDownloadError: {
            if (String(downloadIds.farm) === String(photoData.farm) &&
                String(downloadIds.server) === String(photoData.server) &&
                String(downloadIds.id) === String(photoData.id) &&
                String(downloadIds.photoSize) === "n") {

            }
        }
    }

    Image {

        Component.onCompleted: {
            flickrApi.downloadPhoto(photoData.farm, photoData.server, photoData.id, photoData.secret, "n");
        }

        id: singleImage
        width: parent.width - Theme.paddingSmall
        height: parent.height - Theme.paddingSmall
        anchors.centerIn: parent

        fillMode: Image.PreserveAspectCrop
        autoTransform: true
        asynchronous: true
        MouseArea {
            anchors.fill: parent
            onClicked: {
                pageStack.push(Qt.resolvedUrl("../pages/ImagePage.qml"), { "photoData" : photoData });
            }
        }
    }

    Image {
        id: imageLoadingBackgroundImage
        source: "../../images/background" + ( Theme.colorScheme ? "-black" : "-white" ) + ".png"
        anchors {
            centerIn: parent
        }
        width: parent.width - 2 * Theme.paddingLarge
        height: parent.height - 2 * Theme.paddingLarge
        visible: singleImage.status !== Image.Ready

        fillMode: Image.PreserveAspectFit
        opacity: 0.15
    }

}
