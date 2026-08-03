#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <glob.h>
#include <stdint.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <sys/syscall.h>
#include <stdbool.h>
#include <limits.h>

static const unsigned char app_label_resolver_dex[2332] = { 0x64, 0x65, 0x78, 0x0a, 0x30, 0x33, 0x35, 0x00, 0xf8, 0x9d, 0x21, 0x24, 0x5a, 0x8e, 0xc5, 0xcb, 0x57, 0x1a, 0xc8, 0x99, 0xfd, 0x57, 0x8b, 0x0a, 0xfb, 0x9f, 0x6f, 0x30, 0x64, 0x30, 0x58, 0x16, 0x1c, 0x09, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x08, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xa8, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x5c, 0x02, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x74, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x24, 0x03, 0x00, 0x00, 0xd8, 0x05, 0x00, 0x00, 0x44, 0x03, 0x00, 0x00, 0xd2, 0x04, 0x00, 0x00, 0xda, 0x04, 0x00, 0x00, 0xf1, 0x04, 0x00, 0x00, 0xf4, 0x04, 0x00, 0x00, 0xf7, 0x04, 0x00, 0x00, 0x0b, 0x05, 0x00, 0x00, 0x0f, 0x05, 0x00, 0x00, 0x13, 0x05, 0x00, 0x00, 0x18, 0x05, 0x00, 0x00, 0x33, 0x05, 0x00, 0x00, 0x59, 0x05, 0x00, 0x00, 0x7e, 0x05, 0x00, 0x00, 0x93, 0x05, 0x00, 0x00, 0xaa, 0x05, 0x00, 0x00, 0xc4, 0x05, 0x00, 0x00, 0xd7, 0x05, 0x00, 0x00, 0xee, 0x05, 0x00, 0x00, 0x02, 0x06, 0x00, 0x00, 0x16, 0x06, 0x00, 0x00, 0x31, 0x06, 0x00, 0x00, 0x45, 0x06, 0x00, 0x00, 0x61, 0x06, 0x00, 0x00, 0x77, 0x06, 0x00, 0x00, 0x89, 0x06, 0x00, 0x00, 0x8c, 0x06, 0x00, 0x00, 0x90, 0x06, 0x00, 0x00, 0x94, 0x06, 0x00, 0x00, 0x97, 0x06, 0x00, 0x00, 0xab, 0x06, 0x00, 0x00, 0xc0, 0x06, 0x00, 0x00, 0xd5, 0x06, 0x00, 0x00, 0xf1, 0x06, 0x00, 0x00, 0xf9, 0x06, 0x00, 0x00, 0xff, 0x06, 0x00, 0x00, 0x06, 0x07, 0x00, 0x00, 0x0d, 0x07, 0x00, 0x00, 0x16, 0x07, 0x00, 0x00, 0x2b, 0x07, 0x00, 0x00, 0x45, 0x07, 0x00, 0x00, 0x50, 0x07, 0x00, 0x00, 0x63, 0x07, 0x00, 0x00, 0x75, 0x07, 0x00, 0x00, 0x7e, 0x07, 0x00, 0x00, 0x86, 0x07, 0x00, 0x00, 0x8f, 0x07, 0x00, 0x00, 0x99, 0x07, 0x00, 0x00, 0x9f, 0x07, 0x00, 0x00, 0xa5, 0x07, 0x00, 0x00, 0xaa, 0x07, 0x00, 0x00, 0xb7, 0x07, 0x00, 0x00, 0xca, 0x07, 0x00, 0x00, 0xd3, 0x07, 0x00, 0x00, 0xdf, 0x07, 0x00, 0x00, 0xe9, 0x07, 0x00, 0x00, 0xef, 0x07, 0x00, 0x00, 0xf2, 0x07, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0xa4, 0x04, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xac, 0x04, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0xb4, 0x04, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xac, 0x04, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0xbc, 0x04, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xc4, 0x04, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0xc4, 0x04, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0xac, 0x04, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0xcc, 0x04, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x03, 0x00, 0x0b, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x06, 0x00, 0x2f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0d, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 0x24, 0x00, 0x00, 0x00, 0x04, 0x00, 0x09, 0x00, 0x25, 0x00, 0x00, 0x00, 0x05, 0x00, 0x0a, 0x00, 0x31, 0x00, 0x00, 0x00, 0x06, 0x00, 0x0a, 0x00, 0x22, 0x00, 0x00, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x32, 0x00, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x23, 0x00, 0x00, 0x00, 0x08, 0x00, 0x07, 0x00, 0x26, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x05, 0x00, 0x34, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x0e, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x05, 0x00, 0x35, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x05, 0x00, 0x34, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x0b, 0x00, 0x20, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x04, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0e, 0x00, 0x29, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x03, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x89, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x70, 0x10, 0x0a, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x07, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x8d, 0x04, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x12, 0x06, 0x71, 0x00, 0x05, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x1e, 0x00, 0x71, 0x10, 0x08, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x1a, 0x01, 0x33, 0x00, 0x23, 0x62, 0x13, 0x00, 0x6e, 0x30, 0x09, 0x00, 0x10, 0x02, 0x0c, 0x01, 0x23, 0x62, 0x14, 0x00, 0x12, 0x03, 0x6e, 0x30, 0x12, 0x00, 0x31, 0x02, 0x0c, 0x01, 0x1a, 0x02, 0x28, 0x00, 0x23, 0x63, 0x13, 0x00, 0x6e, 0x30, 0x09, 0x00, 0x20, 0x03, 0x0c, 0x00, 0x23, 0x62, 0x14, 0x00, 0x6e, 0x30, 0x12, 0x00, 0x10, 0x02, 0x0c, 0x00, 0x1f, 0x00, 0x02, 0x00, 0x6e, 0x10, 0x02, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x6e, 0x20, 0x04, 0x00, 0x60, 0x00, 0x0c, 0x01, 0x72, 0x10, 0x15, 0x00, 0x01, 0x00, 0x0c, 0x01, 0x72, 0x10, 0x13, 0x00, 0x01, 0x00, 0x0a, 0x02, 0x38, 0x02, 0x46, 0x00, 0x72, 0x10, 0x14, 0x00, 0x01, 0x00, 0x0c, 0x02, 0x1f, 0x02, 0x03, 0x00, 0x52, 0x23, 0x00, 0x00, 0xdd, 0x03, 0x03, 0x01, 0x39, 0x03, 0xf0, 0xff, 0x6e, 0x20, 0x03, 0x00, 0x20, 0x00, 0x0c, 0x03, 0x38, 0x03, 0x0b, 0x00, 0x6e, 0x10, 0x0b, 0x00, 0x03, 0x00, 0x0c, 0x03, 0x6e, 0x10, 0x0d, 0x00, 0x03, 0x00, 0x0c, 0x03, 0x28, 0x03, 0x54, 0x23, 0x01, 0x00, 0x6e, 0x10, 0x0c, 0x00, 0x03, 0x00, 0x0a, 0x04, 0x38, 0x04, 0x04, 0x00, 0x54, 0x23, 0x01, 0x00, 0x62, 0x04, 0x02, 0x00, 0x22, 0x05, 0x0c, 0x00, 0x70, 0x10, 0x0e, 0x00, 0x05, 0x00, 0x54, 0x22, 0x01, 0x00, 0x6e, 0x20, 0x0f, 0x00, 0x25, 0x00, 0x0c, 0x02, 0x1a, 0x05, 0x36, 0x00, 0x6e, 0x20, 0x0f, 0x00, 0x52, 0x00, 0x0c, 0x02, 0x6e, 0x20, 0x0f, 0x00, 0x32, 0x00, 0x0c, 0x02, 0x6e, 0x10, 0x10, 0x00, 0x02, 0x00, 0x0c, 0x02, 0x6e, 0x20, 0x07, 0x00, 0x24, 0x00, 0x28, 0xb7, 0x62, 0x00, 0x02, 0x00, 0x6e, 0x10, 0x06, 0x00, 0x00, 0x00, 0x71, 0x10, 0x11, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x01, 0x00, 0x00, 0x00, 0x83, 0x00, 0x01, 0x00, 0x01, 0x01, 0x09, 0x84, 0x01, 0x06, 0x00, 0x0e, 0x00, 0x09, 0x01, 0x00, 0x1d, 0x5a, 0x87, 0xf0, 0xc3, 0x4b, 0x4b, 0xff, 0x69, 0x69, 0xb4, 0x87, 0x01, 0x1f, 0x11, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x14, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x13, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x15, 0x00, 0x06, 0x3c, 0x69, 0x6e, 0x69, 0x74, 0x3e, 0x00, 0x15, 0x41, 0x70, 0x70, 0x4c, 0x61, 0x62, 0x65, 0x6c, 0x52, 0x65, 0x73, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x2e, 0x6a, 0x61, 0x76, 0x61, 0x00, 0x01, 0x49, 0x00, 0x01, 0x4c, 0x00, 0x12, 0x4c, 0x41, 0x70, 0x70, 0x4c, 0x61, 0x62, 0x65, 0x6c, 0x52, 0x65, 0x73, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x3b, 0x00, 0x02, 0x4c, 0x49, 0x00, 0x02, 0x4c, 0x4c, 0x00, 0x03, 0x4c, 0x4c, 0x4c, 0x00, 0x19, 0x4c, 0x61, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2f, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x78, 0x74, 0x3b, 0x00, 0x24, 0x4c, 0x61, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2f, 0x70, 0x6d, 0x2f, 0x41, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x49, 0x6e, 0x66, 0x6f, 0x3b, 0x00, 0x23, 0x4c, 0x61, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2f, 0x70, 0x6d, 0x2f, 0x50, 0x61, 0x63, 0x6b, 0x61, 0x67, 0x65, 0x4d, 0x61, 0x6e, 0x61, 0x67, 0x65, 0x72, 0x3b, 0x00, 0x13, 0x4c, 0x61, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x2f, 0x6f, 0x73, 0x2f, 0x4c, 0x6f, 0x6f, 0x70, 0x65, 0x72, 0x3b, 0x00, 0x15, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x69, 0x6f, 0x2f, 0x50, 0x72, 0x69, 0x6e, 0x74, 0x53, 0x74, 0x72, 0x65, 0x61, 0x6d, 0x3b, 0x00, 0x18, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x43, 0x68, 0x61, 0x72, 0x53, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x63, 0x65, 0x3b, 0x00, 0x11, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x3b, 0x00, 0x15, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x45, 0x78, 0x63, 0x65, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x00, 0x12, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x3b, 0x00, 0x12, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x3b, 0x00, 0x19, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x42, 0x75, 0x69, 0x6c, 0x64, 0x65, 0x72, 0x3b, 0x00, 0x12, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x3b, 0x00, 0x1a, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x72, 0x65, 0x66, 0x6c, 0x65, 0x63, 0x74, 0x2f, 0x4d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x3b, 0x00, 0x14, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x75, 0x74, 0x69, 0x6c, 0x2f, 0x49, 0x74, 0x65, 0x72, 0x61, 0x74, 0x6f, 0x72, 0x3b, 0x00, 0x10, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x75, 0x74, 0x69, 0x6c, 0x2f, 0x4c, 0x69, 0x73, 0x74, 0x3b, 0x00, 0x01, 0x56, 0x00, 0x02, 0x56, 0x49, 0x00, 0x02, 0x56, 0x4c, 0x00, 0x01, 0x5a, 0x00, 0x12, 0x5b, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x3b, 0x00, 0x13, 0x5b, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x3b, 0x00, 0x13, 0x5b, 0x4c, 0x6a, 0x61, 0x76, 0x61, 0x2f, 0x6c, 0x61, 0x6e, 0x67, 0x2f, 0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x3b, 0x00, 0x1a, 0x61, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x2e, 0x61, 0x70, 0x70, 0x2e, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79, 0x54, 0x68, 0x72, 0x65, 0x61, 0x64, 0x00, 0x06, 0x61, 0x70, 0x70, 0x65, 0x6e, 0x64, 0x00, 0x04, 0x65, 0x78, 0x69, 0x74, 0x00, 0x05, 0x66, 0x6c, 0x61, 0x67, 0x73, 0x00, 0x05, 0x66, 0x6c, 0x75, 0x73, 0x68, 0x00, 0x07, 0x66, 0x6f, 0x72, 0x4e, 0x61, 0x6d, 0x65, 0x00, 0x13, 0x67, 0x65, 0x74, 0x41, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x4c, 0x61, 0x62, 0x65, 0x6c, 0x00, 0x18, 0x67, 0x65, 0x74, 0x49, 0x6e, 0x73, 0x74, 0x61, 0x6c, 0x6c, 0x65, 0x64, 0x41, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x00, 0x09, 0x67, 0x65, 0x74, 0x4d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x00, 0x11, 0x67, 0x65, 0x74, 0x50, 0x61, 0x63, 0x6b, 0x61, 0x67, 0x65, 0x4d, 0x61, 0x6e, 0x61, 0x67, 0x65, 0x72, 0x00, 0x10, 0x67, 0x65, 0x74, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x78, 0x74, 0x00, 0x07, 0x68, 0x61, 0x73, 0x4e, 0x65, 0x78, 0x74, 0x00, 0x06, 0x69, 0x6e, 0x76, 0x6f, 0x6b, 0x65, 0x00, 0x07, 0x69, 0x73, 0x45, 0x6d, 0x70, 0x74, 0x79, 0x00, 0x08, 0x69, 0x74, 0x65, 0x72, 0x61, 0x74, 0x6f, 0x72, 0x00, 0x04, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x04, 0x6e, 0x65, 0x78, 0x74, 0x00, 0x03, 0x6f, 0x75, 0x74, 0x00, 0x0b, 0x70, 0x61, 0x63, 0x6b, 0x61, 0x67, 0x65, 0x4e, 0x61, 0x6d, 0x65, 0x00, 0x11, 0x70, 0x72, 0x65, 0x70, 0x61, 0x72, 0x65, 0x4d, 0x61, 0x69, 0x6e, 0x4c, 0x6f, 0x6f, 0x70, 0x65, 0x72, 0x00, 0x07, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x6c, 0x6e, 0x00, 0x0a, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x4d, 0x61, 0x69, 0x6e, 0x00, 0x08, 0x74, 0x6f, 0x53, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x00, 0x04, 0x74, 0x72, 0x69, 0x6d, 0x00, 0x01, 0x7c, 0x00, 0x67, 0x7e, 0x7e, 0x44, 0x38, 0x7b, 0x22, 0x62, 0x61, 0x63, 0x6b, 0x65, 0x6e, 0x64, 0x22, 0x3a, 0x22, 0x64, 0x65, 0x78, 0x22, 0x2c, 0x22, 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2d, 0x6d, 0x6f, 0x64, 0x65, 0x22, 0x3a, 0x22, 0x72, 0x65, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x22, 0x2c, 0x22, 0x68, 0x61, 0x73, 0x2d, 0x63, 0x68, 0x65, 0x63, 0x6b, 0x73, 0x75, 0x6d, 0x73, 0x22, 0x3a, 0x66, 0x61, 0x6c, 0x73, 0x65, 0x2c, 0x22, 0x6d, 0x69, 0x6e, 0x2d, 0x61, 0x70, 0x69, 0x22, 0x3a, 0x31, 0x2c, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x22, 0x3a, 0x22, 0x38, 0x2e, 0x32, 0x2e, 0x33, 0x33, 0x22, 0x7d, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x81, 0x80, 0x04, 0xc4, 0x06, 0x01, 0x09, 0xdc, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xa8, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x5c, 0x02, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x74, 0x02, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x24, 0x03, 0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x44, 0x03, 0x00, 0x00, 0x03, 0x20, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x89, 0x04, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xa4, 0x04, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0xd2, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x5b, 0x08, 0x00, 0x00, 0x03, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x6c, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x70, 0x08, 0x00, 0x00 };


