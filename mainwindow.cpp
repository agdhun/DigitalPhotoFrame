#include "mainwindow.h"
#include "ui_mainwindow.h"

//jialeyige current_img duiduibidudeng caozuo jinxing baocun,xujiarudaotexiaobianhuanli
QStringList string_list;
QLabel *label;
QTimer *timer;
Mat old_img, new_img, current_img;
int i = 0, s = 0, wi = 0, ks = 0, ce = 0;
double u = 0.0;
double contrast = 1.0, brightness = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Mat img;
    ui->setupUi(this);
    button = ui->exit;
    label = ui->image;
    QString path= "/home";
    addSubFolderImages(path);
    QString img_init = static_cast<QString>(string_list.at(0));
    img = imread(img_init.toAscii().data());
    Size dsize = Size(label->size().width()-1, label->size().height()-1);
    cv::resize(img, new_img, dsize, 0, 0 , CV_INTER_LINEAR);
    Show_Image1(new_img);
    label->show();
    current_img = new_img.clone();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextpic()
{
    Mat disp;
    Mat rgb;
    QImage img;
    qDebug()<<current_img.rows<<" "<<current_img.cols<<" "<<current_img.type();
    qDebug()<<new_img.rows<<" "<<new_img.cols<<" "<<new_img.type();
    if(s > label->size().width())
    {
        timer->stop();
        s = 0;
        current_img = new_img.clone();
        Show_Image1(new_img);
        ui->sbrt->setValue(0);
        ui->sbtt->setValue(100);
        ui->ce->setValue(0);
        ui->ks->setValue(0);
        ui->next->setEnabled(true);
    }
    else
    {
        disp.create(current_img.rows, current_img.cols, current_img.type());
        translateTransform(current_img, disp, -s, 0);
        translateTransform(new_img, disp, (label->size().width()-1) - s, 0);
        Show_Image1(disp);
        s=s+label->size().width()/100;
    }
}

void MainWindow::previous_pic()
{
    Mat disp;
    Mat rgb;
    QImage img;
    if(s > label->size().width())
    {
        timer->stop();
        s = 0;
        current_img = new_img.clone();
        Show_Image1(new_img);
        ui->sbrt->setValue(0);
        ui->sbtt->setValue(100);
        ui->ce->setValue(0);
        ui->ks->setValue(0);
        ui->previous->setEnabled(true);
    }
    else
    {
        disp.create(new_img.rows, new_img.cols, new_img.type());
        translateTransform(current_img, disp, s , 0);
        translateTransform(new_img, disp, s - label->size().width()+1, 0);
        Show_Image1(disp);
        s=s+label->size().width()/100;
    }
}

void MainWindow::translateTransform(cv::Mat const& src, cv::Mat& dst, int dx, int dy)
{
    CV_Assert(src.depth() == CV_8U);

    const int rows = src.rows;
    const int cols = src.cols;

    Vec3b *p;
    for (int i = 0; i < rows; i++)
    {
        p = dst.ptr<Vec3b>(i);
        for (int j = 0; j < cols; j++)
        {
            int x = j - dx;
            int y = i - dy;

            if (x >= 0 && y >= 0 && x < cols && y < rows)
                p[j] = src.ptr<Vec3b>(y)[x];
            //else
                //p[j] = 0;
        }
    }
}

void MainWindow::next_fifo()
{
    Mat disp;
    QImage img;
    Mat rgb;
    if(u > 1.0)
    {
        timer->stop();
        current_img = new_img.clone();
        ui->sbrt->setValue(0);
        ui->sbtt->setValue(100);
        ui->ce->setValue(0);
        ui->ks->setValue(0);
        ui->next->setEnabled(true);
        u = 0.0;
    }
    else
    {
       cv::addWeighted(current_img, 1.0 - u, new_img, u, 0, disp);
       Show_Image1(disp);
        u += 0.1;
    }
}

void MainWindow::previous_fifo()
{
    Mat disp;
    QImage img;
    Mat rgb;
    if(u > 1.0)
    {
        timer->stop();
        current_img = new_img.clone();
        ui->sbrt->setValue(0);
        ui->sbtt->setValue(100);
        ui->ce->setValue(0);
        ui->ks->setValue(0);
        ui->previous->setEnabled(true);
        u = 0.0;
    }
    else
    {
        cv::addWeighted(current_img, 1.0 - u, new_img, u, 0, disp);
        Show_Image1(disp);
        u += 0.1;
    }
}

