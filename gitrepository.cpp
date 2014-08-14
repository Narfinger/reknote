#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QMutexLocker>
#include <QDebug>

#include "spike.h"
#include "gitrepository.h"

QMutex GitRepository::gitMutex;

GitRepository::GitRepository(const QString& repo) : repodirba_(repo.toUtf8()), repodir_(repodirba_.data()) {
  git_threads_init();
  QMutexLocker l(&gitMutex);
  int error = git_repository_open(&repo_, repodir_);
  if(error <0) {
    l.unlock();
    qDebug() << "creating repository";
    createRepository(); 
    openRepository();
  }
}

GitRepository::~GitRepository() {
  git_repository_free(repo_); 
  git_threads_shutdown();
}

bool GitRepository::openRepository() {
  QMutexLocker l(&gitMutex);
  int error = git_repository_open(&repo_, repodir_);
  if(error < 0) {
    gitErrorHandling();
    return false;
  }
  return true;
}

void GitRepository::createRepository() {
  QMutexLocker l(&gitMutex);
  int error = git_repository_init(&repo_, repodir_, false);
  if (error < 0) {
    gitErrorHandling();
  }
  
  git_signature *sig = NULL;
  git_index *index = NULL;
  git_oid tree_id;
  git_oid commit_id;
  git_tree *tree = NULL;

  // no error handling at the moment
  git_signature_now(&sig, "AutoGit", "auto@localhost");
  git_repository_index(&index, repo_);
  git_index_write_tree(&tree_id, index);
  git_tree_lookup(&tree, repo_, &tree_id);
  git_commit_create_v(&commit_id, repo_, "HEAD", sig, sig, NULL, "Initial commit", tree, 0);

  git_signature_free(sig);
  git_index_free(index);
  git_tree_free(tree);
}

bool GitRepository::commitIndex(GitIndex& index) {
  //  write git index
  git_signature *sig = NULL;
  git_oid tree_id;
  git_oid commit_id;
  git_tree *tree = NULL;

  int error = git_signature_now(&sig, "AutoGit", "auto@localhost");
  if(error < 0) {
    gitErrorHandling();
    return false;  
  }

  /*error = git_repository_index( &(index.index_), repo_);
  if(error < 0) {
    gitErrorHandling();
    return false;
  }*/

  git_commit * commit = NULL; // parent
  git_oid oid_parent_commit;  // the SHA1 for last commit

  error = git_reference_name_to_id( &oid_parent_commit, repo_, "HEAD" );
  if(error < 0) {
    gitErrorHandling();
    return false;
  }

  error = git_commit_lookup( &commit, repo_, &oid_parent_commit );
  if(error < 0) {
    gitErrorHandling();
    return false;
  }

  error = git_index_write(index.index_);
  if(error < 0) {
    gitErrorHandling();
    return false;
  }

  error = git_index_write_tree(&tree_id, index.index_);
  if(error < 0) {
    gitErrorHandling();
    return false;
    
  }

  error = git_tree_lookup(&tree, repo_, &tree_id);
  if(error < 0) {
    gitErrorHandling();
    return false;
  }

  const git_commit* parentarray[] = {commit};
  const QString message("auto commit");
  QByteArray commitmessageba = message.toUtf8();
  const char *commitmessageCString = commitmessageba.data();
  error = git_commit_create(&commit_id, repo_, "HEAD", sig, sig, NULL, commitmessageCString, tree, 1, parentarray);
  if(error < 0) {
    gitErrorHandling();
    return false;
  }
  git_signature_free(sig);
  git_tree_free(tree);
  return true;
}

GitIndex::GitIndex(QSharedPointer<GitRepository> r): repo_(r) {
  int error = git_repository_index(&index_, repo_->repo_);
  if (error <0) index_ = nullptr;
}


void GitIndex::add(const SpikePtr& s) {
  if (index_!=nullptr) {
    const QString dirname = s->dirName() + "/*";
    QByteArray ba = dirname.toUtf8();
    char* baCString = ba.data();
    git_strarray arr = { &baCString, 1};
    
    int error = git_index_add_all(index_, &arr, 0,0,0);
    if (error <0) { 
      qDebug() << "error with adding spike";
      gitErrorHandling();
    }
  } else qDebug() << "Index is null";
}

void GitIndex::add(const SpikesTreeModel& stm) {
  if (index_!=nullptr) {
    const QString path = "spikestree.xml";
    const QByteArray ba = path.toUtf8();
    const char* baCString = ba.data();
    int error = git_index_add_bypath(index_, baCString);
    if (error < 0) {
     qDebug() << "error with adding tree";
     gitErrorHandling();
    }
  } else qDebug() << "Index is null";
}

void gitErrorHandling()
{
    const git_error *e = giterr_last();
    qDebug() << "Error in git (error,class,message)" << e->klass << e->message;
}

