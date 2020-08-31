#ifndef PACIPROJ_COMMON_H
#define PACIPROJ_COMMON_H

#include "pacicore/fontinfo.h"

#include <QMap>
#include <QString>
#include <QStringList>

namespace paci::proj {

#define T const QString
T SEQ = "sequence";
T SEQNAME = "sequence_name";
T SEQNOTE = "note";
T TIMEBASE = "timebase";
T TB_RATE = "rate";
T TB_DF = "dropframe";
T TRUE = "YES";
T FALSE = "NO";
T FONT = "fontinfo";
T TRACKFONTSWITCH = "font_switch";
T TRACK_TAG = "tag";
T TRACKMANAGER = "track_manager";
T TRACK = "track";
T CLIP = "clip";
T STARTTIME = "start";
T DURATION = "duration";
T UNIMARK = "unique_mark";
T RED = "red";
T GREEN = "green";
T BLUE = "blue";
T ALPHA = "alpha";
T LEFT = "left";
T CENTER = "center";
T RIGHT = "right";
T ALIGNMENT = "alignment";
T ACTIVATED = "is_activated";
T PACIDTDFILENAME = "PACI.dtd";
T CONTENT = "content";
T EXTRA_DATA = "extra_data";
T EXTRA_DATA_NAME = "name";
T EXTRA_DATA_VALUE = "value";
#undef T

const QStringList FONT_KEYS_NUMBER = { "size", "lead", "track",
  "outlineWidth", "outlineSoft", "aspect", "opacity" };

const QStringList FONT_KEYS_BOOL = { "italic", "bold" };

const QStringList FONT_KEYS_COLOR = { "color", "outlineColor" };

const QMap<FontInfo::Alignment, QString> ALIGNMENT_VALUES = {
  { FontInfo::Left, LEFT }, { FontInfo::Center, CENTER },
  { FontInfo::Right, RIGHT }
};

const QMap<QString, FontInfo::Alignment> ALIGNMENT_KEYS = {
  { LEFT, FontInfo::Left }, { CENTER, FontInfo::Center },
  { RIGHT, FontInfo::Right }
};

QString bool_value_of(bool x);
bool bool_value_from(const QString& x);

QString dtdString();

} // namespace paci::proj

#endif // PACIPROJ_COMMON_H
