/*
 * Copyright (C) 2017 CAMELab
 *
 * This file is part of SimpleSSD.
 *
 * SimpleSSD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimpleSSD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SimpleSSD.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "util/disk.hh"

#include <cstring>

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace SimpleSSD {

Disk::Disk() : diskSize(0), sectorSize(0) {}

Disk::~Disk() {
  close();
}

uint64_t Disk::open(std::string path, uint64_t desiredSize, uint32_t lbaSize) {
  filename = path;
  sectorSize = lbaSize;

  // Validate size
#ifdef _MSC_VER
  LARGE_INTEGER size;
  HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, NULL,
                             NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  if (GetLastError() == ERROR_ALREADY_EXISTS) {
    if (GetFileSizeEx(hFile, &size)) {
      diskSize = size.QuadPart;
    }
    else {
      // panic("Get file size failed!");
    }
  }
  else {
    size.QuadPart = desiredSize;

    if (SetFilePointerEx(hFile, size, nullptr, FILE_BEGIN)) {
      if (SetEndOfFile(hFile)) {
        diskSize = desiredSize;
      }
      else {
        // panic("SetEndOfFile failed");
      }
    }
    else {
      // panic("SetFilePointerEx failed");
    }
  }

  CloseHandle(hFile);
#else
  struct stat s;

  if (stat(filename.c_str(), &s) == 0) {
    // File exists
    if (S_ISREG(s.st_mode)) {
      diskSize = s.st_size;
    }
    else {
      // panic("nvme_disk: Specified file %s is not regular file.\n",
      //       filename.c_str());
    }
  }
  else {
    // Create zero-sized file
    disk.open(filename, std::ios::out | std::ios::binary);
    disk.close();

    // Set file size
    if (truncate(filename.c_str(), desiredSize) == 0) {
      diskSize = desiredSize;
    }
    else {
      // panic("nvme_disk: Failed to set disk size %" PRIu64 " errno=%d\n",
      //       diskSize, errno);
    }
  }
#endif

  // Open file
  disk.open(filename, std::ios::in | std::ios::out | std::ios::binary);

  if (!disk.is_open()) {
    // panic("failed to open file");
  }

  return diskSize;
}

void Disk::close() {
  if (disk.is_open()) {
    disk.close();
  }
}

uint16_t Disk::read(uint64_t slba, uint16_t nlblk, uint8_t *buffer) {
  uint16_t ret = 0;

  if (disk.is_open()) {
    uint64_t avail;

    slba *= sectorSize;
    avail = nlblk * sectorSize;

    if (slba + avail > diskSize) {
      if (slba >= diskSize) {
        avail = 0;
      }
      else {
        avail = diskSize - slba;
      }
    }

    if (avail > 0) {
      disk.seekg(slba, std::ios::beg);
      if (!disk.good()) {
        // panic("nvme_disk: Fail to seek to %" PRIu64 "\n", slba);
      }

      disk.read((char *)buffer, avail);
    }

    memset(buffer + avail, 0, nlblk * sectorSize - avail);

    // DPRINTF(NVMeDisk, "DISK    | READ  | BYTE %016" PRIX64 " + %X\n",
    //         slba, nlblk * sectorSize);

    ret = nlblk;
  }

  return ret;
}

uint16_t Disk::write(uint64_t slba, uint16_t nlblk, uint8_t *buffer) {
  uint16_t ret = 0;

  if (disk.is_open()) {
    slba *= sectorSize;

    disk.seekp(slba, std::ios::beg);
    if (!disk.good()) {
      // panic("nvme_disk: Fail to seek to %" PRIu64 "\n", slba);
    }

    uint64_t offset = disk.tellp();
    disk.write((char *)buffer, sectorSize * nlblk);
    offset = (uint64_t)disk.tellp() - offset;

    // DPRINTF(NVMeDisk, "DISK    | WRITE | BYTE %016" PRIX64 " + %X\n", slba,
    //         offset);

    ret = offset / sectorSize;
  }

  return ret;
}

uint16_t Disk::erase(uint64_t, uint16_t nlblk) {
  return nlblk;
}

CoWDisk::CoWDisk() {}

CoWDisk::~CoWDisk() {
  close();
}

void CoWDisk::close() {
  table.clear();

  Disk::close();
}

uint16_t CoWDisk::read(uint64_t slba, uint16_t nlblk, uint8_t *buffer) {
  uint16_t read = 0;

  for (uint64_t i = 0; i < nlblk; i++) {
    auto block = table.find(slba + i);

    if (block != table.end()) {
      memcpy(buffer + i * sectorSize, block->second.data(), sectorSize);
      read++;
    }
    else {
      read += Disk::read(slba + i, 1, buffer + i * sectorSize);
    }
  }

  return read;
}

uint16_t CoWDisk::write(uint64_t slba, uint16_t nlblk, uint8_t *buffer) {
  uint16_t write = 0;

  for (uint64_t i = 0; i < nlblk; i++) {
    auto block = table.find(slba + i);

    if (block != table.end()) {
      memcpy(block->second.data(), buffer + i * sectorSize, sectorSize);
    }
    else {
      std::vector<uint8_t> data;

      data.resize(sectorSize);
      memcpy(data.data(), buffer + i * sectorSize, sectorSize);

      table.insert({slba + i, data});
    }

    write++;
  }

  return write;
}

uint64_t MemDisk::open(std::string, uint64_t size, uint32_t lbaSize) {
  diskSize = size;
  sectorSize = lbaSize;

  return size;
}

void MemDisk::close() {
  table.clear();
}

MemDisk::MemDisk() {}

MemDisk::~MemDisk() {
  close();
}

uint16_t MemDisk::read(uint64_t slba, uint16_t nlblk, uint8_t *buffer) {
  uint16_t read = 0;

  for (uint64_t i = 0; i < nlblk; i++) {
    auto block = table.find(slba + i);

    if (block != table.end()) {
      memcpy(buffer + i * sectorSize, block->second.data(), sectorSize);
    }
    else {
      memset(buffer + i * sectorSize, 0, sectorSize);
    }

    read++;
  }

  return read;
}

uint16_t MemDisk::write(uint64_t slba, uint16_t nlblk, uint8_t *buffer) {
  uint16_t write = 0;

  for (uint64_t i = 0; i < nlblk; i++) {
    auto block = table.find(slba + i);

    if (block != table.end()) {
      memcpy(block->second.data(), buffer + i * sectorSize, sectorSize);
    }
    else {
      std::vector<uint8_t> data;

      data.resize(sectorSize);
      memcpy(data.data(), buffer + i * sectorSize, sectorSize);

      table.insert({slba + i, data});
    }

    write++;
  }

  return write;
}
uint16_t MemDisk::erase(uint64_t slba, uint16_t nlblk) {
  uint16_t erase = 0;

  for (uint64_t i = 0; i < nlblk; i++) {
    auto block = table.find(slba + i);

    if (block != table.end()) {
      table.erase(block);
    }

    erase++;
  }

  return erase;
}

}  // namespace SimpleSSD
