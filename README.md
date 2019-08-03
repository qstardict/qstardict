QStarDict, which stands for [quasi-star](https://en.wikipedia.org/wiki/Quasi-star) dictionary, is a free (as [in freedom](https://www.gnu.org/philosophy/free-sw.html)) dictionary application.

### Fuzzy search, cross-links, and history navigation

QStarDict not only finds words as soon as they typed, but also shows _fuzzy matches_ for them. It helps to identify similar words or correct typos. All articles are automatically _cross-linked_, so that click on a word in any article opens the corresponding article if it exists. With _history navigation_ buttons it is easy to move back and forth between articles.

<p align="center">
<img src="https://raw.githubusercontent.com/wiki/a-rodin/qstardict/images/qstardict-main.gif" width="742" height="533" alt="Main Window">
</p>

### Look up words from any application

<p align="center">
<img src="https://raw.githubusercontent.com/wiki/a-rodin/qstardict/images/qstardict-popup.gif" width="738" height="532" alt="Popup Window">
</p>

It can be hidden in system tray

<p align="center">
<img alt="system tray" src="https://raw.githubusercontent.com/wiki/a-rodin/qstardict/images/qstardict-tray.png" width="143" height="31">
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
