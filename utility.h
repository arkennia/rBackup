#ifndef UTILITY_H
#define UTILITY_H

#include <QString>

constexpr char INCREMENTAL_OPTIONS[] = "rsync -auq";

constexpr char FULL_OPTIONS[] = "rsync -aq";

constexpr char NO_DELTA[] = " -W";

constexpr char TRANSFER_COMPRESSION[] = " -z";

constexpr char DELETE_DURING[] = " --delete-during";

constexpr char DELETE_AFTER[] = " --delete-after";

constexpr char DELETE_BEFORE[] = " --delete-before";

constexpr char TAR[] = "tar -cf ";

constexpr char TAR_GZ[] = "tar -czf "; // .gz

constexpr char TAR_BZ[] = "tar -cjf "; // .bz2

constexpr char TAR_XZ[] = "tar -cJf "; //.xz

#endif // UTILITY_H
