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

BackupJob::BackupJob(QString name, QString dest, QString src, QString command, Days days,
                     JobFlags flags, bool enabled)
        : name(name), dest(dest), src(src), command(command), days(days), flags(flags),
          enabled(enabled)
{
}

BackupJob::BackupJob()
{
        name = "";
        dest = "";
        src = "";
        command = "";
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

std::string BackupJob::get_timer() const
{
        if (command == "" || dest == "" || src == "")
                return "";
        QString out = "[Timer]\n";
        out += "Unit=" + name + ".service\n";
        out += "OnCalendar=";
}
