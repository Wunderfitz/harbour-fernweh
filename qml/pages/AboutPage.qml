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

Page {
    id: aboutPage
    allowedOrientations: Orientation.All

    SilicaFlickable {
        id: aboutContainer
        contentHeight: column.height
        anchors.fill: parent

        Column {
            id: column
            width: aboutPage.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("About Fernweh")
            }

            Image {
                id: wunderfitzImage
                source: "../../images/fernweh.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }

                fillMode: Image.PreserveAspectFit
                width: 2/3 * parent.width

            }

            Label {
                text: "Fernweh 0.1.2"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeExtraLarge
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Label {
                wrapMode: Text.Wrap
                x: Theme.horizontalPageMargin
                width: parent.width - ( 2 * Theme.horizontalPageMargin )
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("A Flickr client for Sailfish OS")
                font.pixelSize: Theme.fontSizeSmall
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Label {
                text: qsTr("By Sebastian J. Wolf")
                font.pixelSize: Theme.fontSizeSmall
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                text: "<a href=\"mailto:sebastian@ygriega.de\">" + qsTr("Send E-Mail") + "</a>"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                font.pixelSize: Theme.fontSizeSmall
                linkColor: Theme.highlightColor

                onLinkActivated: Qt.openUrlExternally("mailto:sebastian@ygriega.de")
            }

            Separator {
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            Label {
                text: qsTr("Licensed under GNU GPLv3")
                font.pixelSize: Theme.fontSizeSmall
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                text: "<a href=\"https://github.com/Wunderfitz/harbour-fernweh\">" + qsTr("Sources on GitHub") + "</a>"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                font.pixelSize: Theme.fontSizeSmall
                linkColor: Theme.highlightColor

                onLinkActivated: Qt.openUrlExternally("https://github.com/Wunderfitz/harbour-fernweh")
            }

            SectionHeader {
                text: qsTr("About Flickr")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width  - ( 2 * Theme.horizontalPageMargin )
                text: qsTr("This product uses the Flickr API but is not endorsed or certified by SmugMug, Inc.")
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.Wrap
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Button {
                id: flickrTosButton
                text: qsTr("Terms of Service")
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                onClicked: {
                    Qt.openUrlExternally("https://www.flickr.com/help/terms");
                }
            }

            Button {
                id: flickrPrivacyButton
                text: qsTr("Privacy Policy")
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                onClicked: {
                    Qt.openUrlExternally("https://www.flickr.com/help/privacy")
                }
            }

            SectionHeader {
                text: qsTr("Credits")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width  - ( 2 * Theme.horizontalPageMargin )
                text: qsTr("This project uses OAuth for Qt by Akos Polster. Thanks for making it available under the conditions of the BSD-2-Clause license!")
                font.pixelSize: Theme.fontSizeExtraSmall
                wrapMode: Text.Wrap
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                text: "<a href=\"https://github.com/pipacs/o2\">" + qsTr("Open OAuth for Qt on GitHub") + "</a>"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                font.pixelSize: Theme.fontSizeSmall
                linkColor: Theme.highlightColor

                onLinkActivated: Qt.openUrlExternally("https://github.com/pipacs/o2")
            }

            Text {
                x: Theme.horizontalPageMargin
                width: parent.width  - ( 2 * Theme.horizontalPageMargin )
                text: "Includes a modified version of <pre>DetailItem.qml</pre> - originally part of the Sailfish Silica UI component package, licensed under the terms of BSD license, copyright (C) 2014-2015 Jolla Ltd., copyright (c) 2019 Open Mobile Platform LLC. Contact: <a href=\"mailto:joona.petrell@jollamobile.com\">Joona Petrell</a>, All rights reserved."
                font.pixelSize: Theme.fontSizeExtraSmall
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                linkColor: Theme.highlightColor
                onLinkActivated: Qt.openUrlExternally(link)
                wrapMode: Text.Wrap
                color: Theme.primaryColor
            }

            Label {
                id: separatorLabel
                x: Theme.horizontalPageMargin
                width: parent.width  - ( 2 * Theme.horizontalPageMargin )
                font.pixelSize: Theme.fontSizeExtraSmall
                wrapMode: Text.Wrap
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            VerticalScrollDecorator {}
        }

    }
}
