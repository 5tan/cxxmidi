#include "trackview.h"

#include <QMenu>

TrackView::TrackView(QWidget* parent) : QTableView(parent) {
  this->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, &TrackView::customContextMenuRequested,
          this, &TrackView::ShowContextMenu);
}

TrackView::~TrackView() {}

void TrackView::ShowContextMenu(const QPoint& pos) {
  QAction* action;

  int p = this->indexAt(pos).row();

  QMenu my_menu;

  if (p == -1) {
    action = my_menu.addAction(tr("Add event"));
    action->setData(0);
  } else {
    action = my_menu.addAction(tr("Delete event"));
    action->setData(1);
    action = my_menu.addAction(tr("Add event before"));
    action->setData(2);
    action = my_menu.addAction(tr("Add event after"));
    action->setData(3);
  }

  //! @FIXME menu should not be shown if there is no track selected

  QPoint global_pos = this->mapToGlobal(pos);
  QAction* selected_item = my_menu.exec(global_pos);

  if (selected_item) {
    switch (selected_item->data().toInt()) {
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
