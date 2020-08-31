#include "paciproj_common.h"

namespace paci::proj {

QString bool_value_of(bool x)
{
  return x ? TRUE : FALSE;
}

bool bool_value_from(const QString &x)
{
  return x == TRUE;
}

QString dtdString()
{
  QStringList result;
  result << "<!ELEMENT" << SEQ << QString("(%1,%2,%3,%4,%5)>\n").arg(SEQNAME, SEQNOTE, TIMEBASE, FONT, TRACKMANAGER);
  result << "<!ELEMENT" << SEQNAME << "(#PCDATA)>\n";
  result << "<!ELEMENT" << SEQNOTE << "(#PCDATA)>\n";
  result << "<!ELEMENT" << TIMEBASE << "EMPTY>\n";
  result << "<!ELEMENT" << FONT << "ANY>\n";
  result << "<!ELEMENT" << TRACKMANAGER << QString("(%1*)>\n").arg(TRACK);
  result << "<!ELEMENT" << TRACK << QString("(%1,%2,%3*)>\n").arg(TRACKFONTSWITCH, FONT, CLIP);
  result << "<!ELEMENT" << CLIP << "(#PCDATA)>\n";

  result << "<!ATTLIST" << TIMEBASE << "\n  "
         << TB_RATE << "CDATA" << "#REQUIRED\n  "
         << TB_DF << QString("(%1|%2)").arg(TRUE, FALSE) << "#REQUIRED>\n";

  result << "<!ATTLIST" << TRACK << "\n  "
         << TRACK_TAG << "CDATA" << "#REQUIRED\n  "
         << ACTIVATED << QString("(%1|%2)").arg(TRUE, FALSE) << "#REQUIRED>\n";

  result << "<!ATTLIST" << CLIP << "\n  "
         << STARTTIME << "CDATA" << "#REQUIRED\n  "
         << DURATION << "CDATA" << "#REQUIRED\n  "
         << ACTIVATED << QString("(%1|%2)").arg(TRUE, FALSE) << "#REQUIRED\n  "
         << UNIMARK << "CDATA" << "#REQUIRED>\n";

  return result.join(" ");
}

}
