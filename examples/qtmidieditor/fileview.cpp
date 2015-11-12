#include "fileview.h"

#include <QMenu>

FileView::FileView(QWidget* parent_) : QListView(parent_)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenu(const QPoint&)));
}

FileView::~FileView() {}

void FileView::showContextMenu(const QPoint& pos_)
{
    QAction * action;
    int pos = this->indexAt(pos_).row();

    QMenu myMenu;

    if (pos == -1)
    {
        action = myMenu.addAction(tr("Add track"));
        action->setData(0);
    }
    else
    {
        action = myMenu.addAction(tr("Add track before"));
        action->setData(1);
        action = myMenu.addAction(tr("Add track after"));
        action->setData(2);
        action = myMenu.addAction(tr("Delete track"));
        action->setData(3);
    }

    QPoint globalPos = this->mapToGlobal(pos_);
    QAction* selectedItem = myMenu.exec(globalPos);

    if(selectedItem)
    {
        switch(selectedItem->data().toInt())
        {
        case 0: requestAddTrack(0); break;
        case 1: requestAddTrack(pos); break;
        case 2: requestAddTrack(pos+1); break;
        case 3: requestDeleteTrack(pos); break;
        default: break;
        }
    }
}
