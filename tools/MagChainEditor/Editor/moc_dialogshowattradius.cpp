/****************************************************************************
** Meta object code from reading C++ file 'dialogshowattradius.h'
**
** Created: Thu May 23 08:57:49 2019
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/dialogshowattradius.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogshowattradius.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DialogShowAttradius[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x08,
      54,   50,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DialogShowAttradius[] = {
    "DialogShowAttradius\0\0on_pushButtonClose_clicked()\0"
    "str\0slotShowAttradius(QString*)\0"
};

void DialogShowAttradius::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DialogShowAttradius *_t = static_cast<DialogShowAttradius *>(_o);
        switch (_id) {
        case 0: _t->on_pushButtonClose_clicked(); break;
        case 1: _t->slotShowAttradius((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DialogShowAttradius::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DialogShowAttradius::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DialogShowAttradius,
      qt_meta_data_DialogShowAttradius, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DialogShowAttradius::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DialogShowAttradius::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DialogShowAttradius::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialogShowAttradius))
        return static_cast<void*>(const_cast< DialogShowAttradius*>(this));
    return QDialog::qt_metacast(_clname);
}

int DialogShowAttradius::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