void Window_blinds(Mat const& src1, Mat const& src2, Mat& dst, int width, int i)
{
    CV_Assert(src1.depth() == CV_8U);

    const int rows = src1.rows;
    const int cols = src1.cols;

    //dst.create(rows, cols, src.type());

    Vec3b *p;
    for (int u = 0; u < rows; u++)
    {
         p = dst.ptr<Vec3b>(u);
        for(int s = 0; s < cols/width+1; s ++)
        {
            int m = (width * s + i) < cols?(width * s + i):cols;
            for(int j = width * s + 0; j < m; j++)
            {
                p[j] = src1.ptr<Vec3b>(u)[j];
            }

            m = width * (s + 1) < cols? width * (s + 1) : cols;
            for(int j = width * s + i; j < m; j++)
            {
                p[j] = src2.ptr<Vec3b>(u)[j];

            }

        }
    }
}

void MainWindow::next_wb()
{
    Mat disp;
    qDebug()<<current_img.rows<<" "<<current_img.cols<<" "<<current_img.type();
    qDebug()<<new_img.rows<<" "<<new_img.cols<<" "<<new_img.type();
    disp.create(current_img.rows, current_img.cols, current_img.type());
    QImage img;
    Mat rgb;
    if(wi > current_img.cols/40)
    {
        timer->stop();
        wi = 0;
        current_img = new_img.clone();
        Show_Image1(new_img);
        ui->sbrt->setValue(0);
        ui->sbtt->setValue(100);
        ui->ce->setValue(0);
        ui->ks->setValue(0);
        ui->next->setEnabled(true);
    }
    else
    {
        Window_blinds(new_img, current_img, disp, current_img.cols/20, 2*wi);
        Show_Image1(disp);
        wi ++;
    }

}

void MainWindow::previous_wb()
{
    Mat disp;
    disp.create(current_img.rows, current_img.cols, current_img.type());
    QImage img;
    Mat rgb;
    if(wi > current_img.cols/40)
    {
        timer->stop();
        wi = 0;
        current_img = new_img.clone();
        Show_Image1(new_img);
        ui->sbrt->setValue(0);
        ui->sbtt->setValue(100);
        ui->ce->setValue(0);
        ui->ks->setValue(0);
        ui->previous->setEnabled(true);
    }
    else
    {
        Window_blinds(new_img, current_img, disp, current_img.cols/20, 2*wi);
        Show_Image1(disp);
        wi ++;
    }
}



void MainWindow::on_ks_valueChanged(int value)
{
    ks = value;
    Show_Image();
}

void MainWindow::on_sbrt_valueChanged(int value)
{
    brightness = value;
    Show_Image();
}

void MainWindow::on_ce_valueChanged(int value)
{
    ce = value;
    Show_Image();
}

void MainWindow::on_sbtt_valueChanged(int value)
{
    contrast = (double)(value) * 1.0/ 100;
    Show_Image();
}

