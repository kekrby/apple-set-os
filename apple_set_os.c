// Copyright (c) 2015 Bruno Bierbaumer

#include <efibind.h>
#include <efidef.h>
#include <efidevp.h>
#include <eficon.h>
#include <efiprot.h>
#include <efiapi.h>
#include <efierr.h>

#define APPLE_SET_OS_VENDOR  "Apple Inc."
#define APPLE_SET_OS_VERSION "Mac OS X 10.9"

static EFI_GUID APPLE_SET_OS_GUID = { 0xc5c5da95, 0x7d5c, 0x45e6, { 0xb2, 0xf1, 0x3f, 0xd5, 0x2b, 0xb1, 0x00, 0x77 } };

typedef struct _APPLE_SET_OS_INTERFACE {
    UINT64 Version;
    EFI_STATUS (EFIAPI *SetOsVersion) (IN CHAR8 *Version);
    EFI_STATUS (EFIAPI *SetOsVendor) (IN CHAR8 *Vendor);
} APPLE_SET_OS_INTERFACE;

EFI_STATUS
efi_main(EFI_HANDLE Image, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;
    APPLE_SET_OS_INTERFACE *SetOs = NULL;
    SIMPLE_TEXT_OUTPUT_INTERFACE *ConOut = SystemTable->ConOut;

    ConOut->OutputString(ConOut, L"apple_set_os started\r\n");

    Status = SystemTable->BootServices->LocateProtocol(&APPLE_SET_OS_GUID, NULL, (VOID **) &SetOs);
    if (EFI_ERROR(Status) || SetOs == NULL) {
        ConOut->OutputString(ConOut, L"Could not locate the apple_set_os protocol.\r\n");
        return Status;
    }

    if (SetOs->Version != 0) {
        Status = SetOs->SetOsVersion((CHAR8 *) APPLE_SET_OS_VERSION);
        if (EFI_ERROR(Status)) {
            ConOut->OutputString(ConOut, L"Could not set version.\r\n");
            return Status;
        }

        ConOut->OutputString(ConOut, L"Set OS version to " APPLE_SET_OS_VERSION  ".\r\n");
    }

    Status = SetOs->SetOsVendor((CHAR8 *) APPLE_SET_OS_VENDOR);
    if (EFI_ERROR(Status)) {
        ConOut->OutputString(ConOut, L"Could not set vendor.\r\n");
        return Status;
    }

    ConOut->OutputString(ConOut, L"Set OS vendor to " APPLE_SET_OS_VENDOR  ".\r\n");

    return EFI_SUCCESS;
}
