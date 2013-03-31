#include <QtGui>
#include <QImage>
#include <Qpixmap>
#include <QList>

#include <QShortcut>
#include <QDebug>



#include "edgesetting.h"
#include "rgbdialog.h"
#include "hsvdialog.h"
#include "hsldialog.h"
#include "imageSpace.h"
#include "mainwindow.h"



MainWindow::MainWindow()
{


    splitter = new QSplitter(Qt::Horizontal);
    imageSpace = new ImageSpace;
    imageSpace->setFocusPolicy(Qt::ClickFocus);

    leftSpace = new QFrame;
    leftSpaceLayout =new QHBoxLayout;
    leftSpace->setLayout(leftSpaceLayout);

    leftSpace->setFrameShape(QFrame::Panel);
    leftSpace->setFrameShadow(QFrame::Plain);
    leftSpace->setLineWidth(1);
    leftSpace->setMidLineWidth(1);

    splitter->insertWidget(0,leftSpace);

    splitter->insertWidget(1,imageSpace);
    leftSpace->setVisible(false);
    QList<int> childrensize;
    childrensize.append(10);
    childrensize.append(100);
    splitter->setSizes(childrensize);

    //imageSpace->setStyleSheet("background-color: black;");
    splitter->setStyleSheet("background-color: white ;");
    leftSpace->setStyleSheet("background-color: rgb(240,240,240) ;");
    //splitter->setStretchFactor(1, 1);
    //splitter->setStretchFactor(0, 0);

    setCentralWidget(splitter);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();


    setWindowIcon(QIcon(":/images/snail2.png"));

    curFileDir=QDir::currentPath();
    readSettings();
    if (curFileName!="")
    curFileDir.setPath(curFileName);
    setCurrentFile("");





}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        writeSettings();
        exit();
        //event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (okToContinue()) {
        //spreadsheet->clear();
        //setCurrentFile("");
    }
}

void MainWindow::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Image"), curFileDir.absolutePath(),
                                   tr("All Files(*.*)"));
        if (!fileName.isEmpty())
        {
            loadFile(fileName);

            //curFileDir.setCurrent();
        }
    }
}

