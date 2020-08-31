import QtQuick 2.14
import QtQuick.Controls 2.14
import Qool.Controls 1.0
import Qool.Styles 1.0
import Qool.Components 1.0

QoolControl {
  id: root

  showTitle: true
  title: qsTr("实时预览")

  property string family
  property real size: 18
  property bool isBold
  property bool isItalic
  property int alignment
  property color textColor
  property real lead
  property real track
  property real outlineWidth
  property real outlineSoft
  property color outlineColor
  property real aspect
  property real textOpacity

  property string sampleText: qsTr("示例文字\nSample Text")

  FontMetrics {
    id: fontMetrics
    font.family: root.family
    font.pointSize: root.size
    font.bold: root.isBold
    font.italic: root.isItalic
    font.letterSpacing: root.track
    //    lineSpacing: root.lead
  }

  contentItem: Item {
    TextArea {
      id: sampleTextField
      text: qsTr("SampleText")
      anchors.centerIn: parent
      visible: false
      width: parent.width - 50
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      color: QoolStyle.textColor
      height: parent.height / 2
      background: CutCornerBox {
        id: sampleTextFieldBack
      }
      z: 90
    }
    QoolButton {
      text: qsTr("改完了")
      showTitle: false
      height: 25
      width: sampleTextField.width
      x: sampleTextField.x
      anchors.top: sampleTextField.bottom
      anchors.topMargin: 3
      onClicked: {
        root.sampleText = sampleTextField.text
        sampleTextField.visible = false
      }
      visible: sampleTextField.visible
      z: 90
      backBox.backColor: sampleTextFieldBack.backColor
    }

    Item {
      id: previewArea
      anchors.fill: parent
      clip: true
      Text {
        id: previewText
        text: root.sampleText
        font: fontMetrics.font
        lineHeightMode: Text.FixedHeight
        lineHeight: fontMetrics.font.pixelSize + root.lead
        anchors.centerIn: parent
        horizontalAlignment: root.alignment
        elide: Text.ElideNone
        wrapMode: Text.NoWrap
        color: root.textColor
        opacity: root.textOpacity
      }
    } //previewArea
  } //contentItem
}
