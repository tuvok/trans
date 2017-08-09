#include <Paragraph.h>

#include <QRegExp>
#include <QStringList>

#include <iostream>
#include <set>

Paragraph::Paragraph()
{

}

Paragraph::Paragraph(QString text_, Type t_, int number_) : text(text_), t(t_), number(number_)
{
}

Paragraph::~Paragraph()
{

}

void Paragraph::parseFootnotes()
{
    // Parse footnotes
    QRegExp fx("\\\\footnote\\{([^\\}]*)\\}");
    int index = -1;
    int start = 0;
    while ((index = fx.indexIn(text.mid(start))) != -1)
    {
//        std::cout << "---------------------paragraph------\n";
        QStringList foots = fx.capturedTexts();
        for (auto& f : foots)
        {
            if (f.startsWith("\\"))
            {
                QString ftn = QString("\\footnote{%1}").arg(footnotes.size());
                text.replace(f, ftn);
                start += index + ftn.size();
            }
            else
            {
                footnotes.push_back(Paragraph(f, Type::Footnote, footnotes.size()));
            }
//            std::cout << "FNT: " << f.toStdString() << "\n\n";
        }
    }
}

bool Paragraph::parse(const QString& s)
{
    if (s.startsWith("\\paragraph"))
    {
        t = Type::Paragraph;

        QRegExp rx("\\\\paragraph\\{(\\d+)\\.\\}");

        if (rx.indexIn(s) == -1)
        {
            std::cout << "Paragraph::parse: paragraph parse failed for: "
                    << s.toStdString() << std::endl;
            return false;
        }

        QStringList list = rx.capturedTexts();

        bool ok = false;
        number = list[1].toInt(&ok);

        if (!ok)
        {
            std::cout << "Paragraph::parse: int parse failed for: "
                    << s.toStdString() << std::endl;
            number = -1;

            return false;
        }

        text = s;
        text.remove(list[0]);

        // Parse footnotes

        parseFootnotes();

        return true;
    }

    static const std::set<Type> sections = { Type::Section, Type::SectionEnd,
            Type::SubSubSection, Type::SubSubSectionEnd };

    for (auto sec: sections)
    {
        if (!s.startsWith(type2Tag(sec)))
            continue;

        t = sec;

        QRegExp rx("\\" + type2Tag(sec) + "\\{(.+)\\}");

        if (rx.indexIn(s) == -1)
        {
            std::cout << "Paragraph::parse: paragraph parse failed for: "
                    << s.toStdString() << std::endl;
            return false;
        }

        QStringList list = rx.capturedTexts();

        text = list[1];

        parseFootnotes();

        return true;
    }

    std::cout << "Paragraph::parse: paragraph parse failed: "
            << s.toStdString() << std::endl;

    return false;
}


QString Paragraph::type2Tag(Type t)
{
    switch (t)
    {
    case Type::Section : return "\\section";
    case Type::SectionEnd : return "\\xsectionEnd";
    case Type::SubSubSection : return "\\subsubsection";
    case Type::SubSubSectionEnd : return "\\xsubsubsectionEnd";
    }

    return QString();
}
