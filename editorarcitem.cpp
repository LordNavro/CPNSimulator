#include "editorarcitem.h"
#define PI 3.141592653589
#define SEL_WIDTH 5

EditorArcItem::EditorArcItem(QGraphicsItem *from, QGraphicsItem *to, QGraphicsItem *parent, QGraphicsScene *scene)
    : ArcItem(from, to, parent, scene)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}
