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

    id: profileItem
    height: parent.height
    width: parent.width
    visible: profileModel ? true : false
    opacity: profileModel ? 1 : 0
    Behavior on opacity { NumberAnimation {} }

    function scrollDown() {
        profileTimelineGridView.flick(0, - parent.height);
    }
    function scrollUp() {
        profileTimelineGridView.flick(0, parent.height);
    }
    function pageDown() {
        profileTimelineGridView.flick(0, - parent.height * 2);
    }
    function pageUp() {
        profileTimelineGridView.flick(0, parent.height * 2);
    }
    function scrollToTop() {
        profileTimelineGridView.scrollToTop();
    }
    function scrollToBottom() {
        profileTimelineGridView.scrollToBottom();
    }

    property variant profileModel;
    property variant profileTimeline;
    property bool loadingError : false;
    property bool ownProfile: overviewPage.myLoginData.user.id === profileModel.person.id
    property string componentFontSize: ( flickrAccount.getFontSize() === "fernweh" ? Theme.fontSizeExtraSmall : Theme.fontSizeSmall) ;
    property string iconFontSize: ( flickrAccount.getFontSize() === "fernweh" ? Theme.fontSizeSmall : Theme.fontSizeMedium) ;

    Component.onCompleted: {
        console.log("Profile component initialized for " + profileModel.person.id + ": " + profileModel.person.realname._content);
        if (overviewPage.myLoginData.user.id && profileItem.ownProfile) {
            profileTimelineLoadingIndicator.visible = false;
        }
    }

    onProfileModelChanged: {
        profileTimeline = null;
        flickrApi.peopleGetPhotos(profileModel.person.id);
    }

    AppNotification {
        id: notification
    }

    Connections {
        target: flickrAccount
        onFontSizeChanged: {
            if (fontSize === "fernweh") {
                componentFontSize = Theme.fontSizeExtraSmall;
                iconFontSize = Theme.fontSizeSmall;
            } else {
                componentFontSize = Theme.fontSizeSmall;
                iconFontSize = Theme.fontSizeMedium;
            }
        }
    }

    Connections {
        target: flickrApi
        onPeopleGetPhotosSuccessful: {
            if (profileModel.person.id === userId) {
                console.log("Timeline photos updated for user " + userId)
                profileTimeline = result.photos.photo;
                //albumPhotosGridView.model = profileTimeline;
            }
        }
        onPeopleGetPhotosError: {
            if (profileModel.person.id === userId) {
                loadingError = true;
                notification.show(errorMessage);
            }
        }
    }

    Component {
        id: profileListHeaderComponent
        Column {
            id: profileElementsColumn

            height: profilePicturesRow.height + profileHeader.height + profileItemColumn.height + ( 2 * Theme.paddingMedium )
            width: parent.width
            spacing: Theme.paddingMedium

            ProfileHeader {
                id: profileHeader
                profileModel: profileItem.profileModel
                width: parent.width
            }

            Column {
                id: profileItemColumn
                width: parent.width
                spacing: Theme.paddingMedium

                Row {
                    id: profileDetailsRow
                    spacing: Theme.paddingMedium
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    visible: profileModel.person.description._content ? true : false
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        id: profileDescriptionText
                        text: profileModel.person.description._content
                        font {
                            pixelSize: componentFontSize
                        }
                        color: Theme.primaryColor
                        wrapMode: Text.Wrap
                        width: parent.width
                        textFormat: Text.StyledText
                        onLinkActivated: {
                            Qt.openUrlExternally(link)
                        }
                        linkColor: Theme.highlightColor
                    }
                }

                Separator {
                    id: profileSeparatorTop
                    width: parent.width
                    color: Theme.primaryColor
                    horizontalAlignment: Qt.AlignHCenter
                }

                Row {
                    id: profilePicturesRow
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    spacing: Theme.paddingMedium
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        id: profilePicturesCountText
                        text: qsTr("%1 Photos").arg(Number(profileModel.person.photos.count._content).toLocaleString(Qt.locale(), "f", 0))
                        font.pixelSize: componentFontSize
                        horizontalAlignment: Text.AlignHCenter
                        color: Theme.primaryColor
                        wrapMode: Text.Wrap
                        width: parent.width
                    }
                }

                Row {
                    id: profileJoinedRow
                    spacing: Theme.paddingMedium
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        id: profileJoinedText
                        text: qsTr("Joined in %1").arg(Functions.getDateFromTimestamp(profileModel.person.photos.firstdate._content).toLocaleDateString(Qt.locale(), "MMMM yyyy"))
                        font.pixelSize: componentFontSize
                        color: Theme.primaryColor
                        wrapMode: Text.NoWrap
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        elide: Text.ElideRight
                    }
                }


                Row {
                    id: profileLocationRow
                    spacing: Theme.paddingMedium
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }

                    Row {
                        visible: profileModel.person.profileurl._content ? true : false
                        width: parent.width
                        spacing: Theme.paddingSmall
                        Image {
                            id: profileUrlImage
                            source: "image://theme/icon-m-link"
                            width: iconFontSize
                            height: iconFontSize
                        }
                        Text {
                            id: profileUrlText
                            text: profileModel.person.profileurl._content ? ("<a href=\"" + profileModel.person.profileurl._content + "\">" + profileModel.person.profileurl._content + "</a>") : ""
                            font.pixelSize: componentFontSize
                            color: Theme.primaryColor
                            wrapMode: Text.NoWrap
                            anchors.verticalCenter: parent.verticalCenter
                            onLinkActivated: Qt.openUrlExternally(profileModel.person.profileurl._content)
                            linkColor: Theme.highlightColor
                            elide: Text.ElideRight
                            width: parent.width - profileUrlImage.width - Theme.paddingSmall
                        }
                    }
                }

                Separator {
                    id: profileSeparatorBottom
                    width: parent.width
                    color: Theme.primaryColor
                    horizontalAlignment: Qt.AlignHCenter
                    visible: !profileItem.ownProfile
                }

                SectionHeader {
                    text: qsTr("Statistics")
                    visible: profileItem.ownProfile
                }

                Text {
                    id: totalViewsText
                    text: qsTr("<b>Total Views:</b> %1").arg(Number(overviewPage.myStats.stats.total.views).toLocaleString(Qt.locale(), "f", 0))
                    font.pixelSize: componentFontSize
                    color: Theme.primaryColor
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.Wrap
                    textFormat: Text.StyledText
                    visible: profileItem.ownProfile
                }

                Text {
                    id: photoViewsText
                    text: qsTr("<b>Photo Views:</b> %1").arg(Number(overviewPage.myStats.stats.photos.views).toLocaleString(Qt.locale(), "f", 0))
                    font.pixelSize: componentFontSize
                    color: Theme.primaryColor
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.Wrap
                    textFormat: Text.StyledText
                    visible: profileItem.ownProfile
                }

                Text {
                    id: albumViewsText
                    text: qsTr("<b>Album Views:</b> %1").arg(Number(overviewPage.myStats.stats.sets.views).toLocaleString(Qt.locale(), "f", 0))
                    font.pixelSize: componentFontSize
                    color: Theme.primaryColor
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.Wrap
                    textFormat: Text.StyledText
                    visible: profileItem.ownProfile
                }

                Text {
                    id: streamViewsText
                    text: qsTr("<b>Photostream Views:</b> %1").arg(Number(overviewPage.myStats.stats.photostream.views).toLocaleString(Qt.locale(), "f", 0))
                    font.pixelSize: componentFontSize
                    color: Theme.primaryColor
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.Wrap
                    textFormat: Text.StyledText
                    visible: profileItem.ownProfile
                }

                Text {
                    id: galleryViewsText
                    text: qsTr("<b>Photostream Views:</b> %1").arg(Number(overviewPage.myStats.stats.galleries.views).toLocaleString(Qt.locale(), "f", 0))
                    font.pixelSize: componentFontSize
                    color: Theme.primaryColor
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.Wrap
                    textFormat: Text.StyledText
                    visible: profileItem.ownProfile
                }

                Text {
                    id: collectionViewsText
                    text: qsTr("<b>Collection Views:</b> %1").arg(Number(overviewPage.myStats.stats.collections.views).toLocaleString(Qt.locale(), "f", 0))
                    font.pixelSize: componentFontSize
                    color: Theme.primaryColor
                    width: parent.width - ( 2 * Theme.horizontalPageMargin )
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.Wrap
                    textFormat: Text.StyledText
                    visible: profileItem.ownProfile
                }

            }

        }

    }


    Item {
        id: profileTimelineLoadingIndicator
        visible: profileTimeline || profileItem.loadingError ? false : true
        Behavior on opacity { NumberAnimation {} }
        opacity: profileTimeline ? 0 : 1

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        Rectangle {
            id: profileTimelineLoadingOverlay
            color: "black"
            opacity: 0.4
            width: parent.width
            height: parent.height
            visible: profileTimelineLoadingIndicator.visible
        }

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            running: profileTimelineLoadingIndicator.visible
            size: BusyIndicatorSize.Large
        }
    }

    SilicaGridView {
        id: albumPhotosGridView

        header: profileListHeaderComponent

        onModelChanged: {
            console.log("WAAAAH " + count);
            console.log(width + "x" + height);
        }

        anchors {
            top: parent.top
            topMargin: Theme.paddingSmall
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        cellWidth: width / 3;
        cellHeight: width / 3;
        clip: true

        model: profileTimeline
        delegate: PhotoThumbnail {
            photoData: modelData
            width: albumPhotosGridView.cellWidth
            height: albumPhotosGridView.cellHeight
        }
        VerticalScrollDecorator {}
    }



}
