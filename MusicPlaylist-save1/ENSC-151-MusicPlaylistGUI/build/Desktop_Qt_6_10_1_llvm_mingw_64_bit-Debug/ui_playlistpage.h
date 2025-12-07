/********************************************************************************
** Form generated from reading UI file 'playlistpage.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYLISTPAGE_H
#define UI_PLAYLISTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_playlistpage
{
public:
    QFrame *frame;
    QFrame *navigation;
    QFrame *list_2;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_3;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame_14;
    QFrame *frame_15;
    QLineEdit *lineEdit_8;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_6;
    QFrame *frame_16;
    QFrame *frame_17;
    QLineEdit *lineEdit_9;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_7;
    QFrame *frame_18;
    QFrame *frame_19;
    QLineEdit *lineEdit_12;
    QLineEdit *lineEdit_13;
    QLineEdit *lineEdit_14;
    QFrame *top_2;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit_2;
    QFrame *playbar;
    QPushButton *pushButton_3;
    QProgressBar *progressBar;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QLineEdit *lineEdit_15;
    QLineEdit *lineEdit_16;
    QFrame *frame_20;
    QFrame *frame_21;
    QLineEdit *lineEdit_17;
    QLineEdit *lineEdit_18;
    QFrame *frame_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QFrame *frame_3;
    QFrame *frame_5;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QFrame *frame_4;

    void setupUi(QWidget *playlistpage)
    {
        if (playlistpage->objectName().isEmpty())
            playlistpage->setObjectName("playlistpage");
        playlistpage->resize(1280, 720);
        playlistpage->setMinimumSize(QSize(1280, 720));
        playlistpage->setMaximumSize(QSize(1280, 720));
        frame = new QFrame(playlistpage);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 1281, 721));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        navigation = new QFrame(frame);
        navigation->setObjectName("navigation");
        navigation->setGeometry(QRect(0, 0, 261, 721));
        navigation->setFrameShape(QFrame::Shape::StyledPanel);
        navigation->setFrameShadow(QFrame::Shadow::Raised);
        list_2 = new QFrame(navigation);
        list_2->setObjectName("list_2");
        list_2->setGeometry(QRect(0, 189, 261, 531));
        list_2->setFrameShape(QFrame::Shape::StyledPanel);
        list_2->setFrameShadow(QFrame::Shadow::Raised);
        scrollArea_3 = new QScrollArea(list_2);
        scrollArea_3->setObjectName("scrollArea_3");
        scrollArea_3->setGeometry(QRect(0, 0, 261, 531));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName("scrollAreaWidgetContents_3");
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 259, 529));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        frame_14 = new QFrame(scrollAreaWidgetContents_3);
        frame_14->setObjectName("frame_14");
        frame_14->setFrameShape(QFrame::Shape::StyledPanel);
        frame_14->setFrameShadow(QFrame::Shadow::Raised);
        frame_15 = new QFrame(frame_14);
        frame_15->setObjectName("frame_15");
        frame_15->setGeometry(QRect(0, 0, 241, 41));
        frame_15->setFrameShape(QFrame::Shape::StyledPanel);
        frame_15->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_8 = new QLineEdit(frame_15);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(10, 10, 113, 24));
        verticalLayoutWidget_3 = new QWidget(frame_14);
        verticalLayoutWidget_3->setObjectName("verticalLayoutWidget_3");
        verticalLayoutWidget_3->setGeometry(QRect(0, 40, 241, 131));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);

        verticalLayout_4->addWidget(frame_14);

        frame_16 = new QFrame(scrollAreaWidgetContents_3);
        frame_16->setObjectName("frame_16");
        frame_16->setFrameShape(QFrame::Shape::StyledPanel);
        frame_16->setFrameShadow(QFrame::Shadow::Raised);
        frame_17 = new QFrame(frame_16);
        frame_17->setObjectName("frame_17");
        frame_17->setGeometry(QRect(0, 0, 241, 41));
        frame_17->setFrameShape(QFrame::Shape::StyledPanel);
        frame_17->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_9 = new QLineEdit(frame_17);
        lineEdit_9->setObjectName("lineEdit_9");
        lineEdit_9->setGeometry(QRect(10, 10, 113, 24));
        verticalLayoutWidget_4 = new QWidget(frame_16);
        verticalLayoutWidget_4->setObjectName("verticalLayoutWidget_4");
        verticalLayoutWidget_4->setGeometry(QRect(0, 40, 241, 131));
        verticalLayout_7 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_7->setObjectName("verticalLayout_7");
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);

        verticalLayout_4->addWidget(frame_16);

        frame_18 = new QFrame(scrollAreaWidgetContents_3);
        frame_18->setObjectName("frame_18");
        frame_18->setFrameShape(QFrame::Shape::StyledPanel);
        frame_18->setFrameShadow(QFrame::Shadow::Raised);
        frame_19 = new QFrame(frame_18);
        frame_19->setObjectName("frame_19");
        frame_19->setGeometry(QRect(0, 0, 241, 41));
        frame_19->setFrameShape(QFrame::Shape::StyledPanel);
        frame_19->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_12 = new QLineEdit(frame_19);
        lineEdit_12->setObjectName("lineEdit_12");
        lineEdit_12->setGeometry(QRect(10, 10, 113, 24));
        lineEdit_13 = new QLineEdit(frame_18);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(10, 50, 181, 21));
        lineEdit_14 = new QLineEdit(frame_18);
        lineEdit_14->setObjectName("lineEdit_14");
        lineEdit_14->setGeometry(QRect(10, 80, 181, 21));

        verticalLayout_4->addWidget(frame_18);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);
        top_2 = new QFrame(navigation);
        top_2->setObjectName("top_2");
        top_2->setGeometry(QRect(0, 0, 261, 191));
        top_2->setFrameShape(QFrame::Shape::StyledPanel);
        top_2->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_2 = new QPushButton(top_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(0, 70, 261, 41));
        lineEdit_2 = new QLineEdit(top_2);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(0, 130, 261, 51));
        playbar = new QFrame(frame);
        playbar->setObjectName("playbar");
        playbar->setGeometry(QRect(260, 620, 1021, 101));
        playbar->setFrameShape(QFrame::Shape::StyledPanel);
        playbar->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_3 = new QPushButton(playbar);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(560, 10, 71, 51));
        progressBar = new QProgressBar(playbar);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(340, 70, 511, 23));
        progressBar->setValue(24);
        pushButton_4 = new QPushButton(playbar);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(650, 20, 41, 31));
        pushButton_5 = new QPushButton(playbar);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(500, 20, 41, 31));
        lineEdit_15 = new QLineEdit(playbar);
        lineEdit_15->setObjectName("lineEdit_15");
        lineEdit_15->setGeometry(QRect(280, 70, 41, 24));
        lineEdit_16 = new QLineEdit(playbar);
        lineEdit_16->setObjectName("lineEdit_16");
        lineEdit_16->setGeometry(QRect(870, 70, 41, 24));
        frame_20 = new QFrame(playbar);
        frame_20->setObjectName("frame_20");
        frame_20->setGeometry(QRect(0, 0, 261, 101));
        frame_20->setFrameShape(QFrame::Shape::StyledPanel);
        frame_20->setFrameShadow(QFrame::Shadow::Raised);
        frame_21 = new QFrame(frame_20);
        frame_21->setObjectName("frame_21");
        frame_21->setGeometry(QRect(10, 10, 101, 81));
        frame_21->setFrameShape(QFrame::Shape::StyledPanel);
        frame_21->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_17 = new QLineEdit(frame_20);
        lineEdit_17->setObjectName("lineEdit_17");
        lineEdit_17->setGeometry(QRect(130, 60, 113, 24));
        lineEdit_18 = new QLineEdit(frame_20);
        lineEdit_18->setObjectName("lineEdit_18");
        lineEdit_18->setGeometry(QRect(130, 20, 113, 24));
        frame_2 = new QFrame(frame);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(260, 0, 1021, 621));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        scrollArea = new QScrollArea(frame_2);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setGeometry(QRect(0, 0, 1021, 621));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1019, 619));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName("verticalLayout");
        frame_3 = new QFrame(scrollAreaWidgetContents);
        frame_3->setObjectName("frame_3");
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        frame_5 = new QFrame(frame_3);
        frame_5->setObjectName("frame_5");
        frame_5->setGeometry(QRect(30, 30, 261, 231));
        frame_5->setFrameShape(QFrame::Shape::StyledPanel);
        frame_5->setFrameShadow(QFrame::Shadow::Raised);
        lineEdit_3 = new QLineEdit(frame_3);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(320, 60, 461, 81));
        lineEdit_4 = new QLineEdit(frame_3);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(330, 230, 191, 24));

        verticalLayout->addWidget(frame_3);

        frame_4 = new QFrame(scrollAreaWidgetContents);
        frame_4->setObjectName("frame_4");
        frame_4->setFrameShape(QFrame::Shape::StyledPanel);
        frame_4->setFrameShadow(QFrame::Shadow::Raised);

        verticalLayout->addWidget(frame_4);

        scrollArea->setWidget(scrollAreaWidgetContents);

        retranslateUi(playlistpage);

        QMetaObject::connectSlotsByName(playlistpage);
    } // setupUi

    void retranslateUi(QWidget *playlistpage)
    {
        playlistpage->setWindowTitle(QCoreApplication::translate("playlistpage", "Form", nullptr));
        lineEdit_8->setText(QCoreApplication::translate("playlistpage", "Playlist", nullptr));
        lineEdit_9->setText(QCoreApplication::translate("playlistpage", "Songs", nullptr));
        lineEdit_12->setText(QCoreApplication::translate("playlistpage", "Statistics", nullptr));
        lineEdit_13->setText(QCoreApplication::translate("playlistpage", "Playlist: 0", nullptr));
        lineEdit_14->setText(QCoreApplication::translate("playlistpage", "Songs: 0", nullptr));
        pushButton_2->setText(QCoreApplication::translate("playlistpage", "Home", nullptr));
        pushButton_3->setText(QCoreApplication::translate("playlistpage", "PushButton", nullptr));
        pushButton_4->setText(QCoreApplication::translate("playlistpage", "PushButton", nullptr));
        pushButton_5->setText(QCoreApplication::translate("playlistpage", "PushButton", nullptr));
        lineEdit_17->setText(QCoreApplication::translate("playlistpage", "Song", nullptr));
        lineEdit_18->setText(QCoreApplication::translate("playlistpage", "Playlist", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("playlistpage", "Playlist name", nullptr));
        lineEdit_4->setText(QCoreApplication::translate("playlistpage", "# songs, 0 hr 0 min", nullptr));
    } // retranslateUi

};

namespace Ui {
    class playlistpage: public Ui_playlistpage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYLISTPAGE_H
