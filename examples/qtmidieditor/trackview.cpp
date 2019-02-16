#include "trackview.h"

#include <QMenu>

TrackView::TrackView(QWidget* parent) : QTableView(parent) {
  this->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this,
          SLOT(ShowContextMenu(const QPoint&)));
}

TrackView::~TrackView() {}

void TrackView::ShowContextMenu(const QPoint& pos) {
  QAction* action;

  int p = this->indexAt(pos).row();

  QMenu myMenu;

  if (p == -1) {
    action = myMenu.addAction(tr("Add event"));
    action->setData(0);
  } else {
    action = myMenu.addAction(tr("Delete event"));
    action->setData(1);
    action = myMenu.addAction(tr("Add event before"));
    action->setData(2);
    action = myMenu.addAction(tr("Add event after"));
    action->setData(3);
  }

  //! @FIXME menu should not be shown if there is no track selected

  QPoint globalPos = this->mapToGlobal(pos);
  QAction* selectedItem = myMenu.exec(globalPos);

  if (selectedItem) {
    switch (selectedItem->data().toInt()) {
      case 0:
        this->RequestAddEvent(0);
        break;
      case 1:
        this->RequestDeleteEvent(p);
        break;
      case 2:
        this->RequestAddEvent(p);
        break;
      case 3:
        this->RequestAddEvent(p + 1);
        break;
      default:
        break;
    }
  }
}
