#include "widget.h"
#include <QQtApplication>
#include "qqtdictionary.h"

int main ( int argc, char* argv[] )
{
    QQtApplication a ( argc, argv );

    QString tt;
    tt = QString ( "%1" ).sprintf ( "%4d", 13 );//"  13"
    tt = QString ( "%1" ).arg ( 13, 4, 10 );
    pline() << tt;

    /*
    QQtWidgetFactory::registerObject<QPushButton>();
    QObject* obj = QQtWidgetFactory::createObject ( "QPushButton" );
    pline() << obj->inherits ( "QWidget" );
    pline() << obj->inherits ( "QPushButton" );
    pline() << obj->inherits ( "QObject" );
    obj->setObjectName ( "bbb" );
    pline() << obj->objectName();
    QQtObjectManager::registerObject ( obj );
    */

    //对QQtDictionary的使用进行测试。
    QQtDictionary d0;
    d0["cc"] = "dd";
    pline() << d0["cc"].getValue();

    QQtDictionary d1;
    QQtDictionary d2 ( QVariant ( "CCCC" ) );
    d1.addChild ( d2 );
    pline() << d1[0].getValue();
    d1[0] = "ff";
    pline() << d1[0].getValue();

    QQtDictionary d3;
    d3["cc"]["dd"] = "ee";
    pline() << d3["cc"]["dd"].getValue().toString();

    QQtDictionary d4;

    for ( int i = 0; i < 5; i++ )
    {
        QQtDictionary d ( QVariant ( QString::number ( i ) ) );
        d4.addChild ( d );
    }

    pline() << d4.count();

    for ( int i = 0; i < d4.count(); i++ )
    {
        pline() << d4[i].getValue().toString();
    }

    QQtDictionary d5;

    /*后续有map操作，这一步就没有用了*/
    d5.addChild ( "5.7" );
    d5.addChild ( "5.8" );
    d5.addChild ( "5.9" );

    /*后续有map操作，这一步就没有用了*/
    d5["5.7"].addChild ( "5.7.2" );
    d5["5.7"].addChild ( "5.7.3" );
    d5["5.7"].addChild ( "5.7.4" );

    d5["5.7"]["5.7.4"].addChild ( "xxx.dmg" );
    d5["5.7"]["5.7.4"][0].addChild ( "xxx.dmg" );
    d5["5.7"]["5.7.4"][0].addChild ( "2017-12-12" );
    d5["5.7"]["5.7.4"][0].addChild ( "1.2G" );
    d5["5.7"]["5.7.4"][0].addChild ( "Detail" );
    d5["5.7"]["5.7.4"][0].insertChild ( 4, "Detail2" );

    //pline() << "\n" << d5;
    pline() << "\n" << d5["5.7"];

    QQtDictionaryMapIterator itor ( d5["5.7"].getMap() );

    while ( itor.hasNext() )
    {
        itor.next();
        pline() << "\n" << itor.key() << itor.value();
    }

    pline () << d5["5.7"]["5.7.4"][0].getType();
    pline () << d5["5.7"]["5.7.4"][0].count();

    for ( int i = 0; i < d5["5.7"]["5.7.4"][0].count(); i++ )
    {
        pline() << d5["5.7"]["5.7.4"][0][i].getValue().toString();
    }
    pline() << d5["5.7"]["5.7.4"][0];

    QQtDictionary node = d5;
    pline() << node["5.7"]["5.7.4"][0][0].getValue().toString();

    /*在这里有list操作，前边"5.7"的map就没有用了*/
    /*但是再过去做过的list类型的操作都会被保留，也就是说中间出现过概念错误，没问题还保留着*/
    d5["5.7"].addChild ( "5.7.5" );

    for ( int i = 0; i < d5["5.7"].count(); i++ )
    {
        pline() << d5["5.7"][i].getValue().toString();
    }
    pline() << d5["5.7"];

    char* cc = NULL;
    char* bb = "ddddd";

    QString ccq = QString ( "%1%2" ).arg ( cc ).arg ( bb );
    pline() << ccq;

    //Widget w;
    //w.show();

    //QQtDictionary addChild函数存在一个问题，在这里测试是否解决。
    //内部实现使用list的append函数，会引用外部的类实例，这导致在free空间的时候，存在很多莫名其妙的二次free，尤其外部实例和QObject关联的时候。
    //现在内部实现使用push_back，这里检测QQtDictionary还有没有二次free这个bug。
    QQtDictionary* n0 = new QQtDictionary ( QVariant ( "This is a value" ) );
    //这里打印一下n0的值和地址
    pline() << n0->getValue().value<QString>();
    pline() << hex << n0->getValue().constData();

    QQtDictionary t0;
    //这里addChild，t0变成一个list。
    t0.addChild ( *n0 );

    //这里更改外部实例的值，显然外部实例的值已经改变了。
    n0->getValue().setValue<QString> ( "This is a changed value" );

    //为了证实这一点，这里打印值
    pline() << "外部空间的值" << n0->getValue().value<QString>();
    //这里可能还有一个疑惑，新的值是不是占据了新的空间？其实，没有。这里打印地址证实。
    pline() << "外部空间的地址" << hex << n0->getValue().constData();

    //这里删除外部实例n0，不用把n0置零，不必要，n0和申请的存储空间是两回事。
    delete n0;
    //但是，置零，规范一点。
    n0 = NULL;

    //外部的空间的值被改变了，并且删除了，内部空间的值，发生了什么？
    //现在检查，内部的值是多少？
    pline() << "检测内部空间的值：" << t0[0].getValue().value<QString>();
    pline() << "检测内部空间的地址：" << hex << t0[0].getValue().constData() ;

    //windows
    //经过测试，QQtDictionary内部空间成功和外部空间分离，bug清除。

    //测试QQtDictionary的打印能力
    QQtDictionary v00;
    pline() << v00;

    QQtDictionary v0 ( "This is a value" );
    pline() << v0;

    QQtDictionary v1;
    v1.addChild ( "CCCC" );
    v1.addChild ( "DDDD" );
    v1[0].addChild ( "2-CCCC" );
    pline() << v1;
    pline() << v1[0];

    QQtDictionary v2;
    v2["AAAA"] = "NNNN";
    v2["BBBB"] = "MMMM";
    pline() << v2;
    return 0;//a.exec();
}
