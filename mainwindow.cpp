#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utility.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
        ui->setupUi(this);
}

MainWindow::~MainWindow()
{
        delete ui;
}

void MainWindow::on_browseDest_clicked()
{
        QString fileName = QFileDialog::getExistingDirectory(
                this, tr("Select Directory"), "/home",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        ui->destination->setText(fileName);
}

void MainWindow::on_browseSrc_clicked()
{
        QString fileName = QFileDialog::getExistingDirectory(
                this, tr("Select Directory"), "/home",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        ui->destination->setText(fileName);
}

void MainWindow::on_generateButton_clicked()
{
}

QString MainWindow::generate() const
{
        QString out = selectBackupType();

        if (ui->transferCompression->isChecked())
                out += TRANSFER_COMPRESSION;

        out += selectDeleteType();

        return out;
}

QString MainWindow::selectBackupType() const
{
        QString out = "";

        switch (ui->backupType->currentIndex()) {
        case 0:
                out += INCREMENTAL_OPTIONS;
                break;
        case 1:
                out += INCREMENTAL_OPTIONS;
                out += NO_DELTA;
                break;
        case 2:
                out += FULL_OPTIONS;
                break;
        case 3:
                out += INCREMENTAL_OPTIONS;
                out += NO_DELTA;
                break;
        default:
                throw std::out_of_range("Invalid Backup Type Index");
        }

        return out;
}

QString MainWindow::selectDeleteType() const
{
        QString out = "";

        switch (ui->deleteWhen->currentIndex()) {
        case 0:
                out += DELETE_DURING;
                break;
        case 1:
                out += DELETE_AFTER;
                break;
        case 2:
                out += DELETE_BEFORE;
                break;
        default:
                throw std::out_of_range("Invalid Delete Type Index");
        }

        return out;
}
