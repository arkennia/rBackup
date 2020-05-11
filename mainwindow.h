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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "manager.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <stdexcept>

QT_BEGIN_NAMESPACE
namespace Ui
{
        class MainWindow;
}
QT_END_NAMESPACE

enum Tabs { JOBS, SETTINGS };

/*!
 * \brief The MainWindow class
 * Manages the UI and also has a reference to an object
 * that manages the backup jobs.
 */
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        MainWindow(const MainWindow &) = delete;
        MainWindow(MainWindow &&) = delete;
        MainWindow &operator=(const MainWindow &) = delete;
        MainWindow &operator=(MainWindow &&) = delete;

    private slots:

        /*!
         * \brief Opens a file dialog when the browse button is clicked.
         * This function is for the browse located on the Destination line.
         */
        void on_browseDest_clicked();

        /*!
         * \brief Opens a file dialog when the browse button is clicked.
         * This function is for the browse located on the Source line.
         */
        void on_browseSrc_clicked();

        /*!
         * \brief Generates the rsync command to be used.
         * The generate button gathers the information in the form to create a string
         * containing the commands that would be run when the backup is on. The user
         * can edit this command, and the final input in the Command box is what will be used
         * for the actual backup.
         */
        void on_generateButton_clicked();

        /*!
         * \brief Changes the settings tab to the jobs tab.
         * Saves the job and selects it in the jobs tab.
         */
        void on_finish_clicked();

        /*!
         * \brief Create a new job in the settings tab.
         * Shifts the tab to the settings tab.
         */
        void on_newButton_clicked();

        void on_jobNamesList_itemSelectionChanged();

        void on_editButton_clicked();

        void on_enableButton_clicked();

        void on_actionExit_triggered();

        void on_runButton_clicked();

        void on_disableButton_clicked();

    private:
        Ui::MainWindow *ui;

        Manager *manager;

        std::array<QCheckBox *, 7> checkboxes;

        bool commandGenerated;
        bool isUpdating;

        /*!
         * \brief Populates an array of the days checkboxes.
         */
        void create_checkbox_array();

        /*!
         * \brief Generates the rsync command.
         * \return QString containing the backup commands.
         */
        QString generate() const;

        /*!
         * \brief Selects the backup type.
         * Options: \n
         * 1. Incremental \n
         * 2. Incremental W/O Delta Copy \n
         * 3. Full Backup \n
         * 4. Full Backup W/O Delta Copy \n
         * \return QString containing the flags for the backup type selected.
         */
        QString selectBackupType() const;

        /*!
         * \brief Selects when to delete files.
         * Options: \n
         * 1. During copy \n
         * 2. After Copy \n
         * 3. Before Copy \n
         * \return QString containing proper delete flag.
         */
        QString selectDeleteType() const;

        /*!
         * \brief Selects the type of compression to use for the backup.
         * Options: \n
         * 1. None \n
         * 2. Tarball \n
         * 3. Tar w/ gzip \n
         * 4. Tar w/ bzip2 \n
         * 5. Tar w/ xz \n
         * \return QString containing the compression command.
         */
        QString selectCompressionType() const;

        /*!
         * \brief Creates a BackupJob object based on the fields of the UI.
         * \return BackupJob object with user data.
         */
        BackupJob create_job() const;

        /*!
         * \brief Creates the job flags based on data in form.
         * \return JobFlags with correct values;
         */
        JobFlags create_flags() const;

        /*!
         * \brief Creates the days array based on the checkboxes.
         * \return Days array with correct values set;
         */
        Days create_days() const;

        /*!
         * \brief Creates the string for the time to run the job.
         * \return Time string.
         */
        QString create_time() const;

        /*!
         * \brief Take jobs from manager's jobs map and list them in the QListWidget.
         */
        void add_jobs_to_list();

        /*!
         * \brief Loads a job's information into the Settings form.
         * \param BackupJob to load.
         */
        void edit_job(const BackupJob &job);

        /*!
         * \brief Uses the given Days array to set the checkboxes.
         * \param Days to use for the checkboxes.
         */
        void set_days_from_array(const Days &days);

        /*!
         * \brief Clears the settings after a successful edit or add.
         */
        void clear_form();

    protected:
        void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
