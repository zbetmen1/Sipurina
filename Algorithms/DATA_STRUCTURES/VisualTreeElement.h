#ifndef VISUALTREEELEMENT_H
#define VISUALTREEELEMENT_H
#include <QGraphicsWidget>
#include <vector>
#include <QGraphicsLineItem>
class VisualTreeElement : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(int grana READ grana WRITE setGrana)
public:
    VisualTreeElement();
    ~VisualTreeElement();
    VisualTreeElement(int v);
    VisualTreeElement(int v, VisualTreeElement* left, VisualTreeElement* right, VisualTreeElement* parrent);
    int getValue();
    void setValue(int v);
    void setColor(QColor c){m_color = c;}
    void setLeft(VisualTreeElement* left);
    void setRight(VisualTreeElement* right);
    void setParrent(VisualTreeElement* left);
    void setGrana(int whatever);
    int grana();
    VisualTreeElement* getParrent();
    VisualTreeElement* getLeft();
    VisualTreeElement* getRight();
    QGraphicsLineItem* getLine();
    void setLine(QGraphicsLineItem* l);
    void add_node(VisualTreeElement *t, int value, VisualTreeElement *novi);
    void write_tree(VisualTreeElement* t);

    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    QRectF boundingRect();

    inline void addElements(const std::vector<int> v) {
        if (v.empty())
            return;
        this->setValue(v[0]);
        for (auto i=v.cbegin()+1; i != v.cend(); i++){
            add_node(this, *i, new VisualTreeElement(*i, 0, 0, 0));
        }
    }

    void delete_node(VisualTreeElement* t, int value);
    VisualTreeElement* nadji_minimum(VisualTreeElement* e, int min);
private:
    int m_value;
    VisualTreeElement* m_parrent;
    VisualTreeElement* m_left;
    VisualTreeElement* m_right;
    QGraphicsLineItem* m_line;
    QColor m_color;
};

#endif // VISUALTREEELEMENT_H
