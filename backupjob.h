#ifndef BACKUPJOB_H
#define BACKUPJOB_H

#include <QString>

class BackupJob
{
    public:
        BackupJob();

    private:
        QString name;
        QString dest;
        QString src;
        QString command;
};

#endif // BACKUPJOB_H
