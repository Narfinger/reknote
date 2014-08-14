#ifndef GITWRAPPER_H
#define GITWRAPPER_H

#include <QtCore/QMutex>
#include <QSharedPointer>
#include <QString>

extern "C" {
#include <git2.h>
}

class Spike;
class SpikesTreeModel;
class GitIndex;

class GitRepository {
  friend class GitIndex;
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
};

class GitIndex {
  friend class GitRepository;
public:
  GitIndex(QSharedPointer<GitRepository> r);
  ~GitIndex() { git_index_free(index_); };
  const git_index* index() const { return index_; };	//don't delete this index, it will break
  void add(const QSharedPointer<Spike>& s);
  void add(const SpikesTreeModel& stm);
  void commit() { repo_->commitIndex(*this); };
  
private:
  QSharedPointer<GitRepository> repo_;
  git_index* index_;
  
};
static void gitErrorHandling();

#endif // GITWRAPPER_H
