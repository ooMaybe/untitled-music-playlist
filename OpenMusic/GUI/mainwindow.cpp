/*****************************************************************************\
    Class: mainwindow.cpp
    Description: MainWindow class implementation for OpenMusic application.
        Manages the GUI, playlist operations, song playback, and user interactions.
\*****************************************************************************/

#include <QMessageBox>
#include <QTimer>
#include <QFileIconProvider>
#include <QDir>
#include <QLabel>
#include <QFileDialog>
#include <random>
#include <vector>
#include <algorithm>
#include <string>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlistitemwidget.h"
#include "playlistlistwidgetyes.h"

QList<QString> playlistNames;

/*****************************************************************************\
    Function: MainWindow (Constructor)
    Description: Initializes the MainWindow with UI setup, tree widgets,
        context menus, and loads playlists and songs from database
    Parameters:
        YTDLPManager &manager -> reference to YTDLP manager
        Backend &backendRef -> reference to backend database
        QWidget *parent -> parent widget
    Returns: nothing
\*****************************************************************************/
MainWindow::MainWindow(YTDLPManager &manager, Backend &backendRef, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), ytdlpManager(manager), backend(backendRef), isPlayingFromFile(false), currentSongIndex(-1)
{
    /**********************************\
        UI INITIALIZATION
    \**********************************/
    
    // Initializes the UI components from the .ui file
    ui->setupUi(this);

    // Creates network manager for downloading thumbnails from URLs
    networkManager = new QNetworkAccessManager(this);

    /**********************************\
        CONFIGURE DOWNLOADS TREE WIDGET
    \**********************************/
    
    // Sets up 4 columns for downloaded songs tree
    ui->songsDownload->setColumnCount(4);
    
    // Sets column headers for the tree
    ui->songsDownload->setHeaderLabels(QStringList() << "Title" << "Artist" << "Duration" << "Date");
    
    // Sets width of first column (title) to 250 pixels
    ui->songsDownload->setColumnWidth(0, 250);
    
    // Sets icon size to 64x64 pixels
    ui->songsDownload->setIconSize(QSize(64, 64));

    /**********************************\
        CONFIGURE SEARCH RESULTS TREE WIDGET
    \**********************************/
    
    // Sets up 4 columns for search results tree
    ui->searchList->setColumnCount(4);
    
    // Sets column headers for the tree
    ui->searchList->setHeaderLabels(QStringList() << "Title" << "Artist" << "Duration" << "Date");
    
    // Sets width of first column (title) to 250 pixels
    ui->searchList->setColumnWidth(0, 250);
    
    // Sets icon size to 64x64 pixels
    ui->searchList->setIconSize(QSize(64, 64));

    /**********************************\
        CONNECT CONTEXT MENU SIGNALS
    \**********************************/
    
    // Enables custom context menu for search list
    ui->searchList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->searchList, &QWidget::customContextMenuRequested,
            this, &MainWindow::on_searchList_customContextMenuRequested);

    // Enables custom context menu for sidebar playlist
    ui->sideBarPlaylist->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->sideBarPlaylist, &QWidget::customContextMenuRequested,
            this, &MainWindow::on_sideBarPlaylist_customContextMenuRequested);

    // Enables custom context menu for downloaded songs
    ui->songsDownload->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->songsDownload, &QWidget::customContextMenuRequested,
            this, &MainWindow::on_songsDownload_customContextMenuRequested);
    
    // Enables custom context menu for main playlist tree
    ui->mainPlaylistTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->mainPlaylistTree, &QWidget::customContextMenuRequested,
            this, &MainWindow::on_mainPlaylistTree_customContextMenuRequested);

    /**********************************\
        CONNECT MEDIA PLAYER SIGNALS
    \**********************************/
    
    // Connects position changed signal to update progress bar
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::positionChanged,
            this, [this](qint64 position) {
                updateProgress(static_cast<int>(position));
            });
    
    // Connects duration changed signal to update duration label
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::durationChanged,
            this, [this](qint64 duration) {
                updateDuration(static_cast<int>(duration));
            });
    
    // Connects media status changed signal to handle auto-play
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::mediaStatusChanged,
            this, &MainWindow::onMediaStatusChanged);
    
    /**********************************\
        LOAD DATA FROM DATABASE
    \**********************************/
    
    // Loads all playlists and downloaded songs from database
    loadData();
}

