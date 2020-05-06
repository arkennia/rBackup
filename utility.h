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

#ifndef UTILITY_H
#define UTILITY_H

#include <QMessageBox>
#include <QString>

constexpr char INCREMENTAL_OPTIONS[] = "rsync -auq ";

constexpr char FULL_OPTIONS[] = "rsync -aq ";

constexpr char NO_DELTA[] = "-W ";

constexpr char TRANSFER_COMPRESSION[] = "-z ";

constexpr char DELETE_DURING[] = "--delete-during ";

constexpr char DELETE_AFTER[] = "--delete-after ";

constexpr char DELETE_BEFORE[] = "--delete-before ";

constexpr char TAR[] = "tar -cf ";

constexpr char TAR_GZ[] = "tar -czf "; // .gz

constexpr char TAR_BZ[] = "tar -cjf "; // .bz2

constexpr char TAR_XZ[] = "tar -cJf "; //.xz

inline void show_error_dialog(QString text)
{
        QMessageBox box;
        box.setText(text);
        box.exec();
}

#endif // UTILITY_H
