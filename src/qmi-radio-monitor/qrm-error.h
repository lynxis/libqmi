/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Copyright (C) 2013-2017 Aleksander Morgado <aleksander@aleksander.es>
 */

#ifndef QRM_ERROR_H
#define QRM_ERROR_H

/* Prefixes for errors registered in DBus */
#define QRM_DBUS_ERROR_PREFIX          "org.freedesktop.qrm.Error"
#define QRM_CORE_ERROR_DBUS_PREFIX     QRM_DBUS_ERROR_PREFIX ".Core"

typedef enum { /*< underscore_name=qrm_core_error >*/
    QRM_CORE_ERROR_FAILED           = 0, /*< nick=Failed >*/
    QRM_CORE_ERROR_INVALID_ARGS     = 1, /*< nick=InvalidArgs >*/
    QRM_CORE_ERROR_UNSUPPORTED      = 2  /*< nick=Unsupported >*/
} QrmCoreError;

#endif /* QRM_ERROR_H */
