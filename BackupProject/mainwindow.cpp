#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cv.h"
#include "highgui.h"
#include "qpainter.h"


void MyThread::run()
{
    while(1)
    {
        emit updateCam();
        this->msleep(200);
    }

   /*for(int i = 1; i <= 100; i++)
   {
        sleep(1);
        emit incrementing(i*1);
   }*/
   emit done("Done");
}

// To convert OpenCV images into Qt images
static QImage IplImage2QImage(const IplImage *iplImage)
{
    int height = iplImage->height;
    int width = iplImage->width;

    if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
    {
      const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
      QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
      return img.rgbSwapped();
    } else if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1){
        const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_Indexed8);

        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++){
            colorTable.push_back(qRgb(i, i, i));
        }
        img.setColorTable(colorTable);
        return img;
    }
    return QImage();
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Variables
    // -Threads
    MyThread* mThread = new MyThread(); // Dynamic

    // Signals
    //ui->progressBar->setValue();
    connect(mThread, SIGNAL(incrementing(int)), ui->progressBar, SLOT(setValue(int)));
    connect(mThread, SIGNAL(done(QString)), ui->statusBar, SLOT(showMessage(QString)));
    connect(ui->comboBoxColor, SIGNAL(activated(QString)), ui->txtSend, SLOT(setText(QString)));
    connect(ui->btnAuto, SIGNAL(clicked()), this, SLOT(autoSelected()));
    connect(ui->btnManuel, SIGNAL(clicked()), this, SLOT(manuelSelected()));

    // Testing
    // -Threads and videoFrame
    MyFrame* mA = new MyFrame(ui->groupBox_2);
    mA->setGeometry(20, 20, 420, 420);
    connect(mThread, SIGNAL(updateCam()), mA, SLOT(update()));
    mThread->start();
    mThread->setPriority(QThread::LowestPriority);
}

// TEST: To be used in class 'VideoFrame'
MyFrame::MyFrame(QGroupBox* g) : QFrame(g)
{
    // capture from video device #0
    capture = cvCaptureFromCAM(0);

    // Additional settings
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 480); // Only some predfined sizes seem to work
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480); // Only some predfined sizes seem to work
}
MyFrame::~MyFrame()
{
    // Release capture source
    cvReleaseCapture(&capture);
}

void MyFrame::paintEvent(QPaintEvent* event)
{
    // Variables
    QFrame::paintEvent(event);
    IplImage* iplimg;
    QImage imgA;
    QPainter* painter = new QPainter();

    // Image capturing and drawing
    painter->begin(this);

    if(capture) // If 'capture' initialised
    {
        cvGrabFrame(capture);
        iplimg = cvRetrieveFrame(capture);
    }


    if(iplimg != NULL) // Convert if image exists
        imgA = IplImage2QImage(iplimg);

    painter->drawImage(0, 0, imgA);
    painter->setPen(QColor(255, 0, 0));
    painter->drawEllipse(this->width()/2, this->height()/2, 10, 10);
    painter->drawEllipse(this->width()/2 - 5, this->height()/2 - 5, 20, 20);

    painter->end();	// close the painter

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Functions
// -GUI::Slots
void MainWindow::manuelSelected()
{
    // Disable button for manuel and disable auto related buttons
    ui->btnManuel->setEnabled(false);
    ui->btnSend->setEnabled(false);
    ui->txtSend->setEnabled(false);
    ui->comboBoxColor->setEnabled(false);

    // Enable auto button
    ui->btnAuto->setEnabled(true);
}
void MainWindow::autoSelected()
{
    // Disable button for auto and enable auto related buttons
    ui->btnAuto->setEnabled(false);
    ui->btnSend->setEnabled(true);
    ui->txtSend->setEnabled(true);
    ui->comboBoxColor->setEnabled(true);

    // Enable manuel button
    ui->btnManuel->setEnabled(true);
}
