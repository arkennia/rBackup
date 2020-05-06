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
         */
        void save_jobs();

    private:
        /*
         * Uses unordered map to easily make sure there are no duplicates.
         */
        std::unordered_map<std::string, BackupJob> jobs;
        std::string path;

        QJsonObject days_to_json(const BackupJob &job);
        QJsonObject jobflags_to_json(const BackupJob &job);
};

#endif // MANAGER_H
