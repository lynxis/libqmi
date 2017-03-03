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

#include <config.h>
#include <gtk/gtk.h>

#include "qrm-signal-tab.h"
#include "qrm-graph.h"
#include "qrm-color-icon.h"

struct _QrmSignalTabPrivate {
    QrmDevice *current;

    guint act_updated_id;

    GtkWidget *legend_gsm_box;
    GtkWidget *legend_gsm_icon;
    GtkWidget *legend_gsm_rssi_value_label;

    GtkWidget *legend_umts_box;
    GtkWidget *legend_umts_icon;
    GtkWidget *legend_umts_rssi_value_label;
    GtkWidget *legend_umts_ecio_value_label;

    GtkWidget *legend_lte_box;
    GtkWidget *legend_lte_icon;
    GtkWidget *legend_lte_rssi_value_label;
    GtkWidget *legend_lte_rsrq_value_label;
    GtkWidget *legend_lte_rsrp_value_label;
    GtkWidget *legend_lte_snr_value_label;

    GtkWidget *legend_cdma_box;
    GtkWidget *legend_cdma_icon;
    GtkWidget *legend_cdma_rssi_value_label;
    GtkWidget *legend_cdma_ecio_value_label;

    GtkWidget *legend_evdo_box;
    GtkWidget *legend_evdo_icon;
    GtkWidget *legend_evdo_rssi_value_label;
    GtkWidget *legend_evdo_ecio_value_label;
    GtkWidget *legend_evdo_sinr_level_value_label;
    GtkWidget *legend_evdo_io_value_label;

    GtkWidget *rssi_graph;
    GtkWidget *rssi_graph_frame;
    guint rssi_graph_updated_id;

    GtkWidget *ecio_graph;
    GtkWidget *ecio_graph_frame;
    guint ecio_graph_updated_id;

    GtkWidget *sinr_level_graph;
    GtkWidget *sinr_level_graph_frame;
    guint sinr_level_graph_updated_id;

    GtkWidget *io_graph;
    GtkWidget *io_graph_frame;
    guint io_graph_updated_id;

    GtkWidget *rsrq_graph;
    GtkWidget *rsrq_graph_frame;
    guint rsrq_graph_updated_id;

    GtkWidget *rsrp_graph;
    GtkWidget *rsrp_graph_frame;
    guint rsrp_graph_updated_id;

    GtkWidget *snr_graph;
    GtkWidget *snr_graph_frame;
    guint snr_graph_updated_id;
};

G_DEFINE_TYPE_WITH_PRIVATE (QrmSignalTab, qrm_signal_tab, GTK_TYPE_BOX)

/******************************************************************************/

static void
act_updated (QrmDevice *device,
             QrmDeviceAct act,
             QrmSignalTab *self)
{
    gtk_widget_set_sensitive (self->priv->rssi_graph_frame, act != 0);
    gtk_widget_set_sensitive (self->priv->ecio_graph_frame,
                              (act & (QRM_DEVICE_ACT_UMTS |
                                      QRM_DEVICE_ACT_CDMA |
                                      QRM_DEVICE_ACT_EVDO)));
    gtk_widget_set_sensitive (self->priv->sinr_level_graph_frame,
                              (act & (QRM_DEVICE_ACT_EVDO)));
    gtk_widget_set_sensitive (self->priv->io_graph_frame,
                              (act & (QRM_DEVICE_ACT_EVDO)));
    gtk_widget_set_sensitive (self->priv->rsrq_graph_frame,
                              (act & (QRM_DEVICE_ACT_LTE)));
    gtk_widget_set_sensitive (self->priv->rsrp_graph_frame,
                              (act & (QRM_DEVICE_ACT_LTE)));
    gtk_widget_set_sensitive (self->priv->snr_graph_frame,
                              (act & (QRM_DEVICE_ACT_LTE)));

    gtk_widget_set_sensitive (self->priv->legend_gsm_box,
                              (act & QRM_DEVICE_ACT_GSM));
    gtk_widget_set_sensitive (self->priv->legend_umts_box,
                              (act & QRM_DEVICE_ACT_UMTS));
    gtk_widget_set_sensitive (self->priv->legend_lte_box,
                              (act & QRM_DEVICE_ACT_LTE));
    gtk_widget_set_sensitive (self->priv->legend_cdma_box,
                              (act & QRM_DEVICE_ACT_CDMA));
    gtk_widget_set_sensitive (self->priv->legend_evdo_box,
                              (act & QRM_DEVICE_ACT_EVDO));
}

