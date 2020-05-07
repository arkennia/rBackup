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

const static std::string shortDays[] = {"M", "Tu", "W", "Th", "F", "Sa", "Su"};

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

std::string BackupJob::get_service() const
{
        if (command == "" || dest == "" || src == "")
                return "";
        QString out = "[Unit]\n";
        out += "Description: Runs an rsync command " + name + "\n\n";
        out += "[Service]\n";
        out += "Type=oneshot\n";
        out += "ExecStart=/bin/" + command;
        out += "\n\n";
        out += "[Install]\n";
        out += "WantedBy=multi-user.target\n";
        return out.toStdString();
}

// TODO Fix this function.
std::string BackupJob::get_timer() const
{
        if (command == "" || dest == "" || src == "")
                return "";
        QString out = "[Timer]\n";
        out += "Unit=" + name + ".service\n";
        out += "OnCalendar=";
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
