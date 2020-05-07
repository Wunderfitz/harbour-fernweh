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
    id: profilePage
    allowedOrientations: Orientation.All

    focus: true
    Keys.onLeftPressed: {
        pageStack.pop();
    }
    Keys.onEscapePressed: {
        pageStack.pop();
    }
    Keys.onDownPressed: {
        profileEntity.scrollDown();
    }
    Keys.onUpPressed: {
        profileEntity.scrollUp();
    }
    Keys.onPressed: {
        if (event.key === Qt.Key_T) {
            profileEntity.scrollToTop();
            event.accepted = true;
        }
        if (event.key === Qt.Key_B) {
            profileEntity.scrollToBottom();
            event.accepted = true;
        }
        if (event.key === Qt.Key_PageDown) {
            profileEntity.pageDown();
            event.accepted = true;
        }
        if (event.key === Qt.Key_PageUp) {
            profileEntity.pageUp();
            event.accepted = true;
        }
    }

    property variant profileModel;
    property string userId;
    property bool loaded : false;
    property variant profileEntity;

    Component.onCompleted: {
        if (!profileModel) {
            console.log("Loading profile for " + userId);
            flickrApi.peopleGetInfo(userId);
        } else {
            if (!profileModel.created_at) {
                // For mentions we don't have the complete information...
                userId = profileModel.person.id;
                console.log("Loading complete profile for " + userId);
                twitterApi.showUser(userId);
            } else {
                profilePage.userId = profilePage.profileModel.person.id;
                loaded = true;
            }
        }
    }

    AppNotification {
        id: profileNotification
    }

    Connections {
        target: flickrApi
        onPeopleGetInfoSuccessful: {
            if (profileModel) {
                if (userId === result.person.id) {
                    profileModel = result;
                    userId = result.person.id;
                    loaded = true;
                }
            } else {
                profileModel = result;
                userId = result.person.id;
                loaded = true;
            }
        }
        onPeopleGetInfoError: {
            if (!profileModel) {
                loaded = true;
                profileNotification.show(errorMessage);
            }
        }
    }

    SilicaFlickable {
        id: profileContainer
        anchors.fill: parent

        Loader {
            id: profilePullDownLoader
            active: profilePage.loaded
            anchors.fill: parent
            sourceComponent: profilePullDownComponent
        }

        Component {
            id: profilePullDownComponent
            Item {
                id: profilePullDownContent
                PullDownMenu {
                    MenuItem {
                        onClicked: {
                            Qt.openUrlExternally(profilePage.profileModel.person.profileurl);
                        }
                        text: qsTr("Open in Browser")
                    }
                    MenuItem {
                        onClicked: {
                            Clipboard.text = profilePage.profileModel.person.profileurl;
                        }
                        text: qsTr("Copy URL to Clipboard")
                    }
                }
            }
        }

        LoadingIndicator {
            id: profileLoadingIndicator
            visible: !loaded
            Behavior on opacity { NumberAnimation {} }
            opacity: loaded ? 0 : 1
            height: parent.height
            width: parent.width
        }

        Loader {
            id: profileLoader
            active: profilePage.loaded
            anchors.fill: parent
            sourceComponent: profileComponent
        }

        Component {
            id: profileComponent
            Item {
                id: profileContent
                anchors.fill: parent
                Component.onCompleted: {
                    profilePage.profileEntity = otherProfile;
                }

                Profile {
                    id: otherProfile
                    profileModel: profilePage.profileModel
                }
            }
        }
    }
}
