//
//   Copyright (c) 2012  Finnbarr P. Murphy.   All rights reserved.
//
//   Demo the use of efivarfs to read, write and delete EFI NVRAM variables.
//
//   Parts of the code taken from mokutil and EDK1.
//
//   Writes 12345 to an EFI variable called FPMURPHY with a GUID of EFI_FPMURPHY_GUID
//   Alternatively read or delete the variable.
//


#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BITS_PER_LONG (sizeof(unsigned long) * 8)
#define EFI_ERROR(x) ((x) | (1L << (BITS_PER_LONG - 1)))

#define EFI_SUCCESS     0
#define EFI_INVALID_PARAMETER   EFI_ERROR(2)
#define EFI_UNSUPPORTED     EFI_ERROR(3)
#define EFI_BAD_BUFFER_SIZE     EFI_ERROR(4)
#define EFI_BUFFER_TOO_SMALL    EFI_ERROR(5)
#define EFI_NOT_FOUND           EFI_ERROR(14)
#define EFI_OUT_OF_RESOURCES    EFI_ERROR(15)

#define EFI_VARIABLE_NON_VOLATILE       0x0000000000000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x0000000000000002
#define EFI_VARIABLE_RUNTIME_ACCESS     0x0000000000000004

typedef struct {
    uint32_t  data1;
    uint16_t  data2;
    uint16_t  data3;
    uint8_t   data4[8];
} efi_guid_t;

typedef unsigned long efi_status_t;

typedef struct _efi_variable_t {
    const char    *VariableName;
    efi_guid_t    VendorGuid;
    uint32_t      DataSize;
    uint8_t       *Data;
    uint32_t      Attributes;
} __attribute__((packed)) efi_variable_t;


#define SYSFS_EFI_VARS "/sys/firmware/efi/efivars"

#define EFI_FPMURPHY_GUID \
    (efi_guid_t) {0x605dab50, 0xe046, 0x4300, {0xab, 0xb6, 0x3d, 0xe8, 0x15, 0xcd, 0x8e, 0x26}}

#define EFI_APPLE_BOOT_GUID \
    (efi_guid_t) {0x7C436110, 0xAB2A, 0x4BBB, {0xA8, 0x80, 0xFE, 0x41, 0x99, 0x5C, 0x9F, 0x82}}


int
variable_to_name(efi_variable_t *var, char *name)
{
    char *p = name;
    efi_guid_t *guid = &(var->VendorGuid);

    if (!var->VariableName)
        return -1;

    strcpy (p, var->VariableName);
    p += strlen (p);
    p += sprintf (p, "-");

    sprintf(p, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        guid->data1, guid->data2, guid->data3,
        guid->data4[0], guid->data4[1], guid->data4[2], guid->data4[3],
        guid->data4[4], guid->data4[5], guid->data4[6], guid->data4[7]
    );

    return strlen (name);
}


efi_status_t
read_variable (efi_variable_t *var)
{
    char name[PATH_MAX];
    char filename[PATH_MAX];
    int fd;
    struct stat buf;
    size_t readsize, datasize;
    void *buffer;

    if (!var)
        return EFI_INVALID_PARAMETER;

    variable_to_name (var, name);
    snprintf (filename, PATH_MAX-1, "%s/%s", SYSFS_EFI_VARS, name);
    fd = open (filename, O_RDONLY);
    if (fd == -1) {
        return EFI_NOT_FOUND;
    }

    if (fstat (fd, &buf) != 0) {
        close (fd);
        return EFI_INVALID_PARAMETER;
    }

    readsize = read (fd, &var->Attributes, sizeof(uint32_t));
    if (readsize != sizeof(uint32_t)) {
        close (fd);
        return EFI_INVALID_PARAMETER;
    }

    datasize = buf.st_size - sizeof(uint32_t);

    buffer = malloc (datasize);
    if (buffer == NULL) {
        close (fd);
        return EFI_OUT_OF_RESOURCES;
    }

    readsize = read (fd, buffer, datasize);
    if (readsize != datasize) {
        close (fd);
        free (buffer);
        return EFI_INVALID_PARAMETER;
    }
    var->Data = buffer;
    var->DataSize = datasize;

    close (fd);
    return EFI_SUCCESS;
}


