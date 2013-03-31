#include <QtGui>
#include <QtDebug>


#include "imageSpace.h"




ImageSpace::ImageSpace(QWidget *parent)
    : QScrollArea(parent)
{

    imageData = new QImage;
    imageBackup = new QImage;
    imageDisp = new QPixmap;
    //layout=new QHBoxLayout;

    imageLabel = new QLabel;
    //imageLabel->setBackgroundRole(QPalette::Window);

    imageLabel->setStyleSheet("background-color: white;");
    //imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //imageLabel->setScaledContents(true);

    //layout->addWidget(imageLabel);
    setWidget(imageLabel);
    setWidgetResizable(true);

    imageLabel->setAlignment(Qt::AlignCenter);
    //verticalScrollBar()->setHidden(true);
    //horizontalScrollBar()->setHidden(true);
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    connect(horizontalScrollBar(),SIGNAL(rangeChanged(int ,int)), this, SLOT(adjustHoriBar(int, int)));
    connect(verticalScrollBar(),SIGNAL(rangeChanged(int ,int)), this, SLOT(adjustVertiBar(int, int)));




    //setLayout(layout);


    currentZoom=1;
    currentRotate=0;
    leftMousePressed=0;
    hsbValPercent=0.5;
    vsbValPercent=0.5;


    //clear();
}

void ImageSpace::enterEvent(QEvent*)
{
    if(verticalScrollBar()->minimum()!=verticalScrollBar()->maximum()
        ||horizontalScrollBar()->minimum()!=horizontalScrollBar()->maximum())

        setCursor(Qt::OpenHandCursor);

}
void ImageSpace::leaveEvent(QEvent*)
{
    setCursor(Qt::ArrowCursor);
}

void ImageSpace::mousePressEvent(QMouseEvent* event)
{
        if(event->button() == Qt::LeftButton)
        {
        mousePos = event->pos();
        vsbVal=verticalScrollBar()->value();
        hsbVal=horizontalScrollBar()->value();

        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
        leftMousePressed=1;
        }
        event->accept();

}

void ImageSpace::mouseReleaseEvent(QMouseEvent* event)
{
    double vrange,hrange;
        if(event->button() == Qt::LeftButton)
        {
        leftMousePressed=0;
        vsbVal=verticalScrollBar()->value();
        hsbVal=horizontalScrollBar()->value();
        hrange=horizontalScrollBar()->maximum()-horizontalScrollBar()->minimum();
        vrange=verticalScrollBar()->maximum()-verticalScrollBar()->minimum();
        if(hrange!=0)
        hsbValPercent=(hsbVal-horizontalScrollBar()->minimum())/hrange;
        if(vrange!=0)
        vsbValPercent=(vsbVal-verticalScrollBar()->minimum())/vrange;
        QApplication::restoreOverrideCursor();
        }
        event->accept();


}

void ImageSpace::mouseMoveEvent(QMouseEvent* event)
{
        if(event->buttons() == Qt::LeftButton&&leftMousePressed==1)
        {
        QPoint newPos = event->pos();

        QSize size( newPos.x()-mousePos.x(), newPos.y()-mousePos.y());

        verticalScrollBar()->setValue(vsbVal-size.height());
        horizontalScrollBar()->setValue(hsbVal-size.width());


        //QApplication::restoreOverrideCursor();
        }


        //event->accept();

}

bool ImageSpace::readFile(const QString &fileName)
{
    if (!imageData->load(fileName))
    {
        QMessageBox::warning(this, tr("Raysee"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName));
        return false;
    }
    *imageBackup=*imageData;
    imageShow();

    update();

    return true;
}

bool ImageSpace::writeFile(const QString &fileName)
{
   // QFile file(fileName);
    if (!imageData->save(fileName)) {
        QMessageBox::warning(this, tr("Raysee"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName));

        return false;
    }


    return true;
}

void ImageSpace::imageShow()
{


    double zoomX,zoomY;
    zoomX=0; zoomY=0;
    currentZoom=1;
    currentRotate=0;

    *imageDisp=QPixmap::fromImage(*imageData,0);

    if (imageDisp->width()!=0)
    zoomX = (double)this->width()/imageDisp->width();
    if (imageDisp->height()!=0)
    zoomY = (double)this->height()/imageDisp->height();

    currentZoom = (zoomX > zoomY)?zoomY:zoomX;

    if (currentZoom<1.1)
        {
        currentZoom*=0.9;
        resizeImage();
        }
    else imageLabel->setPixmap(*imageDisp);


}


bool ImageSpace::resizeImage()
{

    if(imageDisp->isNull())
        return false;
    QMatrix m;

    m.scale(currentZoom,currentZoom);
    m.rotate(currentRotate);
    //imageDisp= imageDisp.transformed(m,Qt::FastTransformation);
    QPixmap imageScaled = imageDisp->transformed(m,Qt::FastTransformation);
    //imageDisp=imageScaled;
    //imageLabel->setUpdatesEnabled(false);
    imageLabel->setPixmap(imageScaled);
    //imageLabel->resize(imageScaled.width(),imageScaled.height());
    //imageLabel->setUpdatesEnabled(true);
    //update();
    return true;
}