/*****************************************************************************\
    Function: ~MainWindow (Destructor)
    Description: Cleans up UI resources when window is destroyed
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}

/*****************************************************************************\
    Function: addSidePlaylist
    Description: Adds a playlist entry to the sidebar tree widget with icon
    Parameters:
        const QString &titleName -> name of the playlist
        const QPixmap &icon -> playlist icon image
    Returns: nothing
\*****************************************************************************/
void MainWindow::addSidePlaylist(const QString &titleName,
                                 const QPixmap &icon)
{
    /**********************************\
        TREE WIDGET SETUP
    \**********************************/
    
    // Gets reference to sidebar playlist tree widget
    auto *tree = ui->sideBarPlaylist;
    
    // Sets up 2 columns (icon and name)
    tree->setColumnCount(2);
    tree->setHeaderLabels({"Icon", "Name"});
    
    // Sets icon size to 64x64 pixels
    tree->setIconSize(QSize(64, 64));
    
    // Sets width of icon column to 80 pixels
    tree->setColumnWidth(0, 80);
    
    // Allows variable row heights for better visual appearance
    tree->setUniformRowHeights(false);

    /**********************************\
        CREATE AND ADD ITEM
    \**********************************/
    
    // Creates new tree widget item
    QTreeWidgetItem *item = new QTreeWidgetItem(tree);
    
    // Adds item to tree as top-level entry
    tree->addTopLevelItem(item);

    /**********************************\
        SET ICON IMAGE
    \**********************************/
    
    // Creates label widget to hold playlist icon
    auto *iconLabel = new QLabel(tree);
    
    // Scales icon to 64x64 while maintaining aspect ratio
    iconLabel->setPixmap(icon.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
    // Centers icon in the label
    iconLabel->setAlignment(Qt::AlignCenter);
    
    // Sets label as widget for first column (icon column)
    tree->setItemWidget(item, 0, iconLabel);

    /**********************************\
        SET PLAYLIST NAME
    \**********************************/
    
    // Sets playlist name in second column
    item->setText(1, titleName);

    // Applies CSS styling for padding
    tree->setStyleSheet(
        "QTreeWidget::item { padding-left: 10px; }"
        );
}

/*****************************************************************************\
    Function: addToPlaylistPage
    Description: Adds a song to the main playlist page tree widget
    Parameters:
        const QString &titleName -> song title
        const QString &titleArtist -> artist name
        const QString &titleDuration -> song duration
        const QString &titleDate -> date added
        const QPixmap &icon -> song thumbnail
    Returns: nothing
\*****************************************************************************/
void MainWindow::addToPlaylistPage(const QString &titleName,
                                   const QString &titleArtist,
                                   const QString &titleDuration,
                                   const QString &titleDate,
                                   const QPixmap &icon)
{
    /**********************************\
        TREE WIDGET SETUP
    \**********************************/
    
    // Gets reference to main playlist tree widget
    auto *tree = ui->mainPlaylistTree;
    
    // Sets up 5 columns for song details
    tree->setColumnCount(5);
    tree->setHeaderLabels({"Icon", "Name", "Artist", "Duration", "Date Added"});
    
    // Sets icon size to 64x64 pixels
    tree->setIconSize(QSize(64, 64));
    
    // Sets width of icon column to 80 pixels
    tree->setColumnWidth(0, 80);
    
    // Allows variable row heights
    tree->setUniformRowHeights(false);

    /**********************************\
        CREATE AND ADD ITEM
    \**********************************/
    
    // Creates new tree widget item for song
    QTreeWidgetItem *item = new QTreeWidgetItem(tree);
    
    // Adds item to tree as top-level entry
    tree->addTopLevelItem(item);

    /**********************************\
        SET ICON IMAGE
    \**********************************/
    
    // Creates label widget to hold song thumbnail
    auto *iconLabel = new QLabel(tree);
    
    // Scales thumbnail to 64x64 while maintaining aspect ratio
    iconLabel->setPixmap(icon.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
    // Centers thumbnail in the label
    iconLabel->setAlignment(Qt::AlignCenter);
    
    // Sets label as widget for first column
    tree->setItemWidget(item, 0, iconLabel);

    /**********************************\
        SET SONG DETAILS
    \**********************************/
    
    // Sets song title in second column
    item->setText(1, titleName);
    
    // Sets artist name in third column
    item->setText(2, titleArtist);
    
    // Sets duration in fourth column
    item->setText(3, titleDuration);
    
    // Sets date added in fifth column
    item->setText(4, titleDate);

    // Applies CSS styling for padding
    tree->setStyleSheet(
        "QTreeWidget::item { padding-left: 10px; }"
        );
}

/*****************************************************************************\
    Function: addSearch
    Description: Adds a search result to the search list tree widget
    Parameters:
        const QString &titleName -> song title
        const QString &titleArtist -> artist name
        const QString &titleDuration -> song duration
        const QString &titleDate -> date added
        const QPixmap &icon -> default icon
        const QString &url -> YouTube URL
        const QString &thumbnail -> thumbnail URL
    Returns: nothing
\*****************************************************************************/
void MainWindow::addSearch(const QString &titleName,
                           const QString &titleArtist,
                           const QString &titleDuration,
                           const QString &titleDate,
                           const QPixmap &icon,
                           const QString &url,
                           const QString &thumbnail)
{
    /**********************************\
        CREATE TREE ITEM
    \**********************************/
    
    // Creates new item for search result
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->searchList);

    /**********************************\
        SET VISIBLE DATA
    \**********************************/
    
    // Sets song title in first column
    item->setText(0, titleName);
    
    // Sets artist name in second column
    item->setText(1, titleArtist);
    
    // Sets duration in third column
    item->setText(2, titleDuration);
    
    // Sets date in fourth column
    item->setText(3, titleDate);
    
    // Sets default icon (will be replaced by thumbnail)
    item->setIcon(0, icon);

    /**********************************\
        STORE HIDDEN DATA
    \**********************************/
    
    // Stores YouTube URL for later use (play/download)
    item->setData(0, Qt::UserRole, url);
    
    // Stores thumbnail URL for loading image
    item->setData(0, Qt::UserRole + 1, thumbnail);

    /**********************************\
        ADD TO SEARCH LIST
    \**********************************/
    
    // Adds item to search results tree
    ui->searchList->addTopLevelItem(item);

    // Asynchronously loads thumbnail from URL
    loadThumbnailForTreeItem(thumbnail, item);
}

/*****************************************************************************\
    Function: addSong
    Description: Adds a downloaded song to the songs download tree widget
    Parameters:
        const QString &titleName -> song title
        const QString &titleArtist -> artist name
        const QString &titleDuration -> song duration
        const QString &titleDate -> date added
        const QPixmap &icon -> song thumbnail
        const QString &filePath -> local file path to MP3
    Returns: nothing
\*****************************************************************************/
void MainWindow::addSong(const QString &titleName,
                         const QString &titleArtist,
                         const QString &titleDuration,
                         const QString &titleDate,
                         const QPixmap &icon,
                         const QString &filePath)
{
    /**********************************\
        CREATE TREE ITEM
    \**********************************/
    
    // Creates new item for downloaded song
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->songsDownload);

    /**********************************\
        SET SONG INFORMATION
    \**********************************/
    
    // Sets song title in first column
    item->setText(0, titleName);
    
    // Sets artist name in second column
    item->setText(1, titleArtist);
    
    // Sets duration in third column
    item->setText(2, titleDuration);
    
    // Sets date added in fourth column
    item->setText(3, titleDate);
    
    // Sets thumbnail icon
    item->setIcon(0, QIcon(icon));

    /**********************************\
        STORE FILE PATH
    \**********************************/
    
    // Stores local file path for playback
    item->setData(0, Qt::UserRole, filePath);

    // Adds item to downloads tree
    ui->songsDownload->addTopLevelItem(item);
}

