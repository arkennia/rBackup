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
#include <fstream>
#include <iostream>
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
        load_jobs();
}

int Manager::save_jobs()
{
        QJsonArray arr;
        QJsonObject obj;
        for (const auto &job : jobs) {
                arr.append(job_to_json(job.second));
        }

        QFile backups(backupPath.c_str());

        if (!backups.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                show_error_dialog("Unable to save backups!");
                return -1;
        }
        obj["jobs"] = arr;
        QJsonDocument doc(obj);
        backups.write(doc.toJson());
        return 0;
}

int Manager::load_jobs()
{
        QJsonDocument doc;
        QFile backups(backupPath.c_str());
        if (!backups.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return -1;
        }

        QJsonParseError err;
        doc = QJsonDocument::fromJson(backups.readAll(), &err);
        if (err.error != QJsonParseError::NoError) {
                std::cerr << "Error string: " + err.errorString().toStdString() << "\n";
                return -1;
        } else if (doc.isNull()) {
                return 0;
        }
        QJsonObject obj = doc.object();
        QJsonArray arr = obj["jobs"].toArray();

        BackupJob job;
        for (int i = 0; i < arr.size(); i++) {
                job = job_from_json(arr[i].toObject());
                jobs[job.name.toStdString()] = job;
        }
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

std::list<std::string> Manager::get_job_names() const
{
        std::list<std::string> list;
        for (const auto &key : jobs) {
                list.push_back(key.first);
        }
        return list;
}

QString Manager::get_job_text(QString name)
{
        if (jobs.count(name.toStdString()) != 0)
                return jobs[name.toStdString()].to_string();
        else
                return "";
}

int Manager::update_job(BackupJob job)
{
        std::string jobname = job.name.toStdString();
        if (jobs.count(jobname) != 0) {
                jobs[jobname] = job;
                return 0;
        }
        show_error_dialog("Unable to update job.");
        return -1;
}

const BackupJob &Manager::get_job(const std::string &name)
{
        return jobs[name];
}

int Manager::enable_job(const std::string &name)
{
        if (jobs.count(name) != 0) {
                jobs[name].enabled = true;
                jobs[name].create_systemd_objects();
                return 0;
        }
        return -1;
}

int Manager::run_job(const std::string &name)
{
        if (jobs.count(name) != 0) {
                QDBusInterface interface("org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                         "org.freedesktop.systemd1.Manager",
                                         QDBusConnection::systemBus());
                QDBusReply<QDBusObjectPath> reply;
                if (!interface.isValid()) {
                        show_error_dialog("Unable to connect to systemd.");
                        return -1;
                }
                reply = interface.call("StopUnit", "sshd.service", "replace");
                if (!reply.isValid())
                        std::cout << reply.error().name().toStdString() << "\n";
                else
                        std::cout << reply.value().path().toStdString() << "\n";
                return 0;
        }
        return -1;
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
        json["BackupType"] = job.flags.backupType;
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

BackupJob Manager::job_from_json(const QJsonObject &json) const
{
        BackupJob job;
        QJsonObject jflags = json["JobFlags"].toObject();
        QJsonObject jdays = json["Days"].toObject();
        job.name = json["Name"].toString();
        job.src = json["Src"].toString();
        job.dest = json["Dst"].toString();
        job.time = json["Time"].toString();
        job.command = json["Command"].toString();
        job.flags = jobflags_from_json(jflags);
        job.days = days_from_json(jdays);
        return job;
}

Days Manager::days_from_json(const QJsonObject &json) const
{
        Days days;
        days[0] = json["Mon"].toBool();
        days[1] = json["Tues"].toBool();
        days[2] = json["Wed"].toBool();
        days[3] = json["Thurs"].toBool();
        days[4] = json["Fri"].toBool();
        days[5] = json["Sat"].toBool();
        days[6] = json["Sun"].toBool();
        return days;
}

JobFlags Manager::jobflags_from_json(const QJsonObject &json) const
{
        JobFlags flags;
        flags.delta = json["Delta"].toBool();
        flags.compType = (CompressionType)json["CompressionType"].toInt();
        flags.recurring = json["Recurring"].toBool();
        flags.deleteType = (DeleteType)json["DeleteType"].toInt();
        flags.backupCompression = json["BackupCompression"].toBool();
        flags.transferCompression = json["TransferCompression"].toBool();
        flags.backupType = (BackupType)json["BackupType"].toInt();
        return flags;
}
