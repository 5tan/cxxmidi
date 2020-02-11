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

#include "trackview.h"  // NOLINT(build/include_subdir) INCLUDE_NO_DIR

TrackView::TrackView(QWidget* parent) : QTableView(parent) {
  setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, &TrackView::customContextMenuRequested, this,
          &TrackView::ShowContextMenu);
}

void TrackView::ShowContextMenu(const QPoint& pos) {
  QAction* action;

  int p = indexAt(pos).row();

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

  QPoint global_pos = mapToGlobal(pos);
  QAction* selected_item = my_menu.exec(global_pos);

  if (selected_item) {
    switch (selected_item->data().toInt()) {
      case 0:
        RequestAddEvent(0);
        break;
      case 1:
        RequestDeleteEvent(p);
        break;
      case 2:
        RequestAddEvent(p);
        break;
      case 3:
        RequestAddEvent(p + 1);
        break;
      default:
        break;
    }
  }
}