/*****************************************************************************\
    Function: on_searchButton_clicked
    Description: Handles search button click, searches YouTube and displays results
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_searchButton_clicked()
{
    /**********************************\
        VALIDATE SEARCH QUERY
    \**********************************/
    
    // Converts search bar text to std::string
    std::string query = ui->searchBar->text().toStdString();
    
    // Checks if search query is empty
    if (query.length() == 0){
        QMessageBox::critical(this, "Error", "You must typed something into the search bar before searching!");
        return;
    }

    /**********************************\
        PERFORM YOUTUBE SEARCH
    \**********************************/
    
    // Searches YouTube for top 10 results matching query
    std::vector<SongResult> songs = ytdlpManager.searchSongs(query, 10);

    // Clears previous search results
    ui->searchList->clear();
    
    // Clears search bar text
    ui->searchBar->clear();

    /***************************\
        PROCESS SEARCH RESULTS
    \***************************/
    
    // Iterates through each song result
    for (const auto& s : songs) {
        int durationSec = 0;

        // Validates duration string before converting
        if (!s.duration.empty()) {
            try {
                // Converts duration string to integer seconds
                durationSec = std::stoi(s.duration);
            } catch (const std::exception &e) {
                // Uses 0 as fallback if conversion fails
                durationSec = 0;
            }
        }

        // Converts duration to minutes and seconds
        int minutes = durationSec / 60;
        int seconds = durationSec % 60;
        
        // Formats duration as "MM:SS" with zero-padded seconds
        QString durationStr = QString("%1:%2")
                                  .arg(minutes)
                                  .arg(seconds, 2, 10, QChar('0'));

        // Adds search result to UI tree widget
        addSearch(QString::fromStdString(s.title),
                  QString::fromStdString(s.uploader),
                  durationStr,
                  "Unknown date",
                  QPixmap(":/icons/music.png"),
                  QString::fromStdString(s.url),
                  QString::fromStdString(s.thumbnail));
    }
}

/*****************************************************************************\
    Function: on_sideBarPlaylist_customContextMenuRequested
    Description: Handles right-click context menu on sidebar playlists
    Parameters:
        const QPoint &pos -> position of right-click
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_sideBarPlaylist_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->sideBarPlaylist->itemAt(pos);
    if (!item) return;

    QMenu menu(this);

    QAction *openAction = menu.addAction("Open");
    QAction *removeAction = menu.addAction("Remove");

    QAction *chosen = menu.exec(ui->sideBarPlaylist->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    ui->stackedWidget->setCurrentIndex(1);

    if (chosen == openAction) {
        currentPlaylist = item->text(1);  // Store current playlist name
        
        QWidget *widget = ui->sideBarPlaylist->itemWidget(item, 0);
        auto *label = qobject_cast<QLabel*>(widget);

        QPixmap pixmap = label->pixmap();

        ui->labelPlaylistImageMain->setPixmap(
            pixmap.scaled(ui->labelPlaylistImageMain->size(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation)
            );

        ui->mainPlaylistName->setText(currentPlaylist);
        
        // Load songs from database
        ui->mainPlaylistTree->clear();
        
        // Configure tree structure
        ui->mainPlaylistTree->setColumnCount(5);
        ui->mainPlaylistTree->setHeaderLabels({"Icon", "Name", "Artist", "Duration", "Date Added"});
        ui->mainPlaylistTree->setIconSize(QSize(64, 64));
        ui->mainPlaylistTree->setColumnWidth(0, 80);
        ui->mainPlaylistTree->setUniformRowHeights(false);
        
        std::vector<PlaylistSong> songs = backend.getPlaylistSongs(currentPlaylist.toStdString());
        
        int totalSeconds = 0;
        
        for (const auto& song : songs) {
            QPixmap thumbnail;
            if (!song.thumbnailPath.empty() && QFile::exists(QString::fromStdString(song.thumbnailPath))) {
                thumbnail.load(QString::fromStdString(song.thumbnailPath));
            } else {
                thumbnail = QPixmap(":/icons/music.png");
            }
            
            QTreeWidgetItem *songItem = new QTreeWidgetItem(ui->mainPlaylistTree);
            ui->mainPlaylistTree->addTopLevelItem(songItem);
            
            auto *iconLabel = new QLabel(ui->mainPlaylistTree);
            iconLabel->setPixmap(thumbnail.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            iconLabel->setAlignment(Qt::AlignCenter);
            ui->mainPlaylistTree->setItemWidget(songItem, 0, iconLabel);
            
            songItem->setText(1, QString::fromStdString(song.title));
            songItem->setText(2, QString::fromStdString(song.artist));
            songItem->setText(3, QString::fromStdString(song.duration));
            songItem->setText(4, QString::fromStdString(song.dateAdded));
            
            // Store song ID and file path for playing/deleting
            songItem->setData(0, Qt::UserRole, song.id);
            songItem->setData(0, Qt::UserRole + 1, QString::fromStdString(song.filePath));
            
            // Parse duration and add to total
            QString durationStr = QString::fromStdString(song.duration);
            QStringList parts = durationStr.split(":");
            if (parts.size() == 2) {
                totalSeconds += parts[0].toInt() * 60 + parts[1].toInt();
            }
        }
        
        // Update duration label
        int hours = totalSeconds / 3600;
        int minutes = (totalSeconds % 3600) / 60;
        QString durationText = QString("%1 songs, %2 hr %3 min")
            .arg(songs.size())
            .arg(hours)
            .arg(minutes);
        ui->songDurationLabel->setText(durationText);
        
    } else if (chosen == removeAction) {
        QString playlistName = item->text(1);
        auto reply = QMessageBox::question(this, "Delete Playlist",
                                           "Are you sure you want to delete playlist '" + playlistName + "'?",
                                           QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            backend.deletePlaylist(playlistName.toStdString());
            playlistNames.removeOne(playlistName);
            delete item;
            QMessageBox::information(this, "Deleted", "Playlist deleted successfully.");
        }
    }
}

/*****************************************************************************\
    Function: on_searchList_customContextMenuRequested
    Description: Handles right-click context menu on search results
    Parameters:
        const QPoint &pos -> position of right-click
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_searchList_customContextMenuRequested(const QPoint &pos)
{
    /**********************************\
        GET SELECTED ITEM
    \**********************************/
    
    // Gets the tree item at click position
    QTreeWidgetItem *item = ui->searchList->itemAt(pos);
    
    // Returns if no item was clicked
    if (!item) return;

    /**********************************\
        CREATE CONTEXT MENU
    \**********************************/
    
    // Creates context menu
    QMenu menu(this);

    // Adds "Play" action with play icon
    QAction *playAction = menu.addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Play");
    
    // Adds "Download" action with download icon
    QAction *downloadAction = menu.addAction(style()->standardIcon(QStyle::SP_ArrowDown), "Download");

    // Shows menu and waits for user selection
    QAction *chosen = menu.exec(ui->searchList->viewport()->mapToGlobal(pos));
    
    // Returns if user cancelled menu
    if (!chosen)
        return;

    /**********************************\
        RETRIEVE STORED DATA
    \**********************************/
    
    // Retrieves YouTube URL from item data
    QString url = item->data(0, Qt::UserRole).toString();
    
    // Retrieves thumbnail URL from item data
    QString thumbnail = item->data(0, Qt::UserRole + 1).toString();

    /***************************\
        HANDLE PLAY ACTION
    \***************************/
    
    if (chosen == playAction) {
        qDebug() << "Play:" << item->text(0);
        qDebug() << "URL:" << url;

        // Streams audio from YouTube URL (temporary, stops after 30-60 seconds)
        ytdlpManager.playSong(url.toStdString());
        
        // Sets song name in UI
        ui->songNameBox->setText(item->text(0));
        
        // Sets flag indicating streaming from web
        isPlayingFromFile = false;
        
        // Disables auto-play (not in playlist)
        currentSongIndex = -1;
        
        // Clears playlist name
        ui->playlistBoxName->setText("");

        // Loads thumbnail image from URL
        loadThumbnail(thumbnail, ui->songImage);

        // Updates button to show "Stop"
        ui->stopButton->setText("Stop");
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
        
    } else if (chosen == downloadAction) {
        qDebug() << "Download:" << item->text(0);
        qDebug() << "URL:" << url;

        /**********************************\
            DOWNLOAD SONG FROM YOUTUBE
        \**********************************/
        
        // Downloads audio from YouTube URL and converts to MP3
        bool success = ytdlpManager.downloadSong(url.toStdString(), item->text(0).toStdString());

        if (success) {
            // Shows success message
            QMessageBox::information(this, "Success", "Download complete!\n\nSaved to Downloads folder.");

            /**********************************\
                CONSTRUCT FILE PATHS
            \**********************************/
            
            // Gets application directory path
            QString exeDir = QCoreApplication::applicationDirPath();
            
            // Constructs path to Downloads folder
            QString downloadFolder = QDir(exeDir).filePath("data/Downloads");

            // Removes invalid filename characters from title
            QString safeTitle = item->text(0);
            safeTitle.replace(QRegularExpression("[<>:\"/\\\\|?*]"), "");

            // Constructs full path to MP3 file
            QString mp3Path = QDir(downloadFolder).filePath(safeTitle + ".mp3");
            
            // Constructs full path to PNG thumbnail
            QString pngPath = QDir(downloadFolder).filePath(safeTitle + ".png");

            /**********************************\
                LOAD THUMBNAIL
            \**********************************/
            
            QPixmap thumbnail;
            
            // Checks if thumbnail file exists
            if (QFile::exists(pngPath)) {
                // Loads thumbnail from file
                thumbnail.load(pngPath);
                
                // Scales thumbnail to 64x64
                thumbnail = thumbnail.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            } else {
                // Uses default music icon if thumbnail not found
                thumbnail = QPixmap(":/icons/music.png");
            }
            
            // Gets current date in format "MMM d, yyyy"
            QString dateAdded = QDateTime::currentDateTime().toString("MMM d, yyyy");
            
            /**********************************\
                SAVE TO DATABASE
            \**********************************/
            
            // Saves song information to database
            backend.saveDownloadedSong(item->text(0).toStdString(),
                                      item->text(1).toStdString(),
                                      item->text(2).toStdString(),
                                      dateAdded.toStdString(),
                                      mp3Path.toStdString(),
                                      pngPath.toStdString());

            /**********************************\
                ADD TO UI
            \**********************************/
            
            // Adds song to downloaded songs tree widget
            addSong(item->text(0),
                    item->text(1),
                    item->text(2),
                    dateAdded,
                    thumbnail,
                    mp3Path);

        } else {
            // Shows error message if download failed
            QMessageBox::critical(this, "Error", "Download failed!");
        }
    }
}

