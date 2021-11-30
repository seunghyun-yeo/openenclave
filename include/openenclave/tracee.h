// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_TRACE_E_H
#define _OE_TRACE_E_H

#include <openenclave/bits/defs.h>
#include <openenclave/enclave.h>
#include <openenclave/log.h>

OE_EXTERNC_BEGIN

typedef void (*oe_enclave_log_callback_t)(
    void* context,
    oe_log_level_t level,
    uint64_t host_thread_id,
    const char* message);
oe_result_t oe_enclave_log_set_callback(
    void* context,
    oe_enclave_log_callback_t callback);

OE_EXTERNC_END

#endif
