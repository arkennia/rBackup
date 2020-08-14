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

#include "backupjob.h"
#include <QFile>

const static std::string shortDays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

BackupJob::BackupJob(QString name, QString dest, QString src, QString command, Days days,
                     JobFlags flags, QString time, bool enabled)
        : name(name), dest(dest), src(src), command(command), flags(flags), days(days), time(time),
          enabled(enabled)
{
}

BackupJob::BackupJob()
{
        name = "";
        dest = "";
        src = "";
        command = "";
        time = "";
        days = Days();
        flags = JobFlags();
        enabled = false;
}
// TODO Hardcode this to a user that I ensure is created.
QString BackupJob::get_service() const
{
        if (command == "" || dest == "" || src == "")
                return "";
        QString out = "[Unit]\n";
        out += "Description=Runs an rsync command " + name + "\n\n";
        out += "[Service]\n";
        out += "Type=oneshot\n";
        out += "ExecStart=sh /etc/rbackup/" + name + ".sh\n";
        out += "User=rbackup";
        out += "\n\n";
        out += "[Install]\n";
        out += "WantedBy=multi-user.target\n";
        return out;
}

QString BackupJob::get_timer() const
{
        if (command == "" || dest == "" || src == "")
                return "";
        QString out = "[Unit]\nDescription=Runs the given service at specified time.\n";
        out += "[Timer]\n";
        out += "Unit=" + name + ".service\n";
        out += "OnCalendar=" + make_systemd_calendar();
        out += "\nPersistent=true\n";
        out += "[Install]\n";
        out += "WantedBy=multi-user.target";
        return out;
}

QString BackupJob::to_string() const
{
        QString out = "";
        out += "Job Name: " + name + "\n";
        out += "Destination: " + dest + "\n";
        out += "Source: " + src + "\n";
        out += "Enabled: " + bool_to_string(enabled) + "\n";
        out += jobflags_to_string();
        out += days_to_string();
        out += "Time: " + time + "\n";
        out += "Command: " + command + "\n";
        return out;
}

std::string BackupJob::to_std_string() const
{
        return to_string().toStdString();
}

QString BackupJob::get_name() const
{
        return name;
}

QString BackupJob::get_dest() const
{
        return dest;
}

QString BackupJob::get_src() const
{
        return src;
}

JobFlags BackupJob::get_flags() const
{
        return flags;
}

Days BackupJob::get_days() const
{
        return days;
}

QString BackupJob::get_time() const
{
        return time;
}

QString BackupJob::get_command() const
{
        return command;
}

QString BackupJob::jobflags_to_string() const
{
        QString out = "";
        out += "Flags: \n";
        out += "\tRecurring: " + bool_to_string(flags.recurring) + "\n";
        out += "\tTransfer Compression: " + bool_to_string(flags.transferCompression) + "\n";
        out += "\tBackup Compression: " + bool_to_string(flags.backupCompression) + "\n";
        return out;
}

QString BackupJob::days_to_string() const
{
        std::string out = "";
        out += "Days: ";

        for (size_t i = 0; i < days.size(); i++) {
                if (days[i])
                        out += shortDays[i] + " ";
        }
        out += "\n";

        return QString::fromStdString(out);
}

QString BackupJob::bool_to_string(bool val) const
{
        return (val ? "true" : "false");
}

QString BackupJob::make_systemd_calendar() const
{
        std::string out = "";
        for (size_t i = 0; i < days.size(); i++) {
                if (days[i]) {
                        out += shortDays[i] + ",";
                }
        }
        out += "*-*-* ";
        out += time.toStdString();
        return QString::fromStdString(out);
}

QString BackupJob::make_shell_script() const
{
        return "#!/bin/bash\n\n" + command;
}
