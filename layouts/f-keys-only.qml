/*
 * This file is part of the qvkbd project.
 * Copyright (C) 2016 Ivan Kolesnikov <kiv.apple@gmail.com>
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
 * ----
 * This is a (draft of a) software extension to the hardware keyboard in the Gemini PDA
 *
 */
import QtQuick 2.0
import QtQuick.Layouts 1.3
import "keys"

Rectangle {
    property bool numLockActive: false

    width: childrenRect.width + 20
    height: childrenRect.height + 20
    color: "#00000000"
    border.width: 2

    Row {
        x: 10
        y: 10
        width: childrenRect.width
        height: childrenRect.height

        Column {
            id: column
            width: childrenRect.width
            height: childrenRect.height
            spacing: 4

            Row {
                id: row
                width: childrenRect.width
                height: 30
                spacing: 4

                KeyItem {
                    id: keyItem1
                    width: 40
                    height: 30
                    scanCode: 67
                    label: "F1"
                }

                KeyItem {
                    id: keyItem2
                    width: 40
                    height: 30
                    scanCode: 68
                    label: "F2"
                }

                KeyItem {
                    id: keyItem3
                    width: 40
                    height: 30
                    scanCode: 69
                    label: "F3"
                }

                KeyItem {
                    id: keyItem4
                    width: 40
                    height: 30
                    scanCode: 70
                    label: "F4"
                }

                Item {
                    id: spacer2
                    width: 6
                    height: 30
                }

                KeyItem {
                    id: keyItem5
                    width: 40
                    height: 30
                    scanCode: 71
                    label: "F5"
                }

                KeyItem {
                    id: keyItem6
                    width: 40
                    height: 30
                    scanCode: 72
                    label: "F6"
                }

                KeyItem {
                    id: keyItem7
                    width: 40
                    height: 30
                    scanCode: 73
                    label: "F7"
                }

                KeyItem {
                    id: keyItem8
                    width: 40
                    height: 30
                    scanCode: 74
                    label: "F8"
                }

                Item {
                    id: spacer3
                    width: 6
                    height: 30
                }

                KeyItem {
                    id: keyItem9
                    width: 40
                    height: 30
                    scanCode: 75
                    label: "F9"
                }

                KeyItem {
                    id: keyItem10
                    width: 40
                    height: 30
                    scanCode: 76
                    label: "F10"
                }

                KeyItem {
                    id: keyItem11
                    width: 40
                    height: 30
                    scanCode: 95
                    label: "F11"
                }

                KeyItem {
                    id: keyItem12
                    width: 40
                    height: 30
                    scanCode: 96
                    label: "F12"
                }
        }

        Item {
            width: 10
            height: parent.height
        }

        Column {
            width: childrenRect.width
            height: childrenRect.height
            spacing: 4

            Row {
                width: parent.width
                height: 30
                layoutDirection: Qt.RightToLeft

                KeyItem {
                    id: keyItem27
                    width: 40
                    height: 30
                    scanCode: -2
                    label: "×"
                }
            }
        }
    }

    property double scaleX: 1
    property double scaleY: 1
    transform: Scale { origin.x: 0; origin.y: 0; xScale: scaleX; yScale: scaleY }

}