efi_status_t
write_variable (efi_variable_t *var)
{
    int fd;
    size_t writesize;
    void *buffer;
    unsigned long total;
    char name[PATH_MAX];
    char filename[PATH_MAX];

    if (!var)
        return EFI_INVALID_PARAMETER;

    variable_to_name(var, name);
    snprintf(filename, PATH_MAX-1, "%s/%s", SYSFS_EFI_VARS, name);
    if (!filename )
        return EFI_INVALID_PARAMETER;

    buffer = malloc(var->DataSize + sizeof(uint32_t));
    if (buffer == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    memcpy (buffer, &var->Attributes, sizeof(uint32_t));
    memcpy (buffer + sizeof(uint32_t), var->Data, var->DataSize);
    total = var->DataSize + sizeof(uint32_t);

    fd = open (filename, O_WRONLY | O_CREAT,
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        free (buffer);
        return EFI_INVALID_PARAMETER;
    }

    writesize = write (fd, buffer, total);
    if (writesize != total) {
        free (buffer);
        close (fd);
        return EFI_INVALID_PARAMETER;
    }

    close (fd);
    free (buffer);
    return EFI_SUCCESS;
}


efi_status_t
delete_variable(efi_variable_t *var)
{
    char name[PATH_MAX];
    char filename[PATH_MAX];

    if (!var)
        return EFI_INVALID_PARAMETER;

    variable_to_name(var, name);
    snprintf(filename, PATH_MAX-1, "%s/%s", SYSFS_EFI_VARS, name);

    if (unlink (filename) == 0)
        return EFI_SUCCESS;

    return EFI_OUT_OF_RESOURCES;
}


int
usage(char *name)
{
    fprintf(stderr, "usage: %s [ -w | --write | -r | --read | -d | --delete]\n", name);
    exit(1);
}


/*
int
main(int argc, char *argv[])
{
    efi_variable_t var;
    uint8_t auth[5];
    int status = EFI_SUCCESS;

    auth[0] = 0;
    auth[1] = 1;
    auth[2] = 2;
    auth[3] = 3;
    auth[4] = 4;

    if (argc == 2) {
        memset (&var, 0, sizeof(var));
        var.VariableName = "boot-args";
        var.VendorGuid = EFI_FPMURPHY_GUID;
        if (strcmp (argv[1], "-h") == 0 ||
            strcmp (argv[1], "--help") == 0) {
            usage(argv[0]);
        } else if (strcmp (argv[1], "-w") == 0 ||
                   strcmp (argv[1], "--write") == 0) {
            var.Data = auth;
            var.DataSize = 5;
            var.Attributes = EFI_VARIABLE_NON_VOLATILE |
                             EFI_VARIABLE_BOOTSERVICE_ACCESS |
                             EFI_VARIABLE_RUNTIME_ACCESS;
            if (write_variable (&var) != EFI_SUCCESS) {
                fprintf (stderr, "Failed to write variable: %s\n", var.VariableName);
                status = 1;
            }
        } else if (strcmp (argv[1], "-r") == 0 ||
                   strcmp (argv[1], "--read") == 0) {
            if (read_variable (&var) != EFI_SUCCESS) {
                fprintf (stderr, "Failed to read variable: %s\n", var.VariableName);
                status = 2;
            }
        } else if (strcmp (argv[1], "-d") == 0 ||
                   strcmp (argv[1], "--delete") == 0) {
            if (delete_variable (&var) != EFI_SUCCESS) {
                fprintf (stderr, "Failed to delete variable: %s\n", var.VariableName);
                status = 3;
            }
        } else
           usage(argv[0]);
    } else
       usage(argv[0]);

    exit(status);
}
*/

void writevar(unsigned char* bootxml) {
  efi_variable_t var;
  uint8_t auth[5];
  int status = EFI_SUCCESS;
  var.VariableName = "efi-boot-device";
  var.VendorGuid = EFI_APPLE_BOOT_GUID;
  var.Data = bootxml;
  var.DataSize = strlen((char*)bootxml)+1;
  var.Attributes = EFI_VARIABLE_NON_VOLATILE |
      EFI_VARIABLE_BOOTSERVICE_ACCESS |
      EFI_VARIABLE_RUNTIME_ACCESS;
  if (write_variable (&var) != EFI_SUCCESS) {
    fprintf (stderr, "Failed to write variable: %s\n", var.VariableName);
    status = 1;
  }
}
