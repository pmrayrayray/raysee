#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
/*If not do include,generate error incomplete type*/
//class QDir;
//class QFileInfo;
//class QFileInfoList;




class QAction;
class QLabel;
class ImageSpace;
class QSplitter;

class QFrame;
class QHBoxLayout;
class EdgeSetting;
class RgbDialog;
class HsvDialog;
class HslDialog;

class QShortcut;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();



protected:
    void closeEvent(QCloseEvent *event);
signals:
    void appQuit();
    void edgePram(int lt,int ht, int ks);
    void rgbPram(int r,int g,int b);
    void hsvPram(int h,int s,int v);
    void hslPram(int h,int s,int v);


private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void print();

    bool next();
    bool previous();
    void fullScreen();
    void exitFullScreen();

    void openEdge();
    void closeEdge();
    void openRgb();
    void closeRgb();
    void openHsv();
    void closeHsv();
    void openHsl();
    void closeHsl();


    void about();
    void close();
    void exit();


    void updateStatusBar();
    void imageModified();

    void emitEdgePram();
    void emitRgbPram();
    void emitHsvPram();
    void emitHslPram();

    void redoToggle();
    void undoToggle();



private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QHBoxLayout *leftSpaceLayout;
    ImageSpace *imageSpace;
    QFrame *leftSpace;
    //EdgeSetting *edgeSetting;
    QString curFileName;
    QLabel *statusLabel;
    QLabel *fileNameLabel;
    QLabel *fileSizeLabel;
    QLabel *fileTypeLabel;

    QSplitter *splitter;
    QDir curFileDir;
    QFileInfo curFileInfo;
    QList<QString> curFileList;

    QShortcut *fsShortcut;
    QShortcut *fsNextShortcut;
    QShortcut *fsPreShortcut;






    QAction *separatorAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *zoomMenu;

    QMenu *procMenu;
    QMenu *colorMenu;

    QMenu *aboutMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *viewToolBar;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *closeAction;

    QAction *printAction;
    QAction *exitAction;

    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;

    QAction *nextAction;
    QAction *previousAction;
    QAction *zoominAction;
    QAction *zoomoutAction;
    //QAction *zoomtoAction;
    QAction *rcwAction;
    QAction *rccwAction;
    QAction *fullScreenAction;

    QAction *grayAction;
    QAction *rgbAction;
    QAction *hsvAction;
    QAction *hslAction;
    QAction *edgedetectionAction;


    QAction *aboutAction;
public:
    EdgeSetting *edgeSetting;
    RgbDialog *rgbDialog;
    HsvDialog *hsvDialog;
    HslDialog *hslDialog;


};

#endif
