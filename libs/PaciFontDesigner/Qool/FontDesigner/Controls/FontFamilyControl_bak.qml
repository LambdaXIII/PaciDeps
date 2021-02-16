import QtQuick 2.14
import Qool.Controls 1.0
import Qool.Styles 1.0
import Qool.Controls.Inputs 1.0
import Qool.Components 1.0
import QtQuick.Controls 2.14
import "components"
import QtQuick.Window 2.14

BasicInputControl {
  id: control
  property string value: "Arial"
  property string defaultValue: "Arial"
  function resetValue() {
    value = defaultValue
  }

  contentItem: Item {
    Text {
      id: showText
      text: control.value
      color: QoolStyle.textColor
      anchors.fill: parent
      verticalAlignment: TextInput.AlignVCenter
      horizontalAlignment: TextInput.AlignRight
      font: control.font
      visible: !mainText.visible
    }
    TextInput {
      id: mainText
      text: control.value
      clip: true
      color: QoolStyle.textColor
      font: control.font
      anchors.fill: parent
      leftPadding: editingIndicator.width
      selectionColor: QoolStyle.infoColor
      selectedTextColor: control.backBox.backColor
      selectByMouse: true
      verticalAlignment: TextInput.AlignVCenter
      horizontalAlignment: TextInput.AlignRight
      onEditingFinished: {
        control.value = displayText
        visible = false
      }
      visible: false
      z: 2
      SmallIndicator {
        id: editingIndicator
        text: qsTr("手动编辑")
        backColor: QoolStyle.highlightColor
        y: (parent.height - height) / 2
      }
    }

    MouseArea {
      id: mArea
      anchors.fill: parent
      acceptedButtons: Qt.LeftButton | Qt.RightButton
      onClicked: if (mouse.button === Qt.LeftButton) {
                   sel_dialog.open()
                 } else {
                   cmenu.popup()
                 }
    }
  } //contentItem:
  QoolMenu {
    id: cmenu
    title: qsTr("字体设置")
    showTitle: true
    Action {
      text: qsTr("重置默认值")
      onTriggered: resetValue()
    }
    Action {
      text: qsTr("手动设置字体名称")
      onTriggered: {
        mainText.visible = true
        mainText.selectAll()
      }
    }
  } //menu

  QoolDialogWindow {
    id: sel_dialog
    title: qsTr("字体选择")
    resizable: false
    width: 460
    height: 350
    Loader {
      id: selectorLoader
      enabled: sel_dialog.visible
      anchors.fill: sel_dialog.hiddenBox
      asynchronous: true
      visible: selectorLoader.status == Loader.Ready
      sourceComponent: FontSelector {}
    }
    onAccepted: control.value = selectorLoader.item.value
    okButton.text: qsTr("选好啦")
    cancelButton.text: qsTr("不选了")
  }
}