bool MainWindow::save()
{
    if (curFileName.isEmpty()) {
        return saveAs();
    }
    else {
        return saveFile(curFileName);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                               tr("Save Image"), ".",
                               tr("JPG Files(*.jpg);;JPEG Files(*.jpeg);;BMP Files(*.bmp);;"
                                "PNG Files(*.png);;GIF Files(*.gif);;TIFF Files(*.tiff)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::print()
{
    QPrinter printer;
    QPixmap pix;
    QPainter painter;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Image"));

    if (dialog->exec() == QDialog::Accepted)
    {
        //printer.setOutputFormat(QPrinter::PdfFormat);
        //printer.setOutputFileName("output.pdf");
        pix=QPixmap::grabWidget(imageSpace);
        pix=pix.scaled(QSize(printer.pageRect().width(),printer.pageRect().height())
                   ,Qt::KeepAspectRatio);
        painter.begin(&printer);
        painter.drawPixmap(0,0,pix,0,0,0,0);
        painter.end();
    }
   delete dialog;
}

bool MainWindow::next()
{


    QString nextFileName;

    int curFileIndex;

    curFileIndex=curFileList.indexOf(strippedName(curFileName));
    if ((curFileIndex+1)<curFileList.size())
            nextFileName=curFileList.at((curFileIndex+1));
    else nextFileName=curFileList.at(0);
    nextFileName=curFileDir.absoluteFilePath(nextFileName);
    //nextFileName=QFileInfo(nextFileName).absoluteFilePath();
    if (okToContinue())
    {
    //imageSpace->imageClear();
    //setCurrentFile("");


    loadFile(nextFileName);


    }
    return true;
}

bool MainWindow::previous()
{
    QString preFileName;

    int curFileIndex;

    curFileIndex=curFileList.indexOf(strippedName(curFileName));
    if ((curFileIndex-1)>=0)
            preFileName=curFileList.at((curFileIndex-1));
    else preFileName=curFileList.at(curFileList.size()-1);
    preFileName=curFileDir.absoluteFilePath(preFileName);
    //nextFileName=nextFile.absoluteFilePath();
    if (okToContinue())
    {
    //imageSpace->imageClear();
    //setCurrentFile("");

    loadFile(preFileName);
}

 return true;
}


void MainWindow::fullScreen()
{

    //menuBar()->setVisible(false);
    //statusBar()->setVisible(false);
    //imageSpace->setWindowFlags(Qt::Dialog);
    //imageSpace->setWidgetResizable(false);
    imageSpace->imageShow();
    imageSpace->takeWidget();
    imageSpace->imageLabel->setWindowFlags(Qt::Window);

    imageSpace->imageLabel->showFullScreen();


    //imageSpace->setWindowFlags(Qt::Window);
    //imageSpace->showFullScreen();

    fsShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    fsShortcut->setContext(Qt::ApplicationShortcut);
    connect(fsShortcut, SIGNAL(activated()), this, SLOT(exitFullScreen()));

    fsNextShortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    fsNextShortcut->setContext(Qt::ApplicationShortcut);
    connect(fsNextShortcut, SIGNAL(activated()), this, SLOT(next()));

    fsPreShortcut = new QShortcut(QKeySequence(Qt::Key_Right), this);
    fsPreShortcut->setContext(Qt::ApplicationShortcut);
    connect(fsPreShortcut, SIGNAL(activated()), this, SLOT(previous()));

    QApplication::setOverrideCursor( QCursor( Qt::BlankCursor ) );




}

void MainWindow::exitFullScreen()

{
    imageSpace->setWidget(imageSpace->imageLabel);
    imageSpace->imageLabel->setWindowFlags(Qt::SubWindow);

    imageSpace->imageLabel->showNormal();
    delete fsShortcut;
    delete fsNextShortcut;
    delete fsPreShortcut;

    QApplication::restoreOverrideCursor();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Raysee"),
            tr("<h2>Raysee 1.1</h2>"
               "<p>Copyright &copy; 2011 Raymond Huang."
               "<p>Raysee is a small imageprocess software "
                ));
}

void MainWindow::close()
{
    if (okToContinue())
    {
    imageSpace->imageClear();
    setCurrentFile("");
}
    //imageModified();

}

void MainWindow::exit()
{
    emit appQuit();
    delete imageSpace->imageData;
    delete imageSpace->imageDisp;
    delete imageSpace->imageBackup;
}


void MainWindow::openEdge()
{


    edgeSetting = new EdgeSetting;

    //leftSpace->setStyleSheet("background-color: white;");

    leftSpaceLayout->addWidget(edgeSetting,1,Qt::AlignTop);
    leftSpace->setVisible(true);

    connect(edgeSetting->applyButton, SIGNAL(clicked()), this, SLOT(emitEdgePram()));


    connect(edgeSetting->closeButton, SIGNAL(clicked()), this, SLOT(closeEdge()));

    connect(this,SIGNAL(edgePram(int ,int ,int )),imageSpace,SLOT(doEdge(int ,int ,int )));

    procMenu->setEnabled(false);


}

void MainWindow::closeEdge()
{
    leftSpaceLayout->removeWidget(edgeSetting);
    leftSpace->setVisible(false);
    //disconnect(edgeSetting->applyButton, SIGNAL(clicked()), this, SLOT(emitEdgePram()));
    //disconnect(edgeSetting->closeButton, SIGNAL(clicked()), this, SLOT(closeEdge()));

    disconnect(this,SIGNAL(edgePram(int ,int ,int )),imageSpace,SLOT(doEdge(int ,int ,int )));
    delete edgeSetting;
    procMenu->setEnabled(true);

    update();
}

void MainWindow::emitEdgePram()
{
    int lt,ht,ks;
    lt=edgeSetting->ltspinBox->value();
    ht=edgeSetting->htspinBox->value();
    ks=edgeSetting->ksspinBox->value();
    emit edgePram(lt,ht,ks);

}

void MainWindow::emitRgbPram()
{
    int r,g,b;
    r=rgbDialog->redSpin->value();
    g=rgbDialog->greenSpin->value();
    b=rgbDialog->blueSpin->value();

    emit rgbPram(r,g,b);

}

void MainWindow::emitHsvPram()
{
    int h,s,v;
    h=hsvDialog->hueSpin->value();
    s=hsvDialog->satuSpin->value();
    v=hsvDialog->valSpin->value();

    emit hsvPram(h,s,v);

}

void MainWindow::emitHslPram()
{
    int h,s,l;
    h=hslDialog->hueSpin->value();
    s=hslDialog->satuSpin->value();
    l=hslDialog->lightSpin->value();

    emit hslPram(h,s,l);

}

void MainWindow::openRgb()
{
    rgbDialog= new RgbDialog;
    leftSpaceLayout->addWidget(rgbDialog,1,Qt::AlignTop);
    leftSpace->setVisible(true);


    connect(rgbDialog->redSlider, SIGNAL(sliderReleased()), this, SLOT(emitRgbPram()));
    connect(rgbDialog->greenSlider, SIGNAL(sliderReleased()), this, SLOT(emitRgbPram()));
    connect(rgbDialog->blueSlider, SIGNAL(sliderReleased()), this, SLOT(emitRgbPram()));

    connect(rgbDialog->redSpin, SIGNAL(editingFinished()), this, SLOT(emitRgbPram()));
    connect(rgbDialog->greenSpin, SIGNAL(editingFinished()), this, SLOT(emitRgbPram()));
    connect(rgbDialog->blueSpin, SIGNAL(editingFinished()), this, SLOT(emitRgbPram()));

    connect(this,SIGNAL(rgbPram(int ,int ,int )),imageSpace,SLOT(rgbAdjust(int ,int ,int )));

    connect(rgbDialog->closeButton,SIGNAL(clicked()),this,SLOT(closeRgb()));
    connect(rgbDialog->okButton,SIGNAL(clicked()),imageSpace,SLOT(applyRgb()));
    connect(rgbDialog->cancelButton,SIGNAL(clicked()),imageSpace,SLOT(cancelRgb()));


    procMenu->setEnabled(false);


}

void MainWindow::closeRgb()
{
    leftSpaceLayout->removeWidget(rgbDialog);
    leftSpace->setVisible(false);
    disconnect(this,SIGNAL(rgbPram(int ,int ,int )),imageSpace,SLOT(rgbAdjust(int ,int ,int )));
    delete rgbDialog;
    procMenu->setEnabled(true);
    update();

}

void MainWindow::openHsv()
{
    hsvDialog= new HsvDialog;
    leftSpaceLayout->addWidget(hsvDialog,1,Qt::AlignTop);
    leftSpace->setVisible(true);


    connect(hsvDialog->hueSlider, SIGNAL(sliderReleased()), this, SLOT(emitHsvPram()));
    connect(hsvDialog->satuSlider, SIGNAL(sliderReleased()), this, SLOT(emitHsvPram()));
    connect(hsvDialog->valSlider, SIGNAL(sliderReleased()), this, SLOT(emitHsvPram()));

    connect(hsvDialog->hueSpin, SIGNAL(editingFinished()), this, SLOT(emitHsvPram()));
    connect(hsvDialog->satuSpin, SIGNAL(editingFinished()), this, SLOT(emitHsvPram()));
    connect(hsvDialog->valSpin, SIGNAL(editingFinished()), this, SLOT(emitHsvPram()));


    connect(this,SIGNAL(hsvPram(int ,int ,int )),imageSpace,SLOT(hsvAdjust(int ,int ,int )));

    connect(hsvDialog->closeButton,SIGNAL(clicked()),this,SLOT(closeHsv()));
    connect(hsvDialog->okButton,SIGNAL(clicked()),imageSpace,SLOT(applyHsv()));
    connect(hsvDialog->cancelButton,SIGNAL(clicked()),imageSpace,SLOT(cancelHsv()));

    procMenu->setEnabled(false);
}

void MainWindow::closeHsv()
{
    leftSpaceLayout->removeWidget(hsvDialog);
    leftSpace->setVisible(false);
    disconnect(this,SIGNAL(hsvPram(int ,int ,int )),imageSpace,SLOT(hsvAdjust(int ,int ,int )));

    delete hsvDialog;
    procMenu->setEnabled(true);
    update();
}

void MainWindow::openHsl()
{
    hslDialog= new HslDialog;
    leftSpaceLayout->addWidget(hslDialog,1,Qt::AlignTop);
    leftSpace->setVisible(true);


    connect(hslDialog->hueSlider, SIGNAL(sliderReleased()), this, SLOT(emitHslPram()));
    connect(hslDialog->satuSlider, SIGNAL(sliderReleased()), this, SLOT(emitHslPram()));
    connect(hslDialog->lightSlider, SIGNAL(sliderReleased()), this, SLOT(emitHslPram()));

    connect(hslDialog->hueSpin, SIGNAL(editingFinished()), this, SLOT(emitHslPram()));
    connect(hslDialog->satuSpin, SIGNAL(editingFinished()), this, SLOT(emitHslPram()));
    connect(hslDialog->lightSpin, SIGNAL(editingFinished()), this, SLOT(emitHslPram()));


    connect(this,SIGNAL(hslPram(int ,int ,int )),imageSpace,SLOT(hslAdjust(int ,int ,int )));

    connect(hslDialog->closeButton,SIGNAL(clicked()),this,SLOT(closeHsl()));
    connect(hslDialog->okButton,SIGNAL(clicked()),imageSpace,SLOT(applyHsl()));
    connect(hslDialog->cancelButton,SIGNAL(clicked()),imageSpace,SLOT(cancelHsl()));

    procMenu->setEnabled(false);
}

void MainWindow::closeHsl()
{
    leftSpaceLayout->removeWidget(hslDialog);
    leftSpace->setVisible(false);
    disconnect(this,SIGNAL(hslPram(int ,int ,int )),imageSpace,SLOT(hslAdjust(int ,int ,int )));

    delete hslDialog;
    procMenu->setEnabled(true);
    update();
}
/*
void MainWindow::openRecentFile()
{
    if (okToContinue()) {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}
*/

void MainWindow::redoToggle()
{
    undoAction->setVisible(true);
    redoAction->setVisible(false);

}

void MainWindow::undoToggle()
{
    undoAction->setVisible(false);
    redoAction->setVisible(true);

}

void MainWindow::updateStatusBar()
{
    qint64 fileSize=curFileInfo.size()/1024;
    QString fs=QString::number(fileSize);
    QString w=QString::number(imageSpace->imageData->width());
    QString h=QString::number(imageSpace->imageData->height());
    QString d=QString::number(imageSpace->imageData->depth());
    fileNameLabel->setText(strippedName(curFileName)+"       ");

    fileSizeLabel->setText(fs+"KB"+"       ");
    fileTypeLabel->setText(w+"x"+h+"x"+d);

    //locationLabel->setText(spreadsheet->currentLocation());
    //formulaLabel->setText(spreadsheet->currentFormula());
}

void MainWindow::imageModified()
{
    setWindowModified(true);
    undoAction->setVisible(true);
    redoAction->setVisible(false);
    //setCurrentFile("");
    updateStatusBar();
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images1/Document.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new image file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setIcon(QIcon(":/images1/Folder Open.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing image file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images1/Save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the image"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setStatusTip(tr("Save the image under a new "
                                  "name"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeAction = new QAction(tr("Close..."), this);
    closeAction->setIcon(QIcon(":/images1/Close.png"));
    saveAsAction->setStatusTip(tr("Close the image file "
                                  ));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));



    printAction = new QAction(tr("Print..."), this);
    printAction->setIcon(QIcon(":/images1/Printer.png"));
    printAction->setStatusTip(tr("Print the image file "
                                  ));
    connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    undoAction = new QAction(tr("Undo..."), this);
    undoAction->setIcon(QIcon(":/images1/Action Undo.png"));
    undoAction->setStatusTip(tr("undo "));
    undoAction->setVisible(false);
    connect(undoAction, SIGNAL(triggered()), imageSpace, SLOT(undo()));
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undoToggle()));

    redoAction = new QAction(tr("Redo..."), this);
    redoAction->setIcon(QIcon(":/images1/Action Redo.png"));
    redoAction->setStatusTip(tr("Redo "));
    redoAction->setVisible(false);
    connect(redoAction, SIGNAL(triggered()), imageSpace, SLOT(redo()));
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redoToggle()));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/images1/Clipboard Cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut the current selection's contents "
                               "to the clipboard"));
    connect(cutAction, SIGNAL(triggered()), imageSpace, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/images1/Clipboard Copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy the current selection's contents "
                                "to the clipboard"));
    connect(copyAction, SIGNAL(triggered()), imageSpace, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/images1/Clipboard Paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste the clipboard's contents into "
                                 "the current selection"));
    connect(pasteAction, SIGNAL(triggered()),
            imageSpace, SLOT(paste()));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete the current selection's "
                                  "contents"));
    connect(deleteAction, SIGNAL(triggered()),
            imageSpace, SLOT(del()));

    nextAction = new QAction(tr("Next"), this);
    nextAction->setShortcut(tr("Right"));
    nextAction->setIcon(QIcon(":/images1/Arrow2 - Right.png"));
    nextAction->setStatusTip(tr("Show next image "
                                  ));
    connect(nextAction, SIGNAL(triggered()), this, SLOT(next()));

    previousAction = new QAction(tr("Previous"), this);
    previousAction->setShortcut(tr("Left"));
    previousAction->setIcon(QIcon(":/images1/Arrow2 - Left.png"));
    previousAction->setStatusTip(tr("Show previous image "
                                  ));
    connect(previousAction, SIGNAL(triggered()), this, SLOT(previous()));

    zoominAction = new QAction(tr("Zoom in"), this);
    zoominAction->setShortcut(tr("Ctrl++"));
    zoominAction->setIcon(QIcon(":/images1/Zoom In.png"));
    zoominAction->setStatusTip(tr("Zoom in the image "
                                  ));
    connect(zoominAction, SIGNAL(triggered()), imageSpace, SLOT(zoomin()));

    zoomoutAction = new QAction(tr("Zoom out"), this);
    zoomoutAction->setShortcut(tr("Ctrl+-"));
    zoomoutAction->setIcon(QIcon(":/images1/Zoom Out.png"));
    zoomoutAction->setStatusTip(tr("Zoom out the image "
                                  ));
    connect(zoomoutAction, SIGNAL(triggered()), imageSpace, SLOT(zoomout()));



    rcwAction = new QAction(tr("Rotate Clockwise"), this);
    rcwAction->setIcon(QIcon(":/images1/Rcw.png"));
    rcwAction->setStatusTip(tr("Rotate Clockwise 90 Degree"
                                  ));
    connect(rcwAction, SIGNAL(triggered()), imageSpace, SLOT(rotateCW()));

    rccwAction = new QAction(tr("Rotate Counter Clockwise"), this);
    rccwAction->setIcon(QIcon(":/images1/Rccw.png"));
    rccwAction->setStatusTip(tr("Rotate Counter Clockwise 90 Degree"
                                  ));
    connect(rccwAction, SIGNAL(triggered()), imageSpace, SLOT(rotateCCW()));


    fullScreenAction = new QAction(tr("Full Screen"), this);
    fullScreenAction->setShortcut(tr("Ctrl+F"));
    fullScreenAction->setIcon(QIcon(":/images1/Fullscreen.png"));
    fullScreenAction->setStatusTip(tr("Full Screen Mode"));
    connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(fullScreen()));



    rgbAction = new QAction(tr("RGB"), this);
    rgbAction->setStatusTip(tr("RGB Adjustion"));
    connect(rgbAction, SIGNAL(triggered()), this, SLOT(openRgb()));

    hsvAction = new QAction(tr("HSV"), this);
    hsvAction->setStatusTip(tr("HSV Adjustion"));
    connect(hsvAction, SIGNAL(triggered()), this, SLOT(openHsv()));

    hslAction = new QAction(tr("HSL"), this);
    hslAction->setStatusTip(tr("HSL Adjustion"));
    connect(hslAction, SIGNAL(triggered()), this, SLOT(openHsl()));

    edgedetectionAction = new QAction(tr("Edge Detection"), this);
    edgedetectionAction->setStatusTip(tr("Edge Detection"));
    connect(edgedetectionAction, SIGNAL(triggered()), this, SLOT(openEdge()));


    grayAction = new QAction(tr("To Gray"), this);
    grayAction->setStatusTip(tr("Convert to gray image"));
    connect(grayAction, SIGNAL(triggered()), imageSpace, SLOT(toGray()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));


}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(closeAction);

    fileMenu->addAction(printAction);


    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));

    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);



    viewMenu = menuBar()->addMenu(tr("&View"));


    viewMenu->addAction(previousAction);
    viewMenu->addAction(nextAction);
    viewMenu->addAction(rccwAction);
    viewMenu->addAction(rcwAction);
    viewMenu->addAction(zoominAction);
    viewMenu->addAction(zoomoutAction);
    //viewMenu->addAction(zoomtoAction);
    viewMenu->addAction(fullScreenAction);

    menuBar()->addSeparator();

    procMenu = menuBar()->addMenu(tr("Process"));
    procMenu->addAction(grayAction);
    colorMenu=procMenu->addMenu(tr("Color"));
    colorMenu->addAction(rgbAction);
    colorMenu->addAction(hsvAction);
    colorMenu->addAction(hslAction);


    procMenu->addAction(edgedetectionAction);



    aboutMenu = menuBar()->addMenu(tr("About"));
    aboutMenu->addAction(aboutAction);

}

