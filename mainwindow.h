#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <stdexcept>

QT_BEGIN_NAMESPACE
namespace Ui
{
        class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_browseDest_clicked();
        void on_browseSrc_clicked();

        void on_generateButton_clicked();

    private:
        Ui::MainWindow *ui;

        QString generate() const;
        QString selectBackupType() const;
        QString selectDeleteType() const;
};
#endif // MAINWINDOW_H
