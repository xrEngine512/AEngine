/****************************************************************************
** Meta object code from reading C++ file 'ApproxGUIButton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ApproxGUIButton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ApproxGUIButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ApproxGUIButton_t {
    QByteArrayData data[4];
    char stringdata[38];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ApproxGUIButton_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ApproxGUIButton_t qt_meta_stringdata_ApproxGUIButton = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ApproxGUIButton"
QT_MOC_LITERAL(1, 16, 7), // "dotSize"
QT_MOC_LITERAL(2, 24, 5), // "shift"
QT_MOC_LITERAL(3, 30, 7) // "opacity"

    },
    "ApproxGUIButton\0dotSize\0shift\0opacity"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ApproxGUIButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       3,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Double, 0x00095003,
       2, QMetaType::Double, 0x00095003,
       3, QMetaType::Double, 0x00095003,

       0        // eod
};

void ApproxGUIButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ApproxGUIButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_ApproxGUIButton.data,
      qt_meta_data_ApproxGUIButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ApproxGUIButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApproxGUIButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ApproxGUIButton.stringdata))
        return static_cast<void*>(const_cast< ApproxGUIButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int ApproxGUIButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = GetDotSize(); break;
        case 1: *reinterpret_cast< double*>(_v) = GetShift(); break;
        case 2: *reinterpret_cast< double*>(_v) = GetOpacity(); break;
        default: break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: SetDotSize(*reinterpret_cast< double*>(_v)); break;
        case 1: SetShift(*reinterpret_cast< double*>(_v)); break;
        case 2: SetOpacity(*reinterpret_cast< double*>(_v)); break;
        default: break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
