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
import Sailfish.Silica 1.0

CoverBackground {

    id: coverPage

    Image {
        id: backgroundImage
        source: "../../images/background" + ( Theme.colorScheme ? "-black" : "-white" ) + ".png"
        anchors {
            verticalCenter: parent.verticalCenter

            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium

            right: parent.right
            rightMargin: Theme.paddingMedium
        }

        fillMode: Image.PreserveAspectFit
        opacity: 0.15
    }

    Timer {
        id: searchTimer
        interval: 5000
        running: true
        repeat: true
        onTriggered: {
            coverImageSlideshow.currentIndex = coverImageSlideshow.currentIndex + 1;
        }
    }

    SlideshowView {
        id: coverImageSlideshow
        width: parent.width
        height: parent.height
        itemWidth: width
        itemHeight: height
        clip: true
        model: interestingnessModel
        Behavior on opacity { NumberAnimation {} }
        visible: count > 0
        delegate: Item {
            width: parent.width
            height: parent.height

            Connections {
                target: flickrApi
                onDownloadSuccessful: {
                    if (String(downloadIds.farm) === String(display.farm) &&
                        String(downloadIds.server) === String(display.server) &&
                        String(downloadIds.id) === String(display.id) &&
                        String(downloadIds.photoSize) === "n") {
                        coverImage.source = filePath;
                    }
                }
            }

            Image {

                Component.onCompleted: {
                    flickrApi.downloadPhoto(display.farm, display.server, display.id, display.secret, "n");
                }

                id: coverImage
                source: media_url_https
                width: parent.width
                height: parent.height
                fillMode: Image.PreserveAspectCrop
                autoTransform: true
                asynchronous: true
                visible: status === Image.Ready ? true : false
                opacity: status === Image.Ready ? 1 : 0
                Behavior on opacity { NumberAnimation {} }
            }
        }

    }

}