typedef enum {
    SERIES_RSSI_GSM  = 0,
    SERIES_RSSI_UMTS = 1,
    SERIES_RSSI_LTE  = 2,
    SERIES_RSSI_CDMA = 3,
    SERIES_RSSI_EVDO = 4,
} SeriesRssi;

static void
rssi_updated (QrmDevice *device,
              gdouble gsm_rssi,
              gdouble umts_rssi,
              gdouble lte_rssi,
              gdouble cdma_rssi,
              gdouble evdo_rssi,
              QrmSignalTab *self)
{
    qrm_graph_step_init (QRM_GRAPH (self->priv->rssi_graph));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->rssi_graph),
                              SERIES_RSSI_GSM,
                              gsm_rssi,
                              GTK_LABEL (self->priv->legend_gsm_rssi_value_label));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->rssi_graph),
                              SERIES_RSSI_UMTS,
                              umts_rssi,
                              GTK_LABEL (self->priv->legend_umts_rssi_value_label));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->rssi_graph),
                              SERIES_RSSI_LTE,
                              lte_rssi,
                              GTK_LABEL (self->priv->legend_lte_rssi_value_label));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->rssi_graph),
                              SERIES_RSSI_CDMA,
                              cdma_rssi,
                              GTK_LABEL (self->priv->legend_cdma_rssi_value_label));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->rssi_graph),
                              SERIES_RSSI_EVDO,
                              evdo_rssi,
                              GTK_LABEL (self->priv->legend_cdma_rssi_value_label));
    qrm_graph_step_finish (QRM_GRAPH (self->priv->rssi_graph));
}

typedef enum {
    SERIES_ECIO_UMTS = 0,
    SERIES_ECIO_CDMA = 1,
    SERIES_ECIO_EVDO = 2,
} SeriesEcio;

static void
ecio_updated (QrmDevice *device,
              gdouble umts_ecio,
              gdouble cdma_ecio,
              gdouble evdo_ecio,
              QrmSignalTab *self)
{
    qrm_graph_step_init (QRM_GRAPH (self->priv->ecio_graph));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->ecio_graph),
                              SERIES_ECIO_UMTS,
                              umts_ecio,
                              GTK_LABEL (self->priv->legend_umts_ecio_value_label));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->ecio_graph),
                              SERIES_ECIO_CDMA,
                              cdma_ecio,
                              GTK_LABEL (self->priv->legend_cdma_ecio_value_label));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->ecio_graph),
                              SERIES_ECIO_EVDO,
                              evdo_ecio,
                              GTK_LABEL (self->priv->legend_evdo_ecio_value_label));
    qrm_graph_step_finish (QRM_GRAPH (self->priv->ecio_graph));
}

typedef enum {
    SERIES_SINR_LEVEL_EVDO = 0,
} SeriesSinrLevel;

static void
sinr_level_updated (QrmDevice *device,
                    gdouble evdo_sinr_level,
                    QrmSignalTab *self)
{
    qrm_graph_step_init (QRM_GRAPH (self->priv->sinr_level_graph));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->sinr_level_graph),
                              SERIES_SINR_LEVEL_EVDO,
                              evdo_sinr_level,
                              GTK_LABEL (self->priv->legend_evdo_sinr_level_value_label));
    qrm_graph_step_finish (QRM_GRAPH (self->priv->sinr_level_graph));
}

typedef enum {
    SERIES_IO_EVDO = 0,
} SeriesIo;

