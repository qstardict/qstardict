QStarDict is a StarDict clone written using Qt. The user interface
is similar to StarDict.

### Main features ###
* Full support of StarDict dictionaries
* Working in system tray
* Scanning mouse selection and showing popup window with translation of the
  selected word
* Translations reformatting
* Pronouncing translated word
* Plugins support
* KDE 4 plasmoid

### Installation ###
QStarDict is available in Ubuntu, Debian and other distros repositories. 

If you want to compile it yourself install Qt4 and GLib2 development files and run in the project's directory

    qmake INSTALL_PREFIX=<your install prefix, /usr by default>
    make
    [sudo] make install
    
More information about installation:
* [GNU/Linux](https://github.com/a-rodin/qstardict/blob/master/INSTALL)
* [macOS](https://github.com/a-rodin/qstardict/blob/master/README.MACOS)
* [Windows](https://github.com/a-rodin/qstardict/blob/master/README.WINDOWS)

### Contribute a translation ###
If you want to contribute a translation for the QStarDict user interface, there are two ways to do it.

The first one is to use Qt Linguist [as described in wiki](https://github.com/a-rodin/qstardict/wiki/How-to-make-a-translation).

Alternatively, it is possible to send a translation using [QStarDict's Transifex page](https://www.transifex.com/qstardict/qstardict/).