/*****************************************************************************\
    Function: updateProgress
    Description: Updates the progress bar and time label during playback
    Parameters:
        int position -> current playback position in milliseconds
    Returns: nothing
\*****************************************************************************/
void MainWindow::updateProgress(int position) {
    /**********************************\
        UPDATE PROGRESS BAR
    \**********************************/
    
    // Checks if progress bar has valid maximum value
    if (ui->songProgress->maximum() > 0) {
        // Updates progress bar to current position
        ui->songProgress->setValue(position);
    }

    /***************************\
        CALCULATE TIME COMPONENTS
    \***************************/
    
    // Converts milliseconds to seconds
    int seconds = (position / 1000) % 60;
    
    // Converts milliseconds to minutes
    int minutes = (position / 1000 / 60) % 60;
    
    // Converts milliseconds to hours
    int hours = (position / 1000 / 60 / 60);

    /***************************\
        FORMAT TIME STRING
    \***************************/
    
    QString timeStr;
    
    // Checks if song duration is over 1 hour
    if (hours > 0) {
        // Formats as "H:MM:SS" with zero-padded minutes and seconds
        timeStr = QString("%1:%2:%3")
        .arg(hours)
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    } else {
        // Formats as "M:SS" with zero-padded seconds
        timeStr = QString("%1:%2")
        .arg(minutes)
            .arg(seconds, 2, 10, QChar('0'));
    }

    // Updates starting time label with formatted time
    ui->startingBox->setText(timeStr);
}

/*****************************************************************************\
    Function: updateDuration
    Description: Updates the duration label and progress bar range
    Parameters:
        int duration -> total duration in milliseconds
    Returns: nothing
\*****************************************************************************/
void MainWindow::updateDuration(int duration) {
    /**********************************\
        SET PROGRESS BAR RANGE
    \**********************************/
    
    // Sets progress bar range from 0 to total duration
    ui->songProgress->setRange(0, duration);

    /***************************\
        CALCULATE TIME COMPONENTS
    \***************************/
    
    // Converts milliseconds to seconds
    int seconds = (duration / 1000) % 60;
    
    // Converts milliseconds to minutes
    int minutes = (duration / 1000 / 60) % 60;
    
    // Converts milliseconds to hours
    int hours = (duration / 1000 / 60 / 60);

    /***************************\
        FORMAT TIME STRING
    \***************************/
    
    QString timeStr;
    
    // Checks if song duration is over 1 hour
    if (hours > 0) {
        // Formats as "H:MM:SS" with zero-padded minutes and seconds
        timeStr = QString("%1:%2:%3")
        .arg(hours)
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    } else {
        // Formats as "M:SS" with zero-padded seconds
        timeStr = QString("%1:%2")
        .arg(minutes)
            .arg(seconds, 2, 10, QChar('0'));
    }

    // Updates duration label with formatted time
    ui->durationBox->setText(timeStr);
}

