import QtQuick 2.14

Item {
  property alias optionPanel: panel

  QoolFontControlPane {
    id: panel
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: 200
  }

  QoolFontPreviewer {
    id: previewer
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: panel.left
    anchors.rightMargin: 10

    family: panel.family
    size: panel.size
    isBold: panel.isBold
    isItalic: panel.isItalic
    alignment: panel.alignmentValue
    textColor: panel.textColor
    lead: panel.lead
    track: panel.track
    outlineWidth: panel.outlineWidth
    outlineSoft: panel.outlineSoft
    outlineColor: panel.outlineColor
    aspect: panel.aspect
    textOpacity: panel.textOpacity
  }
}
