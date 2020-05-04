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

function getProfileBackgroundUrl(userInformation) {
    var profileBackgroundUrl = "https://farm" + userInformation.iconfarm + ".staticflickr.com/" + userInformation.iconserver + "/coverphoto/" + userInformation.nsid + ".jpg";
    console.log("Using background image URL " + profileBackgroundUrl);
    return profileBackgroundUrl;
}

function getProfileImageUrl(userInformation) {
    var profileImageUrl = "https://farm" + userInformation.iconfarm + ".staticflickr.com/" + userInformation.iconserver + "/buddyicons/" + userInformation.nsid + "_r.jpg";
    console.log("Using profile image URL " + profileImageUrl);
    return profileImageUrl;
}

function getDateFromTimestamp(timestamp) {
    return new Date(timestamp * 1000);
}

function getUrlForPhoto(photo) {
    var url = "https://farm" + photo.farm + ".static.flickr.com/" + photo.server + "/" + photo.id + "_" + photo.secret + "_m.jpg";
    return url;
}

function getUrlForAlbumPhoto(album) {
    var url = "https://farm" + album.farm + ".static.flickr.com/" + album.server + "/" + album.primary + "_" + album.secret + "_m.jpg";
    return url;
}

function updateFernweh() {
    if (typeof ownPicturesView !== "undefined") {
        ownPicturesView.reloading = true;
    }
    if (typeof searchColumn !== "undefined") {
        if (searchField.text !== "") {
            searchTimer.stop();
            searchTimer.start();
        }
    }
    interestingnessModel.update();
    ownPhotosModel.update();
    ownAlbumsModel.update();
}
