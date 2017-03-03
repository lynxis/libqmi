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

#ifndef QRM_APP_H
#define QRM_APP_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define QRM_TYPE_APP         (qrm_app_get_type ())
#define QRM_APP(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), QRM_TYPE_APP, QrmApp))
#define QRM_APP_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), QRM_TYPE_APP, QrmAppClass))
#define QRM_IS_APP(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), QRM_TYPE_APP))
#define QRM_IS_APP_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), QRM_TYPE_APP))
#define QRM_APP_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), QRM_TYPE_APP, QrmAppClass))

typedef struct _QrmApp        QrmApp;
typedef struct _QrmAppClass   QrmAppClass;
typedef struct _QrmAppPrivate QrmAppPrivate;

struct _QrmApp {
    GtkApplication parent_instance;
    QrmAppPrivate *priv;
};

struct _QrmAppClass {
    GtkApplicationClass parent_class;
};

GType qrm_app_get_type (void) G_GNUC_CONST;

QrmApp   *qrm_app_new                  (void);
void      qrm_app_quit                 (QrmApp *self);
gboolean  qrm_app_is_initial_scan_done (QrmApp *self);
GList    *qrm_app_peek_devices         (QrmApp *self);

G_END_DECLS

#endif /* QRM_APP_H */