#define PERSISTENT_DIR "/data/adb/rod"
#define SUSFS_CFG_FILE "/data/adb/rod/susfs_config.sh"

#define KSU_INSTALL_MAGIC1 0xDEADBEEF
#define SUSFS_MAGIC 0xFAFAFAFA
#define SUSFS_MAX_LEN_PATHNAME 256
#define ERR_CMD_NOT_SUPPORTED 126

#define CMD_SUSFS_ADD_SUS_PATH 0x55550
#define CMD_SUSFS_ADD_SUS_PATH_LOOP 0x55553
#define CMD_SUSFS_ADD_SUS_MAP 0x60020
#define CMD_SUSFS_SET_CMDLINE_OR_BOOTCONFIG 0x555b0
#define CMD_SUSFS_SET_UNAME 0x55590
#define CMD_SUSFS_HIDE_SUS_MNTS_FOR_NON_SU_PROCS 0x55561
#define CMD_SUSFS_ADD_SUS_KSTAT_STATICALLY 0x55572
#define CMD_SUSFS_SHOW_VERSION 0x555e1

#define KSTAT_SPOOF_INO (1 << 0)
#define KSTAT_SPOOF_DEV (1 << 1)
#define KSTAT_SPOOF_NLINK (1 << 2)
#define KSTAT_SPOOF_SIZE (1 << 3)
#define KSTAT_SPOOF_ATIME_TV_SEC (1 << 4)
#define KSTAT_SPOOF_ATIME_TV_NSEC (1 << 5)
#define KSTAT_SPOOF_MTIME_TV_SEC (1 << 6)
#define KSTAT_SPOOF_MTIME_TV_NSEC (1 << 7)
#define KSTAT_SPOOF_CTIME_TV_SEC (1 << 8)
#define KSTAT_SPOOF_CTIME_TV_NSEC (1 << 9)
#define KSTAT_SPOOF_BLOCKS (1 << 10)
#define KSTAT_SPOOF_BLKSIZE (1 << 11)

struct st_susfs_sus_path {
    char target_pathname[SUSFS_MAX_LEN_PATHNAME];
    int err;
};

struct st_susfs_sus_map {
    char target_pathname[SUSFS_MAX_LEN_PATHNAME];
    int err;
};

struct st_susfs_hide_sus_mnts_for_non_su_procs {
    bool enabled;
    int err;
};

struct st_susfs_uname {
    char release[65];
    char version[65];
    int err;
};

struct st_susfs_spoof_cmdline_or_bootconfig {
    char fake_cmdline_or_bootconfig[8192];
    int err;
};

struct st_susfs_sus_kstat {
    bool is_statically;
    unsigned long target_ino;
    char target_pathname[SUSFS_MAX_LEN_PATHNAME];
    unsigned long spoofed_ino;
    unsigned long spoofed_dev;
    unsigned int spoofed_nlink;
    long long spoofed_size;
    long spoofed_atime_tv_sec;
    unsigned long spoofed_atime_tv_nsec;
    long spoofed_mtime_tv_sec;
    unsigned long spoofed_mtime_tv_nsec;
    long spoofed_ctime_tv_sec;
    unsigned long spoofed_ctime_tv_nsec;
    long long spoofed_blocks;
    long spoofed_blksize;
    int flags;
    int err;
};

struct st_susfs_version {
    char susfs_version[16];
    int err;
};

