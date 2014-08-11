#ifndef GITWRAPPER_H
#define GITWRAPPER_H

#include <QtCore/QMutex>
#include <QSharedPointer>
#include <QString>

extern "C" {
#include <git2.h>
}

class Spike;
class GitIndex;

class GitRepository {
  
public:
  GitRepository(const QString& repo);
  ~GitRepository();
  
  static QMutex gitMutex;

private:
  const QByteArray repodirba_;	//this needs to be kept around because otherwise the const char* will vanish
  const char* repodir_;
  git_repository* repo_ = nullptr;
  
  bool openRepository();
  void createRepository();
  bool commitIndex(GitIndex& index);
  
  void gitErrorHandling();
};

class GitIndex {
  friend class GitRepository;
  GitIndex(QSharedPointer<GitRepository> r) : repo_(r) { /*FIXME i need the index here /*git_repository_index(&index_, repo_.data());*/ };
  ~GitIndex() { git_index_free(index_); };
  
  void addSpike(QSharedPointer<Spike> s);
  
private:
  QSharedPointer<GitRepository> repo_;
  git_index* index_;
  
};

#endif // GITWRAPPER_H
