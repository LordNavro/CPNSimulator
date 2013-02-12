#include "cpnetform.h"

CPNetForm::CPNetForm(QWidget *parent) :
    QWidget(parent)
{
    lineName = new QLineEdit(this);
    connect(lineName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));
    inscriptionDeclaration = new InscriptionEdit(this);
    connect(inscriptionDeclaration, SIGNAL(textChanged()), this, SLOT(declarationChanged()));

    layout = new QFormLayout(this);
    layout->addRow(tr("Net &name"), lineName);
    layout->addRow(tr("Net &declarations"), inscriptionDeclaration);
    this->setLayout(layout);
}

void CPNetForm::setNet(CPNet *net)
{
    this->net = net;
    lineName->setText(net->name);
    inscriptionDeclaration->setText(net->declaration);
}

void CPNetForm::nameChanged(QString name)
{
    net->name = name;
}

void CPNetForm::declarationChanged()
{
    net->declaration = inscriptionDeclaration->document()->toPlainText();
}
