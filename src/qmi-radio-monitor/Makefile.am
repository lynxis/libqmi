
SUBDIRS = data .

################################################################################
# Program

bin_PROGRAMS = qmi-radio-monitor

qmi_radio_monitor_SOURCES = \
	qrm-resources.h qrm-resources.c \
	qrm-error.h qrm-error-types.h qrm-error-types.c qrm-error-quarks.c \
	qrm-enum-types.h qrm-enum-types.c \
	qrm-color-icon.h qrm-color-icon.c \
	qrm-graph.h qrm-graph.c \
	qrm-device.h qrm-device.c \
	qrm-signal-tab.h qrm-signal-tab.c \
	qrm-power-tab.h qrm-power-tab.c \
	qrm-window.h qrm-window.c \
	qrm-app.h qrm-app.c \
	qrm-main.c \
	$(NULL)

qmi_radio_monitor_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src/libqmi-glib \
	-I$(top_builddir)/src/libqmi-glib/generated \
	$(GTK_CFLAGS) \
	$(GUDEV_CFLAGS) \
	$(NULL)

qmi_radio_monitor_LDADD = \
	$(top_builddir)/src/libqmi-glib/libqmi-glib.la \
	$(GTK_LIBS) \
	$(GUDEV_LIBS) \
	-lm \
	$(NULL)

################################################################################
# Error types

qrm-error-types.h: qrm-error.h $(top_srcdir)/build-aux/templates/qmi-error-types-template.h
	$(AM_V_GEN) $(GLIB_MKENUMS) \
		--fhead "#ifndef __QRM_ERROR_TYPES_H__\n#define __QRM_ERROR_TYPES_H__\n" \
		--template $(top_srcdir)/build-aux/templates/qmi-error-types-template.h \
		--ftail "#endif /* __QRM_ERROR_TYPES_H__ */\n" \
		qrm-error.h > $@

qrm-error-types.c: Makefile.am qrm-error.h $(top_srcdir)/build-aux/templates/qmi-error-types-template.c qrm-error-types.h
	$(AM_V_GEN) $(GLIB_MKENUMS) \
		--fhead "#include \"qrm-error.h\"\n#include \"qrm-error-types.h\"\n" \
		--template $(top_srcdir)/build-aux/templates/qmi-error-types-template.c \
		qrm-error.h > $@

qrm-error-quarks.c: Makefile.am qrm-error.h qrm-error-types.h $(top_srcdir)/build-aux/templates/qmi-error-quarks-template.c
	$(AM_V_GEN) $(GLIB_MKENUMS) \
		--fhead "#include \"qrm-error.h\"\n#include \"qrm-error-types.h\"\n" \
		--template $(top_srcdir)/build-aux/templates/qmi-error-quarks-template.c \
		qrm-error.h > $@

################################################################################
# Enum types

ENUMS = \
	$(srcdir)/qrm-device.h \
	$(srcdir)/qrm-graph.h

qrm-enum-types.h: Makefile.am $(ENUMS) $(top_srcdir)/build-aux/templates/qmi-enum-types-template.h
	$(AM_V_GEN) $(GLIB_MKENUMS) \
		--fhead "#include \"qrm-device.h\"\n#include \"qrm-graph.h\"\n#ifndef __QRM_ENUM_TYPES_H__\n#define __QRM_ENUM_TYPES_H__\n" \
		--template $(top_srcdir)/build-aux/templates/qmi-enum-types-template.h \
		--ftail "#endif /* __QRM_ENUM_TYPES_H__ */\n" \
		$(ENUMS) > $@

qrm-enum-types.c: Makefile.am $(top_srcdir)/build-aux/templates/qmi-enum-types-template.c qrm-enum-types.h
	$(AM_V_GEN) $(GLIB_MKENUMS) \
		--fhead "#include \"qrm-enum-types.h\"" \
		--template $(top_srcdir)/build-aux/templates/qmi-enum-types-template.c \
		$(ENUMS) > $@

################################################################################
# Resource files

resource_file_dependencies = \
	$(shell $(GLIB_COMPILE_RESOURCES) \
		--sourcedir=$(srcdir) \
		--generate-dependencies \
		$(srcdir)/qrm.gresource.xml)

qrm-resources.c: qrm.gresource.xml $(resource_file_dependencies)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) \
		--target=$@ \
		--sourcedir=$(srcdir) \
		--generate-source \
		--c-name qrm \
		$(srcdir)/qrm.gresource.xml

qrm-resources.h: qrm.gresource.xml $(resource_file_dependencies)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) \
		--target=$@ \
		--sourcedir=$(srcdir) \
		--generate-header \
		--c-name qrm \
		$(srcdir)/qrm.gresource.xml

################################################################################

BUILT_SOURCES = \
	qrm-resources.h qrm-resources.c \
	qrm-error-types.h qrm-error-types.c qrm-error-quarks.c \
	qrm-enum-types.h qrm-enum-types.c \
	$(NULL)

CLEANFILES = \
	qrm-resources.h qrm-resources.c \
	qrm-error-types.h qrm-error-types.c \
	qrm-enum-types.h qrm-enum-types.c \
	$(NULL)

EXTRA_DIST = \
	qrm-window.ui \
	qrm-signal-tab.ui \
	qrm-power-tab.ui \
	qrm.gresource.xml \
	$(NULL)