cv::Mat ImageSpace::QImageTocvMat(QImage &qimage,int format)
{


    //QImage qimage=image.rgbSwapped();
    if (format==1)
    {
    //rgbToGray(qimage);

    qimage=qimage.convertToFormat(QImage::Format_Indexed8,Qt::ThresholdDither);
    cv::Mat cvmat=cv::Mat(qimage.height(), qimage.width(), CV_8UC1, (uchar*)qimage.bits(),qimage.bytesPerLine());
    return cvmat;
    }
    else
    {
        qimage=qimage.convertToFormat(QImage::Format_RGB32);
        cv::Mat cvmat=cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(),qimage.bytesPerLine());
        return cvmat;
    }




}



QImage ImageSpace::cvMatToQImage(const cv::Mat &mat,int format)
{
    if (format==1)
    {

    QImage qImage(
        (uchar*)mat.data,

        mat.cols,
        mat.rows,
        mat.step,
        QImage::Format_Indexed8
    );
    return qImage;
    }
    else
    {

    QImage qImage(
        (uchar*)mat.data,

        mat.cols,
        mat.rows,
        mat.step,
        QImage::Format_RGB32
    );
    return qImage;
    }



}

void ImageSpace::doEdge(int lt,int ht,int ks)
{


  cv::Mat cvimg,edges;
  if (imageData->depth()==32)
  rgbToGray(*imageData);
  cvimg=QImageTocvMat(*imageData,1);

  cv::Canny(cvimg,edges,lt*ks*ks,ht*ks*ks, ks,false);

  *imageData=cvMatToQImage(edges,1);

  *imageData=imageData->convertToFormat(QImage::Format_RGB32);
  *imageDisp=QPixmap::fromImage(*imageData,Qt::AutoColor);


  resizeImage();
  emit imageDataModified();


}

void ImageSpace::rgbAdjust(int r,int g,int b)
{
    QRgb *colorData,*colorDisp;
    //QImage *qimage;

    int width=imageData->width();
    int height=imageData->height();
    *imageBackup =QImage(width,height,imageData->format());
    for (int i = 0; i < height; ++i)
    {
        colorData = reinterpret_cast<QRgb*>(imageData->scanLine(i));
        colorDisp = reinterpret_cast<QRgb*>(imageBackup->scanLine(i));
        for (int j = 0; j < width; ++j)
        {
            int red=qRed(colorData[j])+r;
            int green=qGreen(colorData[j])+g;
            int blue=qBlue(colorData[j])+b;


            if (red>255) red=255;
            else if (red<0) red=0;
            if (green>255) green=255;
            else if (green<0) green=0;
            if (blue>255) blue=255;
            else if (blue<0) blue=0;

            colorDisp[j]=qRgb(red,green,blue);

      }
    }
    *imageDisp=QPixmap::fromImage(*imageBackup,0);
    resizeImage();
    //delete imageBackup;
}

void ImageSpace::hsvAdjust(int h,int s,int v)
{
    QRgb *colorData,*colorDisp;
    QColor temp;
    //QImage *qimage;

    int width=imageData->width();
    int height=imageData->height();
    *imageBackup =QImage(width,height,imageData->format());
    for (int i = 0; i < height; ++i)
    {
        colorData = reinterpret_cast<QRgb*>(imageData->scanLine(i));
        colorDisp = reinterpret_cast<QRgb*>(imageBackup->scanLine(i));
        for (int j = 0; j < width; ++j)
        {
            temp=QColor::fromRgb(colorData[j]);
            //temp=temp.convertTo(QColor::Hsv);
            //int hue=(qRed(colorData[j])+qGreen(colorData[j])+qBlue(colorData[j]))/3;
            int hue=temp.hsvHue()+h;
            int satu=temp.hsvSaturation()+s;
            int val=temp.value()+v;
            //int val=(qRed(colorData[j])+qGreen(colorData[j])+qBlue(colorData[j]))/3+v;
            int alpha=temp.alpha();
            if (hue>359)  hue=359;
            else if (hue<0) hue=0;
            if (satu>255)  satu=255;
            else if (satu<0) satu=0;
            if (val>255)  val=255;
            else if (val<0) val=0;

            temp.setHsv(hue,satu,val,alpha);
            //temp.convertTo(QColor::Rgb);
            colorDisp[j]=temp.rgb();
\



      }
    }
    *imageDisp=QPixmap::fromImage(*imageBackup,0);
    resizeImage();
    //delete imageBackup;
}



