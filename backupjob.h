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

#ifndef BACKUPJOB_H
#define BACKUPJOB_H

#include <QString>
#include <array>

// Forward declaration used to make Manager a friend.
class Manager;

// Enums corresponding to the index on the combo box in the ui.
enum DeleteType { DURING, AFTER, BEFORE };
enum CompressionType { NONE, TARBALL, GZ, BZ2, XZ };
enum BackupType { INCREMENTAL, INCREMENTAL_NO_D, FULL, FULL_NO_D };

struct JobFlags {
        bool transferCompression;
        bool delta;
        bool backupCompression;
        bool recurring;
        DeleteType deleteType;
        CompressionType compType;
        BackupType backupType;
};

typedef std::array<bool, 7> Days;

class BackupJob
{
    public:
        friend class Manager;

        BackupJob(QString name, QString dest, QString src, QString command, Days days,
                  JobFlags flags, QString time, bool enabled = false);
        BackupJob();
        ~BackupJob() = default;
        BackupJob(const BackupJob &) = default;
        BackupJob &operator=(const BackupJob &) = default;
        BackupJob(BackupJob &&) = default;
        BackupJob &operator=(BackupJob &&) = default;

        /*!
         * \brief Gets the text that will go into the .service file.
         * \return std::string of data for the .service file.
         */
        QString get_service() const;

        /*!
         * \brief Gets the text that will go into the .timer file.
         * \return std::string of data for the .timer file.
         */
        QString get_timer() const;

        /*!
         * \brief Converts the job to an easily displayed QString.
         * \return QString containing formatted output of a job.
         */
        QString to_string() const;

        /*!
         * \brief Converts the job to an easily displayed std::string.
         * \return std::string containing formatted output of a job.
         */
        std::string to_std_string() const;

        /*!
         * \brief Retrieves the name of the job.
         * \return Name of the job.
         */
        QString get_name() const;

        /*!
         * \brief Retrieves the destination of the job.
         * \return Destination of the job.
         */
        QString get_dest() const;

        /*!
         * \brief Retrieves the source of the job.
         * \return Source of the job.
         */
        QString get_src() const;

        /*!
         * \brief Retrieves the flags of the job.
         * \return Flags of the job.
         */
        JobFlags get_flags() const;

        /*!
         * \brief Retrieves the days to run of the job.
         * \return Days to run of the job.
         */
        Days get_days() const;

        /*!
         * \brief Retrieves the time to run of the job.
         * \return Time to run of the job.
         */
        QString get_time() const;

        /*!
         * \brief Retrieves the command.
         * \return The command.
         */
        QString get_command() const;

    private:
        QString name;
        QString dest;
        QString src;
        QString command;
        JobFlags flags;
        Days days;
        QString time;

        bool enabled;

        QString jobflags_to_string() const;

        QString days_to_string() const;

        QString bool_to_string(bool val) const;

        /*!
         * \brief Creates the string for "OnCalendar" of the systemd timer.
         * \return Calendar formatted for the timer.
         */
        QString make_systemd_calendar() const;

        QString make_shell_script() const;
};

#endif // BACKUPJOB_H