int susfs_add_sus_path(const char *path) {
    struct st_susfs_sus_path info = {0};
    char resolved_pathname[PATH_MAX];
    if (!realpath(path, resolved_pathname)) {
        return -1;
    }
    strncpy(info.target_pathname, resolved_pathname, SUSFS_MAX_LEN_PATHNAME - 1);
    info.err = ERR_CMD_NOT_SUPPORTED;
    syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_ADD_SUS_PATH, &info);
    return info.err;
}

int susfs_add_sus_path_loop(const char *path) {
    struct st_susfs_sus_path info = {0};
    strncpy(info.target_pathname, path, SUSFS_MAX_LEN_PATHNAME - 1);
    info.err = ERR_CMD_NOT_SUPPORTED;
    syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_ADD_SUS_PATH_LOOP, &info);
    return info.err;
}

int susfs_add_sus_map(const char *path) {
    struct st_susfs_sus_map info = {0};
    strncpy(info.target_pathname, path, SUSFS_MAX_LEN_PATHNAME - 1);
    info.err = ERR_CMD_NOT_SUPPORTED;
    syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_ADD_SUS_MAP, &info);
    return info.err;
}

int susfs_hide_sus_mnts_for_non_su_procs(int enabled) {
    struct st_susfs_hide_sus_mnts_for_non_su_procs info = {0};
    info.enabled = (enabled != 0);
    info.err = ERR_CMD_NOT_SUPPORTED;
    syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_HIDE_SUS_MNTS_FOR_NON_SU_PROCS, &info);
    return info.err;
}

int susfs_set_uname(const char *release, const char *version) {
    struct st_susfs_uname info = {0};
    strncpy(info.release, release, 64);
    strncpy(info.version, version, 64);
    info.err = ERR_CMD_NOT_SUPPORTED;
    syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_SET_UNAME, &info);
    return info.err;
}

int susfs_set_cmdline_or_bootconfig(const char *path) {
    struct st_susfs_spoof_cmdline_or_bootconfig *info = malloc(sizeof(struct st_susfs_spoof_cmdline_or_bootconfig));
    if (!info) return -1;
    memset(info, 0, sizeof(*info));
    char resolved_pathname[PATH_MAX];
    if (!realpath(path, resolved_pathname)) {
        free(info);
        return -1;
    }
    FILE *file = fopen(resolved_pathname, "rb");
    if (!file) {
        free(info);
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size >= 8192) {
        fclose(file);
        free(info);
        return -1;
    }
    rewind(file);
    size_t read_size = fread(info->fake_cmdline_or_bootconfig, 1, file_size, file);
    fclose(file);
    if (read_size != file_size) {
        free(info);
        return -1;
    }
    info->fake_cmdline_or_bootconfig[file_size] = '\0';
    info->err = ERR_CMD_NOT_SUPPORTED;
    syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_SET_CMDLINE_OR_BOOTCONFIG, info);
    int err = info->err;
    free(info);
    return err;
}

