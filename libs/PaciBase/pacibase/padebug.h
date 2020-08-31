#ifndef PADEBUG_H
#define PADEBUG_H

#ifndef QT_NO_DEBUG
  #include <QDebug>
  #include <QString>
#endif

#ifndef QT_NO_DEBUG
  #define PA_INFO(info)                                                \
    qDebug()                                                           \
      << QString("[%1]%2").arg(metaObject()->className()).arg(info);
  #define PA_DEBUG qDebug() << "[" << metaObject()->className() << "]"
#else
  #define PA_INFO(info)
  #define PA_DEBUG
#endif

#endif // PADEBUG_H
