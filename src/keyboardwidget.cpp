/*
 * This file is part of the qvkbd project.
 * Copyright (C) 2016 <kiv.apple@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <QDebug>
#include <QX11Info>
#include <QApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QGridLayout>

#include "keyboardwidget.h"

#include <X11/extensions/XTest.h>
#include <X11/Xlocale.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <X11/XKBlib.h>
#include "keysym2ucs.h"

KeyboardWidget::KeyboardWidget(QWidget *parent) : QWidget(parent), m_quickWidget(this) {
	setFocusPolicy(Qt::NoFocus);
	setAttribute(Qt::WA_ShowWithoutActivating);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::ToolTip | Qt::FramelessWindowHint);
	QGridLayout *layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(&m_quickWidget);
	installEventHandlers();
	layoutListChanged();
	layoutChanged();
	loadKeyLayout("qrc:/layouts/standard.qml");
}

KeyboardWidget::~KeyboardWidget() {
	m_eventListener.stop();
}

void KeyboardWidget::loadKeyLayout(const QString &name) {
	m_quickWidget.setSource(name);
	updateLayout();
}

void KeyboardWidget::updateLayout() {
	m_buttons.clear();
	updateLayout(m_quickWidget.rootObject());
	updateLayoutLabels();
}

void KeyboardWidget::updateLayout(QQuickItem *item) {
	if (item == nullptr) return;
	auto children = item->childItems();
	for (auto it = children.begin(); it != children.end(); ++it) {
		updateLayout(*it);
	}
	QVariant scanCodeVariant = item->property("scanCode");
	if (!scanCodeVariant.isValid()) return;
	QVariant labelVariant = item->property("label");
	if (!labelVariant.isValid()) return;
	int scanCode = scanCodeVariant.toInt();
	m_buttons.insert(scanCode, item);
	connect(item, SIGNAL(xPressed(int)), this, SLOT(buttonPressed(int)));
	connect(item, SIGNAL(xReleased(int)), this, SLOT(buttonReleased(int)));
	QString label = labelVariant.toString();
	if (label.size() > 0) return;
	item->setProperty("dynamicLabel", QVariant(true));
}

void KeyboardWidget::updateLayoutLabels() {
	updateLayoutLabels(m_quickWidget.rootObject());
}

void KeyboardWidget::updateLayoutLabels(QQuickItem *item) {
	if (item == nullptr) return;
	auto children = item->childItems();
	for (auto it = children.begin(); it != children.end(); ++it) {
		updateLayoutLabels(*it);
	}
	QVariant scanCodeVariant = item->property("scanCode");
	if (!scanCodeVariant.isValid()) return;
	QVariant labelVariant = item->property("label");
	if (!labelVariant.isValid()) return;
	int scanCode = scanCodeVariant.toInt();
	if (scanCode == 66) {
		item->setProperty("checked", QVariant(m_capsState));
	}
	if (!item->property("dynamicLabel").toBool()) return;
	QString label = textForScanCode(scanCode);
	item->setProperty("label", QVariant(label));\
}

void KeyboardWidget::layoutChanged() {
	QDBusInterface iface("org.kde.keyboard", "/Layouts", "org.kde.KeyboardLayouts", QDBusConnection::sessionBus());
	QDBusReply<QString> reply = iface.call("getCurrentLayout");
	if (reply.isValid()) {
		QString layout = reply.value();
		m_currentLayout = m_layouts.indexOf(layout);
		updateLayoutLabels();
	}
}

void KeyboardWidget::layoutListChanged() {
	QDBusInterface iface("org.kde.keyboard", "/Layouts", "org.kde.KeyboardLayouts", QDBusConnection::sessionBus());
	QDBusReply<QStringList> reply = iface.call("getLayoutsList");
	if (reply.isValid()) {
		m_layouts = reply.value();
	}
}

void KeyboardWidget::installEventHandlers() {
	QString service = "";
	QString path = "/Layouts";
	QString interface = "org.kde.KeyboardLayouts";
	QDBusConnection session = QDBusConnection::sessionBus();
	session.connect(service, path, interface, "currentLayoutChanged", this, SLOT(layoutChanged()));
	session.connect(service, path, interface, "layoutListChanged", this, SLOT(layoutListChanged()));
	connect(&m_eventListener, SIGNAL(keyEvent(int,bool)), this, SLOT(keyEventReceived(int,bool)));
	m_eventListener.start();
}

QString KeyboardWidget::textForScanCode(int scanCode) {
	if (m_currentLayout < 0) return QString();
	int keySymCount = 0;
	KeySym *keySym = XGetKeyboardMapping(QX11Info::display(), scanCode, 1, &keySymCount);
	int normalIndex = m_currentLayout * 2;
	int shiftedIndex = normalIndex + 1;
	KeySym normal = keySym[normalIndex];
	KeySym shifted = keySym[shiftedIndex];
	QString text = QChar((uint)keysym2ucs(m_shiftState ? shifted : normal));
	if (m_capsState) {
		if (m_shiftState) {
			text = text.toLower();
		} else {
			text = text.toUpper();
		}
	}
	XFree((char*)keySym);
	return text;
}

void KeyboardWidget::keyEventReceived(int scanCode, bool pressed) {
	if (pressed) {
		m_pressedButtons.insert(scanCode);
	} else {
		m_pressedButtons.remove(scanCode);
	}
	auto it = m_buttons.find(scanCode);
	if (it != m_buttons.end()) {
		(*it)->setProperty("checked", QVariant(pressed));
	}
	unsigned int n = 0;
	XkbGetIndicatorState(QX11Info::display(), XkbUseCoreKbd, &n);
	m_capsState = (n & 0x01) != 0;
	m_shiftState = m_pressedButtons.contains(50) || m_pressedButtons.contains(62);
	updateLayoutLabels();
}

void KeyboardWidget::buttonPressed(int scanCode) {
	if (scanCode >= 0) {
		m_pressedButtons.insert(scanCode);
		auto it = m_buttons.find(scanCode);
		if (it != m_buttons.end()) {
			(*it)->setProperty("checked", true);
		}
		XTestFakeKeyEvent(QX11Info::display(), scanCode, true, 0);
	}
}

void KeyboardWidget::buttonReleased(int scanCode) {
	if (scanCode >= 0) {
		m_pressedButtons.insert(scanCode);
		auto it = m_buttons.find(scanCode);
		if (it != m_buttons.end()) {
			(*it)->setProperty("checked", false);
		}
		XTestFakeKeyEvent(QX11Info::display(), scanCode, false, 0);
	} else if (scanCode == -2) {
		close();
		QApplication::quit();
	}
}

void KeyboardWidget::mousePressEvent(QMouseEvent *event) {
	m_clickPos = event->globalPos();
	if ((event->button() == Qt::LeftButton) && !m_dragging) {
		m_dragging = true;
	}
}

void KeyboardWidget::mouseReleaseEvent(QMouseEvent *event) {
	Q_UNUSED(event);
	m_dragging = false;
}

void KeyboardWidget::mouseMoveEvent(QMouseEvent *event) {
	if (m_dragging) {
		QPoint delta = event->globalPos() - m_clickPos;
		m_clickPos = event->globalPos();
		move(x() + delta.x(), y() + delta.y());
	}
}