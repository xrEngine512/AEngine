/****************************************************************************
** Meta object code from reading C++ file 'ApproxGraphicsView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ApproxGraphicsView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ApproxGraphicsView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ApproxGUI__ApproxGraphicsView_t {
    QByteArrayData data[8];
    char stringdata[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ApproxGUI__ApproxGraphicsView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ApproxGUI__ApproxGraphicsView_t qt_meta_stringdata_ApproxGUI__ApproxGraphicsView = {
    {
QT_MOC_LITERAL(0, 0, 29), // "ApproxGUI::ApproxGraphicsView"
QT_MOC_LITERAL(1, 30, 16), // "createShaderElem"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 16), // "ASL::Shader_Type"
QT_MOC_LITERAL(4, 65, 10), // "sceneMoved"
QT_MOC_LITERAL(5, 76, 11), // "viewChanged"
QT_MOC_LITERAL(6, 88, 13), // "onContextMenu"
QT_MOC_LITERAL(7, 102, 8) // "QAction*"

    },
    "ApproxGUI::ApproxGraphicsView\0"
    "createShaderElem\0\0ASL::Shader_Type\0"
    "sceneMoved\0viewChanged\0onContextMenu\0"
    "QAction*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ApproxGUI__ApproxGraphicsView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    0,   37,    2, 0x06 /* Public */,
       5,    0,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   39,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    2,

       0        // eod
};

void ApproxGUI::ApproxGraphicsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ApproxGraphicsView *_t = static_cast<ApproxGraphicsView *>(_o);
        switch (_id) {
        case 0: _t->createShaderElem((*reinterpret_cast< ASL::Shader_Type(*)>(_a[1]))); break;
        case 1: _t->sceneMoved(); break;
        case 2: _t->viewChanged(); break;
        case 3: _t->onContextMenu((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ApproxGraphicsView::*_t)(ASL::Shader_Type );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ApproxGraphicsView::createShaderElem)) {
                *result = 0;
            }
        }
        {
            typedef void (ApproxGraphicsView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ApproxGraphicsView::sceneMoved)) {
                *result = 1;
            }
        }
        {
            typedef void (ApproxGraphicsView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ApproxGraphicsView::viewChanged)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject ApproxGUI::ApproxGraphicsView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_ApproxGUI__ApproxGraphicsView.data,
      qt_meta_data_ApproxGUI__ApproxGraphicsView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ApproxGUI::ApproxGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApproxGUI::ApproxGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ApproxGUI__ApproxGraphicsView.stringdata))
        return static_cast<void*>(const_cast< ApproxGraphicsView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int ApproxGUI::ApproxGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ApproxGUI::ApproxGraphicsView::createShaderElem(ASL::Shader_Type _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ApproxGUI::ApproxGraphicsView::sceneMoved()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void ApproxGUI::ApproxGraphicsView::viewChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
