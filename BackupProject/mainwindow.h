#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qthread.h>
#include <QObject>
#include "cv.h"
#include "highgui.h"
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>

// TEST
class MyFrame : public QFrame
{
public:
    MyFrame(QGroupBox*);
    ~MyFrame();

protected:
    void paintEvent(QPaintEvent* event);

private:
    CvCapture* capture; // For webcam feed
};


static QImage IplImage2QImage(const IplImage *iplImage);
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Functions
    void enableKnapManuel();
    void enableKnapAuto();

public slots:
    void manuelSelected();
    void autoSelected();

private:
    Ui::MainWindow *ui;
};

// Threading
class MyThread : public QThread
{
    Q_OBJECT // Needed for signals and slots
public:
    void run();

signals:
    void incrementing(int value);
    void done(QString value);
    void updateCam();
};

// Classes
// -Auto
struct RGB
{
    char red;
    char green;
    char blue;
};

#endif // MAINWINDOW_H
