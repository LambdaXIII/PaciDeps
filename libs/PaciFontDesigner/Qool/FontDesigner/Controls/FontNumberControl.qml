import QtQuick 2.14
import QtQuick.Controls 2.14
import Qool.Controls.Inputs 1.0
import Qool.Controls 1.0

QoolDragNumberControl {
  id: control
  QoolMenu {
    id: cmenu
    title: control.title
    showTitle: true
    QoolMenuBanner {
      text: control.description == "" ? (qsTr(
                                           "设置") + control.title) : control.description
    }

    Action {
      text: qsTr("重置默认值")
      onTriggered: control.resetValue()
    }
  }

  MouseArea {
    acceptedButtons: Qt.RightButton
    onClicked: cmenu.popup()
    anchors.fill: contentItem
  }
}
