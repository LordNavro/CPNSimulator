#include "editortransitionitem.h"

EditorTransitionItem::EditorTransitionItem() :
    TransitionItem()
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

