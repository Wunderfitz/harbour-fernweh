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

Column {
    id: ownPicturesButton
    property bool isActive: false
    width: parent.width

    IconButton {
        id: ownPicturesButtonImage
        icon.source: ownPicturesButton.isActive ? "image://theme/icon-m-file-image?" + Theme.highlightColor : "image://theme/icon-m-file-image?" + Theme.primaryColor
        height: Theme.iconSizeMedium
        width: Theme.iconSizeMedium
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        onClicked: {
            handleOwnPicturesClicked();
        }

    }

    Label {
        id: ownPicturesButtonText
        text: qsTr("Photos")
        font.pixelSize: Theme.fontSizeTiny * 4 / 5
        color: ownPicturesButton.isActive ? Theme.highlightColor : Theme.primaryColor
        truncationMode: TruncationMode.Elide
        elide: Text.ElideRight
        width: parent.width - Theme.paddingSmall
        horizontalAlignment: Text.AlignHCenter
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        MouseArea {
            anchors.fill: parent
            onClicked: handleOwnPicturesClicked();
        }
    }


}
