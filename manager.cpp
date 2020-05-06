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

Manager::Manager()
{
        path = "/usr/lib/systemd/system";
}

void Manager::set_service_path(QString path)
{
        std::string tmp = path.toStdString();
        if (std::filesystem::exists(tmp)) {
                this->path = tmp;
        } else {
                show_error_dialog("Invalid path!");
        }
}

void Manager::save_jobs()
{
        QJsonArray arr;

        auto create_json = [&](const auto &job) {
                QJsonObject json;
                json["Name"] = job.second.name;
                json["Src"] = job.second.src;
                json["Dst"] = job.second.dest;
                json["Command"] = job.second.command;
                json["Enabled"] = job.second.enabled;
                json["Days"] = days_to_json(job.second);
                json["JobFlags"] = jobflags_to_json(job.second);

                std::string tmp = path + job.second.name.toStdString() + ".service";
                json["Service"] = QString(tmp.c_str());
                arr.append(json);
        };

        for (const auto &job : jobs) {
                create_json(job);
        }
        QFile backups("~/.rbackup/backups.json");

        if (!backups.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                show_error_dialog("Unable to save backups!");
                return;
        }

        QJsonDocument doc(arr);
        backups.write(doc.toJson());
}

QJsonObject Manager::days_to_json(const BackupJob &job)
{
        QJsonObject json;
        json["Mon"] = job.days.days[0];
        json["Tues"] = job.days.days[1];
        json["Wed"] = job.days.days[2];
        json["Thurs"] = job.days.days[3];
        json["Fri"] = job.days.days[4];
        json["Sat"] = job.days.days[5];
        json["Sun"] = job.days.days[6];
        return json;
}

QJsonObject Manager::jobflags_to_json(const BackupJob &job)
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
