#ifndef TRACKVIEW_H
#define TRACKVIEW_H

#include <QObject>
#include <QTableView>

class TrackView : public QTableView {
  Q_OBJECT
 public:
  TrackView(QWidget* parent = 0);
  virtual ~TrackView();

 signals:
  void RequestAddEvent(int num);
  void RequestDeleteEvent(int num);

 public slots:
  void ShowContextMenu(const QPoint& pos);
};

#endif  // TRACKVIEW_H