void ImageSpace::hslAdjust(int h,int s,int l)
{
    QRgb *colorData,*colorDisp;
    QColor temp;
    //QImage *qimage;

    int width=imageData->width();
    int height=imageData->height();
    *imageBackup = QImage(width,height,imageData->format());
    for (int i = 0; i < height; ++i)
    {
        colorData = reinterpret_cast<QRgb*>(imageData->scanLine(i));
        colorDisp = reinterpret_cast<QRgb*>(imageBackup->scanLine(i));
        for (int j = 0; j < width; ++j)
        {
            temp=QColor::fromRgb(colorData[j]);
            //temp=temp.convertTo(QColor::Hsv);
            //int hue=(qRed(colorData[j])+qGreen(colorData[j])+qBlue(colorData[j]))/3;
            int hue=temp.hslHue()+h;
            int satu=temp.hslSaturation()+s;
            int light=temp.lightness()+l;
            //int val=(qRed(colorData[j])+qGreen(colorData[j])+qBlue(colorData[j]))/3+v;
            int alpha=temp.alpha();
            if (hue>359)  hue=359;
            else if (hue<0) hue=0;
            if (satu>255)  satu=255;
            else if (satu<0) satu=0;
            if (light>255)  light=255;
            else if (light<0) light=0;

            temp.setHsl(hue,satu,light,alpha);
            //temp.convertTo(QColor::Rgb);
            colorDisp[j]=temp.rgb();
\



      }
    }
    *imageDisp=QPixmap::fromImage(*imageBackup,0);
    resizeImage();

}


void ImageSpace::applyRgb()
{
    //QImage qimage;
    //QPixmap pimage;
   // QMatrix m;

   // m.scale(1/currentZoom,1/currentZoom);

    //QPixmap pimage = imageDisp->transformed(m,Qt::FastTransformation);

    //*imageData=*imageBackup;
    forImageBackup();

    emit imageDataModified();
}

void ImageSpace::cancelRgb()
{
    //*imageBackup=*imageData;
    *imageDisp=QPixmap::fromImage(*imageData,0);
    resizeImage();

}


void ImageSpace::applyHsv()
{

    //*imageData=*imageBackup;
    forImageBackup();

    emit imageDataModified();
}

void ImageSpace::cancelHsv()
{
    //*imageBackup=*imageData;
    *imageDisp=QPixmap::fromImage(*imageData,0);
    resizeImage();

}

void ImageSpace::applyHsl()
{

    //*imageData=*imageBackup;
    forImageBackup();

    emit imageDataModified();
}

void ImageSpace::cancelHsl()
{
   // *imageBackup=*imageData;//for when the imageBackup is null
    *imageDisp=QPixmap::fromImage(*imageData,0);
    resizeImage();

}

bool ImageSpace::imageClear()
{
    imageLabel->clear();
    emit imageDataModified();
    return true;

}

void ImageSpace::forImageBackup()
{
    QImage qimage;
    qimage=*imageData;
    *imageData=*imageBackup;
    *imageBackup=qimage;
}

void ImageSpace::undo()
{
    forImageBackup();
    *imageDisp=QPixmap::fromImage(*imageData,0);
    resizeImage();


}

void ImageSpace::redo()
{
    forImageBackup();
    *imageDisp=QPixmap::fromImage(*imageData,0);
    resizeImage();

}



void ImageSpace::cut()
{
    copy();
    del();
}

void ImageSpace::copy()
{

}

void ImageSpace::paste()
{

}

void ImageSpace::del()
{

}


void ImageSpace::zoomin()
{
    currentZoom*=1.3;

    qDebug()<<currentZoom;
    if(currentZoom>11)
    {
        currentZoom/=1.3;
        resizeImage();
        return;


    }

    resizeImage();


}
void ImageSpace::zoomout()
{
    currentZoom/=1.3;


    resizeImage();



}


void ImageSpace::adjustHoriBar(int min, int max)
{
    hsbVal=(max-min)*hsbValPercent+min;

    horizontalScrollBar()->setValue(hsbVal);


}

void ImageSpace::adjustVertiBar(int min, int max)
{
    vsbVal=(max-min)*vsbValPercent+min;
    verticalScrollBar()->setValue(vsbVal);

}
void ImageSpace::rotateCW()
{
        currentRotate += 90;
        resizeImage();
}

void ImageSpace::rotateCCW()
{
        currentRotate -= 90;
        resizeImage();
}


void ImageSpace::toGray()
{

    rgbToGray(*imageData);
    *imageDisp=QPixmap::fromImage(*imageData);
    resizeImage();
    emit imageDataModified();
}

QImage ImageSpace::rgbToGray(QImage &qimage)
{
    QRgb color;
    int width,height;
    int gray;
    width=qimage.width();
    height=qimage.height();
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            color = qimage.pixel(i, j);
            gray = qGray(color);
            qimage.setPixel(i, j, qRgb(gray, gray, gray));
      }
    }
    return qimage;
}




