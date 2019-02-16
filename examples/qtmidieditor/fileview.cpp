#include "fileview.h"

#include <QMenu>

FileView::FileView(QWidget* parent) : QListView(parent) {
  this->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this,
          SLOT(ShowContextMenu(const QPoint&)));
}

FileView::~FileView() {}

void FileView::ShowContextMenu(const QPoint& pos) {
  QAction* action;
  int p = this->indexAt(pos).row();

  QMenu my_menu;

  if (p == -1) {
    action = my_menu.addAction(tr("Add track"));
    action->setData(0);
  } else {
    action = my_menu.addAction(tr("Add track before"));
    action->setData(1);
    action = my_menu.addAction(tr("Add track after"));
    action->setData(2);
    action = my_menu.addAction(tr("Delete track"));
    action->setData(3);
  }

  QPoint global_pos = this->mapToGlobal(pos);
  QAction* selected_item = my_menu.exec(global_pos);

  if (selected_item) {
    switch (selected_item->data().toInt()) {
      case 0:
        RequestAddTrack(0);
        break;
      case 1:
        RequestAddTrack(p);
        break;
      case 2:
        RequestAddTrack(p + 1);
        break;
      case 3:
        RequestDeleteTrack(p);
        break;
      default:
        break;
    }
  }
}