int susfs_add_sus_kstat_statically(const char *path, const char *ino_s, const char *dev_s, const char *nlink_s, const char *size_s, const char *atime_s, const char *atime_nsec_s, const char *mtime_s, const char *mtime_nsec_s, const char *ctime_s, const char *ctime_nsec_s, const char *blocks_s, const char *blksize_s) {
    struct st_susfs_sus_kstat info = {0};
    struct stat sb;
    char resolved_pathname[PATH_MAX];
    char *endptr;
    
    if (!realpath(path, resolved_pathname)) return -1;
    if (stat(resolved_pathname, &sb) != 0) return -1;
    
    info.is_statically = true;
    info.target_ino = sb.st_ino;
    
    if (strcmp(ino_s, "default") != 0) {
        sb.st_ino = strtoul(ino_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_INO;
    }
    if (strcmp(dev_s, "default") != 0) {
        sb.st_dev = strtoul(dev_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_DEV;
    }
    if (strcmp(nlink_s, "default") != 0) {
        sb.st_nlink = strtoul(nlink_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_NLINK;
    }
    if (strcmp(size_s, "default") != 0) {
        sb.st_size = strtoul(size_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_SIZE;
    }
    if (strcmp(atime_s, "default") != 0) {
        sb.st_atime = strtol(atime_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_ATIME_TV_SEC;
    }
    if (strcmp(atime_nsec_s, "default") != 0) {
        sb.st_atim.tv_nsec = strtoul(atime_nsec_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_ATIME_TV_NSEC;
    }
    if (strcmp(mtime_s, "default") != 0) {
        sb.st_mtime = strtol(mtime_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_MTIME_TV_SEC;
    }
    if (strcmp(mtime_nsec_s, "default") != 0) {
        sb.st_mtim.tv_nsec = strtoul(mtime_nsec_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_MTIME_TV_NSEC;
    }
    if (strcmp(ctime_s, "default") != 0) {
        sb.st_ctime = strtol(ctime_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_CTIME_TV_SEC;
    }
    if (strcmp(ctime_nsec_s, "default") != 0) {
        sb.st_ctim.tv_nsec = strtoul(ctime_nsec_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_CTIME_TV_NSEC;
    }
    if (strcmp(blocks_s, "default") != 0) {
        sb.st_blocks = strtoul(blocks_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_BLOCKS;
    }
    if (strcmp(blksize_s, "default") != 0) {
        sb.st_blksize = strtoul(blksize_s, &endptr, 10);
        info.flags |= KSTAT_SPOOF_BLKSIZE;
    }
    
    strncpy(info.target_pathname, resolved_pathname, SUSFS_MAX_LEN_PATHNAME - 1);
    
    info.spoofed_ino = sb.st_ino;
    info.spoofed_dev = sb.st_dev;
    info.spoofed_nlink = sb.st_nlink;
    info.spoofed_size = sb.st_size;
    info.spoofed_atime_tv_sec = sb.st_atime;
    info.spoofed_atime_tv_nsec = sb.st_atim.tv_nsec;
    info.spoofed_mtime_tv_sec = sb.st_mtime;
    info.spoofed_mtime_tv_nsec = sb.st_mtim.tv_nsec;
    info.spoofed_ctime_tv_sec = sb.st_ctime;
    info.spoofed_ctime_tv_nsec = sb.st_ctim.tv_nsec;
    info.spoofed_blksize = sb.st_blksize;
    info.spoofed_blocks = sb.st_blocks;
    
    info.err = ERR_CMD_NOT_SUPPORTED;
    syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_ADD_SUS_KSTAT_STATICALLY, &info);
    return info.err;
}

void get_cfg_str(const char *key, char *out, size_t out_len) {
    strncpy(out, "default", out_len);
    FILE *fp = fopen(SUSFS_CFG_FILE, "r");
    if (!fp) return;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *eq = strchr(line, '=');
        if (eq) {
            *eq = '\0';
            char *k = line;
            char *v = eq + 1;
            while (*k == ' ' || *k == '\t') k++;
            char *end = k + strlen(k) - 1;
            while (end > k && (*end == ' ' || *end == '\t')) { *end = '\0'; end--; }
            while (*v == ' ' || *v == '\t') v++;
            end = v + strlen(v) - 1;
            while (end > v && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) { *end = '\0'; end--; }
            
            if (strcmp(k, key) == 0) {
                strncpy(out, v, out_len);
                break;
            }
        }
    }
    fclose(fp);
}

void set_cfg_str(const char *key, const char *val) {
    char keys[100][128];
    char vals[100][256];
    int count = 0;
    int found = 0;

    FILE *fp = fopen(SUSFS_CFG_FILE, "r");
    if (fp) {
        char line[512];
        while (fgets(line, sizeof(line), fp) && count < 100) {
            char *eq = strchr(line, '=');
            if (eq) {
                *eq = '\0';
                char *k = line;
                char *v = eq + 1;
                while (*k == ' ' || *k == '\t') k++;
                char *end = k + strlen(k) - 1;
                while (end > k && (*end == ' ' || *end == '\t')) { *end = '\0'; end--; }
                while (*v == ' ' || *v == '\t') v++;
                end = v + strlen(v) - 1;
                while (end > v && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) { *end = '\0'; end--; }
                
                if (strcmp(k, key) == 0) {
                    strncpy(vals[count], val, sizeof(vals[count]));
                    found = 1;
                } else {
                    strncpy(vals[count], v, sizeof(vals[count]));
                }
                strncpy(keys[count], k, sizeof(keys[count]));
                count++;
            }
        }
        fclose(fp);
    }

    if (!found && count < 100) {
        strncpy(keys[count], key, sizeof(keys[count]));
        strncpy(vals[count], val, sizeof(vals[count]));
        count++;
    }

    mkdir(PERSISTENT_DIR, 0755);
    fp = fopen(SUSFS_CFG_FILE, "w");
    if (fp) {
        for (int i = 0; i < count; i++) {
            fprintf(fp, "%s=%s\n", keys[i], vals[i]);
        }
        fclose(fp);
    }
}

int get_cfg(const char *key) {
    char buf[128] = "0";
    get_cfg_str(key, buf, sizeof(buf));
    return atoi(buf);
}

void set_cfg(const char *key, int val) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", val);
    set_cfg_str(key, buf);
}

void init_config() {
    mkdir(PERSISTENT_DIR, 0755);
    
    char kstat_path[256];
    snprintf(kstat_path, sizeof(kstat_path), "%s/sus_kstat_statically.json", PERSISTENT_DIR);
    
    FILE *fp = fopen(kstat_path, "r");
    if (!fp) {
        fp = fopen(kstat_path, "w");
        if (fp) {
            fprintf(fp, "{\n  \"path\": \"/data/local/tmp\",\n  \"ino\": \"4521\",\n  \"dev\": \"2063\",\n  \"nlink\": \"2\",\n  \"size\": \"4096\",\n  \"atime\": \"default\",\n  \"atime_nsec\": \"default\",\n  \"mtime\": \"default\",\n  \"mtime_nsec\": \"default\",\n  \"ctime\": \"default\",\n  \"ctime_nsec\": \"default\",\n  \"blocks\": \"8\",\n  \"blksize\": \"4096\"\n}\n");
            fclose(fp);
        }
    } else {
        fclose(fp);
    }
    
    fp = fopen(SUSFS_CFG_FILE, "r");
    if (!fp) {
        set_cfg("hide_sus_mnts_for_non_su_procs", 1);
        set_cfg("spoof_cmdline", 1);
        set_cfg("spoof_uname", 0);
        set_cfg_str("kernel_version", "default");
        set_cfg_str("kernel_build", "default");
    } else {
        fclose(fp);
    }

    char tmp[256];
    snprintf(tmp, sizeof(tmp), "%s/sus_path.txt", PERSISTENT_DIR);
    if (access(tmp, F_OK) != 0) {
        FILE *f = fopen(tmp, "w");
        if (f) {
            fprintf(f, "/data/adb/ksu\n/data/adb/magisk\n/data/adb/apatch\n/data/adb/modules\n/data/adb/ksud\n");
            fclose(f);
        }
    }
    snprintf(tmp, sizeof(tmp), "%s/sus_path_loop.txt", PERSISTENT_DIR);
    close(open(tmp, O_WRONLY | O_CREAT | O_APPEND, 0644));
    snprintf(tmp, sizeof(tmp), "%s/sus_maps.txt", PERSISTENT_DIR);
    close(open(tmp, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

int find_vbmeta(char *path, size_t max_len) {
    char slot[32] = "";
    FILE *fp = popen("getprop ro.boot.slot_suffix", "r");
    if (fp) {
        if (!fgets(slot, sizeof(slot), fp)) slot[0] = '\0';
        pclose(fp);
        size_t len = strlen(slot);
        while (len > 0 && (slot[len - 1] == '\n' || slot[len - 1] == '\r')) {
            slot[len - 1] = '\0';
            len--;
        }
    }

    const char *paths[] = {
        "/dev/block/by-name/vbmeta%s",
        "/dev/block/by-name/vbmeta",
        "/dev/block/platform/soc/*/by-name/vbmeta%s",
        "/dev/block/platform/soc/*/by-name/vbmeta",
        NULL
    };

    for (int i = 0; paths[i] != NULL; i++) {
        char formatted[512];
        snprintf(formatted, sizeof(formatted), paths[i], slot);
        
        glob_t glob_results;
        if (glob(formatted, 0, NULL, &glob_results) == 0) {
            if (glob_results.gl_pathc > 0) {
                strncpy(path, glob_results.gl_pathv[0], max_len);
                globfree(&glob_results);
                return 0;
            }
            globfree(&glob_results);
        }
    }
    return -1;
}

int calculate_vbmeta_digest(char *hash_out, size_t out_len) {
    char prop_digest[128] = "";
    FILE *fp = popen("getprop ro.boot.vbmeta.digest", "r");
    if (fp) {
        if (fgets(prop_digest, sizeof(prop_digest), fp)) {
            size_t len = strlen(prop_digest);
            while (len > 0 && (prop_digest[len - 1] == '\n' || prop_digest[len - 1] == '\r')) {
                prop_digest[len - 1] = '\0';
                len--;
            }
        }
        pclose(fp);
    }
    if (strlen(prop_digest) > 0 && strcmp(prop_digest, "0000000000000000000000000000000000000000000000000000000000000000") != 0) {
        strncpy(hash_out, prop_digest, out_len);
        return 0;
    }

    char dev_path[512] = "";
    if (find_vbmeta(dev_path, sizeof(dev_path)) != 0) {
        return -1;
    }

    FILE *fd = fopen(dev_path, "rb");
    if (!fd) return -1;

    uint64_t auth_size = 0;
    fseek(fd, 12, SEEK_SET);
    uint8_t buf[8];
    if (fread(buf, 1, 8, fd) == 8) {
        auth_size = ((uint64_t)buf[0] << 56) | ((uint64_t)buf[1] << 48) |
                    ((uint64_t)buf[2] << 40) | ((uint64_t)buf[3] << 32) |
                    ((uint64_t)buf[4] << 24) | ((uint64_t)buf[5] << 16) |
                    ((uint64_t)buf[6] << 8)  | (uint64_t)buf[7];
    } else {
        fclose(fd);
        return -1;
    }

    uint64_t aux_size = 0;
    fseek(fd, 20, SEEK_SET);
    if (fread(buf, 1, 8, fd) == 8) {
        aux_size = ((uint64_t)buf[0] << 56) | ((uint64_t)buf[1] << 48) |
                   ((uint64_t)buf[2] << 40) | ((uint64_t)buf[3] << 32) |
                   ((uint64_t)buf[4] << 24) | ((uint64_t)buf[5] << 16) |
                   ((uint64_t)buf[6] << 8)  | (uint64_t)buf[7];
    } else {
        fclose(fd);
        return -1;
    }
    fclose(fd);

    uint64_t total_size = 256 + auth_size + aux_size;
    
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "dd if=\"%s\" bs=1 count=%llu 2>/dev/null | sha256sum | cut -d' ' -f1", dev_path, (unsigned long long)total_size);
    FILE *sha_fp = popen(cmd, "r");
    if (sha_fp) {
        if (fgets(hash_out, out_len, sha_fp)) {
            size_t len = strlen(hash_out);
            while (len > 0 && (hash_out[len - 1] == '\n' || hash_out[len - 1] == '\r')) {
                hash_out[len - 1] = '\0';
                len--;
            }
        }
        pclose(sha_fp);
        return 0;
    }
    return -1;
}

int find_hma_dir(char *path, size_t max_len) {
    glob_t glob_results;
    if (glob("/data/misc/hide_my_applist_*", 0, NULL, &glob_results) == 0) {
        if (glob_results.gl_pathc > 0) {
            strncpy(path, glob_results.gl_pathv[0], max_len);
            globfree(&glob_results);
            return 0;
        }
        globfree(&glob_results);
    }
    return -1;
}

void get_teesim_dir(char *path, size_t max_len) {
    struct stat st;
    if (stat("/data/tricky_store", &st) == 0 && S_ISDIR(st.st_mode)) {
        strncpy(path, "/data/tricky_store", max_len);
    } else {
        strncpy(path, "/data/adb/tricky_store", max_len);
    }
}

int check_susfs() {
    struct st_susfs_version info = {0};
    info.err = ERR_CMD_NOT_SUPPORTED;
    syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_SHOW_VERSION, &info);
    if (info.err == 0) return 1;

    // Fallback to dmesg
    FILE *fp = popen("dmesg 2>/dev/null | grep -q \"susfs:\" && echo \"1\"", "r");
    if (fp) {
        char buf[32] = "";
        fgets(buf, sizeof(buf), fp);
        pclose(fp);
        if (atoi(buf) == 1) return 1;
    }
    return 0;
}

void apply_uname_spoof() {
    char kver[128] = "default";
    char kbuild[256] = "default";
    get_cfg_str("kernel_version", kver, sizeof(kver));
    get_cfg_str("kernel_build", kbuild, sizeof(kbuild));
    
    if (strcmp(kver, "default") == 0 || strlen(kver) == 0) {
        char current[128] = "";
        FILE *fp = popen("uname -r", "r");
        if (fp) {
            fgets(current, sizeof(current), fp);
            pclose(fp);
            size_t len = strlen(current);
            while (len > 0 && (current[len - 1] == '\n' || current[len - 1] == '\r')) {
                current[len - 1] = '\0';
                len--;
            }
        }
        
        int major = 0, minor = 0;
        sscanf(current, "%d.%d", &major, &minor);
        if (major > 5 || (major == 5 && minor >= 10)) {
            char base_gki[128] = "";
            FILE *gki_fp = popen("echo \"$current\" | grep -oE -- '^[0-9]+\\.[0-9]+\\.[0-9]+-android[0-9]+(-[0-9]+)?' | head -n1", "r");
            if (gki_fp) {
                fgets(base_gki, sizeof(base_gki), gki_fp);
                pclose(gki_fp);
                size_t len = strlen(base_gki);
                while (len > 0 && (base_gki[len - 1] == '\n' || base_gki[len - 1] == '\r')) {
                    base_gki[len - 1] = '\0';
                    len--;
                }
            }
            if (strlen(base_gki) == 0) {
                char *dash = strchr(current, '-');
                if (dash) {
                    char *dash2 = strchr(dash + 1, '-');
                    if (dash2) *dash2 = '\0';
                }
                strncpy(base_gki, current, sizeof(base_gki));
            }
            
            if (strstr(base_gki, "-android") && !strstr(base_gki, "-android12-") && !strstr(base_gki, "-android14-")) {
                char *android = strstr(base_gki, "-android");
                int ver = atoi(android + 8);
                if (ver == 12) {
                    strcat(base_gki, "-9");
                } else if (ver == 14) {
                    strcat(base_gki, "-11");
                } else {
                    strcat(base_gki, "-9");
                }
            }
            
            char hash_cmd[256];
            snprintf(hash_cmd, sizeof(hash_cmd), "echo \"spoof_%s\" | md5sum | cut -c1-12", current);
            char fake_hash[64] = "";
            FILE *hash_fp = popen(hash_cmd, "r");
            if (hash_fp) {
                fgets(fake_hash, sizeof(fake_hash), hash_fp);
                pclose(hash_fp);
                size_t len = strlen(fake_hash);
                while (len > 0 && (fake_hash[len - 1] == '\n' || fake_hash[len - 1] == '\r')) {
                    fake_hash[len - 1] = '\0';
                    len--;
                }
            }
            snprintf(kver, sizeof(kver), "%s-g%s", base_gki, fake_hash);
        } else {
            strcpy(kver, "default");
        }
    }
    
    if (strcmp(kbuild, "default") == 0 || strlen(kbuild) == 0) {
        char rom_date[128] = "";
        FILE *fp = popen("getprop ro.build.date 2>/dev/null", "r");
        if (fp) {
            fgets(rom_date, sizeof(rom_date), fp);
            pclose(fp);
            size_t len = strlen(rom_date);
            while (len > 0 && (rom_date[len - 1] == '\n' || rom_date[len - 1] == '\r')) {
                rom_date[len - 1] = '\0';
                len--;
            }
        }
        if (strlen(rom_date) > 0) {
            snprintf(kbuild, sizeof(kbuild), "#1 SMP PREEMPT %s", rom_date);
        } else {
            strcpy(kbuild, "default");
        }
    }
    
    if (strcmp(kver, "default") != 0 && strcmp(kbuild, "default") != 0) {
        susfs_set_uname(kver, kbuild);
    }
}

void apply_susfs_post_fs_data() {
    init_config();
    if (!check_susfs()) return;
    
    if (get_cfg("spoof_cmdline") == 1) {
        char fake_bc[256], fake_cmd[256];
        snprintf(fake_bc, sizeof(fake_bc), "%s/fake_bootconfig", PERSISTENT_DIR);
        snprintf(fake_cmd, sizeof(fake_cmd), "%s/fake_cmdline", PERSISTENT_DIR);
        
        struct stat st;
        if (stat("/proc/bootconfig", &st) == 0) {
            char cmd[1024];
            snprintf(cmd, sizeof(cmd), 
                     "cat /proc/bootconfig > \"%s\" && "
                     "sed -i 's/androidboot.warranty_bit = \"1\"/androidboot.warranty_bit = \"0\"/' \"%s\" && "
                     "sed -i 's/androidboot.verifiedbootstate = \"orange\"/androidboot.verifiedbootstate = \"green\"/' \"%s\"",
                     fake_bc, fake_bc, fake_bc);
            if (system(cmd) == 0) {
                susfs_set_cmdline_or_bootconfig(fake_bc);
            }
        }
        if (stat("/proc/cmdline", &st) == 0) {
            char cmd[1024];
            snprintf(cmd, sizeof(cmd), 
                     "cat /proc/cmdline > \"%s\" && "
                     "sed -i 's/androidboot.warranty_bit=1/androidboot.warranty_bit=0/' \"%s\" && "
                     "sed -i 's/androidboot.verifiedbootstate=orange/androidboot.verifiedbootstate=green/' \"%s\"",
                     fake_cmd, fake_cmd, fake_cmd);
            if (system(cmd) == 0) {
                susfs_set_cmdline_or_bootconfig(fake_cmd);
            }
        }
    }
    
    susfs_hide_sus_mnts_for_non_su_procs(get_cfg("hide_sus_mnts_for_non_su_procs"));
    
    if (get_cfg("spoof_uname") == 1) {
        apply_uname_spoof();
    }
    
    // Apply path rules
    char path_file[256];
    snprintf(path_file, sizeof(path_file), "%s/sus_path.txt", PERSISTENT_DIR);
    FILE *fp = fopen(path_file, "r");
    if (fp) {
        char line[512];
        while (fgets(line, sizeof(line), fp)) {
            size_t len = strlen(line);
            while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) { line[len-1] = '\0'; len--; }
            if (len > 0 && line[0] != '#') {
                susfs_add_sus_path(line);
            }
        }
        fclose(fp);
    }
    
    snprintf(path_file, sizeof(path_file), "%s/sus_path_loop.txt", PERSISTENT_DIR);
    fp = fopen(path_file, "r");
    if (fp) {
        char line[512];
        while (fgets(line, sizeof(line), fp)) {
            size_t len = strlen(line);
            while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) { line[len-1] = '\0'; len--; }
            if (len > 0 && line[0] != '#') {
                susfs_add_sus_path_loop(line);
            }
        }
        fclose(fp);
    }
}

void apply_susfs_boot_completed() {
    init_config();
    if (!check_susfs()) return;
    FILE *fp = NULL;
    char cmd[1024];
    // Removed non-essential KernelSU/Android toggles
    
    // Apply static rules
    char kstat_file[256];
    snprintf(kstat_file, sizeof(kstat_file), "%s/sus_kstat_statically.json", PERSISTENT_DIR);
    fp = fopen(kstat_file, "r");
    if (fp) {
        fclose(fp);
        // Simplest way is to reuse a small shell invocation to parse JSON keys cleanly
        snprintf(cmd, sizeof(cmd), 
                 "awk '/^[[:space:]]*{/,/^[[:space:]]*}/' \"%s\" | {"
                 "  current_obj=\"\";"
                 "  while IFS= read -r line; do"
                 "    if echo \"$line\" | grep -q '^[[:space:]]*{'; then current_obj=\"\"; fi;"
                 "    current_obj=\"$current_obj $line\";"
                 "    if echo \"$line\" | grep -q '^[[:space:]]*}'; then"
                 "      IFS='	' read -r path ino dev nlink size atime atime_nsec mtime mtime_nsec ctime ctime_nsec blocks blksize <<EOF\n"
                 "$(echo \"$current_obj\" | awk '"
                 "      {"
                 "        while (match($0, /\"[a-z_]+\"[[:space:]]*:[[:space:]]*\"[^\"]*\"/)) {"
                 "          pair = substr($0, RSTART, RLENGTH);"
                 "          $0 = substr($0, RSTART + RLENGTH);"
                 "          k = pair; sub(/\"[[:space:]]*:.*/, \"\", k); sub(/^\"/, \"\", k);"
                 "          v = pair; sub(/^[^:]*:[[:space:]]*\"/, \"\", v); sub(/\"$/, \"\", v);"
                 "          if (!(k in seen)) { seen[k] = 1; val[k] = v }"
                 "        }"
                 "      }"
                 "      END {"
                 "        printf \"%%s\\t%%s\\t%%s\\t%%s\\t%%s\\t%%s\\t%%s\\t%%s\\t%%s\\t%%s\\t%%s\\t%%s\\t%%s\\n\", \\"
                 "          val[\"path\"], val[\"ino\"], val[\"dev\"], val[\"nlink\"], val[\"size\"], \\"
                 "          val[\"atime\"], val[\"atime_nsec\"], val[\"mtime\"], val[\"mtime_nsec\"], \\"
                 "          val[\"ctime\"], val[\"ctime_nsec\"], val[\"blocks\"], val[\"blksize\"]"
                 "      }')\n"
                 "EOF\n"
                 "      if [ -n \"$path\" ]; then"
                 "        /data/adb/modules/rod/webroot/rodd susfs add_sus_kstat_statically \"$path\" \"$ino\" \"$dev\" \"$nlink\" \"$size\" \"$atime\" \"$atime_nsec\" \"$mtime\" \"$mtime_nsec\" \"$ctime\" \"$ctime_nsec\" \"$blocks\" \"$blksize\" 2>/dev/null;"
                 "      fi;"
                 "      current_obj=\"\";"
                 "    fi;"
                 "  done;"
                 "} 2>/dev/null", kstat_file);
        system(cmd);
    }
    
    // Apply maps rules
    char maps_file[256];
    snprintf(maps_file, sizeof(maps_file), "%s/sus_maps.txt", PERSISTENT_DIR);
    fp = fopen(maps_file, "r");
    if (fp) {
        char line[512];
        while (fgets(line, sizeof(line), fp)) {
            size_t len = strlen(line);
            while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) { line[len-1] = '\0'; len--; }
            if (len > 0 && line[0] != '#') {
                susfs_add_sus_map(line);
            }
        }
        fclose(fp);
    }
}

void handle_command(int argc, char *args[]) {
    char response[8192] = "";

    if (strcmp(args[0], "susfs") == 0) {
        if (argc >= 2 && strcmp(args[1], "status") == 0) {
            init_config();
            int supported = check_susfs();
            snprintf(response, sizeof(response),
                     "{\n"
                     "  \"supported\": %s,\n"
                     "  \"hide_sus_mnts_for_non_su_procs\": %s,\n"
                     "  \"spoof_cmdline\": %s,\n"
                     "  \"spoof_uname\": %s,\n"
                     "  \"kernel_version\": \"%s\",\n"
                     "  \"kernel_build\": \"%s\"\n"
                     "}\n",
                     supported ? "true" : "false",
                     get_cfg("hide_sus_mnts_for_non_su_procs") ? "true" : "false",
                     get_cfg("spoof_cmdline") ? "true" : "false",
                     get_cfg("spoof_uname") ? "true" : "false",
                     ({ char v[128]; get_cfg_str("kernel_version", v, sizeof(v)); v; }),
                     ({ char b[256]; get_cfg_str("kernel_build", b, sizeof(b)); b; }));
        } else if (argc >= 4 && strcmp(args[1], "toggle") == 0) {
            const char *key = args[2];
            int val = strcmp(args[3], "true") == 0 ? 1 : 0;
            set_cfg(key, val);
            apply_susfs_post_fs_data();
            apply_susfs_boot_completed();
            strcpy(response, "OK\n");
        } else if (argc >= 4 && strcmp(args[1], "set_val") == 0) {
            const char *key = args[2];
            const char *val = args[3];
            set_cfg_str(key, val);
            apply_susfs_post_fs_data();
            apply_susfs_boot_completed();
            strcpy(response, "OK\n");
        } else if (argc >= 3 && strcmp(args[1], "add_sus_path") == 0) {
            int ret = susfs_add_sus_path(args[2]);
            snprintf(response, sizeof(response), "ret: %d\n", ret);
        } else if (argc >= 3 && strcmp(args[1], "add_sus_path_loop") == 0) {
            int ret = susfs_add_sus_path_loop(args[2]);
            snprintf(response, sizeof(response), "ret: %d\n", ret);
        } else if (argc >= 3 && strcmp(args[1], "add_sus_map") == 0) {
            int ret = susfs_add_sus_map(args[2]);
            snprintf(response, sizeof(response), "ret: %d\n", ret);
        } else if (argc >= 3 && strcmp(args[1], "hide_sus_mnts_for_non_su_procs") == 0) {
            int ret = susfs_hide_sus_mnts_for_non_su_procs(atoi(args[2]));
            snprintf(response, sizeof(response), "ret: %d\n", ret);
        } else if (argc >= 4 && strcmp(args[1], "set_uname") == 0) {
            int ret = susfs_set_uname(args[2], args[3]);
            snprintf(response, sizeof(response), "ret: %d\n", ret);
        } else if (argc >= 3 && strcmp(args[1], "set_cmdline_or_bootconfig") == 0) {
            int ret = susfs_set_cmdline_or_bootconfig(args[2]);
            snprintf(response, sizeof(response), "ret: %d\n", ret);
        } else if (argc >= 15 && strcmp(args[1], "add_sus_kstat_statically") == 0) {
            int ret = susfs_add_sus_kstat_statically(args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14]);
            snprintf(response, sizeof(response), "ret: %d\n", ret);
        } else if (argc >= 3 && strcmp(args[1], "get_rule") == 0) {
            char rule_path[256];
            snprintf(rule_path, sizeof(rule_path), "%s/%s", PERSISTENT_DIR, args[2]);
            FILE *fp = fopen(rule_path, "r");
            if (fp) {
                size_t r = fread(response, 1, sizeof(response) - 1, fp);
                response[r] = '\0';
                fclose(fp);
            } else {
                strcpy(response, "");
            }
        } else if (argc >= 3 && strcmp(args[1], "set_rule") == 0) {
            char rule_path[256];
            snprintf(rule_path, sizeof(rule_path), "%s/%s", PERSISTENT_DIR, args[2]);
            
            // Reconstruct rule content by joining subsequent arguments
            FILE *fp = fopen(rule_path, "w");
            if (fp) {
                if (argc >= 4) {
                    fprintf(fp, "%s", args[3]);
                }
                fclose(fp);
                apply_susfs_post_fs_data();
                apply_susfs_boot_completed();
                strcpy(response, "OK\n");
            } else {
                strcpy(response, "ERROR: failed to write rule\n");
            }
        } else if (argc >= 2 && strcmp(args[1], "apply_post_fs_data") == 0) {
            apply_susfs_post_fs_data();
            strcpy(response, "OK\n");
        } else if (argc >= 2 && strcmp(args[1], "apply_boot_completed") == 0) {
            apply_susfs_boot_completed();
            strcpy(response, "OK\n");
        }
    } else if (strcmp(args[0], "teesimulator") == 0) {
        char ts_dir[256] = "";
        get_teesim_dir(ts_dir, sizeof(ts_dir));
        char kb_xml[256], tg_txt[256];
        snprintf(kb_xml, sizeof(kb_xml), "%s/keybox.xml", ts_dir);
        snprintf(tg_txt, sizeof(tg_txt), "%s/target.txt", ts_dir);

        if (argc >= 2 && strcmp(args[1], "status") == 0) {
            struct stat st;
            int installed = (stat(ts_dir, &st) == 0 && S_ISDIR(st.st_mode));
            int has_kb = (stat(kb_xml, &st) == 0 && S_ISREG(st.st_mode));
            int has_tg = (stat(tg_txt, &st) == 0 && S_ISREG(st.st_mode));
            
            char boot_hash[128] = "";
            calculate_vbmeta_digest(boot_hash, sizeof(boot_hash));
            
            char applied_hash[128] = "";
            FILE *fp = fopen("/data/adb/boot_hash", "r");
            if (fp) {
                fgets(applied_hash, sizeof(applied_hash), fp);
                fclose(fp);
                size_t len = strlen(applied_hash);
                while (len > 0 && (applied_hash[len - 1] == '\n' || applied_hash[len - 1] == '\r')) {
                    applied_hash[len - 1] = '\0';
                    len--;
                }
            }

            snprintf(response, sizeof(response),
                     "{\n"
                     "  \"installed\": %s,\n"
                     "  \"has_keybox\": %s,\n"
                     "  \"has_target\": %s,\n"
                     "  \"boot_hash\": \"%s\",\n"
                     "  \"applied_hash\": \"%s\",\n"
                     "  \"tricky_dir\": \"%s\"\n"
                     "}\n",
                     installed ? "true" : "false",
                     has_kb ? "true" : "false",
                     has_tg ? "true" : "false",
                     boot_hash, applied_hash, ts_dir);
        } else if (argc >= 2 && strcmp(args[1], "apply_boot_hash") == 0) {
            char boot_hash[128] = "";
            if (calculate_vbmeta_digest(boot_hash, sizeof(boot_hash)) == 0) {
                mkdir("/data/adb", 0755);
                FILE *fp = fopen("/data/adb/boot_hash", "w");
                if (fp) {
                    fprintf(fp, "%s\n", boot_hash);
                    fclose(fp);
                }
                mkdir(ts_dir, 0755);
                char ts_bh[256];
                snprintf(ts_bh, sizeof(ts_bh), "%s/boot_hash", ts_dir);
                fp = fopen(ts_bh, "w");
                if (fp) {
                    fprintf(fp, "%s\n", boot_hash);
                    fclose(fp);
                }
                snprintf(response, sizeof(response), "%s\n", boot_hash);
            } else {
                strcpy(response, "ERROR: failed to calculate vbmeta digest\n");
            }
        } else if (argc >= 2 && strcmp(args[1], "get_targets") == 0) {
            FILE *fp = fopen(tg_txt, "r");
            if (fp) {
                size_t r = fread(response, 1, sizeof(response) - 1, fp);
                response[r] = '\0';
                fclose(fp);
            } else {
                strcpy(response, "");
            }
        } else if (strcmp(args[1], "set_targets") == 0) {
            mkdir(ts_dir, 0755);
            FILE *fp = fopen(tg_txt, "w");
            if (fp) {
                for (int i = 2; i < argc; i++) {
                    fprintf(fp, "%s\n", args[i]);
                }
                fclose(fp);
                chmod(tg_txt, 0644);
                strcpy(response, "OK\n");
            } else {
                strcpy(response, "ERROR: failed to open target.txt for writing\n");
            }
        } else if (argc >= 4 && strcmp(args[1], "fetch_specter_keybox") == 0) {
            const char *source = args[2];
            const char *version = args[3];
            
            char cmd[1024];
            snprintf(cmd, sizeof(cmd),
                     "url='https://rawbin.dpejoh.com/key/%s/%s'; "
                     "tmp_enc='/data/local/tmp/specter_kb_daemon.enc'; "
                     "tmp_dec='/data/local/tmp/specter_kb_daemon.dec'; "
                     "curl -sSL \"$url\" > \"$tmp_enc\" && "
                     "tr '1dgWnocayqxU3r6vA5lCIPYfHmkV08b4tz+KMsp2NQ9LRXihODwSj7BEFJ/ZuGTe' 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/' < \"$tmp_enc\" > \"$tmp_dec\" && "
                     "mkdir -p \"%s\" && "
                     "base64 -d < \"$tmp_dec\" > \"%s\" && "
                     "chown system:system \"%s\" && "
                     "chmod 600 \"%s\" && "
                     "rm -f \"$tmp_enc\" \"$tmp_dec\" && "
                     "echo 'OK: Specter keybox installed'",
                     source, version, ts_dir, kb_xml, kb_xml, kb_xml);
            FILE *fp = popen(cmd, "r");
            if (fp) {
                size_t r = fread(response, 1, sizeof(response) - 1, fp);
                response[r] = '\0';
                pclose(fp);
            } else {
                strcpy(response, "ERROR: failed to run catalog installation\n");
            }
        } else if (argc >= 2 && strcmp(args[1], "check_revocation") == 0) {
            char status[256] = "";
            
            const char *cmd =
                "KEYBOX='/data/adb/tricky_store/keybox.xml'; "
                "[ -d '/data/tricky_store' ] && KEYBOX='/data/tricky_store/keybox.xml'; "
                "if [ ! -f \"$KEYBOX\" ]; then echo 'ERROR: No keybox.xml found'; exit 1; fi; "
                "b64=$(sed -n '/-----BEGIN CERTIFICATE-----/,/-----END CERTIFICATE-----/p; /-----END CERTIFICATE-----/q' \"$KEYBOX\" | grep -v 'CERTIFICATE' | tr -d ' \n'); "
                "if [ -z \"$b64\" ]; then echo 'ERROR: No certificate found'; exit 1; fi; "
                "hex=$(echo \"$b64\" | base64 -d 2>/dev/null | od -v -tx1 | awk 'BEGIN{ORS=\"\"} {for(i=2;i<=NF;i++) printf \"%s\", $i}'); "
                "if [ -z \"$hex\" ]; then echo 'ERROR: Failed to parse hex'; exit 1; fi; "
                "parse_serial() { "
                "  _h=\"$1\"; case \"${_h:0:1}\" in \"\") return 1 ;; esac 2>/dev/null; "
                "  case \"$_h\" in 30*) _h=\"${_h#30}\" ;; *) return 1 ;; esac; "
                "  _l_hex=\"${_h:0:2}\"; _l_dec=$((16#$_l_hex)); [ $_l_dec -ge 128 ] && _h=\"${_h:2 + ($_l_dec - 128) * 2}\" || _h=\"${_h:2}\"; "
                "  case \"$_h\" in 30*) _h=\"${_h#30}\" ;; *) return 1 ;; esac; "
                "  _l_hex=\"${_h:0:2}\"; _l_dec=$((16#$_l_hex)); [ $_l_dec -ge 128 ] && _h=\"${_h:2 + ($_l_dec - 128) * 2}\" || _h=\"${_h:2}\"; "
                "  case \"$_h\" in a0*) _ctx_len_hex=\"${_h:2:2}\"; _ctx_len=$((16#$_ctx_len_hex)); _h=\"${_h:4 + _ctx_len * 2}\" ;; esac; "
                "  case \"$_h\" in 02*) _h=\"${_h#02}\" ;; *) return 1 ;; esac; "
                "  _l_hex=\"${_h:0:2}\"; _l_dec=$((16#$_l_hex)); "
                "  if [ $_l_dec -ge 128 ]; then _n=$((_l_dec - 128)); _sl=$((16#${_h:2:_n * 2})); _serial_hex=\"${_h:2 + _n * 2:$_sl * 2}\"; else _serial_hex=\"${_h:2:$_l_dec * 2}\"; fi; "
                "  _serial=$(echo \"$_serial_hex\" | sed 's/^0*//'); [ -z \"$_serial\" ] && _serial=\"0\"; "
                "  echo \"$_serial\"; return 0; "
                "}; "
                "serial=$(parse_serial \"$hex\"); "
                "if [ -z \"$serial\" ]; then echo 'ERROR: Failed to parse serial'; exit 1; fi; "
                "resp=$(curl -sSL \"https://android.googleapis.com/attestation/status?encrypted=0\" 2>/dev/null); "
                "if [ -z \"$resp\" ]; then echo 'ERROR: Failed to fetch status'; exit 1; fi; "
                "if echo \"$resp\" | grep -q \"\\\"$serial\\\"\"; then echo 'REVOKED'; "
                "else "
                "  if command -v bc >/dev/null 2>&1; then "
                "    dec=$(echo \"ibase=16; $(echo \"$serial\" | tr 'a-f' 'A-F')\" | bc 2>/dev/null); "
                "    if [ -n \"$dec\" ] && echo \"$resp\" | grep -q \"\\\"$dec\\\"\"; then echo 'REVOKED'; exit 0; fi; "
                "  fi; "
                "  echo 'VALID'; "
                "fi";
            FILE *fp = popen(cmd, "r");
            if (fp) {
                fgets(status, sizeof(status), fp);
                pclose(fp);
                size_t len = strlen(status);
                while (len > 0 && (status[len - 1] == '\n' || status[len - 1] == '\r')) {
                    status[len - 1] = '\0';
                    len--;
                }
                snprintf(response, sizeof(response), "%s\n", status);
            } else {
                strcpy(response, "ERROR: failed to run revocation check\n");
            }
        } else if (argc >= 3 && strcmp(args[1], "import_keybox") == 0) {
            mkdir(ts_dir, 0755);
            char cmd[2048];
            snprintf(cmd, sizeof(cmd), "echo \"%s\" | base64 -d > \"%s\" && chown system:system \"%s\" && chmod 600 \"%s\"",
                     args[2], kb_xml, kb_xml, kb_xml);
            if (system(cmd) == 0) {
                strcpy(response, "OK: keybox.xml imported\n");
            } else {
                strcpy(response, "ERROR: failed to import keybox\n");
            }
        } else if (argc >= 2 && strcmp(args[1], "fetch_catalog") == 0) {
            FILE *fp = popen("curl -sSL \"https://rawbin.dpejoh.com/catalog\" 2>/dev/null", "r");
            if (fp) {
                size_t r = fread(response, 1, sizeof(response) - 1, fp);
                response[r] = '\0';
                pclose(fp);
            } else {
                strcpy(response, "ERROR: failed to fetch catalog\n");
            }
        }
    } else if (strcmp(args[0], "hma") == 0) {
        char hma_dir[256] = "";
        int installed = (find_hma_dir(hma_dir, sizeof(hma_dir)) == 0);

        if (argc >= 2 && strcmp(args[1], "status") == 0) {
            char last_apply[64] = "";
            FILE *fp = fopen("/data/adb/rod/hma_last_apply", "r");
            if (fp) {
                fgets(last_apply, sizeof(last_apply), fp);
                fclose(fp);
                size_t len = strlen(last_apply);
                while (len > 0 && (last_apply[len - 1] == '\n' || last_apply[len - 1] == '\r')) {
                    last_apply[len - 1] = '\0';
                    len--;
                }
            }

            snprintf(response, sizeof(response),
                     "{\n"
                     "  \"module_installed\": %s,\n"
                     "  \"service_dir\": \"%s\",\n"
                     "  \"last_apply\": \"%s\"\n"
                     "}\n",
                     installed ? "true" : "false",
                     hma_dir, last_apply);
        } else if (argc >= 2 && strcmp(args[1], "apply_presets") == 0) {
            if (!installed) {
                strcpy(response, "ERROR: HMA-OSS is not installed\n");
            } else {
                char cfg_file[512];
                snprintf(cfg_file, sizeof(cfg_file), "%s/config.json", hma_dir);
                
                int config_version = 3;
                FILE *fp = fopen(cfg_file, "r");
                if (fp) {
                    char buf[4096];
                    size_t r = fread(buf, 1, sizeof(buf)-1, fp);
                    buf[r] = '\0';
                    fclose(fp);
                    char *ver_str = strstr(buf, "\"configVersion\":");
                    if (ver_str) {
                        sscanf(ver_str + 16, "%d", &config_version);
                    }
                }

                // Read exclusions
                char excl_path[256];
                snprintf(excl_path, sizeof(excl_path), "%s/hma_exclude.txt", PERSISTENT_DIR);
                char exclusions[200][128];
                int excl_count = 0;
                
                // Add standard exclusions
                strcpy(exclusions[excl_count++], "org.frknkrc44.hma_oss");
                strcpy(exclusions[excl_count++], "io.github.a13e300.ksuwebui");
                strcpy(exclusions[excl_count++], "me.weishu.kernelsu");
                strcpy(exclusions[excl_count++], "com.topjohnwu.magisk");
                strcpy(exclusions[excl_count++], "com.google.android.gms");
                strcpy(exclusions[excl_count++], "com.google.android.gsf");
                strcpy(exclusions[excl_count++], "com.android.vending");
                strcpy(exclusions[excl_count++], "android");

                fp = fopen(excl_path, "r");
                if (fp) {
                    char line[128];
                    while (fgets(line, sizeof(line), fp) && excl_count < 200) {
                        size_t len = strlen(line);
                        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
                            line[len - 1] = '\0';
                            len--;
                        }
                        if (len > 0) {
                            strncpy(exclusions[excl_count++], line, 128);
                        }
                    }
                    fclose(fp);
                }

                // Get packages
                FILE *pkg_fp = popen("pm list packages -3 2>/dev/null", "r");
                if (!pkg_fp) {
                    pkg_fp = popen("ls /data/app 2>/dev/null", "r");
                }
                
                char packages[500][128];
                int pkg_count = 0;
                if (pkg_fp) {
                    char line[256];
                    while (fgets(line, sizeof(line), pkg_fp) && pkg_count < 500) {
                        size_t len = strlen(line);
                        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
                            line[len - 1] = '\0';
                            len--;
                        }
                        char *pkg_name = line;
                        if (strncmp(line, "package:", 8) == 0) {
                            pkg_name = line + 8;
                        }
                        // Strip suffix if ls /data/app
                        char *hyphen = strchr(pkg_name, '-');
                        if (hyphen) *hyphen = '\0';
                        
                        if (strlen(pkg_name) > 0) {
                            // Check if excluded
                            int excluded = 0;
                            for (int i = 0; i < excl_count; i++) {
                                if (strcmp(pkg_name, exclusions[i]) == 0) {
                                    excluded = 1;
                                    break;
                                }
                            }
                            if (!excluded) {
                                strncpy(packages[pkg_count++], pkg_name, 128);
                            }
                        }
                    }
                    pclose(pkg_fp);
                }

                // Open config.json
                fp = fopen(cfg_file, "w");
                if (fp) {
                    fprintf(fp, "{\n"
                                "  \"configVersion\": %d,\n"
                                "  \"detailLog\": false,\n"
                                "  \"errorOnlyLog\": false,\n"
                                "  \"maxLogSize\": 512,\n"
                                "  \"forceMountData\": true,\n"
                                "  \"disableActivityLaunchProtection\": false,\n"
                                "  \"altAppDataIsolation\": false,\n"
                                "  \"altVoldAppDataIsolation\": false,\n"
                                "  \"skipSystemAppDataIsolation\": true,\n"
                                "  \"packageQueryWorkaround\": false,\n"
                                "  \"webViewProtection\": true,\n"
                                "  \"defaultConfig\": null,\n"
                                "  \"templates\": {},\n"
                                "  \"settingsTemplates\": {},\n"
                                "  \"disabledHooks\": [],\n"
                                "  \"scope\": {\n", config_version);
                    for (int i = 0; i < pkg_count; i++) {
                        fprintf(fp, "    \"%s\": {\n"
                                    "      \"useWhitelist\": false,\n"
                                    "      \"excludeSystemApps\": true,\n"
                                    "      \"hideInstallationSource\": true,\n"
                                    "      \"hideSystemInstallationSource\": true,\n"
                                    "      \"excludeTargetInstallationSource\": false,\n"
                                    "      \"invertActivityLaunchProtection\": false,\n"
                                    "      \"excludeVoldIsolation\": false,\n"
                                    "      \"restrictedZygotePermissions\": [],\n"
                                    "      \"applyTemplates\": [],\n"
                                    "      \"applyPresets\": [\"accessibility_apps\",\"custom_rom\",\"detector_apps\",\"root_apps\",\"shizuku_dhizuku\",\"sus_apps\",\"xposed\"],\n"
                                    "      \"applySettingTemplates\": [],\n"
                                    "      \"applySettingsPresets\": [\"accessibility\",\"dev_options\",\"input_method\"],\n"
                                    "      \"extraAppList\": [],\n"
                                    "      \"extraOppositeAppList\": []\n"
                                    "    }%s\n", packages[i], (i == pkg_count - 1) ? "" : ",");
                    }
                    fprintf(fp, "  }\n}\n");
                    fclose(fp);
                    chmod(cfg_file, 0600);
                    
                    system("am force-stop org.frknkrc44.hma_oss >/dev/null 2>&1");
                    
                    time_t now = time(NULL);
                    mkdir(PERSISTENT_DIR, 0755);
                    char apply_time_file[256];
                    snprintf(apply_time_file, sizeof(apply_time_file), "%s/hma_last_apply", PERSISTENT_DIR);
                    FILE *t_fp = fopen(apply_time_file, "w");
                    if (t_fp) {
                        fprintf(t_fp, "%ld\n", (long)now);
                        fclose(t_fp);
                    }
                    strcpy(response, "OK\n");
                } else {
                    strcpy(response, "ERROR: failed to open config.json for writing\n");
                }
            }
        } else if (argc >= 3 && strcmp(args[1], "get_rule") == 0) {
            char rule_path[256];
            snprintf(rule_path, sizeof(rule_path), "%s/%s", PERSISTENT_DIR, args[2]);
            FILE *fp = fopen(rule_path, "r");
            if (fp) {
                size_t r = fread(response, 1, sizeof(response) - 1, fp);
                response[r] = '\0';
                fclose(fp);
            } else {
                strcpy(response, "");
            }
        } else if (argc >= 3 && strcmp(args[1], "set_rule") == 0) {
            char rule_path[256];
            snprintf(rule_path, sizeof(rule_path), "%s/%s", PERSISTENT_DIR, args[2]);
            FILE *fp = fopen(rule_path, "w");
            if (fp) {
                if (argc >= 4) {
                    fprintf(fp, "%s", args[3]);
                }
                fclose(fp);
                strcpy(response, "OK\n");
            } else {
                strcpy(response, "ERROR: failed to write rule\n");
            }
        } else if (argc >= 2 && strcmp(args[1], "get_packages") == 0) {
            FILE *fp = popen("pm list packages -3 | sed 's/^package://' | tr -d '\\r'", "r");
            if (fp) {
                size_t r = fread(response, 1, sizeof(response) - 1, fp);
                response[r] = '\0';
                pclose(fp);
            }
        } else if (argc >= 2 && strcmp(args[1], "get_all_labels") == 0) {
            const char *temp_dex = "/data/local/tmp/AppLabelResolver.dex";
            FILE *out_dex = fopen(temp_dex, "wb");
            if (out_dex) {
                fwrite(app_label_resolver_dex, 1, sizeof(app_label_resolver_dex), out_dex);
                fclose(out_dex);
                chmod(temp_dex, 0644);
            }
            FILE *fp = popen("CLASSPATH=/data/local/tmp/AppLabelResolver.dex app_process /system/bin AppLabelResolver 2>/dev/null", "r");
            if (fp) {
                size_t r = fread(response, 1, sizeof(response) - 1, fp);
                response[r] = '\0';
                pclose(fp);
            }
            unlink(temp_dex);
        }
    } else if (strcmp(args[0], "zn") == 0) {
        struct stat st;
        int installed = (stat("/data/adb/modules/zygisksu/module.prop", &st) == 0);
        
        if (argc >= 2 && strcmp(args[1], "status") == 0) {
            char ver[128] = "";
            if (installed) {
                FILE *fp = fopen("/data/adb/modules/zygisksu/module.prop", "r");
                if (fp) {
                    char line[256];
                    while (fgets(line, sizeof(line), fp)) {
                        if (strncmp(line, "version=", 8) == 0) {
                            strncpy(ver, line + 8, sizeof(ver));
                            size_t len = strlen(ver);
                            while (len > 0 && (ver[len-1] == '\n' || ver[len-1] == '\r')) { ver[len-1] = '\0'; len--; }
                            break;
                        }
                    }
                    fclose(fp);
                }
            }

            char state[256] = "Not configured yet";
            FILE *fp = fopen("/data/adb/rod/zn_state", "r");
            if (fp) {
                fgets(state, sizeof(state), fp);
                fclose(fp);
                size_t len = strlen(state);
                while (len > 0 && (state[len-1] == '\n' || state[len-1] == '\r')) { state[len-1] = '\0'; len--; }
            }

            char last_apply[64] = "";
            fp = fopen("/data/adb/rod/zn_last_apply", "r");
            if (fp) {
                fgets(last_apply, sizeof(last_apply), fp);
                fclose(fp);
                size_t len = strlen(last_apply);
                while (len > 0 && (last_apply[len-1] == '\n' || last_apply[len-1] == '\r')) { last_apply[len-1] = '\0'; len--; }
            }

            snprintf(response, sizeof(response),
                     "{\n"
                     "  \"installed\": %s,\n"
                     "  \"version\": \"%s\",\n"
                     "  \"state\": \"%s\",\n"
                     "  \"last_apply\": \"%s\"\n"
                     "}\n",
                     installed ? "true" : "false",
                     ver, state, last_apply);
        } else if (argc >= 2 && strcmp(args[1], "apply") == 0) {
            char zygiskd[256] = "";
            if (stat("/data/adb/modules/zygisksu/bin/zygiskd", &st) == 0) {
                strcpy(zygiskd, "/data/adb/modules/zygisksu/bin/zygiskd");
            } else if (stat("/data/adb/modules/zygisksu/bin/zygiskd64", &st) == 0) {
                strcpy(zygiskd, "/data/adb/modules/zygisksu/bin/zygiskd64");
            }

            if (strlen(zygiskd) > 0) {
                char cmd[1024];
                snprintf(cmd, sizeof(cmd), "\"%s\" enforce-denylist just_umount", zygiskd);
                system(cmd);
                snprintf(cmd, sizeof(cmd), "\"%s\" memory-type anonymous", zygiskd);
                system(cmd);
                snprintf(cmd, sizeof(cmd), "\"%s\" linker builtin", zygiskd);
                system(cmd);
                
                mkdir(PERSISTENT_DIR, 0755);
                FILE *fp = fopen("/data/adb/rod/zn_state", "w");
                if (fp) {
                    fprintf(fp, "Denylist: umount | Memory: anon | Linker: builtin\n");
                    fclose(fp);
                }
                fp = fopen("/data/adb/rod/zn_last_apply", "w");
                if (fp) {
                    fprintf(fp, "%ld\n", (long)time(NULL));
                    fclose(fp);
                }
                strcpy(response, "OK\n");
            } else {
                strcpy(response, "ERROR: zygiskd binary not found\n");
            }
        }
    } else if (strcmp(args[0], "pif") == 0) {
        struct stat st;
        int installed = (stat("/data/adb/modules/playintegrityfix", &st) == 0);

        if (argc >= 2 && strcmp(args[1], "status") == 0) {
            char ver[128] = "";
            if (installed) {
                FILE *fp = fopen("/data/adb/modules/playintegrityfix/module.prop", "r");
                if (fp) {
                    char line[256];
                    while (fgets(line, sizeof(line), fp)) {
                        if (strncmp(line, "version=", 8) == 0) {
                            strncpy(ver, line + 8, sizeof(ver));
                            size_t len = strlen(ver);
                            while (len > 0 && (ver[len-1] == '\n' || ver[len-1] == '\r')) { ver[len-1] = '\0'; len--; }
                            break;
                        }
                    }
                    fclose(fp);
                }
            }

            char last_fetch[64] = "";
            FILE *fp = fopen("/data/adb/rod/pif_last_fetch", "r");
            if (fp) {
                fgets(last_fetch, sizeof(last_fetch), fp);
                fclose(fp);
                size_t len = strlen(last_fetch);
                while (len > 0 && (last_fetch[len-1] == '\n' || last_fetch[len-1] == '\r')) { last_fetch[len-1] = '\0'; len--; }
            }

            snprintf(response, sizeof(response),
                     "{\n"
                     "  \"installed\": %s,\n"
                     "  \"version\": \"%s\",\n"
                     "  \"last_fetch\": \"%s\"\n"
                     "}\n",
                     installed ? "true" : "false", ver, last_fetch);
        } else if (argc >= 2 && strcmp(args[1], "fetch") == 0) {
            if (stat("/data/adb/modules/playintegrityfix/autopif.sh", &st) == 0) {
                int exit_code = system("sh /data/adb/modules/playintegrityfix/autopif.sh >/dev/null 2>&1");
                if (exit_code == 0) {
                    mkdir(PERSISTENT_DIR, 0755);
                    FILE *fp = fopen("/data/adb/rod/pif_last_fetch", "w");
                    if (fp) {
                        fprintf(fp, "%ld\n", (long)time(NULL));
                        fclose(fp);
                    }
                    strcpy(response, "OK\n");
                } else {
                    strcpy(response, "ERROR: autopif.sh returned non-zero\n");
                }
            } else {
                strcpy(response, "ERROR: autopif.sh not found\n");
            }
        }
    } else {
        strcpy(response, "ERROR: unknown submodule\n");
    }

    printf("%s", response);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage:\n  rodd <submodule> <cmd> [args...]\n");
        return 1;
    }

    char *args[50];
    int cmd_argc = 0;
    for (int i = 1; i < argc && cmd_argc < 50; i++) {
        args[cmd_argc++] = argv[i];
    }
    handle_command(cmd_argc, args);
    return 0;
}
