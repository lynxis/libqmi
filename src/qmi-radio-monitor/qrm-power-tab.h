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

#ifndef QRM_POWER_TAB_H
#define QRM_POWER_TAB_H

#include <gtk/gtk.h>

#include "qrm-device.h"

G_BEGIN_DECLS

#define QRM_TYPE_POWER_TAB         (qrm_power_tab_get_type ())
#define QRM_POWER_TAB(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), QRM_TYPE_POWER_TAB, QrmPowerTab))
#define QRM_POWER_TAB_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), QRM_TYPE_POWER_TAB, QrmPowerTabClass))
#define QRM_IS_POWER_TAB(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), QRM_TYPE_POWER_TAB))
#define QRM_IS_POWER_TAB_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), QRM_TYPE_POWER_TAB))
#define QRM_POWER_TAB_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), QRM_TYPE_POWER_TAB, QrmPowerTabClass))

typedef struct _QrmPowerTab        QrmPowerTab;
typedef struct _QrmPowerTabClass   QrmPowerTabClass;
typedef struct _QrmPowerTabPrivate QrmPowerTabPrivate;

struct _QrmPowerTab {
    GtkBox parent_instance;
    QrmPowerTabPrivate *priv;
};

struct _QrmPowerTabClass {
    GtkBoxClass parent_class;
};

GType qrm_power_tab_get_type (void) G_GNUC_CONST;

void qrm_power_tab_change_current_device (QrmPowerTab *self,
                                          QrmDevice *new_device);

G_END_DECLS

#endif /* QRM_POWER_TAB_H */
