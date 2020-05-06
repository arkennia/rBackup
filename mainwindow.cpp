/*
        Copyright Jonathan Manly 2020

        This file is part of rBackup.

        rBackup is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        rBackup is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with rBackup.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        manager = new Manager();
}

MainWindow::~MainWindow()
{
        delete ui;
        delete manager;
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
        ui->command->setPlainText(generate());
}

QString MainWindow::generate() const
{
        QString tmp = "", out = selectBackupType();

        if (ui->transferCompression->isChecked())
                out += TRANSFER_COMPRESSION;

        out += selectDeleteType();
        out += ui->source->text() + " ";
        out += ui->destination->text() + " ";
        out += " && ";
        out += selectCompressionType();

        return out;
}

QString MainWindow::selectBackupType() const
{
        QString out = "";

        switch (ui->backupType->currentIndex()) {
                void on_actionServicePath_triggered();
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
        switch (ui->deleteWhen->currentIndex()) {
        case 0:
                return DELETE_DURING;
        case 1:
                return DELETE_AFTER;
        case 2:
                return DELETE_BEFORE;
        default:
                throw std::out_of_range("Invalid Delete Type Index");
        }
}

QString MainWindow::selectCompressionType() const
{
        QString out = "";
        QString dest = ui->destination->text();
        switch (ui->backupCompression->currentIndex()) {
        case 0:
                break;
        case 1:
                out += TAR;
                out += dest + ".tar " + dest;
                break;
        case 2:
                out += TAR_GZ;
                out += dest + ".tar.gz " + dest;
                break;
        case 3:
                out += TAR_BZ;
                out += dest + ".tar.bz2 " + dest;
                break;
        case 4:
                out += TAR_XZ;
                out += dest + ".tar.xz " + dest;
                break;
        default:
                throw std::out_of_range("Invalid Compression Type Index");
        }
        return out;
}

void MainWindow::on_finish_clicked()
{
        ui->tabs->setCurrentIndex(0);
}

void MainWindow::on_newButton_clicked()
{
        ui->tabs->setCurrentIndex(1);
}

void MainWindow::on_actionService_Path_triggered()
{
        QString fileName = QFileDialog::getExistingDirectory(
                this, tr("Select Directory"), "/usr/lib/systemd/system",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}
