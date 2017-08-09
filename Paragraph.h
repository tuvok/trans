#ifndef PARAGRAPH_H_
#define PARAGRAPH_H_

#include <QString>

#include <vector>

class Paragraph
{
public:
    enum class Type {
        Undefined,
        Paragraph,
        Footnote,
        Section,
        SectionEnd,
        SubSubSection,
        SubSubSectionEnd
    };

    Paragraph();
    Paragraph(QString text_, Type t_);
    virtual ~Paragraph();

    bool parse(const QString& s);
    QString type2Tag(Type t);

    Type t = Type::Undefined;
    QString text;
    int number = -1;
    std::vector<Paragraph> footnotes;

private:
    void parseFootnotes();
};

#endif /* PARAGRAPH_H_ */
