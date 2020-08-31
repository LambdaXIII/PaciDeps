import QtQuick 2.14
import QtQuick.Controls 2.14
import Qool.Controls 1.0
import Qool.Styles 1.0
import Qool.Components 1.0

QoolAbstractButton {
  id: control

  property int index
  property string family

  signal familySelected(string f)

  checkable: true

  topPadding: backBox.contentTopSpace
  leftPadding: backBox.strokeWidth
  bottomPadding: backBox.strokeWidth
  rightPadding: backBox.strokeWidth

  FontMetrics {
    id: metrics
    font.family: control.enabled ? family : control.font
    font.pixelSize: control.font.pixelSize * 2
  }

  contentItem: Text {
    id: mainText
    text: family
    font: metrics.font
    fontSizeMode: Text.Fit
    clip: true
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    color: checked ? QoolStyle.highlightColor : control.enabled ? QoolStyle.textColor : QoolStyle.negativeColor
  }

  clip: true

  onClicked: {
    checked = true
    familySelected(family)
  }

  ControlLockedCover{}
}
