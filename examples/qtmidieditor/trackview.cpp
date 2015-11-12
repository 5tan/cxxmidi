#include "trackview.h"

#include <QMenu>

TrackView::TrackView(QWidget* parent_) : QTableView(parent_)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenu(const QPoint&)));
}

TrackView::~TrackView()
{
}

void TrackView::showContextMenu(const QPoint& pos_)
{
    QAction * action ;

    int pos = this->indexAt(pos_).row();

    QMenu myMenu;

    if (pos == -1)
    {
        action = myMenu.addAction(tr("Add event"));
        action->setData(0);
    }
    else
    {
        action = myMenu.addAction(tr("Delete event"));
        action->setData(1);
        action = myMenu.addAction(tr("Add event before"));
        action->setData(2);
        action = myMenu.addAction(tr("Add event after"));
        action->setData(3);
    }

    //! @FIXME menu should not be shown if there is no track selected

    QPoint globalPos = this->mapToGlobal(pos_);
    QAction* selectedItem = myMenu.exec(globalPos);

    if(selectedItem)
    {
        switch(selectedItem->data().toInt())
        {
        case 0: this->requestAddEvent(0); break;
        case 1: this->requestDeleteEvent(pos); break;
        case 2: this->requestAddEvent(pos); break;
        case 3: this->requestAddEvent(pos+1); break;
        default: break;

        }
    }
}
