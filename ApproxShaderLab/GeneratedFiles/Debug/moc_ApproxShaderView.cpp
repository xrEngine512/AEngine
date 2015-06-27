/****************************************************************************
** Meta object code from reading C++ file 'ApproxShaderView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ApproxShaderView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ApproxShaderView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ApproxShaderView_t {
    QByteArrayData data[15];
    char stringdata[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ApproxShaderView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ApproxShaderView_t qt_meta_stringdata_ApproxShaderView = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ApproxShaderView"
QT_MOC_LITERAL(1, 17, 16), // "CreateShaderElem"
QT_MOC_LITERAL(2, 34, 20), // "ShaderLabGUIElement*"
QT_MOC_LITERAL(3, 55, 0), // ""
QT_MOC_LITERAL(4, 56, 16), // "ASL::Shader_Type"
QT_MOC_LITERAL(5, 73, 10), // "on_Compile"
QT_MOC_LITERAL(6, 84, 11), // "on_Settings"
QT_MOC_LITERAL(7, 96, 13), // "on_SaveShader"
QT_MOC_LITERAL(8, 110, 14), // "on_SaveProject"
QT_MOC_LITERAL(9, 125, 7), // "on_Load"
QT_MOC_LITERAL(10, 133, 8), // "on_Frame"
QT_MOC_LITERAL(11, 142, 11), // "UpdateLinks"
QT_MOC_LITERAL(12, 154, 13), // "setShaderName"
QT_MOC_LITERAL(13, 168, 6), // "FadeIn"
QT_MOC_LITERAL(14, 175, 7) // "FadeOut"

    },
    "ApproxShaderView\0CreateShaderElem\0"
    "ShaderLabGUIElement*\0\0ASL::Shader_Type\0"
    "on_Compile\0on_Settings\0on_SaveShader\0"
    "on_SaveProject\0on_Load\0on_Frame\0"
    "UpdateLinks\0setShaderName\0FadeIn\0"
    "FadeOut"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ApproxShaderView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    3, 0x08 /* Private */,
       5,    0,   72,    3, 0x08 /* Private */,
       6,    0,   73,    3, 0x08 /* Private */,
       7,    0,   74,    3, 0x08 /* Private */,
       8,    0,   75,    3, 0x08 /* Private */,
       9,    0,   76,    3, 0x08 /* Private */,
      10,    0,   77,    3, 0x08 /* Private */,
      11,    0,   78,    3, 0x08 /* Private */,
      12,    1,   79,    3, 0x0a /* Public */,
      13,    0,   82,    3, 0x0a /* Public */,
      14,    0,   83,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2, 0x80000000 | 4,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ApproxShaderView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ApproxShaderView *_t = static_cast<ApproxShaderView *>(_o);
        switch (_id) {
        case 0: { ShaderLabGUIElement* _r = _t->CreateShaderElem((*reinterpret_cast< ASL::Shader_Type(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< ShaderLabGUIElement**>(_a[0]) = _r; }  break;
        case 1: _t->on_Compile(); break;
        case 2: _t->on_Settings(); break;
        case 3: _t->on_SaveShader(); break;
        case 4: _t->on_SaveProject(); break;
        case 5: _t->on_Load(); break;
        case 6: _t->on_Frame(); break;
        case 7: _t->UpdateLinks(); break;
        case 8: _t->setShaderName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->FadeIn(); break;
        case 10: _t->FadeOut(); break;
        default: ;
        }
    }
}

const QMetaObject ApproxShaderView::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ApproxShaderView.data,
      qt_meta_data_ApproxShaderView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ApproxShaderView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApproxShaderView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ApproxShaderView.stringdata))
        return static_cast<void*>(const_cast< ApproxShaderView*>(this));
    return QFrame::qt_metacast(_clname);
}

int ApproxShaderView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
