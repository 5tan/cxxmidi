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

  QMenu myMenu;

  if (p == -1) {
    action = myMenu.addAction(tr("Add track"));
    action->setData(0);
  } else {
    action = myMenu.addAction(tr("Add track before"));
    action->setData(1);
    action = myMenu.addAction(tr("Add track after"));
    action->setData(2);
    action = myMenu.addAction(tr("Delete track"));
    action->setData(3);
  }

  QPoint globalPos = this->mapToGlobal(pos);
  QAction* selectedItem = myMenu.exec(globalPos);

  if (selectedItem) {
    switch (selectedItem->data().toInt()) {
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
