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

struct JobFlags {
        bool transferCompression;
        bool delta;
        bool backupCompression;
        bool recurring;
        DeleteType deleteType;
        CompressionType compType;
};

struct Days {
        std::array<bool, 7> days;
};

class BackupJob
{
    public:
        friend class Manager;

        BackupJob(QString name, QString dest, QString src, QString command, Days days,
                  JobFlags flags, bool enabled = false);
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
        std::string get_service() const;

        /*!
         * \brief Gets the text that will go into the .timer file.
         * \return std::string of data for the .timer file.
         */
        std::string get_timer() const;

    private:
        QString name;
        QString dest;
        QString src;
        QString command;
        Days days;
        JobFlags flags;
        bool enabled;
};

#endif // BACKUPJOB_H
