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
 */

#ifndef QRM_GRAPH_H
#define QRM_GRAPH_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define QRM_TYPE_GRAPH            (qrm_graph_get_type ())
#define QRM_GRAPH(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), QRM_TYPE_GRAPH, QrmGraph))
#define QRM_GRAPH_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  QRM_TYPE_GRAPH, QrmGraphClass))
#define QRM_IS_GRAPH(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), QRM_TYPE_GRAPH))
#define QRM_IS_GRAPH_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  QRM_TYPE_GRAPH))
#define QRM_GRAPH_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  QRM_TYPE_GRAPH, QrmGraphClass))

typedef struct _QrmGraph QrmGraph;
typedef struct _QrmGraphClass QrmGraphClass;
typedef struct _QrmGraphPrivate QrmGraphPrivate;

typedef enum {
    QRM_GRAPH_LEGEND_POSITION_NONE   = 0,
    QRM_GRAPH_LEGEND_POSITION_TOP    = 1,
    QRM_GRAPH_LEGEND_POSITION_BOTTOM = 2,
} QrmGraphLegendPosition;

/**
 * QrmGraph:
 *
 * The #QrmGraph structure contains private data and should only be accessed
 * using the provided API.
 */
struct _QrmGraph {
    /*< private >*/
    GtkBox parent;
    QrmGraphPrivate *priv;
};

struct _QrmGraphClass {
    /*< private >*/
    GtkBoxClass parent;
};

GType qrm_graph_get_type (void);

GtkWidget *qrm_graph_new (void);

void qrm_graph_setup_series (QrmGraph *self,
                             guint series_index,
                             const gchar *label,
                             guint8 color_red,
                             guint8 color_green,
                             guint8 color_blue);
void qrm_graph_clear_series (QrmGraph *self,
                             guint series_index);

void qrm_graph_step_init      (QrmGraph *self);
void qrm_graph_step_set_value (QrmGraph *self,
                               guint series_index,
                               gdouble value,
                               GtkLabel *additional_label);
void qrm_graph_step_finish    (QrmGraph *self);

G_END_DECLS

#endif /* QRM_GRAPH_H */
