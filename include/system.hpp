#define _GNU_SOURCE //为了调用syscall,另外还包含了unistd.h，sys/syscall.h
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>   
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <dirent.h>
#include <stdarg.h>
#include <mntent.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>