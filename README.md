## Table of Contents
- [About](#about)
- [Getting Started](#getting-started)
- [Documentation](#documentation)
- [TODO](#todo)
- [Building from Source](#building)

## About

rBackups is a QT GUI built around the rsync program. It utilizes systemd timers to set up automatic backups. It is written in C++ and uses json to store conifgurations. CMake is the provided build system.  
  
The way it works is by creating .service and .timer files with the given job name. These can be found in /usr/lib/systemd/system/.  

The configurations and shell scripts are stored in /etc/rbackup. The shell scripts are run by the systemd service.

Provided is the ability to set most major rsync settings through the GUI itself, however the final command used comes from the "Backup Command" box which can be edited directly by the user. It is then written to a shell script.  

If you want to use compression for the backup, make sure you have the correct software installed.

## Getting Started
Requirements:  
* QT libraries
* systemd
* rsync
* root access
1. After downloading the binary or compiling from source, run the executable rBackup.
2. Select New in the Jobs tab.
3. Input all of the required information to run your job.  
    a. Verify the backup command is correct.
4. Select Finish
5. You can verify the job was set up properly by running:  
    ```
    sudo systemctl status <name of job>.service
    ```

## Documentation
All of the code has Doxygen compatible comments.

## TODO
TODO List

## Building

cd build  
cmake ..  
make  
sudo make install
sudo ./rBackup  
