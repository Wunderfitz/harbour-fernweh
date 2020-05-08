# Fernweh
A Flickr client for Sailfish OS

## Author
Sebastian J. Wolf [sebastian@ygriega.de](mailto:sebastian@ygriega.de)

## License
Licensed under GNU GPLv3

## Build
Simply clone this repository and use the project file `harbour-fernweh.pro` to import the sources in your SailfishOS IDE. To build and run Fernweh or an application which is based on Fernweh, you need to create the file `harbour-fernweh/src/o2/o1flickrglobals.h` and enter the required constants in the following format:
```
#ifndef O1FLICKRGLOBALS_H
#define O1FLICKRGLOBALS_H
const char FLICKR_CLIENT_KEY[] = "abcdef";
const char FLICKR_CLIENT_SECRET[] = "ghijkl";
const char FLICKR_STORE_DEFAULT_ENCRYPTION_KEY[] = "mnopqr";
#endif // O1FLICKRGLOBALS_H
```

You get the Flickr client key and client secret as soon as you've registered your own application on [https://www.flickr.com/services/apps/create/](https://www.flickr.com/services/apps/create/). The default encryption key is only used in case Fernweh is unable to determine a unique encryption key from the user's device. Under normal circumstances, Fernweh uses an encryption key which was generated automatically. This key is used to encrypt the user's generated Flickr oAuth token (not the username/password!) on the user's device. Please use a password generator to generate the default key for your application.

## Credits
This project uses
- OAuth for Qt, by Akos Polster. Available on [GitHub.com](https://github.com/pipacs/o2) - Thanks for making it available under the conditions of the BSD-2-Clause license! Details about the license of OAuth for Qt in [its license file](src/o2/LICENSE).
