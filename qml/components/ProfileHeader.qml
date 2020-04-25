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

Item {
    id: profileHeader

    property variant profileModel;
    width: parent.width
    height: ((profilePictureColumn.y + profilePictureColumn.height) > (profileOverviewColumn.y + profileOverviewColumn.height) ? (profilePictureColumn.y + profilePictureColumn.height) : (profileOverviewColumn.y + profileOverviewColumn.height)) + Theme.paddingSmall

    Connections {
        target: flickrAccount
        onFontSizeChanged: {
            if (fontSize === "fernweh") {
                profileNameText.font.pixelSize = Theme.fontSizeMedium;
                profileScreenNameText.font.pixelSize = Theme.fontSizeSmall;
            } else {
                profileNameText.font.pixelSize = Theme.fontSizeLarge;
                profileScreenNameText.font.pixelSize = Theme.fontSizeMedium;
            }
        }
    }

    Item {
        id: profileBackgroundItem
        width: parent.width
        height: appWindow.height / 8
        Component {
            id: profileBannerComponent
            Image {
                id: profileBackgroundImage
                source: Functions.getProfileBackgroundUrl(profileModel.person)
                fillMode: Image.PreserveAspectCrop
            }
        }

        Loader {
            id: profileBannerLoader
            active: profileModel.person.nsid ? true : false
            sourceComponent: profileBannerComponent
            anchors.fill: parent
        }
    }

    Column {
        id: profilePictureColumn
        width: appWindow.width > appWindow.height ? ( appWindow.height / 3 ) : ( appWindow.width / 3 )
        height: appWindow.width > appWindow.height ? ( appWindow.height / 3 ) : ( appWindow.width / 3 )
        x: Theme.horizontalPageMargin
        anchors {
            verticalCenter: profileBackgroundItem.bottom
        }
        Item {
            id: profilePictureItem
            width: parent.width
            height: parent.height

            Image {
                id: profilePicture
                source: Functions.getProfileImageUrl(profileModel.person)
                width: parent.width - parent.width / 10
                height: parent.height - parent.height / 10
                fillMode: Image.PreserveAspectCrop
                anchors.margins: Theme.horizontalPageMargin + parent.width / 60
                anchors.centerIn: profilePictureBackground
                visible: false
            }

            Rectangle {
                id: profilePictureMask
                width: parent.width - parent.width / 10
                height: parent.height - parent.height / 10
                color: Theme.primaryColor
                radius: (parent.width - parent.width / 10) / 2
                anchors.margins: Theme.horizontalPageMargin + parent.width / 60
                anchors.centerIn: profilePictureBackground
                visible: false
            }

            OpacityMask {
                id: maskedProfilePicture
                source: profilePicture
                maskSource: profilePictureMask
                anchors.fill: profilePicture
                visible: profilePicture.status === Image.Ready ? true : false
                opacity: profilePicture.status === Image.Ready ? 1 : 0
                Behavior on opacity { NumberAnimation {} }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
//                        pageStack.push( singleImageComponent );
                    }
                }
            }

            ImageProgressIndicator {
                image: profilePicture
                withPercentage: false
            }

        }
    }

    Column {
        id: profileOverviewColumn
        width: appWindow.width > appWindow.height ? ( appWindow.height * 2 / 3 ) : ( appWindow.width * 2 / 3 )
        height: profileNameText.height + profileScreenNameText.height + ( Theme.paddingMedium )
        spacing: Theme.paddingSmall
        anchors {
            top: profileBackgroundItem.bottom
            topMargin: Theme.paddingMedium
            left: profilePictureColumn.right
            leftMargin: Theme.horizontalPageMargin
        }
        Text {
            id: profileNameText
            text: Emoji.emojify(profileModel.name, Theme.fontSizeMedium)
            font {
                pixelSize: accountModel.getFontSize() === "fernweh" ? Theme.fontSizeMedium : Theme.fontSizeLarge
                bold: true
            }
            color: Theme.primaryColor
            elide: Text.ElideRight
            textFormat: Text.StyledText
            maximumLineCount: 2
            width: parent.width - ( 2 * Theme.horizontalPageMargin ) - Theme.paddingSmall
            wrapMode: Text.Wrap
            onTruncatedChanged: {
                // There is obviously a bug in QML in truncating text with images.
                // We simply remove Emojis then...
                if (truncated) {
                    text = text.replace(/\<img [^>]+\/\>/g, "");
                }
            }
        }
        Text {
            id: profileScreenNameText
            text: qsTr("@%1").arg(profileModel.screen_name)
            font {
                pixelSize: accountModel.getFontSize() === "fernweh" ? Theme.fontSizeSmall : Theme.fontSizeMedium
                bold: true
            }
            color: Theme.primaryColor
            elide: Text.ElideRight
            width: parent.width
        }
    }


}
