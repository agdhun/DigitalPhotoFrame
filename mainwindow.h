#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QImage>
#include <QDebug>
#include <QDir>
#include <QTimer>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QMessageBox>
#include <QDirIterator>


using namespace std;
using namespace cv;

extern QPushButton *button;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void addSubFolderImages(QString path);
    void translateTransform(cv::Mat const& src, cv::Mat& dst, int dx, int dy);
    void nextpic();
    void previous_pic();
    void next_fifo();
    void previous_fifo();
    void previous_wb();
    void next_wb();

public:
    IplImage* iplImg;
    QImage QImg;
private slots:
    void on_ks_valueChanged(int value);
    void on_sbrt_valueChanged(int value);
    void on_ce_valueChanged(int value);
    void on_sbtt_valueChanged(int value);
    void on_previous_clicked();
    void on_next_clicked();
};

void Window_blinds(Mat const& src, Mat& dst, int width, int i);
void Show_Image();
void Show_Image1(Mat const& disp);
#endif // MAINWINDOW_H
