#ifndef TRACKVIEW_H
#define TRACKVIEW_H

#include <QObject>
#include <QTableView>

class TrackView : public QTableView {
  Q_OBJECT
 public:
  TrackView(QWidget* parent_ = 0);
  virtual ~TrackView();

 signals:
  void RequestAddEvent(int num_);
  void RequestDeleteEvent(int num_);

 public slots:
  void ShowContextMenu(const QPoint& pos_);
};

#endif  // TRACKVIEW_H