/*****************************************************************************\
    Function: on_stopButton_clicked
    Description: Toggles playback between play and pause states
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_stopButton_clicked()
{
    /**********************************\
        TOGGLE PLAYBACK STATE
    \**********************************/
    
    // Toggles between play and pause
    ytdlpManager.stopSong();

    /***************************\
        UPDATE BUTTON TEXT/ICON
    \***************************/
    
    // Checks if button currently shows "Play"
    if (ui->stopButton->text().contains("Play")){
        // Changes button to show "Stop"
        ui->stopButton->setText("Stop");
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
    }else{
        // Changes button to show "Play"
        ui->stopButton->setText("Play");
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-start"));
    }
}

/*****************************************************************************\
    Function: loadThumbnail
    Description: Loads a thumbnail image from URL and displays it in a label
    Parameters:
        const QString &url -> thumbnail URL
        QLabel *label -> label widget to display image
    Returns: nothing
\*****************************************************************************/
void MainWindow::loadThumbnail(const QString &url, QLabel *label) {
    /**********************************\
        CREATE NETWORK REQUEST
    \**********************************/
    
    // Creates HTTP request for thumbnail URL
    QNetworkRequest request(url);
    
    // Sends GET request and gets reply object
    QNetworkReply *reply = networkManager->get(request);

    /***************************\
        HANDLE RESPONSE
    \***************************/
    
    // Connects to finished signal for asynchronous processing
    connect(reply, &QNetworkReply::finished, this, [reply, label]() {
        // Checks if request completed successfully
        if (reply->error() == QNetworkReply::NoError) {
            // Reads image data from response
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            
            // Loads image from binary data
            pixmap.loadFromData(imageData);

            // Checks if image loaded successfully
            if (!pixmap.isNull()) {
                // Scales and displays image in label
                label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        } else {
            // Logs error message
            qDebug() << "[MainWindow] Failed to load thumbnail:" << reply->errorString();
            
            // Uses default music icon as fallback
            label->setPixmap(QPixmap(":/icons/music.png"));
        }
        
        // Schedules reply object for deletion
        reply->deleteLater();
    });
}

/*****************************************************************************\
    Function: loadThumbnailForTreeItem
    Description: Loads a thumbnail from URL and sets it as tree item icon
    Parameters:
        const QString &url -> thumbnail URL
        QTreeWidgetItem *item -> tree item to set icon
    Returns: nothing
\*****************************************************************************/
void MainWindow::loadThumbnailForTreeItem(const QString &url, QTreeWidgetItem *item) {
    /**********************************\
        VALIDATE URL
    \**********************************/
    
    // Checks if URL is empty or invalid
    if (url.isEmpty() || !url.startsWith("http")) {
        qDebug() << "[MainWindow] Invalid thumbnail URL:" << url;
        return;
    }

    /**********************************\
        FIX YOUTUBE THUMBNAIL URL
    \**********************************/
    
    QString fixedUrl = url;

    // Replaces WebP path with standard image path
    fixedUrl.replace("/vi_webp/", "/vi/", Qt::CaseInsensitive);

    // Replaces WebP extension with JPEG extension
    fixedUrl.replace(".webp", ".jpg", Qt::CaseInsensitive);

    /**********************************\
        CREATE NETWORK REQUEST
    \**********************************/
    
    // Creates HTTP request with fixed URL
    QNetworkRequest request(fixedUrl);
    
    // Sets user agent to avoid request blocking
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

    // Sends GET request
    QNetworkReply *reply = networkManager->get(request);

    /***************************\
        SET TIMEOUT
    \***************************/
    
    // Creates 2 second timeout timer
    QTimer::singleShot(2000, reply, [reply]() {
        // Checks if request is still running
        if (reply->isRunning()) {
            qDebug() << "[MainWindow] Thumbnail request timed out";
            
            // Aborts request
            reply->abort();
        }
    });

    /***************************\
        HANDLE RESPONSE
    \***************************/
    
    // Connects to finished signal
    connect(reply, &QNetworkReply::finished, this, [reply, item, fixedUrl]() {
        // Checks if request completed successfully
        if (reply->error() == QNetworkReply::NoError) {
            // Reads image data from response
            QByteArray imageData = reply->readAll();

            QPixmap pixmap;
            
            // Loads image from binary data
            pixmap.loadFromData(imageData);

            // Checks if image loaded successfully
            if (!pixmap.isNull()) {
                // Scales image to 64x64
                QPixmap scaledPixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                
                // Sets image as tree item icon
                item->setIcon(0, QIcon(scaledPixmap));
            } else {
                // Logs decoding failure
                qDebug() << "[MainWindow] Failed to load/decode JPEG data from:" << fixedUrl;
                
                // Uses default music icon as fallback
                item->setIcon(0, QIcon(":/icons/music.png"));
            }
        } else {
            // Logs network error
            qDebug() << "[MainWindow] Network error loading thumbnail:" << reply->errorString() << "URL:" << fixedUrl;
        }
        
        // Schedules reply object for deletion
        reply->deleteLater();
    });
}

/*****************************************************************************\
    Function: on_songsDownload_customContextMenuRequested
    Description: Handles right-click context menu on downloaded songs
    Parameters:
        const QPoint &pos -> position of right-click
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_songsDownload_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->songsDownload->itemAt(pos);
    if (!item) return;

    QMenu menu(this);

    QAction *playAction = menu.addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Play");
    QAction *deleteAction = menu.addAction(style()->standardIcon(QStyle::SP_TrashIcon), "Delete");
    QMenu *addToMenu = menu.addMenu(style()->standardIcon(QStyle::SP_ComputerIcon), "Add to Playlist");

    QList<QAction*> playlistActions;
    for (const QString &name : playlistNames) {
        playlistActions.append(addToMenu->addAction("Add to " + name));
    }

    QAction *chosen = menu.exec(ui->songsDownload->viewport()->mapToGlobal(pos));
    if (!chosen)
        return;

    QString filePath = item->data(0, Qt::UserRole).toString();

    if (chosen == playAction) {
        qDebug() << "[MainWindow] Playing local file:" << filePath;

        if (QFile::exists(filePath)) {
            // Play from LOCAL FILE (not URL!)
            ytdlpManager.getMediaPlayer()->setSource(QUrl::fromLocalFile(filePath));
            ytdlpManager.getMediaPlayer()->play();
            
            isPlayingFromFile = true;  // Playing from local file
            currentSongIndex = -1;  // Not playing from playlist, disable auto-play
            ui->playlistBoxName->setText("");  // Clear playlist name

            ui->songNameBox->setText(item->text(0));

            // Load thumbnail from local PNG
            QString pngPath = filePath;
            pngPath.replace(".mp3", ".png");

            if (QFile::exists(pngPath)) {
                QPixmap pixmap(pngPath);
                ui->songImage->setPixmap(pixmap.scaled(ui->songImage->size(),
                                                       Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation));
            } else {
                ui->songImage->setPixmap(QPixmap(":/icons/music.png"));
            }

            ui->stopButton->setText("Stop");
            ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
        } else {
            QMessageBox::warning(this, "Error", "File not found!\n\n" + filePath);
        }

    } else if (chosen == deleteAction) {
        auto reply = QMessageBox::question(this, "Delete Song",
                                           "Are you sure you want to delete:\n" + item->text(0),
                                           QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            ytdlpManager.getMediaPlayer()->setSource(QUrl());

            // Delete from database
            backend.removeDownloadedSong(filePath.toStdString());

            // Delete MP3 file
            if (QFile::exists(filePath)) {
                QFile::remove(filePath);
            }

            // Delete PNG thumbnail
            QString pngPath = filePath;
            pngPath.replace(".mp3", ".png");
            if (QFile::exists(pngPath)) {
                QFile::remove(pngPath);
            }

            // Remove from tree
            delete item;

            qDebug() << "[MainWindow] Deleted:" << filePath;
        }
    }

    for (int i = 0; i < playlistActions.size(); i++) {
        if (chosen == playlistActions[i]) {

            QString playlistName = playlistNames[i];
            QString title = item->text(0);
            QString artist = item->text(1);
            QString duration = item->text(2);
            QString date = item->text(3);
            QString filePath = item->data(0, Qt::UserRole).toString();
            QString pngPath = filePath;
            pngPath.replace(".mp3", ".png");  // Create separate copy for thumbnail

            qDebug() << "[MainWindow] Adding to playlist:";
            qDebug() << "  Title:" << title;
            qDebug() << "  FilePath:" << filePath;
            qDebug() << "  ThumbnailPath:" << pngPath;

            // Save to database
            backend.addSongToPlaylist(playlistName.toStdString(),
                                     title.toStdString(),
                                     artist.toStdString(),
                                     duration.toStdString(),
                                     date.toStdString(),
                                     filePath.toStdString(),
                                     pngPath.toStdString());

            QMessageBox::information(this,
                                     "Added",
                                     "Added \"" + title + "\" to playlist \"" + playlistName + "\".");

            return;
        }
    }
}

/*****************************************************************************\
    Function: on_homeButton_clicked
    Description: Returns to home page of the application
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_homeButton_clicked()
{
    /**********************************\
        NAVIGATE TO HOME PAGE
    \**********************************/
    
    // Sets stacked widget to show home page (index 0)
    ui->stackedWidget->setCurrentIndex(0);
    
    qDebug() << "Hi";
}

