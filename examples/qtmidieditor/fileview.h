#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QListView>
#include <QObject>

class FileView : public QListView {
  Q_OBJECT

 public:
  FileView(QWidget* parent_ = 0);
  virtual ~FileView();

 signals:
  void requestDeleteTrack(int num_);
  void requestAddTrack(int num_);

 public slots:
  void showContextMenu(const QPoint& pos_);
};

#endif /* FILEVIEW_H */
