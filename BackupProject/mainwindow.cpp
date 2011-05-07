#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cv.h"
#include "highgui.h"
#include "qpainter.h"


void MyThread::run()
{
   for(int i = 1; i <= 100; i++)
   {
        sleep(1);
        emit incrementing(i*1);
   }
   emit done("Done");
}

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

    // Starting
    // -Threads
    mThread->start();

    // Testing
    /*const char* imagename = "lol.jpg";
    cv::Ptr<IplImage> iplimg = cvLoadImage(imagename);

    QImage imgA;
    if(iplimg != NULL)
        imgA = IplImage2QImage(iplimg);

    if(!imgA.isNull())
        ui->progressBar->setValue(50);*/

    /*QPainter* painter = new QPainter();
    painter->begin(this);  // Tell your QPainter to  paint onto the "this"-
                            // Widget.(The "this" pointer points to the object
                            // it is used in; in our case it's your "YourFrame"
                            // object)

    painter->drawRect(10, 10, this->width()-10, this->height()-10);
            // Draw a rectangle at 10, 10, widget_width - 10, widget_height - 10

    painter->end();	// close the painter*/
    MyFrame* mA = new MyFrame(ui->groupBox);
    mA->setGeometry(0, 0, 200, 200);
}

// TEST: To be used in class 'VideoFrame'
MyFrame::MyFrame(QGroupBox* g) : QFrame(g)
{

}
void MyFrame::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);

    QPainter* painter = new QPainter();
    painter->begin(this);  // Tell your QPainter to  paint onto the "this"-
                            // Widget.(The "this" pointer points to the object
                            // it is used in; in our case it's your "YourFrame"
                            // object)

    //painter->drawRect(10, 10, this->width()-10, this->height()-10);

    const char* imagename = "lol.jpg";
    cv::Ptr<IplImage> iplimg = cvLoadImage(imagename);

    QImage imgA;
    if(iplimg != NULL)
        imgA = IplImage2QImage(iplimg);



    painter->drawImage(0, 0, imgA);
            // Draw a rectangle at 10, 10, widget_width - 10, widget_height - 10

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
