
EXTRA_DIST =

# Desktop file
desktopdir    = $(datadir)/applications
desktop_DATA  = qmi-radio-monitor.desktop
EXTRA_DIST   += qmi-radio-monitor.desktop

# Application icon
# NOTE: yes, this is gnome-system-monitor's icon with other colors, yes.
# All credits go to jimmac, who I believe is the one who draw it
icon256dir    = $(datadir)/icons/hicolor/256x256/apps
icon256_DATA  = qmi-radio-monitor.png
EXTRA_DIST   += qmi-radio-monitor.png

# Polkit policy
edit = @sed -e 's|@bindir[@]|$(bindir)|g'
org.freedesktop.QmiRadioMonitor.pkexec.policy: org.freedesktop.QmiRadioMonitor.pkexec.policy.in
	$(edit) $< >$@

polkit_policydir    = $(datadir)/polkit-1/actions
polkit_policy_DATA  = org.freedesktop.QmiRadioMonitor.pkexec.policy
DISTCLEANFILES      = org.freedesktop.QmiRadioMonitor.pkexec.policy
EXTRA_DIST         += org.freedesktop.QmiRadioMonitor.pkexec.policy.in
