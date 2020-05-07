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
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import "../components"
import "../js/functions.js" as Functions

Page {
    id: imagePage
    allowedOrientations: Orientation.All

    focus: true
    Keys.onLeftPressed: {
        pageStack.pop();
    }
    Keys.onEscapePressed: {
        pageStack.pop();
    }

    property variant photoData;
    property variant photoInfo;
    property string photoPageUrl;
    property string photoFavorites;
    property variant photoExif;
    property string locationString;
    property int progress: 0;

    property string imageUrl;
    property int imageWidth;
    property int imageHeight;

    property real imageSizeFactor : imageWidth / imageHeight;
    property real screenSizeFactor: imagePage.width / imagePage.height;
    property real sizingFactor    : imageSizeFactor >= screenSizeFactor ? imagePage.width / imageWidth : imagePage.height / imageHeight;

    property real previousScale : 1;
    property real centerX;
    property real centerY;
    property real oldCenterX;
    property real oldCenterY;

    Component.onCompleted: {
        flickrApi.downloadPhoto(photoData.farm, photoData.server, photoData.id, photoData.secret, "b");
        flickrApi.photosGetInfo(photoData.id);
        flickrApi.photosGetFavorites(photoData.id);
    }

    Connections {
        target: flickrApi
        onDownloadSuccessful: {
            if (String(downloadIds.farm) === String(photoData.farm) &&
                String(downloadIds.server) === String(photoData.server) &&
                String(downloadIds.id) === String(photoData.id) &&
                String(downloadIds.photoSize) === "b") {
                imagePage.imageUrl = filePath;
                imagePage.progress = 100;
                imagePage.imageWidth = downloadIds.width;
                imagePage.imageHeight = downloadIds.height;
            }
        }
        onDownloadError: {
            if (String(downloadIds.farm) === String(photoData.farm) &&
                String(downloadIds.server) === String(photoData.server) &&
                String(downloadIds.id) === String(photoData.id) &&
                String(downloadIds.photoSize) === "b") {

            }
        }
        onDownloadStatus:{
            if (String(downloadIds.farm) === String(photoData.farm) &&
                String(downloadIds.server) === String(photoData.server) &&
                String(downloadIds.id) === String(photoData.id) &&
                String(downloadIds.photoSize) === "b") {
                imagePage.progress = percentCompleted;
            }
        }
        onPhotosGetInfoSuccessful: {
            imagePage.photoInfo = result;
            if (imagePage.photoInfo.photo.urls.url) {
                imagePage.photoPageUrl = imagePage.photoInfo.photo.urls.url[0]._content;
            }
            imagePage.locationString = Functions.getLocationString(imagePage.photoInfo);
            flickrApi.photosGetExif(photoData.id, result.photo.secret);
        }
        onPhotosGetFavoritesSuccessful: {
            if (photoId === photoData.id) {
                imagePage.photoFavorites = result.photo.total;
            }
        }
        onPhotosGetExifSuccessful: {
            if (photoId === photoData.id) {
                imagePage.photoExif = result;
                exifRepeater.model = result.photo.exif;
            }
        }
    }

    SilicaFlickable {
        id: imageFlickable
        anchors.fill: parent
        clip: true
        contentWidth: imagePinchArea.width;
        contentHeight: imagePinchArea.height;

        PullDownMenu {
            visible: photoData ? true : false
            MenuItem {
                text: qsTr("Copy URL to Clipboard")
                onClicked: {
                    Clipboard.text = imagePage.photoPageUrl;
                }
                visible: imagePage.photoPageUrl
            }
            MenuItem {
                text: qsTr("Open in Browser")
                onClicked: {
                    Qt.openUrlExternally(imagePage.photoPageUrl);
                }
                visible: imagePage.photoPageUrl
            }
            MenuItem {
                text: qsTr("Go to Author's Profile")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("../pages/ProfilePage.qml"), {"userId": photoData.owner});
                }
            }
            MenuItem {
                text: qsTr("Download Photo")
                onClicked: {
                    flickrApi.copyPhotoToDownloads(photoData.farm, photoData.server, photoData.id, photoData.secret, "b");
                }
            }
        }

        transitions: Transition {
            NumberAnimation { properties: "contentX, contentY"; easing.type: Easing.Linear }
        }

        Connections {
            target: flickrApi

            onCopyToDownloadsSuccessful: {
                imageNotification.show(qsTr("Download of %1 successful.").arg(fileName), filePath);
            }

            onCopyToDownloadsError: {
                imageNotification.show(qsTr("Download failed."));
            }
        }

        AppNotification {
            id: imageNotification
        }

        PinchArea {
            id: imagePinchArea
            width:  Math.max( singleImage.width * singleImage.scale, imageFlickable.width )
            height: Math.max( singleImage.height * singleImage.scale, imageFlickable.height )

            enabled: singleImage.visible
            pinch {
                target: singleImage
                minimumScale: 1
                maximumScale: 4
            }

            onPinchUpdated: {
                imagePage.centerX = pinch.center.x;
                imagePage.centerY = pinch.center.y;
            }

            Image {
                id: singleImage
                source: imageUrl
                width: imagePage.imageWidth * imagePage.sizingFactor
                height: imagePage.imageHeight * imagePage.sizingFactor
                anchors.centerIn: parent

                fillMode: Image.PreserveAspectFit

                visible: status === Image.Ready ? true : false
                opacity: status === Image.Ready ? 1 : 0
                Behavior on opacity { NumberAnimation {} }
                onScaleChanged: {
                    var newWidth = singleImage.width * singleImage.scale;
                    var newHeight = singleImage.height * singleImage.scale;
                    var oldWidth = singleImage.width * imagePage.previousScale;
                    var oldHeight = singleImage.height * imagePage.previousScale;
                    var widthDifference = newWidth - oldWidth;
                    var heightDifference = newHeight - oldHeight;

                    if (oldWidth > imageFlickable.width || newWidth > imageFlickable.width) {
                        var xRatioNew = imagePage.centerX / newWidth;
                        var xRatioOld = imagePage.centerX / oldHeight;
                        imageFlickable.contentX = imageFlickable.contentX + ( xRatioNew * widthDifference );
                    }
                    if (oldHeight > imageFlickable.height || newHeight > imageFlickable.height) {
                        var yRatioNew = imagePage.centerY / newHeight;
                        var yRatioOld = imagePage.centerY / oldHeight;
                        imageFlickable.contentY = imageFlickable.contentY + ( yRatioNew * heightDifference );
                    }

                    imagePage.previousScale = singleImage.scale;
                    imagePage.oldCenterX = imagePage.centerX;
                    imagePage.oldCenterY = imagePage.centerY;
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                imageInformationFlickable.visible = true;
            }
        }

    }

    Flickable {
        id: imageInformationFlickable
        anchors.fill: imagePage
        visible: false
        boundsBehavior: Flickable.StopAtBounds
        contentHeight: imageInformationColumn.height

        Rectangle {
            id: imageInformationBackground
            color: "black"
            width: parent.width
            height: imageInformationColumn.height >= imagePage.height ? imageInformationColumn.height : imagePage.height
            opacity: visible ? 0.7 : 0
            Behavior on opacity { NumberAnimation {} }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    imageInformationFlickable.visible = false;
                }
            }
        }

        Column {
            id: imageInformationColumn
            spacing: Theme.paddingSmall
            width: parent.width
            opacity: imageInformationFlickable.visible ? 1 : 0
            Behavior on opacity { NumberAnimation {} }

            Label {
                id: separatorLabel
                width: parent.width
                font.pixelSize: Theme.fontSizeExtraSmall
            }
            InfoLabel {
                text: qsTr("Photo Details")
            }
            DetailItem {
                label: qsTr("Title")
                value: photoInfo && photoInfo.photo.title._content ? photoInfo.photo.title._content : qsTr("n/a")
            }
            DetailItem {
                label: qsTr("Description")
                value: photoInfo && photoInfo.photo.description._content ? photoInfo.photo.description._content : qsTr("n/a")
            }
            DetailItem {
                label: qsTr("Date Taken")
                value: photoInfo && photoInfo.photo.dates.taken ? (new Date(photoInfo.photo.dates.taken)).toLocaleString(Qt.locale(), Locale.ShortFormat) : qsTr("n/a")
            }
            DetailItem {
                label: qsTr("Views")
                value: photoInfo && photoInfo.photo.views ? (Number(photoInfo.photo.views).toLocaleString(Qt.locale(), "f", 0)) : qsTr("n/a")
            }
            DetailItem {
                label: qsTr("Favorited")
                value: photoFavorites ? (Number(photoFavorites).toLocaleString(Qt.locale(), "f", 0)) : qsTr("n/a")
            }
            DetailItem {
                label: qsTr("Author")
                value: photoInfo && photoInfo.photo.owner.username ? (photoInfo.photo.owner.username) : qsTr("n/a")
            }
            DetailItem {
                label: qsTr("License")
                value: photoInfo && photoInfo.photo.license ? Functions.getLicenseInfoById(appWindow.licenses, photoInfo.photo.license).name : qsTr("n/a")
            }
            DetailItem {
                label: qsTr("Location")
                value: imagePage.locationString ? imagePage.locationString : qsTr("n/a")
            }
            DetailItem {
                label: qsTr("Camera")
                value: photoExif && photoExif.photo.camera ? photoExif.photo.camera : qsTr("n/a")
            }
            Repeater {
                id: exifRepeater
                DetailItem {
                    label: modelData.label
                    value: modelData.clean ? modelData.clean._content : modelData.raw._content
                }
            }
        }

        VerticalScrollDecorator {}
    }

    BackgroundProgressIndicator {
        progress: imagePage.progress
        withPercentage: true
    }

}