/*****************************************************************************\
    Function: on_mainPlaylistTree_customContextMenuRequested
    Description: Handles right-click context menu on playlist songs
    Parameters:
        const QPoint &pos -> position of right-click
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_mainPlaylistTree_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->mainPlaylistTree->itemAt(pos);
    if (!item || currentPlaylist.isEmpty()) return;

    QMenu menu(this);

    QAction *playAction = menu.addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Play");
    QAction *deleteAction = menu.addAction(style()->standardIcon(QStyle::SP_TrashIcon), "Remove from Playlist");

    QAction *chosen = menu.exec(ui->mainPlaylistTree->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    if (chosen == playAction) {
        QString filePath = item->data(0, Qt::UserRole + 1).toString();
        
        qDebug() << "[MainWindow] Attempting to play from playlist:";
        qDebug() << "  Song:" << item->text(1);
        qDebug() << "  FilePath:" << filePath;
        qDebug() << "  File exists:" << QFile::exists(filePath);
        
        if (QFile::exists(filePath)) {
            QUrl fileUrl = QUrl::fromLocalFile(filePath);
            qDebug() << "  Setting source:" << fileUrl;
            
            ytdlpManager.getMediaPlayer()->setSource(fileUrl);
            ytdlpManager.getMediaPlayer()->play();
            
            isPlayingFromFile = true;  // Playing from local file
            currentSongIndex = ui->mainPlaylistTree->indexOfTopLevelItem(item);  // Track position for auto-play

            ui->songNameBox->setText(item->text(1));
            ui->playlistBoxName->setText(currentPlaylist);  // Show playlist name

            // Load thumbnail
            QString pngPath = filePath;
            pngPath.replace(".mp3", ".png");

            if (QFile::exists(pngPath)) {
                QPixmap pixmap(pngPath);
                ui->songImage->setPixmap(pixmap.scaled(ui->songImage->size(),
                                                       Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation));
            } else {
                ui->songImage->setPixmap(QPixmap(":/icons/music.png"));
            }

            ui->stopButton->setText("Stop");
            ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
        } else {
            QMessageBox::warning(this, "Error", "File not found!\n\n" + filePath);
        }

    } else if (chosen == deleteAction) {
        int songId = item->data(0, Qt::UserRole).toInt();
        
        auto reply = QMessageBox::question(this, "Remove Song",
                                           "Remove \"" + item->text(1) + "\" from this playlist?",
                                           QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            // Remove from database
            backend.removeSongFromPlaylist(currentPlaylist.toStdString(), songId);
            
            // Remove from UI
            delete item;
            
            QMessageBox::information(this, "Removed", "Song removed from playlist.");
        }
    }
}

/*****************************************************************************\
    Function: on_chooseImageButton_clicked
    Description: Opens file dialog to choose playlist image
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_chooseImageButton_clicked()
{
    /**********************************\
        OPEN FILE DIALOG
    \**********************************/
    
    // Opens file dialog to select PNG image file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open PNG Image"),
        QDir::homePath(), tr("PNG Images (*.png)"));

    // Sets button text to show selected file path
    ui->chooseImageButton->setText(filePath);
}

