import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.13
import Qool.Components 1.0
import Qool.Controls 1.0
import Qool.Styles 1.0

ScrollView {
  id: root
  property string value

  background: CutCornerBox {
    id: backBox
    backColor: QoolStyle.controlBackgroundColor2
  }

  ScrollBar.horizontal.interactive: true
  ScrollBar.vertical.interactive: false

  implicitHeight: 250
  implicitWidth: 400
  padding: backBox.cutSize
  clip: true

  Grid {
    id: grid
    spacing: 4
    columns: 4
    //    anchors.fill: parent
    Repeater {
      model: Qt.fontFamilies()
      delegate: FontSelectorFamilyButton {
        enabled: !family.startsWith('.')
        width: 100
        height: 45
        index: model.index
        family: modelData
        groupName: "family_name_buttons"
        Component.onCompleted: {
          if (family == root.value)
            checked = true
        }
        onFamilySelected: root.value = family
      } //delegate
    }
  } //grid
}
