#include <DbAbstraction.h>
#include <Importer.h>

#include <QApplication>
#include <QObject>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QTextEdit>

#include <memory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Trans v1");

    std::shared_ptr<QQuickView> view(new QQuickView);

    qmlRegisterType<QTextEdit>("org.qt.qtextedit", 1, 0, "QTextEdit");

    DbAbstraction dba;

    Importer imp(dba);
    view->rootContext()->setContextProperty("Importer", &imp);

    view->setSource(QUrl::fromLocalFile("../trans/ui/MainWindow.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);

    view->show();


    QObject* impFD = view->rootObject()->findChild<QObject*>("importFileDialog");
    if (impFD)
        QObject::connect(impFD, SIGNAL(accept(QUrl, QObject*)), &imp, SLOT(importTex(QUrl, QObject*)));


    QObject* tip = view->rootObject()->findChild<QObject*>("texImportPreview");
    if (tip)
    {
        QObject::connect(tip, SIGNAL(accept(int, QString)), &imp, SLOT(importAccepted(int, QString)));
        QObject::connect(tip, SIGNAL(decline(int)), &imp, SLOT(importDeclined(int)));
        QObject::connect(tip, SIGNAL(provideCollections(int, QString)), &imp, SLOT(provideCollections(int, QString)));
    }

    

    return app.exec();
}