static void
io_updated (QrmDevice *device,
            gdouble evdo_io,
            QrmSignalTab *self)
{
    qrm_graph_step_init (QRM_GRAPH (self->priv->io_graph));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->io_graph),
                              SERIES_IO_EVDO,
                              evdo_io,
                              GTK_LABEL (self->priv->legend_evdo_io_value_label));
    qrm_graph_step_finish (QRM_GRAPH (self->priv->io_graph));
}

typedef enum {
    SERIES_RSRQ_LTE = 0,
} SeriesRsrqLte;

static void
rsrq_updated (QrmDevice *device,
              gdouble lte_rsrq,
              QrmSignalTab *self)
{
    qrm_graph_step_init (QRM_GRAPH (self->priv->rsrq_graph));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->rsrq_graph),
                              SERIES_RSRQ_LTE,
                              lte_rsrq,
                              GTK_LABEL (self->priv->legend_lte_rsrq_value_label));
    qrm_graph_step_finish (QRM_GRAPH (self->priv->rsrq_graph));
}

typedef enum {
    SERIES_RSRP_LTE = 0,
} SeriesRsrpLte;

static void
rsrp_updated (QrmDevice *device,
              gdouble lte_rsrp,
              QrmSignalTab *self)
{
    qrm_graph_step_init (QRM_GRAPH (self->priv->rsrp_graph));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->rsrp_graph),
                              SERIES_RSRP_LTE,
                              lte_rsrp,
                              GTK_LABEL (self->priv->legend_lte_rsrp_value_label));
    qrm_graph_step_finish (QRM_GRAPH (self->priv->rsrp_graph));
}

typedef enum {
    SERIES_SNR_LTE = 0,
} SeriesSnrLte;

static void
snr_updated (QrmDevice *device,
             gdouble lte_snr,
             QrmSignalTab *self)
{
    qrm_graph_step_init (QRM_GRAPH (self->priv->snr_graph));
    qrm_graph_step_set_value (QRM_GRAPH (self->priv->snr_graph),
                              SERIES_SNR_LTE,
                              lte_snr,
                              GTK_LABEL (self->priv->legend_lte_snr_value_label));
    qrm_graph_step_finish (QRM_GRAPH (self->priv->snr_graph));
}