void Show_Image()
{
    Mat rgb;
    QImage img;
    Mat disp1, disp2;
    Mat element = getStructuringElement(MORPH_RECT, Size(2*ks+1,2*ks+1),Point( ks, ks ));


    addWeighted(new_img, contrast, new_img, 0, brightness, disp1, -1);
    if(ce== 0)
        erode(disp1, disp2, element);
    else
        dilate(disp1, disp2, element);

    current_img = disp2;
    if(disp2.channels() == 3)
    {
        cvtColor(disp2, rgb, CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_RGB888);
    }
    else
    {
        rgb = disp2;
        img = QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_Indexed8);
    }
    label->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_previous_clicked()
{
    ui->previous->setEnabled(false);
    //old_img = new_img;
    //int ij = i;
    i = (i == 0)?(string_list.length() - 1) : (i-1);
    //QString next_img = static_cast<QString>(string_list.at(ij));
    QString img_init = static_cast<QString>(string_list.at(i));

    Mat current_image = imread(img_init.toAscii().data());
    //Mat next_image = imread(next_img.toAscii().data());

    //tiaozhengtupiandaxiao,yishiying label kongjian
    Size dsize = Size(label->size().width()-1, label->size().height()-1);
    cv::resize(current_image, new_img, dsize, 0, 0 , CV_INTER_LINEAR);
    //cv::resize(next_image, old_img, dsize, 0, 0 , CV_INTER_LINEAR);
    //current_img = new_img.clone();

    //CheckBox xuanze
    if(ui->wb->isChecked())
    {
        timer = new QTimer(this);
        timer->setInterval(50);
        connect(timer, SIGNAL(timeout()), this, SLOT(previous_wb()));
        timer->start();
    }
    if(ui->fifo->isChecked())
    {
        timer = new QTimer(this);
        timer->setInterval(50);
        connect(timer, SIGNAL(timeout()), this, SLOT(previous_fifo()));
        timer->start();
    }
    if(ui->ltr->isChecked())
    {
        timer = new QTimer(this);
        timer->setInterval(1);
        connect(timer, SIGNAL(timeout()), this, SLOT(previous_pic()));
        timer->start();
    }
    if(ui->none->isChecked())
    {
        Mat rgb;
        QImage img;
        if(new_img.channels() == 3)
        {
            cvtColor(new_img, rgb, CV_BGR2RGB);
            img = QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_RGB888);
         }else
         {
             rgb = new_img;
             img = QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_Indexed8);
         }
         QImg = img;
         label->setPixmap(QPixmap::fromImage(QImg));
         ui->sbrt->setValue(0);
         ui->sbtt->setValue(100);
         ui->ce->setValue(0);
         ui->ks->setValue(0);
         ui->previous->setEnabled(true);
    }
}

void MainWindow::on_next_clicked()
{
    //shezhianniubukeyong,fangzhixinhaojingzeng
    ui->next->setEnabled(false);
    //old_img = new_img;
    //duquqianhouliangfutupian
    //int ij = i;
    i = (i == (string_list.length() - 1))?0 : (i+1);
    //QString next_img = static_cast<QString>(string_list.at(ij));
    QString img_init = static_cast<QString>(string_list.at(i));

    Mat current_image = imread(img_init.toAscii().data());
    //Mat next_image = imread(next_img.toAscii().data());

    //tiaozhengtupiandaxiao,yishiying label kongjian
    Size dsize = Size(label->size().width()-1, label->size().height()-1);
    cv::resize(current_image, new_img, dsize, 0, 0 , CV_INTER_LINEAR);
    //cv::resize(next_image, old_img, dsize, 0, 0 , CV_INTER_LINEAR);
    //current_img = new_img;
    //CheckBox xuanze
    if(ui->wb->isChecked())
    {
        timer = new QTimer(this);
        timer->setInterval(50);
        connect(timer, SIGNAL(timeout()), this, SLOT(next_wb()));
        timer->start();
    }
    if(ui->fifo->isChecked())
    {
        timer = new QTimer(this);
        timer->setInterval(50);
        connect(timer, SIGNAL(timeout()), this, SLOT(next_fifo()));
        timer->start();
    }
    if(ui->ltr->isChecked())
    {
        timer = new QTimer(this);
        timer->setInterval(1);
        connect(timer, SIGNAL(timeout()), this, SLOT(nextpic()));
        timer->start();
    }
    if(ui->none->isChecked())
    {
        Mat rgb;
        QImage img;
        if(new_img.channels() == 3)
        {
            cvtColor(new_img, rgb, CV_BGR2RGB);
            img = QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_RGB888);
         }else
         {
             rgb = new_img;
             img = QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_Indexed8);
         }
         QImg = img;
         label->setPixmap(QPixmap::fromImage(QImg));
         ui->sbrt->setValue(0);
         ui->sbtt->setValue(100);
         ui->ce->setValue(0);
         ui->ks->setValue(0);
         ui->next->setEnabled(true);
    }
}

void Show_Image1(Mat const& disp)
{
    Mat rgb;
    QImage img;
    if(disp.channels() == 3)
    {
        cvtColor(disp, rgb, CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_RGB888);
    }else
    {
        rgb = disp;
        img = QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_Indexed8);
    }
    label->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::addSubFolderImages(QString path)
{
    QDir dir(path);
    if(!dir.exists())
    {
        return;
    }
    QStringList filters;
    filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");

    QDirIterator dir_iterator(path,
                              filters,
                              QDir::Files | QDir::NoSymLinks,
                              QDirIterator::Subdirectories);

    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString file_path = file_info.absoluteFilePath();
        string_list.append(file_path);
        qDebug() << file_path;
    }
}
