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
import "pages"

ApplicationWindow
{
    id: appWindow

    property variant licenses

    Connections {
        target: flickrApi

        onPhotosLicensesGetInfoSuccessful: {
            appWindow.licenses = result.licenses;
        }
    }

    Component {
        id: welcomePage
        WelcomePage {}
    }

    Component {
        id: overviewPage
        OverviewPage {}
    }

    initialPage: flickrAccount.isLinked() ? overviewPage : welcomePage
    cover: Qt.resolvedUrl("pages/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
}
