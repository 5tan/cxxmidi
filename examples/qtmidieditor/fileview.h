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
  void RequestDeleteTrack(int num_);
  void RequestAddTrack(int num_);

 public slots:
  void ShowContextMenu(const QPoint& pos_);
};

#endif /* FILEVIEW_H */
