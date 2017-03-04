#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void Split();

private slots:
    void on_pushButton_Output_clicked();

    void on_pushButton_Input_clicked();

    void on_pushButton_Split_clicked();

private:
    Ui::Widget *ui;
    QSettings *settings;
};

#endif // WIDGET_H
