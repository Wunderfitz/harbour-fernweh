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
import QtMultimedia 5.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0


import "../components"
import "../js/functions.js" as Functions

Page {
    id: overviewPage
    allowedOrientations: Orientation.All

    property bool initializationCompleted : false;
    property int activeTabId: 0;
    property variant myLoginData;
    property variant myUser;
    property variant profileEntity;

    Component.onCompleted: {
        flickrApi.testLogin();
    }

    function resetFocus() {
        searchField.focus = false;
        overviewPage.focus = true;
    }

    function hideAccountVerificationColumn() {
        accountVerificationColumn.opacity = 0;
        accountVerificationIndicator.running = false;
        accountVerificationColumn.visible = false;
    }

    function showAccountVerificationColumn() {
        accountVerificationColumn.opacity = 1;
        accountVerificationIndicator.running = true;
        accountVerificationColumn.visible = true;
    }

    function getNavigationRowSize() {
        return Theme.iconSizeMedium + Theme.fontSizeMedium + Theme.paddingMedium;
    }

    function handleOwnPicturesClicked() {
        if (overviewPage.activeTabId === 0) {
            ownPicturesListView.scrollToTop();
        } else {
            viewsSlideshow.opacity = 0;
            slideshowVisibleTimer.goToTab(0);
            openTab(0);
        }
    }

    function handleOwnAlbumsClicked() {
        if (overviewPage.activeTabId === 1) {
            ownAlbumsListView.scrollToTop();
        } else {
            viewsSlideshow.opacity = 0;
            slideshowVisibleTimer.goToTab(1);
            openTab(1);
        }
    }

    function handleTrendingClicked() {
        viewsSlideshow.opacity = 0;
        slideshowVisibleTimer.goToTab(2);
        openTab(2);
    }

    function handleSearchClicked() {
        if (overviewPage.activeTabId === 3) {
            searchResultsListView.scrollToTop();
            usersSearchResultsListView.scrollToTop();
        } else {
            viewsSlideshow.opacity = 0;
            slideshowVisibleTimer.goToTab(3);
            openTab(3);
        }
    }

    function handleProfileClicked() {
        if (overviewPage.activeTabId === 4) {
            profileEntity.scrollToTop();
        } else {
            viewsSlideshow.opacity = 0;
            slideshowVisibleTimer.goToTab(4);
            openTab(4);
        }
    }

    function openTab(tabId) {

        activeTabId = tabId;

        switch (tabId) {
        case 0:
            ownPicturesButtonPortrait.isActive = true;
            ownPicturesButtonLandscape.isActive = true;
            ownAlbumsButtonPortrait.isActive = false;
            ownAlbumsButtonLandscape.isActive = false;
            interestingnessButtonPortrait.isActive = false;
            interestingnessButtonLandscape.isActive = false;
            searchButtonPortrait.isActive = false;
            searchButtonLandscape.isActive = false;
            profileButtonPortrait.isActive = false;
            profileButtonLandscape.isActive = false;
            break;
        case 1:
            ownPicturesButtonPortrait.isActive = false;
            ownPicturesButtonLandscape.isActive = false;
            ownAlbumsButtonPortrait.isActive = true;
            ownAlbumsButtonLandscape.isActive = true;
            interestingnessButtonPortrait.isActive = false;
            interestingnessButtonLandscape.isActive = false;
            searchButtonPortrait.isActive = false;
            searchButtonLandscape.isActive = false;
            profileButtonPortrait.isActive = false;
            profileButtonLandscape.isActive = false;
            break;
        case 2:
            ownPicturesButtonPortrait.isActive = false;
            ownPicturesButtonLandscape.isActive = false;
            ownAlbumsButtonPortrait.isActive = false;
            ownAlbumsButtonLandscape.isActive = false;
            interestingnessButtonPortrait.isActive = true;
            interestingnessButtonLandscape.isActive = true;
            searchButtonPortrait.isActive = false;
            searchButtonLandscape.isActive = false;
            profileButtonPortrait.isActive = false;
            profileButtonLandscape.isActive = false;
            break;
        case 3:
            ownPicturesButtonPortrait.isActive = false;
            ownPicturesButtonLandscape.isActive = false;
            ownAlbumsButtonPortrait.isActive = false;
            ownAlbumsButtonLandscape.isActive = false;
            interestingnessButtonPortrait.isActive = false;
            interestingnessButtonLandscape.isActive = false;
            searchButtonPortrait.isActive = true;
            searchButtonLandscape.isActive = true;
            profileButtonPortrait.isActive = false;
            profileButtonLandscape.isActive = false;
            break;
        case 4:
            ownPicturesButtonPortrait.isActive = false;
            ownPicturesButtonLandscape.isActive = false;
            ownAlbumsButtonPortrait.isActive = false;
            ownAlbumsButtonLandscape.isActive = false;
            interestingnessButtonPortrait.isActive = false;
            interestingnessButtonLandscape.isActive = false;
            searchButtonPortrait.isActive = false;
            searchButtonLandscape.isActive = false;
            profileButtonPortrait.isActive = true;
            profileButtonLandscape.isActive = true;
            break;
        default:
            console.log("Some strange navigation happened!")
        }
    }

    Connections {
        target: flickrApi
        onTestLoginSuccessful: {
            if (!overviewPage.initializationCompleted) {
                overviewPage.myLoginData = result;
                hideAccountVerificationColumn();
                overviewPage.initializationCompleted = true;
                console.log("Successfully authenticated user " + result.user.username._content);
                overviewContainer.visible = true;
                overviewColumn.visible = true;
                overviewColumn.opacity = 1;
                openTab(0);
                flickrApi.peopleGetInfo(overviewPage.myLoginData.user.id);
            }
        }
        onTestLoginError: {
            if (!overviewPage.initializationCompleted) {
                hideAccountVerificationColumn();
                verificationFailedColumn.visible = true;
                verificationFailedColumn.opacity = 1;
            }
        }
        onPeopleGetInfoSuccessful: {
            if (userId === overviewPage.myLoginData.user.id) {
                ownProfileView.loaded = true;
                overviewPage.myUser = result;
                profileLoader.active = true;
            }
        }
        onPeopleGetInfoError: {
            if (userId === overviewPage.myLoginData.user.id) {
                ownProfileView.loaded = true;
                profileLoader.active = true;
            }
        }

    }

    Item {
        id: persistentNotificationItem
        enabled: false
        width: parent.width
        height: persistentNotification.height
        y: parent.height - getNavigationRowSize() - persistentNotification.height - Theme.paddingSmall
        z: 42

        AppNotificationItem {
            id: persistentNotification
            visible: persistentNotificationItem.enabled
            opacity: persistentNotificationItem.enabled ? 1 : 0
        }
    }

    Column {
        y: ( parent.height - ( accountVerificationImage.height + accountVerificationIndicator.height + accountVerificationLabel.height + ( 3 * Theme.paddingSmall ) ) ) / 2
        width: parent.width
        id: accountVerificationColumn
        spacing: Theme.paddingSmall
        Behavior on opacity { NumberAnimation {} }

        Image {
            id: accountVerificationImage
            source: "../../images/fernweh.png"
            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            fillMode: Image.PreserveAspectFit
            width: 1/2 * parent.width
        }

        InfoLabel {
            id: accountVerificationLabel
            text: qsTr("Saying hello to Flickr...")
        }

        BusyIndicator {
            id: accountVerificationIndicator
            anchors.horizontalCenter: parent.horizontalCenter
            running: true
            size: BusyIndicatorSize.Large
        }

    }

    Column {
        y: ( parent.height - ( verificationFailedImage.height + verificationFailedInfoLabel.height + verifyAgainButton.height + reauthenticateButton.height + ( 4 * Theme.paddingSmall ) ) ) / 2
        width: parent.width
        id: verificationFailedColumn
        spacing: Theme.paddingSmall
        Behavior on opacity { NumberAnimation {} }
        opacity: 0
        visible: false

        Image {
            id: verificationFailedImage
            source: "../../images/fernweh.png"
            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            fillMode: Image.PreserveAspectFit
            width: 1/2 * parent.width
        }

        InfoLabel {
            id: verificationFailedInfoLabel
            font.pixelSize: Theme.fontSizeLarge
            text: qsTr("Fernweh could not log you in!")
        }

        Button {
            id: verifyAgainButton
            text: qsTr("Try again")
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            onClicked: {
                verificationFailedColumn.visible = false;
                showAccountVerificationColumn();
                flickrApi.testLogin();
            }
        }

        Button {
            id: reauthenticateButton
            text: qsTr("Authenticate")
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            onClicked: {
                verificationFailedColumn.visible = false;
                pageStack.clear()
                pageStack.push(welcomePage)
            }
        }

    }


    SilicaFlickable {
        id: overviewContainer
        anchors.fill: parent
        visible: false
        contentHeight: parent.height
        contentWidth: parent.width

        PullDownMenu {
            MenuItem {
                text: qsTr("About Fernweh")
                onClicked: pageStack.push(Qt.resolvedUrl("../pages/AboutPage.qml"))
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("../pages/SettingsPage.qml"))
            }
        }

        Loader {
            id: pushUpMenuLoader
            active: overviewPage.isPortrait
            sourceComponent: pushUpMenuComponent
        }

        Component {
            id: pushUpMenuComponent
            PushUpMenu {
                MenuItem {
                    text: qsTr("Settings")
                    onClicked: pageStack.push(Qt.resolvedUrl("../pages/SettingsPage.qml"))
                }
                MenuItem {
                    text: qsTr("About Fernweh")
                    onClicked: pageStack.push(Qt.resolvedUrl("../pages/AboutPage.qml"))
                }
            }
        }

        Column {
            id: overviewColumn
            opacity: 0
            visible: false
            Behavior on opacity { NumberAnimation {} }
            width: parent.width
            height: parent.height

            Row {
                id: overviewRow
                width: parent.width
                height: parent.height - ( overviewPage.isLandscape ? 0 : getNavigationRowSize() )
                spacing: Theme.paddingSmall

                VisualItemModel {
                    id: viewsModel

                    Item {
                        id: ownPicturesView
                        width: viewsSlideshow.width
                        height: viewsSlideshow.height

                        property bool loaded : false;
                        property bool reloading: false;

//                        Connections {
//                            target: timelineModel
//                            onHomeTimelineStartUpdate: {
//                                ownPicturesListView.currentIndex = -1;
//                                ownPicturesListView.footer = ownPicturesFooterComponent;
//                            }

//                            onHomeTimelineUpdated: {
//                                ownPicturesListView.currentIndex = modelIndex;
//                                ownPicturesView.loaded = true;
//                                ownPicturesView.reloading = false;
//                            }
//                            onHomeTimelineError: {
//                                ownPicturesView.loaded = true;
//                                ownPicturesView.reloading = false;
//                                overviewNotification.show(errorMessage);
//                            }
//                            onHomeTimelineEndReached: {
//                                ownPicturesListView.footer = null;
//                                overviewNotification.show(qsTr("No tweets found. Follow more people to get their tweets in your timeline!"));
//                            }
//                        }

                        Column {
                            width: parent.width
                            height: ownPicturesProgressLabel.height + ownPicturesProgressIndicator.height + Theme.paddingSmall
                            visible: !ownPicturesView.loaded
                            opacity: ownPicturesView.loaded ? 0 : 1
                            id: ownPicturesProgressColumn
                            spacing: Theme.paddingSmall
                            Behavior on opacity { NumberAnimation {} }
                            anchors.verticalCenter: parent.verticalCenter

                            InfoLabel {
                                id: ownPicturesProgressLabel
                                text: qsTr("Loading own pictures...")
                            }

                            BusyIndicator {
                                id: ownPicturesProgressIndicator
                                anchors.horizontalCenter: parent.horizontalCenter
                                running: !ownPicturesView.loaded
                                size: BusyIndicatorSize.Large
                            }
                        }

                        SilicaListView {
                            id: ownPicturesListView
                            opacity: ownPicturesView.loaded ? 1 : 0
                            Behavior on opacity { NumberAnimation {} }
                            visible: ownPicturesView.loaded
                            width: parent.width
                            height: parent.height
                            contentHeight: homeTimelineTweet.height
                            clip: true

                            model: timelineModel

//                            delegate: Tweet {
//                                id: homeTimelineTweet
//                                tweetModel: display
//                                userId: overviewPage.myUser.id_str
//                            }

                            onMovementEnded: {
                                timelineModel.setCurrentTweetId(ownPicturesListView.itemAt(ownPicturesListView.contentX, ( ownPicturesListView.contentY + Math.round(overviewPage.height / 2))).tweetModel.id_str);
                            }

                            onQuickScrollAnimatingChanged: {
                                if (!quickScrollAnimating) {
                                    timelineModel.setCurrentTweetId(ownPicturesListView.itemAt(ownPicturesListView.contentX, ( ownPicturesListView.contentY + Math.round(overviewPage.height / 2))).tweetModel.id_str);
                                }
                            }

                            onCurrentIndexChanged: {
                                timelineModel.setCurrentTweetId(currentItem.tweetModel.id_str);
                            }

                            footer: ownPicturesFooterComponent;

                            VerticalScrollDecorator {}
                        }

                        Component {
                            id: ownPicturesFooterComponent
                            Item {
                                id: ownPicturesLoadMoreRow
                                width: overviewPage.width
                                height: ownPicturesLoadMoreButton.height + ( 2 * Theme.paddingLarge )
                                Button {
                                    id: ownPicturesLoadMoreButton
                                    Behavior on opacity { NumberAnimation {} }
                                    text: qsTr("Load more tweets")
                                    preferredWidth: Theme.buttonWidthLarge
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    opacity: visible ? 1 : 0
                                    onClicked: {
                                        console.log("Loading more tweets for timeline");
                                        timelineModel.loadMore();
                                        ownPicturesLoadMoreBusyIndicator.visible = true;
                                        ownPicturesLoadMoreButton.visible = false;
                                    }
                                }
                                BusyIndicator {
                                    id: ownPicturesLoadMoreBusyIndicator
                                    Behavior on opacity { NumberAnimation {} }
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    visible: false
                                    opacity: visible ? 1 : 0
                                    running: visible
                                    size: BusyIndicatorSize.Medium
                                }
                                Connections {
                                    target: timelineModel
                                    onHomeTimelineUpdated: {
                                        ownPicturesLoadMoreBusyIndicator.visible = false;
                                        ownPicturesLoadMoreButton.visible = true;
                                    }
                                }
                            }
                        }


                        LoadingIndicator {
                            id: ownPicturesLoadingIndicator
                            visible: ownPicturesView.reloading
                            Behavior on opacity { NumberAnimation {} }
                            opacity: ownPicturesView.reloading ? 1 : 0
                            height: parent.height
                            width: parent.width
                        }
                    }

                    Item {
                        id: ownAlbumsColumn
                        width: viewsSlideshow.width
                        height: viewsSlideshow.height

                        property bool updateInProgress : false;

//                        Connections {

//                            target: mentionsModel
//                            onUpdateMentionsFinished: {
//                                ownAlbumsColumn.updateInProgress = false;
//                            }
//                            onUpdateMentionsError: {
//                                ownAlbumsColumn.updateInProgress = false;
//                                overviewNotification.show(errorMessage);
//                            }
//                        }

                        SilicaListView {
                            anchors {
                                fill: parent
                            }
                            id: ownAlbumsListView

                            clip: true

                            model: mentionsModel
                            delegate: Component {
                                Loader {
                                    width: ownAlbumsListView.width
                                    property variant mentionsData: display
                                    property bool isRetweet : display.retweeted_status ? (( display.retweeted_status.user.id_str === overviewPage.myUser.id_str ) ? true : false ) : false

                                    sourceComponent: if (display.followed_at) {
                                                         mentionsData.description = qsTr("follows you now!");
                                                         return componentMentionsUser;
                                                     } else {
                                                         return componentMentionsTweet;
                                                     }
                                }
                            }

                            VerticalScrollDecorator {}
                        }

                        Column {
                            anchors {
                                fill: parent
                            }

                            id: ownAlbumsUpdateInProgressColumn
                            Behavior on opacity { NumberAnimation {} }
                            opacity: ownAlbumsColumn.updateInProgress ? 1 : 0
                            visible: ownAlbumsColumn.updateInProgress ? true : false

                            LoadingIndicator {
                                id: ownAlbumsLoadingIndicator
                                visible: ownAlbumsColumn.updateInProgress
                                Behavior on opacity { NumberAnimation {} }
                                opacity: ownAlbumsColumn.updateInProgress ? 1 : 0
                                height: parent.height
                                width: parent.width
                            }
                        }

                    }

                    Item {
                        id: interestingnessColumn
                        width: viewsSlideshow.width
                        height: viewsSlideshow.height

                        property bool updateInProgress : false;

//                        Connections {

//                            target: directMessagesModel

//                            onUpdateMessagesStarted: {
//                                interestingnessColumn.updateInProgress = true;
//                            }

//                            onUpdateMessagesFinished: {
//                                interestingnessColumn.updateInProgress = false;
//                            }

//                            onUpdateMessagesError: {
//                                interestingnessColumn.updateInProgress = false;
//                                overviewNotification.show(errorMessage);
//                            }
//                        }

                        SilicaListView {
                            anchors {
                                fill: parent
                            }
                            id: interestingnessListView

                            clip: true

                            model: directMessagesModel
                            delegate: ListItem {

                                id: messageContactItem

                                contentHeight: messageContactRow.height + messageContactSeparator.height + 2 * Theme.paddingMedium
                                contentWidth: parent.width

//                                onClicked: {
//                                    pageStack.push(Qt.resolvedUrl("../pages/ConversationPage.qml"), { "conversationModel" : display, "myUserId": overviewPage.myUser.id_str, "configuration": overviewPage.configuration });
//                                }

                            }


                            VerticalScrollDecorator {}
                        }

                        Column {
                            anchors {
                                fill: parent
                            }

                            id: interestingnessUpdateInProgressColumn
                            Behavior on opacity { NumberAnimation {} }
                            opacity: interestingnessColumn.updateInProgress ? 1 : 0
                            visible: interestingnessColumn.updateInProgress ? true : false

                            LoadingIndicator {
                                id: interestingnessLoadingIndicator
                                visible: interestingnessColumn.updateInProgress
                                Behavior on opacity { NumberAnimation {} }
                                opacity: interestingnessColumn.updateInProgress ? 1 : 0
                                height: parent.height
                                width: parent.width
                            }
                        }

                    }

                    Item {
                        id: searchColumn
                        width: viewsSlideshow.width
                        height: viewsSlideshow.height

                        property bool tweetSearchInProgress : false;
                        property bool usersSearchInProgress : false;
                        property bool tweetSearchInTransition : false;
                        property bool usersSearchInTransition : false;

                        property bool usersSearchSelected : false;

//                        Connections {
//                            target: searchModel
//                            onSearchFinished: {
//                                searchColumn.tweetSearchInProgress = false;
//                                searchColumn.tweetSearchInTransition = false;
//                                resetFocus();
//                            }
//                            onSearchError: {
//                                searchColumn.tweetSearchInProgress = false;
//                                searchColumn.tweetSearchInTransition = false;
//                                overviewNotification.show(errorMessage);
//                                resetFocus();
//                            }
//                        }

//                        Connections {
//                            target: searchUsersModel
//                            onSearchFinished: {
//                                searchColumn.usersSearchInProgress = false;
//                                searchColumn.usersSearchInTransition = false;
//                                resetFocus();
//                            }
//                            onSearchError: {
//                                searchColumn.usersSearchInProgress = false;
//                                searchColumn.usersSearchInTransition = false;
//                                overviewNotification.show(errorMessage);
//                                resetFocus();
//                            }
//                        }

//                        Connections {
//                            target: savedSearchesModel
//                            onSaveSuccessful: {
//                                overviewNotification.show(qsTr("Search query '%1' saved successfully").arg(query));
//                            }
//                            onSaveError: {
//                                overviewNotification.show(errorMessage);
//                            }
//                            onRemoveError: {
//                                overviewNotification.show(errorMessage);
//                            }
//                        }

                        Timer {
                            id: searchTimer
                            interval: 1500
                            running: false
                            repeat: false
                            onTriggered: {
                                searchColumn.tweetSearchInProgress = true;
                                searchColumn.usersSearchInProgress = true;
                                searchModel.search(searchField.text);
                                searchUsersModel.search(searchField.text);
                            }
                        }

                        Row {
                            id: searchFieldRow
                            width: parent.width
                            height: searchField.height

                            SearchField {
                                id: searchField
                                width: parent.width
                                placeholderText: qsTr("Search on Flickr...")
                                anchors {
                                    top: parent.top
                                }

                                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                                EnterKey.onClicked: {
                                    resetFocus();
                                }

                                onTextChanged: {
                                    searchColumn.tweetSearchInTransition = true;
                                    searchColumn.usersSearchInTransition = true;
                                    searchTimer.stop();
                                    searchTimer.start();
                                }
                            }
                        }

                        Row {
                            id: searchTypeRow
                            width: parent.width
                            height: Theme.fontSizeLarge + Theme.paddingMedium
                            anchors.top: searchFieldRow.bottom
                            anchors.topMargin: Theme.paddingMedium
                            opacity: ( searchColumn.usersSearchInProgress || searchColumn.tweetSearchInProgress || (searchResultsListView.count === 0 && usersSearchResultsListView.count === 0)) ? 0 : 1
                            visible: ( searchColumn.usersSearchInProgress || searchColumn.tweetSearchInProgress || (searchResultsListView.count === 0 && usersSearchResultsListView.count === 0)) ? false : true
                            Behavior on opacity { NumberAnimation {} }
                            Text {
                                id: searchTypeTweets
                                width: ( parent.width / 2 )
                                font.pixelSize: Theme.fontSizeMedium
                                font.capitalization: Font.SmallCaps
                                horizontalAlignment: Text.AlignHCenter
                                color: searchColumn.usersSearchSelected ? Theme.primaryColor : Theme.highlightColor
                                textFormat: Text.PlainText
                                anchors.top: parent.top
                                text: qsTr("Pictures")
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        if (searchColumn.usersSearchSelected) {
                                            searchColumn.usersSearchSelected = false;
                                        }
                                    }
                                }
                            }
                            Separator {
                                width: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Qt.AlignHCenter
                                anchors.top: parent.top
                                anchors.topMargin: Theme.paddingSmall
                                transform: Rotation { angle: 90 }
                            }
                            Text {
                                id: searchTypeUsers
                                width: ( parent.width / 2 ) - ( 2 * Theme.fontSizeMedium )
                                font.pixelSize: Theme.fontSizeMedium
                                font.capitalization: Font.SmallCaps
                                horizontalAlignment: Text.AlignHCenter
                                color: searchColumn.usersSearchSelected ? Theme.highlightColor : Theme.primaryColor
                                textFormat: Text.PlainText
                                anchors.top: parent.top
                                text: qsTr("Users")
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        if (!searchColumn.usersSearchSelected) {
                                            searchColumn.usersSearchSelected = true;
                                        }
                                    }
                                }
                            }
                        }

                        SilicaListView {
                            anchors {
                                top: searchTypeRow.bottom
                            }
                            id: searchResultsListView
                            width: parent.width
                            height: parent.height - searchField.height - searchTypeRow.height - Theme.paddingMedium
                            anchors.horizontalCenter: parent.horizontalCenter
                            opacity: ( !searchColumn.tweetSearchInProgress && !searchColumn.usersSearchSelected ) ? 1 : 0
                            visible: ( !searchColumn.tweetSearchInProgress && !searchColumn.usersSearchSelected ) ? true : false
                            Behavior on opacity { NumberAnimation {} }

                            clip: true

                            model: searchModel
//                            delegate: Tweet {
//                                tweetModel: display
//                                userId: overviewPage.myUser.id_str
//                            }
                            VerticalScrollDecorator {}
                        }

                        SilicaListView {
                            anchors {
                                top: searchTypeRow.bottom
                            }
                            id: usersSearchResultsListView
                            width: parent.width
                            height: parent.height - searchField.height - searchTypeRow.height - Theme.paddingMedium
                            anchors.horizontalCenter: parent.horizontalCenter
                            opacity: ( !searchColumn.usersSearchInProgress && searchColumn.usersSearchSelected ) ? 1 : 0
                            visible: ( !searchColumn.usersSearchInProgress && searchColumn.usersSearchSelected ) ? true : false
                            Behavior on opacity { NumberAnimation {} }

                            clip: true

                            model: searchUsersModel
                            delegate: User {
                                userModel: display
                            }
                            VerticalScrollDecorator {}
                        }

                        Column {
                            anchors {
                                verticalCenter: parent.verticalCenter
                            }
                            width: parent.width

                            id: searchInProgressColumn
                            Behavior on opacity { NumberAnimation {} }
                            opacity: ( searchColumn.usersSearchInProgress || searchColumn.tweetSearchInProgress ) ? 1 : 0
                            visible: ( searchColumn.usersSearchInProgress || searchColumn.tweetSearchInProgress ) ? true : false

                            BusyIndicator {
                                id: searchInProgressIndicator
                                anchors.horizontalCenter: parent.horizontalCenter
                                running: ( searchColumn.usersSearchInProgress || searchColumn.tweetSearchInProgress )
                                size: BusyIndicatorSize.Medium
                            }

                            InfoLabel {
                                id: searchInProgressIndicatorLabel
                                text: qsTr("Searching...")
                                font.pixelSize: Theme.fontSizeLarge
                                width: parent.width - 2 * Theme.horizontalPageMargin
                            }
                        }

                        Column {
                            anchors {
                                verticalCenter: parent.verticalCenter
                            }
                            width: parent.width

                            id: searchNoResultsColumn
                            Behavior on opacity { NumberAnimation {} }
                            opacity: ( ((!searchColumn.usersSearchSelected && searchResultsListView.count === 0) || (searchColumn.usersSearchSelected && usersSearchResultsListView.count === 0)) && !( searchColumn.usersSearchInProgress || searchColumn.tweetSearchInProgress ) && searchField.text !== "" ) ? 1 : 0
                            visible: ( ((!searchColumn.usersSearchSelected && searchResultsListView.count === 0) || (searchColumn.usersSearchSelected && usersSearchResultsListView.count === 0)) && !( searchColumn.usersSearchInProgress || searchColumn.tweetSearchInProgress ) && searchField.text !== "" ) ? true : false

                            Image {
                                id: searchNoResultsImage
                                source: "../../images/fernweh.png"
                                anchors {
                                    horizontalCenter: parent.horizontalCenter
                                }

                                fillMode: Image.PreserveAspectFit
                                width: 1/3 * parent.width
                            }

                            InfoLabel {
                                id: searchNoResultsText
                                text: ( searchField.text === "" || searchColumn.tweetSearchInTransition || searchColumn.usersSearchInTransition ) ? qsTr("What are you looking for?") : qsTr("No results found")
                                color: Theme.primaryColor
                                font.pixelSize: Theme.fontSizeLarge
                                width: parent.width - 2 * Theme.horizontalPageMargin
                            }
                        }

                    }

                    Item {
                        id: ownProfileView
                        width: viewsSlideshow.width
                        height: viewsSlideshow.height

                        property bool loaded : false;

                        Column {
                            width: parent.width
                            height: ownProfileProgressLabel.height + ownProfileProgressIndicator.height + Theme.paddingSmall
                            visible: !ownProfileView.loaded
                            opacity: ownProfileView.loaded ? 0 : 1
                            id: ownProfileProgressColumn
                            spacing: Theme.paddingSmall
                            Behavior on opacity { NumberAnimation {} }
                            anchors.verticalCenter: parent.verticalCenter

                            InfoLabel {
                                id: ownProfileProgressLabel
                                text: qsTr("Loading profile...")
                            }

                            BusyIndicator {
                                id: ownProfileProgressIndicator
                                anchors.horizontalCenter: parent.horizontalCenter
                                running: !ownProfileView.loaded
                                size: BusyIndicatorSize.Large
                            }
                        }

                        Loader {
                            id: profileLoader
                            active: false
                            width: parent.width
                            height: parent.height
                            sourceComponent: profileComponent
                        }

                        Component {
                            id: profileComponent

                            Item {
                                id: profileContent
                                anchors.fill: parent
                                Component.onCompleted: {
                                    overviewPage.profileEntity = ownProfile;
                                }

                                Profile {
                                    id: ownProfile
                                    profileModel: overviewPage.myUser

                                    Connections {
                                        target: flickrApi
                                        onPeopleGetInfoSuccessful: {
                                            if (userId === overviewPage.myLoginData.user.id) {
                                                ownProfile.profileModel = result;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                }

                Timer {
                    id: slideshowVisibleTimer
                    property int tabId: 0
                    interval: 50
                    repeat: false
                    onTriggered: {
                        viewsSlideshow.positionViewAtIndex(tabId, PathView.SnapPosition);
                        viewsSlideshow.opacity = 1;
                    }
                    function goToTab(newTabId) {
                        tabId = newTabId;
                        start();
                    }
                }

                Connections {
                    target: flickrAccount
                    onSwipeNavigationChanged: {
                        viewsSlideshow.interactive = flickrAccount.getUseSwipeNavigation();
                    }
                }

                SlideshowView {
                    id: viewsSlideshow
                    width: parent.width - ( overviewPage.isLandscape ? getNavigationRowSize() + ( 2 * Theme.horizontalPageMargin ) : 0 )
                    height: parent.height
                    itemWidth: width
                    clip: true
                    interactive: flickrAccount.getUseSwipeNavigation()
                    model: viewsModel
                    onCurrentIndexChanged: {
                        openTab(currentIndex);
                    }
                    Behavior on opacity { NumberAnimation {} }
                    onOpacityChanged: {
                        if (opacity === 0) {
                            slideshowVisibleTimer.start();
                        }
                    }
                }

                Item {
                    id: navigationColumn
                    width: overviewPage.isLandscape ? getNavigationRowSize() + ( 2 * Theme.horizontalPageMargin ) : 0
                    height: parent.height
                    visible: overviewPage.isLandscape
                    property bool squeezed: height < ( ( Theme.iconSizeMedium + Theme.fontSizeTiny ) * 6 ) ? true : false

                    Separator {
                        id: navigatorColumnSeparator
                        width: parent.height
                        color: Theme.primaryColor
                        horizontalAlignment: Qt.AlignHCenter
                        anchors.top: parent.top
                        anchors.topMargin: Theme.paddingSmall
                        transform: Rotation { angle: 90 }
                    }

                    Column {

                        anchors.left: parent.left
                        anchors.leftMargin: Theme.paddingSmall
                        anchors.top: parent.top

                        height: parent.height
                        width: parent.width

                        Item {
                            id: homeButtonColumnLandscape
                            height: parent.height / 5
                            width: parent.width - Theme.paddingMedium
                            OwnPicturesButton {
                                id: ownPicturesButtonLandscape
                                visible: (isActive || !navigationColumn.squeezed)
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                        Item {
                            id: ownAlbumsButtonColumnLandscape
                            height: parent.height / 5
                            width: parent.width - Theme.paddingMedium
                            OwnPicturesButton {
                                id: ownAlbumsButtonLandscape
                                visible: (isActive || !navigationColumn.squeezed)
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        Item {
                            id: interestingnessButtonColumnLandscape
                            height: parent.height / 5
                            width: parent.width - Theme.paddingMedium
                            TrendingButton {
                                id: interestingnessButtonLandscape
                                visible: (isActive || !navigationColumn.squeezed)
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        Item {
                            id: searchButtonColumnLandscape
                            height: parent.height / 5
                            width: parent.width - Theme.paddingMedium
                            SearchButton {
                                id: searchButtonLandscape
                                visible: (isActive || !navigationColumn.squeezed)
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                        Item {
                            id: profileButtonColumnLandscape
                            height: parent.height / 5
                            width: parent.width - Theme.paddingMedium
                            ProfileButton {
                                id: profileButtonLandscape
                                visible: (isActive || !navigationColumn.squeezed)
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                    }

                }

            }

            Column {
                id: navigationRow
                width: parent.width
                height: overviewPage.isPortrait ? getNavigationRowSize() : 0
                visible: overviewPage.isPortrait
                Column {
                    id: navigationRowSeparatorColumn
                    width: parent.width
                    height: Theme.paddingMedium
                    Separator {
                        id: navigationRowSeparator
                        width: parent.width
                        color: Theme.primaryColor
                        horizontalAlignment: Qt.AlignHCenter
                    }
                }

                Row {
                    y: Theme.paddingSmall
                    width: parent.width
                    Item {
                        id: homeButtonColumn
                        width: parent.width / 5
                        height: parent.height - Theme.paddingMedium
                        OwnPicturesButton {
                            id: ownPicturesButtonPortrait
                            anchors.top: parent.top
                        }
                    }

                    Item {
                        id: ownAlbumsButtonColumn
                        width: parent.width / 5
                        height: parent.height - navigationRowSeparator.height
                        OwnAlbumsButton {
                            id: ownAlbumsButtonPortrait
                            anchors.top: parent.top
                        }
                    }
                    Item {
                        id: interestingnessButtonColumn
                        width: parent.width / 5
                        height: parent.height - navigationRowSeparator.height
                        TrendingButton {
                            id: interestingnessButtonPortrait
                            anchors.top: parent.top
                        }
                    }
                    Item {
                        id: searchButtonColumn
                        width: parent.width / 5
                        height: parent.height - navigationRowSeparator.height
                        SearchButton {
                            id: searchButtonPortrait
                            anchors.top: parent.top
                        }
                    }

                    Item {
                        id: profileButtonColumn
                        width: parent.width / 5
                        height: parent.height - navigationRowSeparator.height
                        ProfileButton {
                            id: profileButtonPortrait
                            anchors.top: parent.top
                        }
                    }
                }
            }


        }

    }



}
