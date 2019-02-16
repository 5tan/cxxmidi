#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QListView>
#include <QObject>

class FileView : public QListView {
  Q_OBJECT

 public:
  FileView(QWidget* parent = 0);
  virtual ~FileView();

 signals:
  void RequestDeleteTrack(int num);
  void RequestAddTrack(int num);

 public slots:
  void ShowContextMenu(const QPoint& pos);
};

#endif /* FILEVIEW_H */
