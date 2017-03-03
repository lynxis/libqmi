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
 * GNU General Public License for more details:
 *
 * Copyright (C) 2013-2017 Aleksander Morgado <aleksander@aleksander.es>
 *
 * Based on the Gnome system monitor colour pickers
 *  Copyright (C) 2007 Karl Lattimer <karl@qdh.org.uk>
 */

#ifndef QRM_COLOR_ICON_H
#define QRM_COLOR_ICON_H

#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define QRM_TYPE_COLOR_ICON            (qrm_color_icon_get_type ())
#define QRM_COLOR_ICON(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), QRM_TYPE_COLOR_ICON, QrmColorIcon))
#define QRM_COLOR_ICON_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), QRM_TYPE_COLOR_ICON, QrmColorIconClass))
#define QRM_IS_COLOR_ICON(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), QRM_TYPE_COLOR_ICON))
#define QRM_IS_COLOR_ICON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), QRM_TYPE_COLOR_ICON))
#define QRM_COLOR_ICON_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), QRM_TYPE_COLOR_ICON, QrmColorIconClass))

typedef struct _QrmColorIcon           QrmColorIcon;
typedef struct _QrmColorIconClass      QrmColorIconClass;
typedef struct _QrmColorIconPrivate    QrmColorIconPrivate;

struct _QrmColorIcon
{
    GtkDrawingArea widget;
    QrmColorIconPrivate *priv;
};

struct _QrmColorIconClass
{
    GtkDrawingAreaClass parent_class;
};

GType qrm_color_icon_get_type (void) G_GNUC_CONST;

GtkWidget *qrm_color_icon_new (const GdkRGBA *color);

void qrm_color_icon_set_color (QrmColorIcon *self,
                               guint8 color_red,
                               guint8 color_green,
                               guint8 color_blue);

G_END_DECLS
#endif /* QRM_COLOR_ICON_H */
