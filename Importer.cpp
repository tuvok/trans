#include <Importer.h>

#include <Paragraph.h>

#include <QFile>
#include <QQuickTextDocument>
#include <QString>
#include <QStringList>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextStream>
#include <QUrl>

#include <iostream>
#include <vector>

Importer::Importer(IDbAbstraction& idb_) : idb(idb_)
{

}

Importer::~Importer()
{

}

void Importer::importTex(QUrl path, QObject* doc)
{
    QFile file(path.toLocalFile());

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cout << "Importer::importTex: failed to open file: " << path.toLocalFile().toStdString() << std::endl;
    }

    QTextStream in(&file);

    QString text = in.readAll();
    QStringList lines = text.split("\n");

    std::vector<Paragraph> parg;

    int count = -1;
    for (const auto& l: lines)
    {
        count++;

        if (l.isEmpty())
            continue;

        if (!l.startsWith("\\"))
        {
            std::cout << "Importer::importTex: failed to import file,"
                    " some lines don't start with '\\'\n";

            return;
        }

        Paragraph p;
        if (p.parse(l))
            parg.push_back(p);
        else
            return;
    }

    auto docu = dynamic_cast<QQuickTextDocument*>(doc);
    if (!docu)
    {
        std::cout << "Importer::importTex: failed to import file, object provided "
                "is not a QQuickTextDocument\n";
        return;
    }
    prepareDocument(parg, docu->textDocument());

    int pargCount;
    {
        std::unique_lock<std::mutex> lock(inProgress_mtx);
        inProgress.insert({counter, parg});
        pargCount = counter;
        counter++;
    }

    QStringList collections = idb.getCollections();

    emit setCollectionCombos(0, collections);
    emit showTexImportPreview(pargCount);
}

void Importer::importAccepted(int id, QString collection)
{
    std::vector<Paragraph> par;
    {
        std::unique_lock<std::mutex> lock(inProgress_mtx);
        par = inProgress.at(id);
    }

    try
    {
        idb.updateCollections(collection);
        prepareAndSave(par, collection);
    }
    catch (std::exception& e)
    {
        std::cout << "Importer::prepareAndSave failed: " << e.what() << std::endl;
    }

    {
        std::unique_lock<std::mutex> lock(inProgress_mtx);
        inProgress.erase(id);
    }
}

void Importer::importDeclined(int id)
{
    std::unique_lock<std::mutex> lock(inProgress_mtx);
    inProgress.erase(id);
}

void Importer::insertParagraphNumber(int number, QTextCursor& cur)
{
    cur.beginEditBlock();
    auto format = cur.charFormat();
    auto oldWeight = format.fontWeight();

    format.setFontWeight(QFont::Bold);
    cur.setCharFormat(format);

    cur.insertText(QString::number(number) + ". ");

    format.setFontWeight(oldWeight);
    cur.setCharFormat(format);
    cur.endEditBlock();
}

void Importer::insertParagraphBody(const QString& text, QTextCursor& cur)
{
    cur.beginEditBlock();
    cur.insertText(text);
    cur.insertText("\n\n");
    cur.endEditBlock();
}

void Importer::insertFootnotes(const Paragraph& p, QTextCursor& cur)
{
    for (const auto& f : p.footnotes)
    {
        insertParagraphNumber(f.number, cur);
        insertParagraphBody(f.text, cur);
    }
}

QTextDocument* Importer::prepareDocument(const std::vector<Paragraph>& par, QTextDocument* doc)
{
    doc->clear();
    QTextCursor cur(doc);

    for (const auto& p: par)
    {
        if (p.t == Paragraph::Type::Paragraph)
        {
            insertParagraphNumber(p.number, cur);
            insertParagraphBody(p.text, cur);

            insertFootnotes(p, cur);
        }
        else
        {
            cur.beginEditBlock();

            auto format = cur.charFormat();
            auto oldWeight = format.fontWeight();
            double oldSize = 9;//format.fontPointSize();

            format.setFontWeight(QFont::Bold);
            if (p.t == Paragraph::Type::Section || p.t == Paragraph::Type::SectionEnd)
                format.setFontPointSize(oldSize + 5);
            if (p.t == Paragraph::Type::SubSubSection || p.t == Paragraph::Type::SubSubSectionEnd)
                format.setFontPointSize(oldSize + 3);
            if (p.t == Paragraph::Type::SectionEnd || p.t == Paragraph::Type::SubSubSectionEnd)
                format.setFontItalic(true);

            cur.setCharFormat(format);
            cur.insertText(p.text);
            cur.insertText("\n\n");

            format.setFontWeight(oldWeight);
            format.setFontPointSize(oldSize);
            format.setFontItalic(false);

            cur.setCharFormat(format);
            cur.endEditBlock();

            insertFootnotes(p, cur);
        }

    }

    return doc;
}

void Importer::provideCollections(int level, QString parent)
{
    QStringList collections = idb.getCollections(level, parent);

    emit setCollectionCombos(level, collections);
}

void Importer::prepareAndSave(const std::vector<Paragraph>& par, QString collection)
{
    for (const auto& p: par)
    {
        if (p.t == Paragraph::Type::Paragraph)
        {
            p.text.split('.');
        }
    }
}