void
qrm_signal_tab_change_current_device (QrmSignalTab *self,
                                      QrmDevice *new_device)
{
    if (self->priv->current) {
        /* If same device, nothing else needed */
        if (new_device &&
            (self->priv->current == new_device ||
             g_str_equal (qrm_device_get_name (self->priv->current), qrm_device_get_name (new_device))))
            return;

        /* Changing current device, cleanup */
        if (self->priv->act_updated_id) {
            g_signal_handler_disconnect (self->priv->current, self->priv->act_updated_id);
            self->priv->act_updated_id = 0;
        }

        if (self->priv->rssi_graph_updated_id) {
            g_signal_handler_disconnect (self->priv->current, self->priv->rssi_graph_updated_id);
            self->priv->rssi_graph_updated_id = 0;
        }

        if (self->priv->ecio_graph_updated_id) {
            g_signal_handler_disconnect (self->priv->current, self->priv->ecio_graph_updated_id);
            self->priv->ecio_graph_updated_id = 0;
        }

        if (self->priv->sinr_level_graph_updated_id) {
            g_signal_handler_disconnect (self->priv->current, self->priv->sinr_level_graph_updated_id);
            self->priv->sinr_level_graph_updated_id = 0;
        }

        if (self->priv->io_graph_updated_id) {
            g_signal_handler_disconnect (self->priv->current, self->priv->io_graph_updated_id);
            self->priv->io_graph_updated_id = 0;
        }

        if (self->priv->rsrq_graph_updated_id) {
            g_signal_handler_disconnect (self->priv->current, self->priv->rsrq_graph_updated_id);
            self->priv->rsrq_graph_updated_id = 0;
        }

        if (self->priv->rsrp_graph_updated_id) {
            g_signal_handler_disconnect (self->priv->current, self->priv->rsrp_graph_updated_id);
            self->priv->rsrp_graph_updated_id = 0;
        }

        if (self->priv->snr_graph_updated_id) {
            g_signal_handler_disconnect (self->priv->current, self->priv->snr_graph_updated_id);
            self->priv->snr_graph_updated_id = 0;
        }

        g_clear_object (&self->priv->current);

        /* Clear graphs */

        qrm_graph_clear_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_GSM);
        qrm_graph_clear_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_UMTS);
        qrm_graph_clear_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_LTE);
        qrm_graph_clear_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_CDMA);
        qrm_graph_clear_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_EVDO);

        qrm_graph_clear_series (QRM_GRAPH (self->priv->ecio_graph), SERIES_ECIO_UMTS);
        qrm_graph_clear_series (QRM_GRAPH (self->priv->ecio_graph), SERIES_ECIO_CDMA);
        qrm_graph_clear_series (QRM_GRAPH (self->priv->ecio_graph), SERIES_ECIO_EVDO);

        qrm_graph_clear_series (QRM_GRAPH (self->priv->sinr_level_graph), SERIES_SINR_LEVEL_EVDO);

        qrm_graph_clear_series (QRM_GRAPH (self->priv->io_graph), SERIES_IO_EVDO);

        qrm_graph_clear_series (QRM_GRAPH (self->priv->rsrq_graph), SERIES_RSRQ_LTE);

        qrm_graph_clear_series (QRM_GRAPH (self->priv->rsrp_graph), SERIES_RSRP_LTE);

        qrm_graph_clear_series (QRM_GRAPH (self->priv->snr_graph), SERIES_SNR_LTE);
    }

    if (new_device) {
        /* Keep a ref to current device */
        self->priv->current = g_object_ref (new_device);
        self->priv->act_updated_id = g_signal_connect (new_device,
                                                       "act-updated",
                                                       G_CALLBACK (act_updated),
                                                       self);
        self->priv->rssi_graph_updated_id = g_signal_connect (new_device,
                                                              "rssi-updated",
                                                              G_CALLBACK (rssi_updated),
                                                              self);
        self->priv->ecio_graph_updated_id = g_signal_connect (new_device,
                                                              "ecio-updated",
                                                              G_CALLBACK (ecio_updated),
                                                              self);
        self->priv->sinr_level_graph_updated_id = g_signal_connect (new_device,
                                                                    "sinr-level-updated",
                                                                    G_CALLBACK (sinr_level_updated),
                                                                    self);
        self->priv->io_graph_updated_id = g_signal_connect (new_device,
                                                            "io-updated",
                                                            G_CALLBACK (io_updated),
                                                            self);
        self->priv->rsrq_graph_updated_id = g_signal_connect (new_device,
                                                              "rsrq-updated",
                                                              G_CALLBACK (rsrq_updated),
                                                              self);
        self->priv->rsrp_graph_updated_id = g_signal_connect (new_device,
                                                              "rsrp-updated",
                                                              G_CALLBACK (rsrp_updated),
                                                              self);
        self->priv->snr_graph_updated_id = g_signal_connect (new_device,
                                                             "snr-updated",
                                                             G_CALLBACK (snr_updated),
                                                             self);
    }
}

/******************************************************************************/

