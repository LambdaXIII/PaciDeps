import QtQuick 2.14
import Qool.Controls 1.0

QoolDialogWindow {
  title: qsTr("字体设计器")
  width: 600
  height: 400
  property alias optionPanel : pane.optionPanel
  QoolFontDesignerPane {
    id:pane
    anchors.fill: hiddenBox
  }
}