/*****************************************************************************\
    Function: on_createPlaylistButton_clicked
    Description: Creates a new playlist with name and image
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_createPlaylistButton_clicked()
{
    /**********************************\
        VALIDATE INPUTS
    \**********************************/
    
    // Checks if playlist name is empty
    if (ui->playlistBox->text().length() == 0){
        QMessageBox::critical(this, "Error", "You must give the playlist a name!");
        return;
    }

    // Checks if image was selected
    if (ui->chooseImageButton->text().length() == 0){
        QMessageBox::critical(this, "Error", "You must choose a valid image!");
        return;
    }

    /**********************************\
        GET PLAYLIST DATA
    \**********************************/
    
    // Gets playlist name from text box
    QString name = ui->playlistBox->text();
    
    // Gets image path from button text
    QString imagePath = ui->chooseImageButton->text();

    /**********************************\
        CREATE PLAYLIST
    \**********************************/
    
    // Adds playlist to sidebar
    addSidePlaylist(name, imagePath);
    
    // Adds name to playlist names list
    playlistNames.append(name);
    
    // Saves playlist to database
    backend.savePlaylist(name.toStdString(), imagePath.toStdString());
    
    // Shows success message
    QMessageBox::information(this, "Success", "Playlist '" + name + "' created!");
    
    /**********************************\
        CLEAR INPUT FIELDS
    \**********************************/
    
    // Clears playlist name box
    ui->playlistBox->clear();
    
    // Clears image button text
    ui->chooseImageButton->setText("");
}

/*****************************************************************************\
    Function: on_forwardTen_clicked
    Description: Skips forward 10 seconds in current song
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_forwardTen_clicked()
{
    /**********************************\
        CHECK IF FEATURE AVAILABLE
    \**********************************/
    
    // Checks if playing from local file (not web stream)
    if (!isPlayingFromFile) {
        QMessageBox::information(this, "Feature Not Available", 
                                "Skip forward/backward is only available when playing from downloaded files.\n\n"
                                "Web streaming does not support this feature.");
        return;
    }
    
    /**********************************\
        GET PLAYER AND POSITIONS
    \**********************************/
    
    // Gets reference to media player
    QMediaPlayer *player = ytdlpManager.getMediaPlayer();
    
    // Gets current playback position in milliseconds
    qint64 currentPos = player->position();
    
    // Calculates new position (10 seconds forward)
    qint64 newPos = currentPos + 10000;
    
    /***************************\
        SET NEW POSITION
    \***************************/
    
    // Checks if new position is before end of track
    if (newPos < player->duration()) {
        // Skips forward 10 seconds
        player->setPosition(newPos);
        qDebug() << "[MainWindow] Skipped forward 10 seconds";
    } else {
        // Skips to end of track
        player->setPosition(player->duration());
        qDebug() << "[MainWindow] Skipped to end of track";
    }
}

/*****************************************************************************\
    Function: loadData
    Description: Loads playlists and downloaded songs from database on startup
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void MainWindow::loadData()
{
    // Load playlists from database
    std::vector<Playlist> playlists = backend.getAllPlaylists();
    
    for (const auto& playlist : playlists) {
        QString name = QString::fromStdString(playlist.name);
        QString imagePath = QString::fromStdString(playlist.imagePath);
        
        addSidePlaylist(name, QPixmap(imagePath));
        playlistNames.append(name);
    }
    
    // Load downloaded songs from database
    std::vector<DownloadedSong> songs = backend.getAllDownloadedSongs();
    
    for (const auto& song : songs) {
        QString filePath = QString::fromStdString(song.filePath);
        
        // Check if file still exists
        if (!QFile::exists(filePath)) {
            continue;  // Skip deleted files
        }
        
        QString title = QString::fromStdString(song.title);
        QString artist = QString::fromStdString(song.artist);
        QString duration = QString::fromStdString(song.duration);
        QString dateAdded = QString::fromStdString(song.dateAdded);
        QString thumbnailPath = QString::fromStdString(song.thumbnailPath);
        
        // Load thumbnail
        QPixmap thumbnail;
        if (!thumbnailPath.isEmpty() && QFile::exists(thumbnailPath)) {
            thumbnail.load(thumbnailPath);
            thumbnail = thumbnail.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        } else {
            thumbnail = QPixmap(":/icons/music.png");
        }
        
        // Add to songsDownload tree
        addSong(title, artist, duration, dateAdded, thumbnail, filePath);
    }
    
    qDebug() << "[MainWindow] Loaded" << playlists.size() << "playlists and" << songs.size() << "songs";
}

/*****************************************************************************\
    Function: on_backwardTen_clicked
    Description: Skips backward 10 seconds in current song
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_backwardTen_clicked()
{
    /**********************************\
        CHECK IF FEATURE AVAILABLE
    \**********************************/
    
    // Checks if playing from local file (not web stream)
    if (!isPlayingFromFile) {
        QMessageBox::information(this, "Feature Not Available", 
                                "Skip forward/backward is only available when playing from downloaded files.\n\n"
                                "Web streaming does not support this feature.");
        return;
    }
    
    /**********************************\
        GET PLAYER AND POSITIONS
    \**********************************/
    
    // Gets reference to media player
    QMediaPlayer *player = ytdlpManager.getMediaPlayer();
    
    // Gets current playback position in milliseconds
    qint64 currentPos = player->position();
    
    // Calculates new position (10 seconds backward)
    qint64 newPos = currentPos - 10000;
    
    /***************************\
        SET NEW POSITION
    \***************************/
    
    // Checks if new position is after beginning of track
    if (newPos > 0) {
        // Skips backward 10 seconds
        player->setPosition(newPos);
        qDebug() << "[MainWindow] Skipped backward 10 seconds to";
    } else {
        // Skips to beginning of track
        player->setPosition(0);
        qDebug() << "[MainWindow] Skipped to beginning of track";
    }
}

