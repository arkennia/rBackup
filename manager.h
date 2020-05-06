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

#ifndef MANAGER_H
#define MANAGER_H

#include "backupjob.h"
#include "utility.h"
#include <QJsonObject>
#include <unordered_map>

class Manager
{
    public:
        Manager();
        ~Manager() = default;
        Manager(const Manager &) = delete;
        Manager &operator=(const Manager &) = delete;
        Manager(Manager &&) = default;
        Manager &operator=(Manager &&) = default;

        /*!
         * \brief Sets the path to store the services.
         * \param QString path to service folder.
         */
        void set_service_path(QString path);

        /*!
         * \brief Save all jobs to json.
         * \return 0 for success, -1 for failure.
         */
        int save_jobs();

        /*!
         * \brief Adds the given job to the jobs map.
         * \param Creates a copy of the BackupJob passed to it.
         * \return 0 for success, -1 for failure.
         */
        int add_new_job(BackupJob job);

    private:
        /*
         * Uses unordered map to easily make sure there are no duplicates.
         */
        std::unordered_map<std::string, BackupJob> jobs;

        // Service path
        std::string path;

        /*!
         * \brief Creates an object containing the days to run on.
         * \param BackupJob containg days array to serialize.
         * \return QJsonObject to add to the main object.
         */
        QJsonObject days_to_json(const BackupJob &job);

        /*!
         * \brief Creates an object containing the job flags.
         * \param BackupJob containg JobFlags struct to serialize.
         * \return QJsonObject to add to the main object.
         */
        QJsonObject jobflags_to_json(const BackupJob &job);
};

#endif // MANAGER_H
