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

#include "manager.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <filesystem>
#include <pwd.h>
#include <unistd.h>

Manager::Manager()
{
        servicePath = "/usr/lib/systemd/system";
        configPath = find_home_directory() + "/.rbackup/";
        backupPath = configPath + "backups.json";
        if (!std::filesystem::exists(configPath)) {
                bool status = std::filesystem::create_directory(configPath);
                if (!status) {
                        throw std::system_error();
                }
        }
}

void Manager::set_service_path(QString path)
{
        std::string tmp = path.toStdString();
        if (std::filesystem::exists(tmp)) {
                this->servicePath = tmp;
        } else {
                show_error_dialog("Invalid path!");
        }
}

int Manager::save_jobs()
{
        QJsonArray arr;

        for (const auto &job : jobs) {
                arr.append(job_to_json(job.second));
        }

        QFile backups(backupPath.c_str());

        if (!backups.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                show_error_dialog("Unable to save backups!");
                return -1;
        }

        QJsonDocument doc(arr);
        backups.write(doc.toJson());
        return 0;
}

int Manager::add_new_job(BackupJob job)
{
        std::string tmp = job.name.toStdString();
        if (jobs.count(tmp) != 0) {
                show_error_dialog("A job with that name already exists!");
                return -1;
        }
        jobs[tmp] = job;
        return 0;
}

QJsonObject Manager::job_to_json(const BackupJob &job) const
{
        QJsonObject json;
        json["Name"] = job.name;
        json["Src"] = job.src;
        json["Dst"] = job.dest;
        json["Command"] = job.command;
        json["Enabled"] = job.enabled;
        json["Time"] = job.time;
        json["Days"] = days_to_json(job);
        json["JobFlags"] = jobflags_to_json(job);

        std::string tmp = servicePath + "/" + job.name.toStdString() + ".service";
        json["Service"] = QString(tmp.c_str());
        return json;
}

QJsonObject Manager::days_to_json(const BackupJob &job) const
{
        QJsonObject json;
        json["Mon"] = job.days[0];
        json["Tues"] = job.days[1];
        json["Wed"] = job.days[2];
        json["Thurs"] = job.days[3];
        json["Fri"] = job.days[4];
        json["Sat"] = job.days[5];
        json["Sun"] = job.days[6];
        return json;
}

QJsonObject Manager::jobflags_to_json(const BackupJob &job) const
{
        QJsonObject json;
        json["TransferCompression"] = job.flags.transferCompression;
        json["Delta"] = job.flags.delta;
        json["BackupCompression"] = job.flags.backupCompression;
        json["Recurring"] = job.flags.recurring;
        json["DeleteType"] = job.flags.deleteType;
        json["CompressionType"] = job.flags.compType;
        return json;
}

std::string Manager::find_home_directory() const
{
        char *path = nullptr;
        path = getenv("XDG_CONFIG_HOME");
        if (path != nullptr)
                return path;

        path = getenv("HOME");
        if (path != nullptr)
                return path;

        return getpwuid(getuid())->pw_dir;
}
