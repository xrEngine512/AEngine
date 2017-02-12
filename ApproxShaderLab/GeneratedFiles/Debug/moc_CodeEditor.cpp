/****************************************************************************
** Meta object code from reading C++ file 'CodeEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CodeEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CodeEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ASL__CodeEditor_t {
    QByteArrayData data[5];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ASL__CodeEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ASL__CodeEditor_t qt_meta_stringdata_ASL__CodeEditor = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ASL::CodeEditor"
QT_MOC_LITERAL(1, 16, 7), // "focused"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 9), // "unFocused"
QT_MOC_LITERAL(4, 35, 24) // "On_cursorPositionChanged"

    },
    "ASL::CodeEditor\0focused\0\0unFocused\0"
    "On_cursorPositionChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ASL__CodeEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   31,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ASL::CodeEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CodeEditor *_t = static_cast<CodeEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->focused(); break;
        case 1: _t->unFocused(); break;
        case 2: _t->On_cursorPositionChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CodeEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CodeEditor::focused)) {
                *result = 0;
            }
        }
        {
            typedef void (CodeEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CodeEditor::unFocused)) {
                *result = 1;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ASL::CodeEditor::staticMetaObject = {
    { &ApproxGUI::AnimatedGraphicsWidget<QPlainTextEdit>::staticMetaObject, qt_meta_stringdata_ASL__CodeEditor.data,
      qt_meta_data_ASL__CodeEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ASL::CodeEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ASL::CodeEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ASL__CodeEditor.stringdata0))
        return static_cast<void*>(const_cast< CodeEditor*>(this));
    if (!strcmp(_clname, "ICodeEditor"))
        return static_cast< ICodeEditor*>(const_cast< CodeEditor*>(this));
    return ApproxGUI::AnimatedGraphicsWidget<QPlainTextEdit>::qt_metacast(_clname);
}

int ASL::CodeEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ApproxGUI::AnimatedGraphicsWidget<QPlainTextEdit>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ASL::CodeEditor::focused()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void ASL::CodeEditor::unFocused()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