void MainWindow::createContextMenu()
{
    imageSpace->addAction(cutAction);
    imageSpace->addAction(copyAction);
    imageSpace->addAction(pasteAction);
    imageSpace->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{

    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    fileToolBar->addAction(printAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);


    viewToolBar = addToolBar(tr("&View"));
    viewToolBar->addAction(previousAction);
    viewToolBar->addAction(nextAction);
    viewToolBar->addAction(rccwAction);
    viewToolBar->addAction(rcwAction);
    viewToolBar->addAction(zoominAction);
    viewToolBar->addAction(zoomoutAction);
    viewToolBar->addAction(fullScreenAction);
}



void MainWindow::createStatusBar()
{
    statusLabel = new QLabel(tr(" Ready       "));
    fileNameLabel = new QLabel;
    fileSizeLabel = new QLabel;
    fileTypeLabel = new QLabel;

    //fileLabel->setAlignment(Qt::AlignHCenter);
    //fileLabel->setMinimumSize(fileLabel->sizeHint());



    statusBar()->addWidget(statusLabel);
    statusBar()->addWidget(fileNameLabel);
    statusBar()->addWidget(fileSizeLabel);
    statusBar()->addWidget(fileTypeLabel);



    connect(imageSpace, SIGNAL(imageDataModified()),
            this, SLOT(imageModified()));

    updateStatusBar();
}

void MainWindow::readSettings()
{
    QSettings settings("Raymond Huang", "Raysee");

    restoreGeometry(settings.value("geometry").toByteArray());
    curFileName=settings.value("curFileName").toString();

    //recentFiles = settings.value("recentFiles").toStringList();
    //updateRecentFileActions();


}

void MainWindow::writeSettings()
{
    QSettings settings("Raymond Huang", "Raysee");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("curFileName",curFileName);
    //settings.setValue("recentFiles", recentFiles);
    //settings.setValue("showGrid", showGridAction->isChecked());
    //settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Image"),
                        tr("The Image has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QStringList filters;


    if (!imageSpace->readFile(fileName)) {
        statusBar()->showMessage(tr("Loading canceled"), 250);
        return false;
    }
    setCurrentFile(fileName);



    curFileInfo.setFile(fileName);

    curFileDir=curFileInfo.dir();

    filters << "*.jpg" << "*.jpeg" << "*.bmp"<< "*.png"<< "*.gif"<< "*.tiff";
    //curFileDir.setNameFilters(filters);
    //curFileDir.setFilter(filters);

    curFileList=curFileDir.entryList(filters,QDir::Files,QDir::Name);

    statusBar()->showMessage(tr("File loaded"), 250);

    updateStatusBar();
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (!imageSpace->writeFile(fileName)) {
        statusBar()->showMessage(tr("Saving canceled"), 250);
        return false;
    }

    setCurrentFile(fileName);

    statusBar()->showMessage(tr("File saved"), 250);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFileName = fileName;
    setWindowModified(false);

    QString shownName = tr("");
    if (!curFileName.isEmpty()) {
        shownName = strippedName(curFileName);
        //recentFiles.removeAll(curFile);
        //recentFiles.prepend(curFile);
        //updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*]  %2").arg(shownName)
                                   .arg(tr("Raysee")));
}



QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
