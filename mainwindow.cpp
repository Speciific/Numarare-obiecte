#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->thresholdSlider->setEnabled(false);
    ui->thresholdSlider2->setEnabled(false);
    ui->dilationSlider->setEnabled(false);
    ui->saveButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.jpg *.bmp *.jpeg)"));

    if (!fileName.isEmpty()) {
        originalImage.load(fileName);
        ui->originalImageLabel->setPixmap(QPixmap::fromImage(originalImage));
        ui->originalImageLabel->setScaledContents(true);
        originalImage2 = imread(fileName.toStdString());
    }
    ui->thresholdSlider->setEnabled(true);
    ui->thresholdSlider2->setEnabled(true);
    ui->dilationSlider->setEnabled(true);
    ui->saveButton->setEnabled(true);
}

void MainWindow::svd2(){

    cvtColor(originalImage2, grayImage, COLOR_BGR2GRAY);
    GaussianBlur(grayImage, grayImage, Size(5, 5), 0);
    Canny(grayImage, binaryImage, ui->thresholdSlider->value(), ui->thresholdSlider2->value(), 3);
    dilate(binaryImage, dilatedImage, Mat(), Point(-1, -1), ui->dilationSlider->value());

    vector<vector<Point>> contours;
    findContours(dilatedImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int numObjects = 0;

    filledImage = Mat::zeros(dilatedImage.size(), CV_8UC3);

    for (size_t i = 0; i < contours.size(); ++i) {
        double arcLength = cv::arcLength(contours[i], true);
        double epsilon = 0.02 * arcLength;

        vector<Point> approxCurve;
        approxPolyDP(contours[i], approxCurve, epsilon, true);

        Moments M = moments(approxCurve);
        Point center(M.m10 / M.m00, M.m01 / M.m00);

        Scalar color = Scalar(rand() % 256, rand() % 256, rand() % 256);
        fillPoly(filledImage, vector<vector<Point>>{approxCurve}, color);
        putText(filledImage, to_string(numObjects + 1), center, FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 255), 2);
        numObjects++;
    }

    QString sstr2("Obiecte gasite: ");
    sstr2.append(QString::number(numObjects));
    this->ui->label->setText(sstr2);

    QImage resImg = QImage((uchar*) filledImage.data, filledImage.cols, filledImage.rows, filledImage.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(resImg);

    ui->processedImageLabel->setPixmap(pixmap);
    ui->processedImageLabel->setScaledContents(true);
}

void MainWindow::on_thresholdSlider_valueChanged(int value)
{
    QString sstr("Threshold: ");
    sstr.append(QString::number(value));
    this->ui->thresholdLabel->setText(sstr);

    svd2();

}

void MainWindow::on_thresholdSlider2_valueChanged(int value)
{
    QString sstr("Threshold2: ");
    sstr.append(QString::number(value));
    this->ui->threshold2Label->setText(sstr);

    svd2();
}

void MainWindow::on_dilationSlider_valueChanged(int value)
{
    QString sstr("Dilate: ");
    sstr.append(QString::number(value));
    this->ui->dilateLabel->setText(sstr);

    svd2();

}


void MainWindow::on_saveButton_clicked()
{
    if (!filledImage.empty())
    {
        QString savePath = QFileDialog::getSaveFileName(this, "Salveaza imaginea", "", "Imagine (*.png)");
        if (!savePath.isEmpty())
        {
            Mat imageCopyRGB;
            cvtColor(filledImage, imageCopyRGB, cv::COLOR_BGR2RGB);
            imwrite(savePath.toStdString(), imageCopyRGB);
        }
    }
}

