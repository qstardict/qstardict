QStarDict, which stands for [quasi-star](https://en.wikipedia.org/wiki/Quasi-star) dictionary, is a free (as [in freedom](https://www.gnu.org/philosophy/free-sw.html)) dictionary application.

### Main Window
<p align="center">
<img src="https://raw.githubusercontent.com/wiki/a-rodin/qstardict/images/qstardict-main.gif" width="742" height="533" alt="Main Window">
</p>

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
