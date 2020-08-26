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
        servicePath = "/usr/lib/systemd/system/";
        configPath = "/etc/rbackup/";
        backupPath = configPath + "backups.json";
        if (!std::filesystem::exists(configPath.toStdString())) {
                bool status = std::filesystem::create_directory(configPath.toStdString());
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

        QFile backups(backupPath);

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
        QJsonObject obj = load_json_document(backupPath);
        if (obj.isEmpty())
                return -1;
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

int Manager::enable_job(const QString &name)
{
        int status = -1;
        if (jobs.count(name.toStdString()) != 0) {
                jobs[name.toStdString()].enabled = true;
                status = create_systemd_objects(name);
                QDBusInterface interface("org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                         "org.freedesktop.systemd1.Manager",
                                         QDBusConnection::systemBus());
                QDBusReply<QDBusVariant> reply;
                QDBusMessage msg;
                QStringList arg = {name + ".timer"};
                if (!interface.isValid()) {
                        show_error_dialog("Unable to connect to systemd.");
                        return -1;
                }
                msg = interface.call("EnableUnitFiles", arg, false, true);
                std::cerr << msg.errorMessage().toStdString() << "\n";
                interface.call("Reload");
        }
        return status;
}

int Manager::disable_job(const QString &name)
{
        int status = -1;
        if (jobs.count(name.toStdString()) != 0) {
                jobs[name.toStdString()].enabled = false;
                status = create_systemd_objects(name);
                QDBusInterface interface("org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                         "org.freedesktop.systemd1.Manager",
                                         QDBusConnection::systemBus());
                QDBusReply<QDBusVariant> reply;
                QDBusMessage msg;
                QStringList arg = {name + ".timer"};
                if (!interface.isValid()) {
                        show_error_dialog("Unable to connect to systemd.");
                        return -1;
                }
                msg = interface.call("DisableUnitFiles", arg, false);
                std::cerr << msg.errorMessage().toStdString() << "\n";
                interface.call("Reload");
        }
        return status;
}

int Manager::run_job(const QString &name)
{
        if (jobs.count(name.toStdString()) != 0) {
                QDBusInterface interface("org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                         "org.freedesktop.systemd1.Manager",
                                         QDBusConnection::systemBus());
                QDBusReply<QDBusObjectPath> reply;
                if (!interface.isValid()) {
                        show_error_dialog("Unable to connect to systemd.");
                        return -1;
                }
                reply = interface.call("StartUnit", name + ".service", "replace");
                if (!reply.isValid()) {
                        std::cerr << reply.error().name().toStdString() << "\n";
                        return -1;
                } else {
                        std::cerr << reply.value().path().toStdString() << "\n";
                        return 0;
                }
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
        json["Service"] = servicePath + job.name + ".service";
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

QString Manager::find_home_directory() const
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
        job.enabled = json["Enabled"].toBool();
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

QJsonObject Manager::load_json_document(const QString &path)
{
        QJsonDocument doc;
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                std::cerr << "File not found or could not be opened.\n";
        } else {
                QJsonParseError err;
                doc = QJsonDocument::fromJson(file.readAll(), &err);
                if (err.error != QJsonParseError::NoError) {
                        std::cerr << "Error: " + err.errorString().toStdString() << "\n";
                } else if (doc.isNull()) {
                        std::cerr << "Configuration file not found.\n";
                }
        }
        return doc.object();
}

int Manager::create_systemd_objects(const QString &name)
{
        QFile timer(servicePath + name + ".timer");
        QFile service(servicePath + name + ".service");
        QFile script(configPath + name + ".sh");

        BackupJob *job = &jobs[name.toStdString()];

        if (!timer.open(QIODevice::WriteOnly | QIODevice::Truncate))
                return -1;
        if (!service.open(QIODevice::WriteOnly | QIODevice::Truncate))
                return -1;
        if (!script.open(QIODevice::WriteOnly | QIODevice::Truncate))
                return -1;
        script.write(job->make_shell_script().toUtf8());
        service.write(job->get_service().toUtf8());
        if (jobs[name.toStdString()].flags.recurring)
                timer.write(job->get_timer().toUtf8());

        script.setPermissions(QFileDevice::ExeUser | QFileDevice::ExeGroup | QFileDevice::ReadUser
                              | QFileDevice::ReadOther);

        return 0;
}

int Manager::delete_job(const QString &name) {
       if(jobs.count(name.toStdString()) != 0) {
                QFile timer(servicePath + name + ".timer");
                QFile service(servicePath + name + ".service");
                QFile script(configPath + name + ".sh");

                bool status = timer.remove();
                if (!status) {
                        std::cerr << "Failed to remove timer\r\n";
                }
                status = service.remove();
                if(!status) {
                        std::cerr << "Failed to remove service\r\n";
                }
                status = script.remove();
                if(!status) {
                        std::cerr << "Failed to remove status\r\n";
                        return -1;
                }
                jobs.erase(name.toStdString());
                save_jobs();
       } 
        return 0;
}
