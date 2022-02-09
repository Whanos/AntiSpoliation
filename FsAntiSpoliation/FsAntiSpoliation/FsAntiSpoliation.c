/*++

FsAntiSpoliation

Requested by Lyrus

KMode

*/

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>

// Globals

PFLT_FILTER FilterHandle = NULL;
PFLT_PORT Port = NULL;
PFLT_PORT ClientPortGlobal = NULL;

// Generics
NTSTATUS FsFilterUnload(FLT_FILTER_UNLOAD_FLAGS Flags);

// Callbacks
// IRP_MJ_CREATE, IRP_MJ_SET_INFORMATION, IRP_MJ_CLEANUP
FLT_PREOP_CALLBACK_STATUS FsPreCreateCallback(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext);
FLT_PREOP_CALLBACK_STATUS FsfPreSetInfoCallback(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext);
FLT_PREOP_CALLBACK_STATUS FsPreCleanupCallback(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext);
FLT_POSTOP_CALLBACK_STATUS FsPostCreateCallback(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext, FLT_POST_OPERATION_FLAGS Flags);
FLT_POSTOP_CALLBACK_STATUS FsPostSetInfoCallback(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext, FLT_POST_OPERATION_FLAGS Flags);
FLT_POSTOP_CALLBACK_STATUS FsPostCleanupCallback(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext, FLT_POST_OPERATION_FLAGS Flags);

// Operations

CONST FLT_OPERATION_REGISTRATION Callbacks[] = {

    { IRP_MJ_CREATE,
      0,
      FsPreCreateCallback,
      FsPostCreateCallback },

    { IRP_MJ_SET_INFORMATION,
      FLTFL_OPERATION_REGISTRATION_SKIP_PAGING_IO,
      FsfPreSetInfoCallback,
      FsPostSetInfoCallback },

    { IRP_MJ_CLEANUP,
      0,
      FsPreCleanupCallback,
      FsPostCleanupCallback },

    { IRP_MJ_OPERATION_END }

};

// Filter Registration

const FLT_REGISTRATION FilterRegistration = {
    sizeof(FLT_REGISTRATION),   // Size
    FLT_REGISTRATION_VERSION,   // Version
    0,                          // Flags
    NULL,                       // *ContextRegistration
    Callbacks,                  // *OperationRegistration
    FsFilterUnload,             // FilterUnloadCallback (Unload Function)
    NULL,                       // InstanceSetupCallback
    NULL,                       // InstanceQueryTeardownCallback
    NULL,                       // InstanceTeardownStartCallback
    NULL,                       // InstanceTeardownCompleteCallback
    NULL,                       // GenerateFileNameCallback
    NULL,                       // NormalizeNameComponentCallback
    NULL,                       // NormalizeContextCleanupCallback
    NULL,                       // TransactionNotificationCallback
};

// PreOp Callbacks

FLT_PREOP_CALLBACK_STATUS FsPreCreateCallback(
    PFLT_CALLBACK_DATA Data,
    PCFLT_RELATED_OBJECTS FltObjects,
    PVOID* CompletionContext
) {
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);

    return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS FsfPreSetInfoCallback(
    PFLT_CALLBACK_DATA Data,
    PCFLT_RELATED_OBJECTS FltObjects,
    PVOID* CompletionContext
) {
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);

    return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS FsPreCleanupCallback(
    PFLT_CALLBACK_DATA Data,
    PCFLT_RELATED_OBJECTS FltObjects,
    PVOID* CompletionContext
) {
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);

    return FLT_POSTOP_FINISHED_PROCESSING;
}

// PostOp Callbacks

FLT_POSTOP_CALLBACK_STATUS FsPostCreateCallback(
    PFLT_CALLBACK_DATA Data,
    PCFLT_RELATED_OBJECTS FltObjects,
    PVOID* CompletionContext,
    FLT_POST_OPERATION_FLAGS Flags
) {

    UNREFERENCED_PARAMETER(Flags);
    UNREFERENCED_PARAMETER(CompletionContext);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Data);

    return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_POSTOP_CALLBACK_STATUS FsPostSetInfoCallback(
    PFLT_CALLBACK_DATA Data,
    PCFLT_RELATED_OBJECTS FltObjects,
    PVOID* CompletionContext,
    FLT_POST_OPERATION_FLAGS Flags
) {

    UNREFERENCED_PARAMETER(Flags);
    UNREFERENCED_PARAMETER(CompletionContext);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Data);

    return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_POSTOP_CALLBACK_STATUS FsPostCleanupCallback(
    PFLT_CALLBACK_DATA Data,
    PCFLT_RELATED_OBJECTS FltObjects,
    PVOID* CompletionContext,
    FLT_POST_OPERATION_FLAGS Flags
) {

    UNREFERENCED_PARAMETER(Flags);
    UNREFERENCED_PARAMETER(CompletionContext);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Data);

    return FLT_POSTOP_FINISHED_PROCESSING;
}

// Unload routine for the minifilter.
NTSTATUS FsFilterUnload(FLT_FILTER_UNLOAD_FLAGS Flags) {

    UNREFERENCED_PARAMETER(Flags);

    KdPrint(("Unloading FsAntiScam filter driver! \r\n"));
    FltCloseCommunicationPort(Port);
    FltUnregisterFilter(FilterHandle);

    return STATUS_SUCCESS;
}


NTSTATUS DriverEntry(
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath
) {
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS status;

    status = FltRegisterFilter(
        DriverObject,
        &FilterRegistration,
        &FilterHandle
    );

    if (NT_SUCCESS(status)) {
        status = FltStartFiltering(FilterHandle);
        if (!NT_SUCCESS(status)) {
            KdPrint(("Error starting filtering!"));
            FltUnregisterFilter(FilterHandle);
            FltCloseCommunicationPort(Port);
        }
    }

    return status;
}