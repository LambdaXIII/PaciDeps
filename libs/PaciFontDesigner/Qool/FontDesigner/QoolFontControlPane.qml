import QtQuick 2.14
import Qool.Controls.Inputs 1.0
import QtQuick.Controls 2.14
import Qool.Components 1.0
import Qool.Styles 1.0
import "Controls"

ScrollView {
  id: root

  property alias family: familyControl.value
  property alias size: sizeControl.value
  property alias isBold: styleControl.isBold
  property alias isItalic: styleControl.isItalic
  property alias alignment: alignmentControl.currentIndex
  property alias alignmentValue: alignmentControl.value
  property alias textColor: textColorControl.value
  property alias lead: leadControl.value
  property alias track: trackControl.value
  property alias outlineWidth: outlineWidthControl.value
  property alias outlineSoft: outlineSoftControl.value
  property alias outlineColor: outlineColorControl.value
  property alias aspect: aspectControl.value
  property alias textOpacity: opacityControl.value

  function setBoldItalic(b, i) {
    if (b && i)
      styleControl.currentIndex = 3
    else if (b)
      styleControl.currentIndex = 1
    else if (i)
      styleControl.currentIndex = 2
    else
      styleControl.currentIndex = 0
  }

  background: CutCornerBox {
    cutSize: QoolStyle.controlCutSize
    backColor: QoolStyle.controlBackgroundColor2
  }

  padding: 10
  clip: true

  Column {
    z: 30
    width: root.width - root.padding * 2
    spacing: 6
    FontFamilyControl {
      id: familyControl
      title: qsTr("字体名称")
      width: parent.width
    }
    FontNumberControl {
      id: sizeControl
      title: qsTr("字号")
      width: parent.width
      value: 18
      defaultValue: 18
      minValue: 0
      maxValue: 200
      stepValue: 1
      decimals: 1
    }
    QoolSwitchControl {
      id: styleControl
      title: qsTr("文字样式")
      tags: [qsTr("普通"), qsTr("粗体"), qsTr("斜体"), qsTr("粗斜体")]
      width: parent.width
      readonly property bool isBold: currentIndex == 1 || currentIndex == 3
      readonly property bool isItalic: currentIndex >= 2
    }
    QoolSwitchControl {
      id: alignmentControl
      title: qsTr("对齐方向")
      tags: [qsTr("左对齐"), qsTr("居中"), qsTr("右对齐")]
      width: parent.width
      defaultIndex: 1
      currentIndex: 1
      readonly property int value: currentIndex == 0 ? Text.AlignLeft : currentIndex
                                                       == 2 ? Text.AlignRight : Text.AlignHCenter
    }
    QoolColorControl {
      id: textColorControl
      title: qsTr("文本颜色")
      width: parent.width
    }
    FontNumberControl {
      id: leadControl
      title: qsTr("行间距")
      minValue: 0
      maxValue: 200
      defaultValue: 0
      value: 0
      decimals: 2
      stepValue: 1
      width: parent.width
    }
    FontNumberControl {
      id: trackControl
      title: qsTr("字间距")
      minValue: 0
      maxValue: 200
      defaultValue: 0
      value: 0
      decimals: 2
      stepValue: 1
      width: parent.width
    }
    FontNumberControl {
      id: outlineWidthControl
      title: qsTr("线框宽度")
      minValue: 0
      maxValue: 200
      decimals: 2
      stepValue: 1
      defaultValue: 1
      value: 1
      width: parent.width
    }
    FontNumberControl {
      id: outlineSoftControl
      title: qsTr("线框柔和度")
      minValue: 0
      maxValue: 100
      decimals: 0
      stepValue: 1
      defaultValue: 5
      value: 5
      width: parent.width
    }
    QoolColorControl {
      id: outlineColorControl
      title: qsTr("线框颜色")
      width: parent.width
    }
    FontNumberControl {
      id: aspectControl
      title: qsTr("长宽比")
      minValue: -1
      maxValue: 2
      decimals: 2
      stepValue: 0.1
      defaultValue: 1
      value: 1
      width: parent.width
    }
    FontNumberControl {
      id: opacityControl
      title: qsTr("不透明度")
      minValue: 0
      maxValue: 1
      decimals: 2
      stepValue: 0.01
      defaultValue: 1
      value: 1
      width: parent.width
    }
  } //column
}
