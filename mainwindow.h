#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadImageButton_clicked();
    void on_thresholdSlider_valueChanged(int value);
    void on_thresholdSlider2_valueChanged(int value);
    void on_dilationSlider_valueChanged(int value);

    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage originalImage;
    Mat originalImage2, grayImage, binaryImage, dilatedImage, filledImage;
    void svd2();
};

#endif // MAINWINDOW_H
