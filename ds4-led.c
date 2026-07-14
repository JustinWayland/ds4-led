#include <fcntl.h>
#include <linux/hidraw.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char** argv) {
  char path[14];
  uint8_t report[32];
  DIR* dir;
  struct dirent* entry;
  int count = 0;
  // Initialization
  memset(report, 0, 32);
  if (argc <= 1) {
    perror("Color must be supplied to this program.\n");
    return 1;
  }
  report[0] = 0x05;
  report[1] = 0x07;
  if (sscanf(argv[1], "#%2x%2x%2x",&report[6],&report[7],&report[8]) != 3) {
    perror("argv[1] not in the hex string color form.\n");
    return -1;
  }
  // Count hidraw devices
  dir = opendir("/sys/class/hidraw");
  if (!dir) {
    perror("Unable to open /sys/class/hidraw");
    return -1;
  }
  while ((entry = readdir(dir)) != NULL) {
    if (strncmp(entry->d_name, "hidraw", 6) == 0)
      count++;
  }
  closedir(dir);
  int i, fd;
  for (i = 0; i < count; i++) {
    sprintf(path, "/dev/hidraw%d", i);
    fd = open(path, O_RDWR | O_NONBLOCK);
    struct hidraw_devinfo info;

    if (ioctl(fd, HIDIOCGRAWINFO, &info) >= 0) {
      if (info.vendor == 0x054c && (info.product == 0x05c4 || info.product == 0x09cc)) {
          break;
      }
    }
    close(fd);
  }
  if (i == count) {
    perror("No Dualshock 4 Controllers connected.\n");
    return -1;
  }
  int res = ioctl(fd, HIDIOCSOUTPUT(sizeof(report)), report);
  if (res < 0) {
    perror("Failed to change Dualshock 4 LED color\n");
    return res;
  }
  return 0;
}
