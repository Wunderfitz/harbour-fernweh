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
import "../pages"
import "../js/functions.js" as Functions

ListItem {

    id: singleAlbum

    property variant albumModel;
    property string componentFontSize: ( flickrAccount.getFontSize() === "fernweh" ? Theme.fontSizeExtraSmall : Theme.fontSizeSmall) ;

    Connections {
        target: flickrAccount
        onFontSizeChanged: {
            if (fontSize === "fernweh") {
                componentFontSize = Theme.fontSizeExtraSmall;
            } else {
                componentFontSize = Theme.fontSizeSmall;
            }
        }
    }

    Connections {
        target: flickrApi
        onDownloadSuccessful: {
            if (String(downloadIds.farm) === String(albumModel.farm) &&
                String(downloadIds.server) === String(albumModel.server) &&
                String(downloadIds.id) === String(albumModel.primary) &&
                String(downloadIds.photoSize) === "n") {
                albumPicture.source = filePath;
            }
        }
        onDownloadError: {
            if (String(downloadIds.farm) === String(albumModel.farm) &&
                String(downloadIds.server) === String(albumModel.server) &&
                String(downloadIds.id) === String(albumModel.primary) &&
                String(downloadIds.photoSize) === "n") {

            }
        }
    }

    Component.onCompleted: {
        flickrApi.downloadPhoto(albumModel.farm, albumModel.server, albumModel.primary, albumModel.secret, "n")
    }

    contentHeight: albumRow.height + albumSeparator.height + 2 * Theme.paddingMedium
    contentWidth: parent.width

    onClicked: {
        pageStack.push(Qt.resolvedUrl("../pages/AlbumPhotosPage.qml"), {"albumModel": albumModel});
    }

    menu: ContextMenu {
        MenuItem {
            onClicked: {
                Clipboard.text = Functions.getAlbumUrl(albumModel);
            }
            text: qsTr("Copy URL to Clipboard")
        }
    }

    Column {
        id: albumColumn
        width: parent.width - ( 2 * Theme.horizontalPageMargin )
        spacing: Theme.paddingSmall
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        Row {
            id: albumRow
            width: parent.width
            spacing: Theme.paddingMedium

            Column {
                id: albumPictureColumn
                width: parent.width / 6
                height: parent.width / 6
                spacing: Theme.paddingSmall

                Item {
                    id: albumPictureItem
                    width: parent.width
                    height: parent.width

                    Image {
                        id: albumPicture
                        z: 42
                        width: parent.width
                        height: parent.height
                        visible: false
                        fillMode: Image.PreserveAspectCrop
                    }

                    Rectangle {
                        id: albumPictureErrorShade
                        z: 42
                        width: parent.width
                        height: parent.height
                        color: "lightgrey"
                        visible: false
                    }

                    Rectangle {
                        id: albumPictureMask
                        z: 42
                        width: parent.width
                        height: parent.height
                        color: Theme.primaryColor
                        radius: parent.width / 7
                        visible: false
                    }

                    OpacityMask {
                        id: maskedalbumPicture
                        z: 42
                        source: albumPicture.status === Image.Error ? albumPictureErrorShade : albumPicture
                        maskSource: albumPictureMask
                        anchors.fill: albumPicture
                        visible: ( albumPicture.status === Image.Ready || albumPicture.status === Image.Error ) ? true : false
                        opacity: albumPicture.status === Image.Ready ? 1 : ( albumPicture.status === Image.Error ? 0.3 : 0 )
                        Behavior on opacity { NumberAnimation {} }
                    }

                    Image {
                        id: albumPictureLoadingBackgroundImage
                        source: "../../images/background" + ( Theme.colorScheme ? "-black" : "-white" ) + ".png"
                        anchors {
                            centerIn: parent
                        }
                        width: parent.width - 2 * Theme.paddingSmall
                        height: parent.height - 2 * Theme.paddingSmall
                        visible: albumPicture.status !== Image.Ready

                        fillMode: Image.PreserveAspectFit
                        opacity: 0.15
                    }

                    ImageProgressIndicator {
                        image: albumPicture
                        withPercentage: false
                    }

                }
            }

            Column {
                id: albumContentColumn
                width: parent.width * 5 / 6 - Theme.horizontalPageMargin

                spacing: Theme.paddingSmall

                Text {
                    id: albumNameText
                    font.pixelSize: componentFontSize
                    font.bold: true
                    color: Theme.primaryColor
                    text: albumModel.title._content
                    textFormat: Text.StyledText
                    elide: Text.ElideRight
                    maximumLineCount: 1
                    width: parent.width
                }

                Text {
                    id: albumDescriptionText
                    text: qsTr("%1 photos").arg(albumModel.count_photos)
                    font.pixelSize: componentFontSize
                    color: Theme.primaryColor
                    wrapMode: Text.Wrap
                    width: parent.width
                    textFormat: Text.StyledText
                }

            }
        }

    }

    Separator {
        id: albumSeparator

        anchors {
            top: albumColumn.bottom
            topMargin: Theme.paddingMedium
        }

        width: parent.width
        color: Theme.primaryColor
        horizontalAlignment: Qt.AlignHCenter
    }

}
