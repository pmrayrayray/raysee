#ifndef IMAGESPACE_H
#define IMAGESPACE_H

#include "cv.h"
#include <QWidget>
#include <QScrollArea>


class QImage;
class QPixmap;
class QLabel;
class QHBoxLayout;



class ImageSpace : public QScrollArea
{
    Q_OBJECT

public:
    ImageSpace(QWidget *parent = 0);
    friend class MainWindow;




    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    void imageShow();
    bool resizeImage();
    static cv::Mat QImageTocvMat(QImage &image,int format);
    static QImage cvMatToQImage(const cv::Mat &mat,int format);
    bool imageClear();
    void forImageBackup();

    static QImage rgbToGray(QImage &qimage);
protected:

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    //void dragEnterEvent(QDragEnterEvent *);
    //void dragMoveEvent(QDragMoveEvent *);




public slots:
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void del();


    void zoomin();
    void zoomout();
    //void zoomto();
    void rotateCW();
    void rotateCCW();

    void adjustHoriBar(int min, int max);
    void adjustVertiBar(int min, int max);

    void doEdge(int lt,int ht,int ks);
    void rgbAdjust(int r,int g,int b);
    void hsvAdjust(int h,int s,int v);
    void hslAdjust(int h,int s,int l);

    void applyRgb();
    void cancelRgb();
    void applyHsv();
    void cancelHsv();
    void applyHsl();
    void cancelHsl();
    void toGray();


signals:
    void imageDataModified();


private slots:
    //void somethingChanged();

private:
    double currentZoom;
    double currentRotate;
    QPoint mousePos;
    bool leftMousePressed;
    unsigned int vsbVal;
    unsigned int hsbVal;
    double hsbValPercent;
    double vsbValPercent;

    QImage *imageData;
    QImage *imageBackup;
    QPixmap *imageDisp;
    QLabel *imageLabel;
    QHBoxLayout *layout;


};



#endif // IMAGESPACE_H
