/* *****************************************************************************
Copyright (c) 2018 Stan Chlebicki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
***************************************************************************** */

#include <QMenu>

#include "fileview.h"  // NOLINT(build/include_subdir) INCLUDE_NO_DIR

FileView::FileView(QWidget* parent) : QListView(parent) {
  setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, &QListView::customContextMenuRequested, this,
          &FileView::ShowContextMenu);
}

void FileView::ShowContextMenu(const QPoint& pos) {
  QAction* action;
  int p = indexAt(pos).row();

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

  QPoint global_pos = mapToGlobal(pos);
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