static void
qrm_signal_tab_init (QrmSignalTab *self)
{
    self->priv = qrm_signal_tab_get_instance_private (self);

    /* Ensure we register the QrmGraph and QrmColorIcon before initiating
     * the template */
    g_warn_if_fail (qrm_color_icon_get_type ());
    g_warn_if_fail (qrm_graph_get_type ());

    gtk_widget_init_template (GTK_WIDGET (self));

#define GSM_RGB  76,  153, 0   /* green */
#define UMTS_RGB 204, 0,   0   /* red */
#define LTE_RGB  0,   76,  153 /* blue */
#define CDMA_RGB 153, 153, 0   /* yellow */
#define EVDO_RGB 153, 0,   153 /* purple */

    /* Main legend box */
    qrm_color_icon_set_color (QRM_COLOR_ICON (self->priv->legend_gsm_icon),  GSM_RGB);
    qrm_color_icon_set_color (QRM_COLOR_ICON (self->priv->legend_umts_icon), UMTS_RGB);
    qrm_color_icon_set_color (QRM_COLOR_ICON (self->priv->legend_lte_icon),  LTE_RGB);
    qrm_color_icon_set_color (QRM_COLOR_ICON (self->priv->legend_cdma_icon), CDMA_RGB);
    qrm_color_icon_set_color (QRM_COLOR_ICON (self->priv->legend_evdo_icon), EVDO_RGB);

    /* RSSI graph */
    qrm_graph_setup_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_GSM,  "GSM",  GSM_RGB);
    qrm_graph_setup_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_UMTS, "UMTS", UMTS_RGB);
    qrm_graph_setup_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_LTE,  "LTE",  LTE_RGB);
    qrm_graph_setup_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_CDMA, "CDMA", CDMA_RGB);
    qrm_graph_setup_series (QRM_GRAPH (self->priv->rssi_graph), SERIES_RSSI_EVDO, "EVDO", EVDO_RGB);

    /* ECIO graph */
    qrm_graph_setup_series (QRM_GRAPH (self->priv->ecio_graph), SERIES_ECIO_UMTS, "UMTS", UMTS_RGB);
    qrm_graph_setup_series (QRM_GRAPH (self->priv->ecio_graph), SERIES_ECIO_CDMA, "CDMA", CDMA_RGB);
    qrm_graph_setup_series (QRM_GRAPH (self->priv->ecio_graph), SERIES_ECIO_EVDO, "EVDO", EVDO_RGB);

    /* SINR level graph */
    qrm_graph_setup_series (QRM_GRAPH (self->priv->sinr_level_graph), SERIES_SINR_LEVEL_EVDO, "EVDO", EVDO_RGB);

    /* Io graph */
    qrm_graph_setup_series (QRM_GRAPH (self->priv->io_graph), SERIES_IO_EVDO, "EVDO", EVDO_RGB);

    /* RSRQ graph */
    qrm_graph_setup_series (QRM_GRAPH (self->priv->rsrq_graph), SERIES_RSRQ_LTE, "LTE", LTE_RGB);

    /* RSRP graph */
    qrm_graph_setup_series (QRM_GRAPH (self->priv->rsrp_graph), SERIES_RSRP_LTE, "LTE", LTE_RGB);

    /* SNR graph */
    qrm_graph_setup_series (QRM_GRAPH (self->priv->snr_graph), SERIES_SNR_LTE, "LTE", LTE_RGB);
}

static void
dispose (GObject *object)
{
    QrmSignalTab *self = QRM_SIGNAL_TAB (object);

    g_clear_object (&self->priv->current);

    G_OBJECT_CLASS (qrm_signal_tab_parent_class)->dispose (object);
}

static void
qrm_signal_tab_class_init (QrmSignalTabClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    object_class->dispose = dispose;

    /* Bind class to template */
    gtk_widget_class_set_template_from_resource  (widget_class, "/org/freedesktop/qrm/qrm-signal-tab.ui");
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, rssi_graph);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, ecio_graph);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, sinr_level_graph);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, io_graph);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, rsrq_graph);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, rsrp_graph);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, snr_graph);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, rssi_graph_frame);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, ecio_graph_frame);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, sinr_level_graph_frame);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, io_graph_frame);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, rsrq_graph_frame);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, rsrp_graph_frame);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, snr_graph_frame);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_gsm_box);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_umts_box);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_lte_box);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_cdma_box);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_evdo_box);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_gsm_icon);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_umts_icon);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_lte_icon);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_cdma_icon);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_evdo_icon);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_gsm_rssi_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_umts_rssi_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_lte_rssi_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_cdma_rssi_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_evdo_rssi_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_umts_ecio_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_cdma_ecio_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_evdo_ecio_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_evdo_sinr_level_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_evdo_io_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_lte_rsrq_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_lte_rsrp_value_label);
    gtk_widget_class_bind_template_child_private (widget_class, QrmSignalTab, legend_lte_snr_value_label);

}
