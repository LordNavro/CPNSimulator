#include "statespaceexplorer.h"

StateSpaceExplorer::StateSpaceExplorer(StateSpaceGraph graph, QWidget *parent) :
    QDialog(parent), graph(graph)
{
    setModal(true);
    setGeometry(parent->geometry().adjusted(20,20,-20,-20));
    QGridLayout *layout = new QGridLayout(this);
    listWidget = new QListWidget(this);
    int i=0;
    foreach(StateSpaceVertex vertex, graph.vertices)
    {
        i++;
        QListWidgetItem *item = new QListWidgetItem(tr("(State %1) %2").arg(QString::number(i), vertex.toString()), listWidget);
        Q_UNUSED(item);
    }
    textEdit = new QTextEdit(this);
    textEdit->setTextInteractionFlags(Qt::NoTextInteraction);

    setWindowTitle(tr("State space analysis"));

    connect(listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(slotItemSelected(int)));
    layout->addWidget(listWidget, 1, 0);
    layout->addWidget(textEdit, 1, 1);
    if(graph.depthLimitReached)
    {
        QLabel *label = new QLabel("Warning: Depth limit reached during generation of state space. The state space is incomplete and bounded to the given limit of steps.");
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label, 2,0,1,2);
    }
    setLayout(layout);
}

void StateSpaceExplorer::slotItemSelected(int i)
{
    QList<int> path = graph.findPath(i);
    QString text = tr("Path from initial state to this state: \n");
    for(int i = 0; i < path.size(); i++)
    {
        if(i%2)
        {
            text += graph.vertices.at(path[i-1]).edges.at(path[i]).toString() + "\n";
        }
        else
        {
            text += tr("State %1: %2\n").arg(QString::number(path[i] + 1), graph.vertices.at(path[i]).toString());
        }
    }
    textEdit->setText(text);
}
