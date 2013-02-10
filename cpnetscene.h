#ifndef CPNETSCENE_H
#define CPNETSCENE_H

#include <QGraphicsScene>

class CPNetScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CPNetScene(QObject *parent = 0);
    

    typedef enum{SELECT, PLACE, TRANSITION, ARC, DELETE} Tool;

    Tool currentTool;

signals:
    
public slots:
    
};

#endif // CPNETSCENE_H
