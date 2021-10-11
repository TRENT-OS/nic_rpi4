/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

extern "C" {
#include <platsupport/mach/mailbox_util.h>

int mbox_init(ps_io_ops_t *io_ops);
}
