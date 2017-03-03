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

#ifndef QRM_DEVICE_H
#define QRM_DEVICE_H

#include <gtk/gtk.h>
#include <libqmi-glib.h>

G_BEGIN_DECLS

#define QRM_TYPE_DEVICE         (qrm_device_get_type ())
#define QRM_DEVICE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), QRM_TYPE_DEVICE, QrmDevice))
#define QRM_DEVICE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), QRM_TYPE_DEVICE, QrmDeviceClass))
#define QRM_IS_DEVICE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), QRM_TYPE_DEVICE))
#define QRM_IS_DEVICE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), QRM_TYPE_DEVICE))
#define QRM_DEVICE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), QRM_TYPE_DEVICE, QrmDeviceClass))

typedef enum {
    QRM_DEVICE_STATUS_UNKNOWN,
    QRM_DEVICE_STATUS_READY,
    QRM_DEVICE_STATUS_SIM_PIN_LOCKED,
    QRM_DEVICE_STATUS_SIM_PUK_LOCKED,
    QRM_DEVICE_STATUS_SIM_ERROR,
} QrmDeviceStatus;

typedef enum {
    QRM_DEVICE_ACT_GSM  = 1 << 0,
    QRM_DEVICE_ACT_UMTS = 1 << 1,
    QRM_DEVICE_ACT_LTE  = 1 << 2,
    QRM_DEVICE_ACT_CDMA = 1 << 3,
    QRM_DEVICE_ACT_EVDO = 1 << 4,
} QrmDeviceAct;

typedef struct _QrmDevice        QrmDevice;
typedef struct _QrmDeviceClass   QrmDeviceClass;
typedef struct _QrmDevicePrivate QrmDevicePrivate;

struct _QrmDevice {
    GObject parent_instance;
    QrmDevicePrivate *priv;
};

struct _QrmDeviceClass {
    GObjectClass parent_class;

    /* Signals */

    void (*act_updated) (QrmDevice *device,
                         QrmDeviceAct act);

    void (*rssi_updated) (QrmDevice *device,
                          gdouble gsm_rssi,
                          gdouble umts_rssi,
                          gdouble lte_rssi,
                          gdouble cdma_rssi,
                          gdouble evdo_rssi);

    void (*ecio_updated) (QrmDevice *device,
                          gdouble umts_ecio,
                          gdouble cdma_ecio,
                          gdouble evdo_ecio);

    void (*sinr_level_updated) (QrmDevice *device,
                                gdouble evdo_sinr_level);

    void (*io_updated) (QrmDevice *device,
                        gdouble evdo_io);

    void (*rsrq_updated) (QrmDevice *device,
                          gdouble lte_rsrq);

    void (*rsrp_updated) (QrmDevice *device,
                          gdouble lte_rsrp);

    void (*snr_updated) (QrmDevice *device,
                         gdouble lte_snr);

    void (*rx0_updated) (QrmDevice *device,
                         gdouble gsm_rx0,
                         gdouble umts_rx0,
                         gdouble lte_rx0,
                         gdouble cdma_rx0,
                         gdouble evdo_rx0);

    void (*rx1_updated) (QrmDevice *device,
                         gdouble gsm_rx1,
                         gdouble umts_rx1,
                         gdouble lte_rx1,
                         gdouble cdma_rx1,
                         gdouble evdo_rx1);

    void (*tx_updated) (QrmDevice *device,
                         gdouble gsm_tx,
                         gdouble umts_tx,
                         gdouble lte_tx,
                         gdouble cdma_tx,
                         gdouble evdo_tx);
};

GType qrm_device_get_type (void) G_GNUC_CONST;

void       qrm_device_new        (GFile *file,
                                  GCancellable *cancellable,
                                  GAsyncReadyCallback callback,
                                  gpointer user_data);
QrmDevice *qrm_device_new_finish (GAsyncResult *res,
                                  GError **error);

const gchar     *qrm_device_get_name              (QrmDevice *self);
const gchar     *qrm_device_get_manufacturer      (QrmDevice *self);
const gchar     *qrm_device_get_model             (QrmDevice *self);
const gchar     *qrm_device_get_revision          (QrmDevice *self);
QrmDeviceStatus  qrm_device_get_status            (QrmDevice *self);
gint             qrm_device_get_pin_attempts_left (QrmDevice *self);

QmiDevice       *qrm_device_peek_qmi_device  (QrmDevice *self);

void     qrm_device_unlock        (QrmDevice *self,
                                   const gchar *pin,
                                   GAsyncReadyCallback callback,
                                   gpointer user_data);
gboolean qrm_device_unlock_finish (QrmDevice *self,
                                   GAsyncResult *res,
                                   GError **error);

void     qrm_device_start_nas        (QrmDevice *self,
                                      GAsyncReadyCallback callback,
                                      gpointer user_data);
gboolean qrm_device_start_nas_finish (QrmDevice *self,
                                      GAsyncResult *res,
                                      GError **error);

void     qrm_device_stop_nas        (QrmDevice *self,
                                     GAsyncReadyCallback callback,
                                     gpointer user_data);
gboolean qrm_device_stop_nas_finish (QrmDevice *self,
                                     GAsyncResult *res,
                                     GError **error);

void     qrm_device_close        (QrmDevice *self,
                                  GCancellable *cancellable,
                                  GAsyncReadyCallback callback,
                                  gpointer user_data);
gboolean qrm_device_close_finish (QrmDevice *self,
                                  GAsyncResult *res,
                                  GError **error);

G_END_DECLS

#endif /* QRM_DEVICE_H */
