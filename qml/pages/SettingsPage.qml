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
import "../js/functions.js" as Functions

Page {
    id: settingsPage
    allowedOrientations: Orientation.All

    SilicaFlickable {
        id: settingsContainer
        contentHeight: column.height
        anchors.fill: parent

        Column {
            id: column
            width: settingsPage.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Settings")
            }

            SectionHeader {
                text: qsTr("Style")
            }

            TextSwitch {
                checked: flickrAccount.getUseSwipeNavigation()
                text: qsTr("Enable swipe navigation")
                description: qsTr("Use swipe navigation to switch categories (e.g. from pictures to albums)")
                onCheckedChanged: {
                    flickrAccount.setUseSwipeNavigation(checked);
                }
            }

            ComboBox {
                id: fontSizeComboBox
                label: qsTr("Font Size")
                currentIndex: (flickrAccount.getFontSize() === "fernweh") ? 0 : 1
                description: qsTr("Choose the font size here")
                menu: ContextMenu {
                     MenuItem {
                        text: qsTr("Normal (Fernweh default)")
                     }
                     MenuItem {
                        text: qsTr("Large (Sailfish OS default)")
                     }
                    onActivated: {
                        var fontSize = ( index === 0 ? "fernweh" : "sailfish" );
                        flickrAccount.setFontSize(fontSize);
                    }
                }
            }

            SectionHeader {
                text: qsTr("Behavior")
            }

            TextSwitch {
                checked: flickrAccount.getUseOpenWith()
                text: qsTr("Open-with menu integration")
                description: qsTr("Integrate Fernweh into open-with menu of Sailfish OS")
                onCheckedChanged: {
                    flickrAccount.setUseOpenWith(checked);
                }
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