/*****************************************************************************\
    Function: onMediaStatusChanged
    Description: Handles media status changes, auto-plays next song when current ends
    Parameters:
        QMediaPlayer::MediaStatus status -> current media status
    Returns: nothing
\*****************************************************************************/
void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    /**********************************\
        CHECK FOR END OF MEDIA
    \**********************************/
    
    // Checks if song ended and auto-play is enabled
    if (status == QMediaPlayer::EndOfMedia && currentSongIndex >= 0 && !currentPlaylist.isEmpty()) {
        qDebug() << "[MainWindow] Song ended, playing next in playlist";
        
        // Plays next song in playlist
        playNextSongInPlaylist();
    }
}

/*****************************************************************************\
    Function: playNextSongInPlaylist
    Description: Plays the next song in the current playlist, loops to start if at end
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void MainWindow::playNextSongInPlaylist()
{
    int totalSongs = ui->mainPlaylistTree->topLevelItemCount();
    
    if (totalSongs == 0) {
        qDebug() << "[MainWindow] No songs in playlist";
        return;
    }
    
    // If only one song, replay it
    if (totalSongs == 1) {
        qDebug() << "[MainWindow] Only one song in playlist, replaying";
        QTreeWidgetItem *item = ui->mainPlaylistTree->topLevelItem(0);
        QString filePath = item->data(0, Qt::UserRole + 1).toString();
        
        if (QFile::exists(filePath)) {
            ytdlpManager.getMediaPlayer()->setSource(QUrl::fromLocalFile(filePath));
            ytdlpManager.getMediaPlayer()->play();
            currentSongIndex = 0;
        }
        return;
    }
    
    // Move to next song, loop back to start if at end
    currentSongIndex++;
    if (currentSongIndex >= totalSongs) {
        currentSongIndex = 0;
        qDebug() << "[MainWindow] Reached end of playlist, looping to first song";
    }
    
    QTreeWidgetItem *nextItem = ui->mainPlaylistTree->topLevelItem(currentSongIndex);
    if (!nextItem) {
        qDebug() << "[MainWindow] Could not find next item";
        return;
    }
    
    QString filePath = nextItem->data(0, Qt::UserRole + 1).toString();
    qDebug() << "[MainWindow] Playing next song:" << nextItem->text(1);
    qDebug() << "  FilePath:" << filePath;
    
    if (QFile::exists(filePath)) {
        ytdlpManager.getMediaPlayer()->setSource(QUrl::fromLocalFile(filePath));
        ytdlpManager.getMediaPlayer()->play();
        
        ui->songNameBox->setText(nextItem->text(1));
        ui->playlistBoxName->setText(currentPlaylist);  // Show playlist name
        
        // Load thumbnail
        QString pngPath = filePath;
        pngPath.replace(".mp3", ".png");
        
        if (QFile::exists(pngPath)) {
            QPixmap pixmap(pngPath);
            ui->songImage->setPixmap(pixmap.scaled(ui->songImage->size(),
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
        } else {
            ui->songImage->setPixmap(QPixmap(":/icons/music.png"));
        }
        
        ui->stopButton->setText("Stop");
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
    } else {
        QMessageBox::warning(this, "Error", "File not found!\n\n" + filePath);
    }
}

/*****************************************************************************\
    Function: on_sortSelection_currentIndexChanged
    Description: Sorts playlist songs based on selected criteria
    Parameters:
        int index -> selected sort option index
    Returns: nothing
\*****************************************************************************/
void MainWindow::on_sortSelection_currentIndexChanged(int index)
{
    // Helper function to parse "MM:SS" duration format
    auto parseDuration = [](const QString &duration) -> int {
        QStringList parts = duration.split(":");
        if (parts.size() == 2) {
            return parts[0].toInt() * 60 + parts[1].toInt(); // convert to total seconds
        }
        return duration.toInt(); // fallback if format unexpected
    };

    // Extract existing items and their icon pixmaps
    struct ItemData {
        QTreeWidgetItem *item;
        QPixmap iconPixmap;
    };
    
    QList<ItemData> itemDataList;
    
    while (ui->mainPlaylistTree->topLevelItemCount() > 0) {
        QTreeWidgetItem *item = ui->mainPlaylistTree->takeTopLevelItem(0);
        
        // Get the pixmap from the widget before it's destroyed
        QPixmap pixmap;
        QWidget *widget = ui->mainPlaylistTree->itemWidget(item, 0);
        if (widget) {
            QLabel *label = qobject_cast<QLabel*>(widget);
            if (label) {
                pixmap = label->pixmap(Qt::ReturnByValue);
            }
        }
        
        itemDataList.append({item, pixmap});
    }

    // Apply selected sorting method
    switch (index)
    {
    case 0: // Title A-Z
        std::sort(itemDataList.begin(), itemDataList.end(),
                  [](const ItemData &a, const ItemData &b) {
                      return a.item->text(1).compare(b.item->text(1), Qt::CaseInsensitive) < 0;
                  });
        break;

    case 1: // Title Z-A
        std::sort(itemDataList.begin(), itemDataList.end(),
                  [](const ItemData &a, const ItemData &b) {
                      return a.item->text(1).compare(b.item->text(1), Qt::CaseInsensitive) > 0;
                  });
        break;

    case 2: // Duration Short → Long
        std::sort(itemDataList.begin(), itemDataList.end(),
                  [&](const ItemData &a, const ItemData &b) {
                      return parseDuration(a.item->text(3)) < parseDuration(b.item->text(3));
                  });
        break;

    case 3: // Duration Long → Short
        std::sort(itemDataList.begin(), itemDataList.end(),
                  [&](const ItemData &a, const ItemData &b) {
                      return parseDuration(a.item->text(3)) > parseDuration(b.item->text(3));
                  });
        break;

    case 4: // Random Shuffle
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(itemDataList.begin(), itemDataList.end(), g);
    }
    break;
    }

    // Re-insert sorted items back into the tree and recreate their icon widgets
    for (const auto &data : itemDataList) {
        ui->mainPlaylistTree->addTopLevelItem(data.item);
        
        // Recreate the icon label widget
        if (!data.iconPixmap.isNull()) {
            QLabel *iconLabel = new QLabel(ui->mainPlaylistTree);
            iconLabel->setPixmap(data.iconPixmap);
            iconLabel->setAlignment(Qt::AlignCenter);
            ui->mainPlaylistTree->setItemWidget(data.item, 0, iconLabel);
        }
    }
}
