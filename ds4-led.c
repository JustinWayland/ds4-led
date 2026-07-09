#include <fcntl.h>
#include <linux/hidraw.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
  char path[14];
  uint8_t report[32];
  memset(report, 0, 32);
  if (argc <= 1) {
    printf("Color must be supplied to this program.\n");
    return 1;
  }
  report[0] = 0x05;
  report[1] = 0x07;
  if (sscanf(argv[1], "#%2x%2x%2x",&report[6],&report[7],&report[8]) != 3) {
    printf("argv[1] not in the hex string color form.\n");
    return -1;
  }
  int i, fd;
  for (i = 0; i < 8; i++) {
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
  if (i == 8) {
    printf("No Dualshock 4 Controllers connected.\n");
    return -1;
  }
  int res = ioctl(fd, HIDIOCSOUTPUT(sizeof(report)), report);
  if (res < 0) {
    printf("Failed to change Dualshock 4 LED color\n");
    return res;
  }
  return 0;
}
