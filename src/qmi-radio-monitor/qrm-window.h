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

#ifndef QRM_WINDOW_H
#define QRM_WINDOW_H

#include <gtk/gtk.h>

#include "qrm-app.h"

G_BEGIN_DECLS

#define QRM_TYPE_WINDOW         (qrm_window_get_type ())
#define QRM_WINDOW(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), QRM_TYPE_WINDOW, QrmWindow))
#define QRM_WINDOW_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), QRM_TYPE_WINDOW, QrmWindowClass))
#define QRM_IS_WINDOW(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), QRM_TYPE_WINDOW))
#define QRM_IS_WINDOW_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), QRM_TYPE_WINDOW))
#define QRM_WINDOW_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), QRM_TYPE_WINDOW, QrmWindowClass))

typedef struct _QrmWindow        QrmWindow;
typedef struct _QrmWindowClass   QrmWindowClass;
typedef struct _QrmWindowPrivate QrmWindowPrivate;

struct _QrmWindow {
    GtkApplicationWindow parent_instance;
    QrmWindowPrivate *priv;
};

struct _QrmWindowClass {
    GtkApplicationWindowClass parent_class;
};

GType qrm_window_get_type(void) G_GNUC_CONST;

GtkWidget *qrm_window_new (QrmApp *application);

G_END_DECLS

#endif /* QRM_WINDOW_H */
