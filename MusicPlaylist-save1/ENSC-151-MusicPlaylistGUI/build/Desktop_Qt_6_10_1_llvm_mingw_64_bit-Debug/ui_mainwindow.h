/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *mainFrame;
    QFrame *navigation;
    QFrame *list;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_8;
    QFrame *frame_11;
    QLineEdit *lineEdit_5;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_9;
    QFrame *frame_12;
    QLineEdit *lineEdit_6;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_5;
    QFrame *frame_10;
    QFrame *frame_13;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QFrame *top;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QFrame *playbar;
    QPushButton *pushButton_2;
    QProgressBar *progressBar;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLineEdit *lineEdit_8;
    QLineEdit *lineEdit_9;
    QFrame *frame_14;
    QFrame *frame_15;
    QLineEdit *lineEdit_12;
    QLineEdit *lineEdit_13;
    QStackedWidget *stackedWidget;
    QWidget *page_3;
    QFrame *maining;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QLineEdit *lineEdit_2;
    QFrame *frame_2;
    QFrame *frame_4;
    QLineEdit *lineEdit_3;
    QFrame *frame_6;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QFrame *frame_3;
    QFrame *frame_5;
    QLineEdit *lineEdit_4;
    QFrame *frame_7;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_3;
    QWidget *page_4;
    QFrame *frame_16;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_3;
    QVBoxLayout *verticalLayout_6;
    QFrame *frame_17;
    QFrame *frame_18;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_15;
    QFrame *frame_19;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 720);
        MainWindow->setMinimumSize(QSize(1280, 720));
        MainWindow->setMaximumSize(QSize(1280, 720));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        centralwidget->setMinimumSize(QSize(1280, 720));
        centralwidget->setMaximumSize(QSize(1280, 720));
        mainFrame = new QFrame(centralwidget);
        mainFrame->setObjectName("mainFrame");
        mainFrame->setGeometry(QRect(0, 0, 1281, 721));
        mainFrame->setFrameShape(QFrame::Shape::StyledPanel);
        mainFrame->setFrameShadow(QFrame::Shadow::Raised);
        navigation = new QFrame(mainFrame);
        navigation->setObjectName("navigation");
        navigation->setGeometry(QRect(0, 0, 261, 721));
        navigation->setFrameShape(QFrame::Shape::StyledPanel);
        navigation->setFrameShadow(QFrame::Shadow::Raised);
        list = new QFrame(navigation);
        list->setObjectName("list");
        list->setGeometry(QRect(0, 189, 261, 531));
        list->setFrameShape(QFrame::Shape::StyledPanel);
        list->setFrameShadow(QFrame::Shadow::Raised);
        scrollArea_2 = new QScrollArea(list);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setGeometry(QRect(0, 0, 261, 531));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName("scrollAreaWidgetContents_2");
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 259, 529));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        frame_8 = new QFrame(scrollAreaWidgetContents_2);
        frame_8->setObjectName("frame_8");
        frame_8->setFrameShape(QFrame::Shape::StyledPanel);
        frame_8->setFrameShadow(QFrame::Shadow::Raised);
        frame_11 = new QFrame(frame_8);
        frame_11->setObjectName("frame_11");
        frame_11->setGeometry(QRect(0, 0, 241, 41));
        frame_11->setFrameShape(QFrame::Shape::StyledPanel);
        frame_11->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_5 = new QLineEdit(frame_11);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(10, 10, 113, 24));
        verticalLayoutWidget = new QWidget(frame_8);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 40, 241, 131));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);

        verticalLayout_2->addWidget(frame_8);

        frame_9 = new QFrame(scrollAreaWidgetContents_2);
        frame_9->setObjectName("frame_9");
        frame_9->setFrameShape(QFrame::Shape::StyledPanel);
        frame_9->setFrameShadow(QFrame::Shadow::Raised);
        frame_12 = new QFrame(frame_9);
        frame_12->setObjectName("frame_12");
        frame_12->setGeometry(QRect(0, 0, 241, 41));
        frame_12->setFrameShape(QFrame::Shape::StyledPanel);
        frame_12->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_6 = new QLineEdit(frame_12);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(10, 10, 113, 24));
        verticalLayoutWidget_2 = new QWidget(frame_9);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(0, 40, 241, 131));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);

        verticalLayout_2->addWidget(frame_9);

        frame_10 = new QFrame(scrollAreaWidgetContents_2);
        frame_10->setObjectName("frame_10");
        frame_10->setFrameShape(QFrame::Shape::StyledPanel);
        frame_10->setFrameShadow(QFrame::Shadow::Raised);
        frame_13 = new QFrame(frame_10);
        frame_13->setObjectName("frame_13");
        frame_13->setGeometry(QRect(0, 0, 241, 41));
        frame_13->setFrameShape(QFrame::Shape::StyledPanel);
        frame_13->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_7 = new QLineEdit(frame_13);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(10, 10, 113, 24));
        lineEdit_10 = new QLineEdit(frame_10);
        lineEdit_10->setObjectName("lineEdit_10");
        lineEdit_10->setGeometry(QRect(10, 50, 181, 21));
        lineEdit_11 = new QLineEdit(frame_10);
        lineEdit_11->setObjectName("lineEdit_11");
        lineEdit_11->setGeometry(QRect(10, 80, 181, 21));

        verticalLayout_2->addWidget(frame_10);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        top = new QFrame(navigation);
        top->setObjectName("top");
        top->setGeometry(QRect(0, 0, 261, 191));
        top->setFrameShape(QFrame::Shape::StyledPanel);
        top->setFrameShadow(QFrame::Shadow::Raised);
        pushButton = new QPushButton(top);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(0, 70, 261, 41));
        lineEdit = new QLineEdit(top);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(0, 130, 261, 51));
        playbar = new QFrame(mainFrame);
        playbar->setObjectName("playbar");
        playbar->setGeometry(QRect(260, 619, 1021, 101));
        playbar->setFrameShape(QFrame::Shape::StyledPanel);
        playbar->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_2 = new QPushButton(playbar);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(560, 10, 71, 51));
        progressBar = new QProgressBar(playbar);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(340, 70, 511, 23));
        progressBar->setValue(24);
        pushButton_3 = new QPushButton(playbar);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(650, 20, 41, 31));
        pushButton_4 = new QPushButton(playbar);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(500, 20, 41, 31));
        lineEdit_8 = new QLineEdit(playbar);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(280, 70, 41, 24));
        lineEdit_9 = new QLineEdit(playbar);
        lineEdit_9->setObjectName("lineEdit_9");
        lineEdit_9->setGeometry(QRect(870, 70, 41, 24));
        frame_14 = new QFrame(playbar);
        frame_14->setObjectName("frame_14");
        frame_14->setGeometry(QRect(0, 0, 261, 101));
        frame_14->setFrameShape(QFrame::Shape::StyledPanel);
        frame_14->setFrameShadow(QFrame::Shadow::Raised);
        frame_15 = new QFrame(frame_14);
        frame_15->setObjectName("frame_15");
        frame_15->setGeometry(QRect(10, 10, 101, 81));
        frame_15->setFrameShape(QFrame::Shape::StyledPanel);
        frame_15->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_12 = new QLineEdit(frame_14);
        lineEdit_12->setObjectName("lineEdit_12");
        lineEdit_12->setGeometry(QRect(130, 60, 113, 24));
        lineEdit_13 = new QLineEdit(frame_14);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(130, 20, 113, 24));
        stackedWidget = new QStackedWidget(mainFrame);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(260, 0, 1021, 621));
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        maining = new QFrame(page_3);
        maining->setObjectName("maining");
        maining->setGeometry(QRect(0, 0, 1021, 621));
        maining->setFrameShape(QFrame::Shape::StyledPanel);
        maining->setFrameShadow(QFrame::Shadow::Raised);
        scrollArea = new QScrollArea(maining);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setGeometry(QRect(0, 0, 1021, 621));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1019, 619));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName("verticalLayout");
        frame = new QFrame(scrollAreaWidgetContents);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_2 = new QLineEdit(frame);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(10, 10, 371, 51));

        verticalLayout->addWidget(frame);

        frame_2 = new QFrame(scrollAreaWidgetContents);
        frame_2->setObjectName("frame_2");
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        frame_4 = new QFrame(frame_2);
        frame_4->setObjectName("frame_4");
        frame_4->setGeometry(QRect(0, 0, 1001, 51));
        frame_4->setFrameShape(QFrame::Shape::StyledPanel);
        frame_4->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_3 = new QLineEdit(frame_4);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(10, 10, 113, 24));
        frame_6 = new QFrame(frame_2);
        frame_6->setObjectName("frame_6");
        frame_6->setGeometry(QRect(0, 50, 1001, 211));
        frame_6->setFrameShape(QFrame::Shape::StyledPanel);
        frame_6->setFrameShadow(QFrame::Shadow::Raised);
        gridLayoutWidget = new QWidget(frame_6);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(0, 0, 1001, 211));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(frame_2);

        frame_3 = new QFrame(scrollAreaWidgetContents);
        frame_3->setObjectName("frame_3");
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        frame_5 = new QFrame(frame_3);
        frame_5->setObjectName("frame_5");
        frame_5->setGeometry(QRect(0, 0, 1001, 51));
        frame_5->setFrameShape(QFrame::Shape::StyledPanel);
        frame_5->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_4 = new QLineEdit(frame_5);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(10, 10, 113, 24));
        frame_7 = new QFrame(frame_3);
        frame_7->setObjectName("frame_7");
        frame_7->setGeometry(QRect(0, 50, 1001, 211));
        frame_7->setFrameShape(QFrame::Shape::StyledPanel);
        frame_7->setFrameShadow(QFrame::Shadow::Raised);
        gridLayoutWidget_2 = new QWidget(frame_7);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(0, 0, 1001, 211));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(frame_3);

        scrollArea->setWidget(scrollAreaWidgetContents);
        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        frame_16 = new QFrame(page_4);
        frame_16->setObjectName("frame_16");
        frame_16->setGeometry(QRect(0, 0, 1021, 621));
        frame_16->setFrameShape(QFrame::Shape::StyledPanel);
        frame_16->setFrameShadow(QFrame::Shadow::Raised);
        scrollArea_3 = new QScrollArea(frame_16);
        scrollArea_3->setObjectName("scrollArea_3");
        scrollArea_3->setGeometry(QRect(0, 0, 1021, 621));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName("scrollAreaWidgetContents_3");
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 1019, 619));
        verticalLayout_6 = new QVBoxLayout(scrollAreaWidgetContents_3);
        verticalLayout_6->setObjectName("verticalLayout_6");
        frame_17 = new QFrame(scrollAreaWidgetContents_3);
        frame_17->setObjectName("frame_17");
        frame_17->setFrameShape(QFrame::Shape::StyledPanel);
        frame_17->setFrameShadow(QFrame::Shadow::Raised);
        frame_18 = new QFrame(frame_17);
        frame_18->setObjectName("frame_18");
        frame_18->setGeometry(QRect(30, 30, 261, 231));
        frame_18->setFrameShape(QFrame::Shape::StyledPanel);
        frame_18->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_14 = new QLineEdit(frame_17);
        lineEdit_14->setObjectName("lineEdit_14");
        lineEdit_14->setGeometry(QRect(320, 60, 461, 81));
        lineEdit_15 = new QLineEdit(frame_17);
        lineEdit_15->setObjectName("lineEdit_15");
        lineEdit_15->setGeometry(QRect(330, 230, 191, 24));

        verticalLayout_6->addWidget(frame_17);

        frame_19 = new QFrame(scrollAreaWidgetContents_3);
        frame_19->setObjectName("frame_19");
        frame_19->setFrameShape(QFrame::Shape::StyledPanel);
        frame_19->setFrameShadow(QFrame::Shadow::Raised);

        verticalLayout_6->addWidget(frame_19);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);
        stackedWidget->addWidget(page_4);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lineEdit_5->setText(QCoreApplication::translate("MainWindow", "Playlist", nullptr));
        lineEdit_6->setText(QCoreApplication::translate("MainWindow", "Songs", nullptr));
        lineEdit_7->setText(QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        lineEdit_10->setText(QCoreApplication::translate("MainWindow", "Playlist: 0", nullptr));
        lineEdit_11->setText(QCoreApplication::translate("MainWindow", "Songs: 0", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Home", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        lineEdit_12->setText(QCoreApplication::translate("MainWindow", "Song", nullptr));
        lineEdit_13->setText(QCoreApplication::translate("MainWindow", "Playlist", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("MainWindow", "Playlist", nullptr));
        lineEdit_4->setText(QCoreApplication::translate("MainWindow", "Songs", nullptr));
        lineEdit_14->setText(QCoreApplication::translate("MainWindow", "Playlist name", nullptr));
        lineEdit_15->setText(QCoreApplication::translate("MainWindow", "# songs, 0 hr 0 min", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
